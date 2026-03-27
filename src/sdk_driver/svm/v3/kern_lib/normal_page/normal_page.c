/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#include "ka_memory_pub.h"
#include "ka_system_pub.h"
#include "ka_common_pub.h"
#include "ka_sched_pub.h"

#include "pbl_feature_loader.h"
#include "pbl_chip_config.h"

#include "svm_kern_log.h"
#include "svm_gfp.h"
#include "svm_pub.h"
#include "svm_slab.h"
#include "normal_page.h"

#define SVM_CONT_PAGES_MAX_NUM      512ULL  /* 2M is 512 * 4K */

static void svm_clear_normal_single_page(ka_page_t *pg)
{
    svm_clear_single_page(pg, KA_MM_PAGE_SIZE);
}

static void svm_clear_compound_page(ka_page_t *pg)
{
    unsigned long stamp = (unsigned long)ka_jiffies;
    ka_page_t *head_page = ka_mm_compound_head(pg);
    unsigned int order = ka_mm_compound_order(head_page);
    u64 pg_num = 1 << order;
    u64 i;

    for (i = 0; i < pg_num; i++) {
        ka_try_cond_resched(&stamp);
        svm_clear_single_page(head_page + i, KA_MM_PAGE_SIZE);
    }
}

static inline void svm_put_page(ka_page_t *pg)
{
    ka_mm_put_page(pg);
}

static void svm_free_normal_page(ka_page_t *pg, u32 flag)
{
    if (ka_mm_PageCompound(pg) == 0) {
        svm_page_ref_dec(pg, (svm_page_is_need_clear(flag) ? svm_clear_normal_single_page : NULL), svm_put_page);
    } else {
        svm_page_ref_dec(pg, (svm_page_is_need_clear(flag) ? svm_clear_compound_page : NULL), svm_put_page);
    }
}

static void svm_free_normal_pages(ka_page_t **pages, u64 page_num, u32 flag)
{
    unsigned long stamp = ka_jiffies;
    u64 i;

    for (i = 0; i < page_num; i++) {
        svm_free_normal_page(pages[i], flag);
        ka_try_cond_resched(&stamp);
    }
}

static int svm_alloc_pages_node(int nid, ka_page_t **pages, u64 pg_num)
{
    unsigned long stamp = (unsigned long)ka_jiffies;
    ka_page_t *comp_pg_head = NULL;
    ka_page_t *pg = NULL;
    u64 i;

    /* Because is comp page, pg_num should be power of 2. */
    if (ka_mm_is_power_of_2(pg_num) == false) {
        return -EINVAL;
    }

    comp_pg_head = _svm_alloc_pages_node(nid, svm_get_alloc_page_mask(true), ka_mm_get_order(pg_num << KA_MM_PAGE_SHIFT));
    if (comp_pg_head == NULL) {
        return -ENOMEM;
    }

    for (i = 0, pg = comp_pg_head; i < pg_num; pg++, i++) {
        ka_try_cond_resched(&stamp);
        pages[i] = pg;
        if (i != 0) {
            ka_mm_get_page(pages[i]);
        }
    }

    return 0;
}

static int svm_alloc_continous_pages(int *latest_nid, int nids[], u32 nid_num,
    ka_page_t **pages, u64 pg_num)
{
    unsigned long stamp = (unsigned long)ka_jiffies;
    int ret, i;

    ret = svm_alloc_pages_node(*latest_nid, pages, pg_num);
    if (ret == 0) {
        return 0;
    }

    for (i = 0; i < nid_num; i++) {
        ka_try_cond_resched(&stamp);
        if (nids[i] == *latest_nid) {
            continue;
        }
        ret = svm_alloc_pages_node(nids[i], pages, pg_num);
        if (ret == 0) {
            *latest_nid = nids[i];
            return 0;
        }
    }

    return -ENOMEM;
}

/* Alloc continuous pages every 2MB, and return got_num. */
static u64 _svm_try_alloc_continous_pages(int *latest_nid, int nids[], u32 nid_num,
    ka_page_t **pages, u64 pg_num)
{
    unsigned long stamp = ka_jiffies;
    u64 num, i = 0;
    int ret;

    for (i = 0; i < pg_num; i += num) {
        num = ka_base_min(SVM_CONT_PAGES_MAX_NUM, (pg_num - i));
        ret = svm_alloc_continous_pages(latest_nid, nids, nid_num, &pages[i], num);
        if (ret != 0) {
            return i;
        }
        ka_try_cond_resched(&stamp);
    }

    return pg_num;
}

static bool svm_is_necessary_to_alloc_continous_pages(void)
{
/* If page_size >= 64k, the performance of dma_copy is not greatly improved.(Doubtful, keep the status quo first) */
#if (KA_MM_PAGE_SIZE >= (64ULL * SVM_BYTES_PER_KB))
    return false;
#else
    return true;
#endif
}

static int svm_alloc_normal_pages_one_by_one(int *latest_nid, int nids[], u32 nid_num,
    ka_page_t **pages, u64 pg_num)
{
    unsigned long stamp = ka_jiffies;
    u64 i;
    int ret;

    for (i = 0; i < pg_num; i++) {
        ret = svm_alloc_continous_pages(latest_nid, nids, nid_num, &pages[i], 1);
        if (ret != 0) {
            svm_free_normal_pages(pages, i, 0);
            return -ENOMEM;
        }
        ka_try_cond_resched(&stamp);
    }

    return 0;
}

/* The returned pages is not necessarily continuous, but is as continuous as possible. */
static int svm_try_alloc_continous_pages(int nids[], u32 nid_num, ka_page_t **pages, u64 pg_num)
{
    int latest_nid = nids[0];
    u64 got_num = 0;
    int ret;

    if (svm_is_necessary_to_alloc_continous_pages()) {
        got_num = _svm_try_alloc_continous_pages(&latest_nid, nids, nid_num, pages, pg_num);
        if (got_num == pg_num) {
            return 0;
        }
    }

    ret = svm_alloc_normal_pages_one_by_one(&latest_nid, nids, nid_num, &pages[got_num], pg_num - got_num);
    if (ret != 0) {
        svm_free_normal_pages(pages, got_num, 0);
    }

    return ret;
}

static int svm_alloc_normal_pages(int nids[], u32 nid_num, ka_page_t **pages, u64 page_num, u32 flag)
{
    int latest_nid = nids[0];

    if (svm_page_is_continuous(flag)) {
        return svm_alloc_continous_pages(&latest_nid, nids, nid_num, pages, page_num);
    }

    return svm_try_alloc_continous_pages(nids, nid_num, pages, page_num);
}

static const struct svm_page_ops normal_page_ops = {
    .alloc = svm_alloc_normal_pages,
    .free = svm_free_normal_pages,
};

int svm_normal_page_init(void)
{
    svm_register_page_ops(SVM_PAGE_GRAN_NORMAL, &normal_page_ops);
    return 0;
}
DECLAER_FEATURE_AUTO_INIT(svm_normal_page_init, FEATURE_LOADER_STAGE_0);

