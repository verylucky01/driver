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

#include "pbl_feature_loader.h"

#include "svm_kern_log.h"
#include "svm_pgtable.h"
#include "svm_gfp.h"
#include "normal_pgtable.h"

static int svm_remap_normal_pages(ka_vm_area_struct_t *vma, u64 va, u64 pa, u64 page_num, ka_mm_pgprot_t pg_prot)
{
    int ret;

    ret = ka_mm_remap_pfn_range(vma, va, __ka_mm_phys_to_pfn(pa), page_num * KA_MM_PAGE_SIZE, pg_prot);
    if (ret != 0) {
        svm_err("Remap_pfn_range failed. (ret=%d; va=0x%llx; page_num=%llu)\n", ret, va, page_num);
        return ret;
    }

    return 0;
}

static void svm_unmap_normal_pages(ka_vm_area_struct_t *vma, u64 va, u64 page_num)
{
    ka_mm_zap_vma_ptes(vma, va, page_num * KA_MM_PAGE_SIZE);
}


static const struct svm_page_table_ops normal_pgtbl_ops = {
    .remap = svm_remap_normal_pages,
    .unmap = svm_unmap_normal_pages,
};

int svm_normal_pgtbl_init(void)
{
    svm_register_page_table_ops(SVM_PAGE_GRAN_NORMAL, &normal_pgtbl_ops);
    return 0;
}
DECLAER_FEATURE_AUTO_INIT(svm_normal_pgtbl_init, FEATURE_LOADER_STAGE_0);

