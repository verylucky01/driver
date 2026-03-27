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
#include "pbl_uda.h"

#include "svm_kern_log.h"
#include "dbi_kern.h"
#include "svm_gfp.h"
#include "compound_large_page.h"

static void svm_free_compound_large_pages(ka_page_t **pages, u64 page_num, u64 compound_size)
{
    u64 normal_page_num = 0x1ULL << ka_mm_get_order(compound_size);
    unsigned long stamp = ka_jiffies;
    u64 i, j;

    for (i = 0; i < page_num; i++) {
        ka_page_t *comp_page_head = pages[i];

        for (j = 1; j < normal_page_num; j++) {
            ka_mm_put_page(comp_page_head + j);
        }

        _svm_free_pages(comp_page_head, ka_mm_get_order(compound_size));
        pages[i] = NULL;
        ka_try_cond_resched(&stamp);
    }
}

static int svm_alloc_compound_large_pages(int nids[], u32 nid_num, ka_page_t **pages, u64 page_num,
    u64 compound_size, u32 flag)
{
    u64 normal_page_num = 0x1ULL << ka_mm_get_order(compound_size);
    unsigned long stamp = ka_jiffies;
    u64 i, j;

    for (i = 0; i < page_num; i++) {
        ka_page_t *comp_page_head = NULL;

        comp_page_head = _svm_alloc_pages_node(nids[0], svm_get_alloc_page_mask(true), ka_mm_get_order(compound_size));
        if (comp_page_head == NULL) {
            svm_free_compound_large_pages(pages, i, compound_size);
            return -ENOMEM;
        }

        pages[i] = comp_page_head;

        /* free get page size by va is normal page will call svm_free_normal_pages, so added page ref */
        for (j = 1; j < normal_page_num; j++) {
            ka_mm_get_page(comp_page_head + j);
        }

        ka_try_cond_resched(&stamp);
    }

    return 0;
}

static int svm_alloc_compound_huge_pages(int nids[], u32 nid_num, ka_page_t **pages, u64 page_num, u32 flag)
{
    u64 hpage_size;
    int ret;

    ret = svm_dbi_kern_query_hpage_size(uda_get_host_id(), &hpage_size);
    if (ret != 0) {
        return ret;
    }

    return svm_alloc_compound_large_pages(nids, nid_num, pages, page_num, hpage_size, flag);
}

static void svm_free_compound_huge_pages(ka_page_t **pages, u64 page_num, u32 flag)
{
    u64 hpage_size;
    int ret;

    ret = svm_dbi_kern_query_hpage_size(uda_get_host_id(), &hpage_size);
    if (ret != 0) {
        return;
    }

    svm_free_compound_large_pages(pages, page_num, hpage_size);
}

static const struct svm_page_ops compound_huge_page_ops = {
    .alloc = svm_alloc_compound_huge_pages,
    .free = svm_free_compound_huge_pages,
};

int svm_compound_large_page_init(void)
{
    svm_register_page_ops(SVM_PAGE_GRAN_HUGE, &compound_huge_page_ops);
    return 0;
}
DECLAER_FEATURE_AUTO_INIT(svm_compound_large_page_init, FEATURE_LOADER_STAGE_0);

