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
#include "ka_system_pub.h"
#include "ka_memory_pub.h"
#include "ka_compiler_pub.h"
#include "ka_sched_pub.h"

#include "pbl_feature_loader.h"

#include "svm_kern_log.h"
#include "svm_pgtable.h"

int hugetlb_insert_hugepage_pte_by_pa(ka_mm_struct_t *mm, unsigned long addr, ka_mm_pgprot_t prot, unsigned long pa);
void svm_unmap_huge_pages(ka_vm_area_struct_t *vma, u64 va, u64 page_num);

#ifndef EMU_ST /* Simulation ST ignore pte */
static int svm_pmd_entry_of_free_ptes(ka_pmd_t *pmd, u64 addr, u64 next, struct ka_pgwalk *walk)
{
    ka_vm_area_struct_t *vma = walk->vma;
    u64 start = addr;
    u64 end = next;

#ifndef CFG_FEATURE_ENABLE_ASAN
    ka_pgtable_t pte = ka_mm_pmd_pgtable(*pmd);
#endif
    ka_mm_pmd_clear(pmd);
    ka_mm_flush_tlb_range(vma, start, end);
#ifndef CFG_FEATURE_ENABLE_ASAN
    ka_mm_pte_free(vma->vm_mm, pte);   /* asan do not find ptlock_free api */
    ka_mm_dec_nr_ptes(vma->vm_mm);
#endif
    return 0;
}

static void svm_free_ptes(ka_vm_area_struct_t *vma, u64 start, u64 end)
{
    struct ka_pgwalk_ops ops = {NULL};
    ops.pmd_entry = svm_pmd_entry_of_free_ptes;
    (void)ka_walk_page_range(vma, start, end, &ops, NULL);
}

static int svm_pte_entry_of_unmap_huge_pages(ka_pte_t *pte, u64 addr, u64 next,
    enum ka_pte_level level, struct ka_pgwalk *walk)
{
    ka_pmd_t *pmd = NULL;

    if (ka_unlikely(level != KA_PMD_LEVEL)) {
        svm_err("Isn't huge pgtable. (addr=0x%llx; next=0x%llx; level=%u)\n", addr, next, level);
        return -EFAULT;
    }

    pmd = (ka_pmd_t *)pte;
    ka_mm_pmd_clear(pmd);
    return 0;
}

void svm_unmap_huge_pages(ka_vm_area_struct_t *vma, u64 va, u64 page_num)
{
    struct ka_pgwalk_ops ops = {NULL};
    u64 start = va;
    u64 end = va + (page_num * KA_HPAGE_SIZE);

    ops.pte_entry = svm_pte_entry_of_unmap_huge_pages;
    (void)ka_walk_page_range(vma, start, end, &ops, NULL);

    ka_mm_flush_tlb_range(vma, start, end);
}
#endif

static int svm_remap_huge_pages(ka_vm_area_struct_t *vma, u64 va, u64 pa, u64 page_num, ka_mm_pgprot_t pg_prot)
{
    unsigned long stamp = ka_jiffies;
    u64 offset = 0;
    u64 i;

#ifndef EMU_ST /* Simulation ST ignore pte */
    /* no need to restore when unmap, os will create pte when remap in next time */
    svm_free_ptes(vma, va, va + page_num * KA_HPAGE_SIZE);
#endif

    for (i = 0; i < page_num; i++) {
        int ret = hugetlb_insert_hugepage_pte_by_pa(vma->vm_mm, va + offset, pg_prot, pa + offset);
        if (ret != 0) {
            if (i > 0) {
                svm_unmap_huge_pages(vma, va, i);
            }
            svm_err("Remap failed. (page_num=%llu; cur_page=%llu; ret=%d)\n", page_num, i, ret);
            return ret;
        }
        offset += KA_HPAGE_SIZE;
        ka_try_cond_resched(&stamp);
    }

    return 0;
}

static const struct svm_page_table_ops huge_pgtbl_ops = {
    .remap = svm_remap_huge_pages,
    .unmap = svm_unmap_huge_pages,
};

int svm_huge_pgtbl_init(void)
{
    svm_register_page_table_ops(SVM_PAGE_GRAN_HUGE, &huge_pgtbl_ops);
    return 0;
}
DECLAER_FEATURE_AUTO_INIT(svm_huge_pgtbl_init, FEATURE_LOADER_STAGE_0);

