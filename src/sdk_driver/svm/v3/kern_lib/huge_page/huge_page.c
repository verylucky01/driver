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

#include "svm_gfp.h"

#define MPL_HUGEPAGE_2M_ORDER 9

#ifndef HUGETLB_ALLOC_NONE
#define HUGETLB_ALLOC_NONE             0x00
#endif
#ifndef HUGETLB_ALLOC_NORMAL
#define HUGETLB_ALLOC_NORMAL           0x01
#endif
#ifndef HUGETLB_ALLOC_BUDDY
#define HUGETLB_ALLOC_BUDDY            0x02
#endif
#ifndef HUGETLB_ALLOC_NORECLAIM
#define HUGETLB_ALLOC_NORECLAIM        0x04
#endif

static void svm_clear_huge_page(ka_page_t *pg)
{
    svm_clear_single_page(pg, KA_HPAGE_SIZE);
}

static inline void svm_put_page(ka_page_t *pg)
{
    ka_mm_put_page(pg);
}

static void svm_free_huge_page(ka_page_t *pg, u32 flag)
{
    svm_page_ref_dec(pg, (svm_page_is_need_clear(flag) ? svm_clear_huge_page : NULL), svm_put_page);
}

static void svm_free_huge_pages(ka_page_t **pages, u64 page_num, u32 flag)
{
    unsigned long stamp = ka_jiffies;
    u64 i;

    for (i = 0; i < page_num; i++) {
        if (pages[i] != NULL) {
            svm_free_huge_page(pages[i], flag);
            pages[i] = NULL;
        }
        ka_try_cond_resched(&stamp);
    }
}

static ka_page_t *svm_alloc_hpage(int nid, u32 flag)
{
    u32 tmp_flag = flag;
    ka_gfp_t gfp_mask;

    if (tmp_flag == HUGETLB_ALLOC_NORMAL) {
#ifdef CFG_FEATURE_SURPORT_HUGETLB_FOLIO
        return (ka_page_t *)alloc_np_page_nid(KA_HPAGE_SIZE, nid);
#else
        return (ka_page_t*)alloc_hugetlb_folio_size(nid, KA_HPAGE_SIZE);
#endif
    } else if (tmp_flag == (HUGETLB_ALLOC_BUDDY | HUGETLB_ALLOC_NORECLAIM)) {
#ifdef CFG_FEATURE_SURPORT_HUGETLB_FOLIO
        gfp_mask = ((KA_GFP_HIGHUSER_MOVABLE | __KA_GFP_THISNODE | KA_GFP_KERNEL | __KA_GFP_COMP | __KA_GFP_ACCOUNT | __KA_GFP_NOWARN |
            __KA_GFP_RETRY_MAYFAIL) & (~__KA_GFP_RECLAIM));
        return (ka_page_t *)alloc_temporary_hugetlb_folio(nid, gfp_mask);
    } else if (tmp_flag == HUGETLB_ALLOC_BUDDY) {
        gfp_mask = (KA_GFP_HIGHUSER_MOVABLE | __KA_GFP_THISNODE | KA_GFP_KERNEL | __KA_GFP_COMP | __KA_GFP_ACCOUNT | __KA_GFP_NOWARN |
            __KA_GFP_RETRY_MAYFAIL);
        return (ka_page_t *)alloc_temporary_hugetlb_folio(nid, gfp_mask);
#else
        gfp_mask = ((KA_GFP_KERNEL | __KA_GFP_COMP | __KA_GFP_ACCOUNT) & (~__KA_GFP_RECLAIM));
        return ka_mm_alloc_pages_node(nid, gfp_mask, MPL_HUGEPAGE_2M_ORDER);
    } else if (tmp_flag == HUGETLB_ALLOC_BUDDY) {
        gfp_mask = (KA_GFP_KERNEL | __KA_GFP_COMP | __KA_GFP_ACCOUNT);
        return ka_mm_alloc_pages_node(nid, gfp_mask, MPL_HUGEPAGE_2M_ORDER);
#endif
    } else {
        return NULL;
    }
}

static ka_page_t *_svm_alloc_huge_page(int nids[], u32 nid_num, u32 *latest_nid, u32 flag)
{
    unsigned long stamp = (unsigned long)ka_jiffies;
    ka_page_t *hpage = NULL;
    u32 i;

    hpage = svm_alloc_hpage(*latest_nid, (int)flag);
    if (hpage != NULL) {
        return hpage;
    }

    for (i = 0; i < nid_num; i++) {
        ka_try_cond_resched(&stamp);
        if (nids[i] == *latest_nid) {
            continue;
        }

        hpage = svm_alloc_hpage(nids[i], flag);
        if (hpage != NULL) {
            *latest_nid = nids[i];
            return hpage;
        }
    }

    return NULL;
}

static ka_page_t *svm_alloc_huge_page(int nids[], u32 nid_num, u32 *latest_nid)
{
    ka_page_t *hpage = NULL;

    hpage = _svm_alloc_huge_page(nids, nid_num, latest_nid, HUGETLB_ALLOC_NORMAL);
    if (hpage != NULL) {
        return hpage;
    }

    hpage = _svm_alloc_huge_page(nids, nid_num, latest_nid, HUGETLB_ALLOC_BUDDY | HUGETLB_ALLOC_NORECLAIM);
    if (hpage != NULL) {
        return hpage;
    }

    return _svm_alloc_huge_page(nids, nid_num, latest_nid, HUGETLB_ALLOC_BUDDY);
}

static int svm_alloc_huge_pages(int nids[], u32 nid_num, ka_page_t **pages, u64 page_num, u32 flag)
{
    unsigned long stamp = ka_jiffies;
    int latest_nid = nids[0];
    u64 i;

    for (i = 0; i < page_num; i++) {
        pages[i] = svm_alloc_huge_page(nids, nid_num, &latest_nid);
        if (pages[i] == NULL) {
            svm_free_huge_pages(pages, i, 0);
            return -ENOMEM;
        }
        ka_try_cond_resched(&stamp);
    }

    return 0;
}

static const struct svm_page_ops huge_page_ops = {
    .alloc = svm_alloc_huge_pages,
    .free = svm_free_huge_pages,
};

int svm_huge_page_init(void)
{
    svm_register_page_ops(SVM_PAGE_GRAN_HUGE, &huge_page_ops);
    return 0;
}
DECLAER_FEATURE_AUTO_INIT(svm_huge_page_init, FEATURE_LOADER_STAGE_0);

