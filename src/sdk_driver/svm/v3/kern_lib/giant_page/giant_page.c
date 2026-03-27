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

#include "pbl/pbl_feature_loader.h"

#include "svm_gfp.h"

static void svm_clear_giant_page(ka_page_t *pg)
{
    svm_clear_single_page(pg, SVM_GPAGE_SIZE);
}

static inline void svm_put_page(ka_page_t *pg)
{
    ka_mm_put_page(pg);
}

static void svm_free_giant_page(ka_page_t *pg, u32 flag)
{
    svm_page_ref_dec(pg, (svm_page_is_need_clear(flag) ? svm_clear_giant_page : NULL), svm_put_page);
}

static void svm_free_giant_pages(ka_page_t **pages, u64 page_num, u32 flag)
{
    unsigned long stamp = ka_jiffies;
    u64 i;

    for (i = 0; i < page_num; i++) {
        if (pages[i] != NULL) {
            svm_free_giant_page(pages[i], flag);
            pages[i] = NULL;
        }
        ka_try_cond_resched(&stamp);
    }
}

static ka_page_t *svm_alloc_gpage(int nid)
{
#ifdef CFG_FEATURE_SURPORT_HUGETLB_FOLIO
    return (ka_page_t *)alloc_np_page_nid(SVM_GPAGE_SIZE, nid);
#else
    return (ka_page_t *)alloc_hugetlb_folio_size(nid, SVM_GPAGE_SIZE);
#endif
}

static ka_page_t *svm_alloc_giant_page(int nids[], u32 nid_num, u32 *latest_nid)
{
    unsigned long stamp = (unsigned long)ka_jiffies;
    ka_page_t *gpage = NULL;
    u32 i;

    gpage = svm_alloc_gpage(*latest_nid);
    if (gpage != NULL) {
        return gpage;
    }

    for (i = 0; i < nid_num; i++) {
        ka_try_cond_resched(&stamp);
        if (nids[i] == *latest_nid) {
            continue;
        }

        gpage = svm_alloc_gpage(nids[i]);
        if (gpage != NULL) {
            *latest_nid = nids[i];
            return gpage;
        }
    }

    return NULL;
}

static int svm_alloc_giant_pages(int nids[], u32 nid_num, ka_page_t **pages, u64 page_num, u32 flag)
{
    unsigned long stamp = ka_jiffies;
    int latest_nid = nids[0];
    u64 i;

    for (i = 0; i < page_num; i++) {
        pages[i] = svm_alloc_giant_page(nids, nid_num, &latest_nid);
        if (pages[i] == NULL) {
            svm_free_giant_pages(pages, i, 0);
            return -ENOMEM;
        }
        ka_try_cond_resched(&stamp);
    }

    return 0;
}

static const struct svm_page_ops giant_page_ops = {
    .alloc = svm_alloc_giant_pages,
    .free = svm_free_giant_pages,
};

int svm_giant_page_init(void)
{
    svm_register_page_ops(SVM_PAGE_GRAN_GIANT, &giant_page_ops);
    return 0;
}
DECLAER_FEATURE_AUTO_INIT(svm_giant_page_init, FEATURE_LOADER_STAGE_0);

