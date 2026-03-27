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
#include "ka_kernel_def_pub.h"
#include "ka_task_pub.h"
#include "ka_system_pub.h"
#include "ka_memory_pub.h"
#include "ka_common_pub.h"
#include "ka_sched_pub.h"

#include "ascend_kernel_hal.h"

#include "framework_vma.h"
#include "svm_kern_log.h"
#include "svm_pgtable.h"
#include "svm_gup.h"
#include "svm_mm.h"
#include "pmq.h"

void svm_pmq_pa_put(struct svm_pa_seg pa_seg[], u64 seg_num)
{
    unsigned long stamp = (unsigned long)ka_jiffies;
    ka_page_t *pg = NULL;
    u64 i;

    for (i = 0; i < seg_num; ++i) {
        ka_try_cond_resched(&stamp);
        if (ka_mm_page_is_ram(KA_MM_PFN_DOWN(pa_seg[i].pa))) {
            pg = ka_mm_pfn_to_page((unsigned long)KA_MM_PFN_DOWN(pa_seg[i].pa));
            ka_mm_put_page(pg);
        }
    }
}

static int _svm_pmq_pa_query(ka_mm_struct_t *mm, u64 va, u64 size,
    struct svm_pa_seg pa_seg[], u64 *seg_num)
{
    ka_vm_area_struct_t *vma = NULL;
    u64 query_size;

    vma = ka_mm_find_vma(mm, va);
    if (vma == NULL) {
        svm_err("Find vma failed. (va=0x%llx)\n", va);
        return -ENOSYS;
    }

    if (((va + size) > ka_mm_get_vm_end(vma)) || (svm_check_vma(vma, va, size) != 0)) {
        svm_err("Vma is invalid. (va=0x%llx; size=0x%llx; vma_start=0x%lx; vma_end=0x%lx)\n",
            va, size, ka_mm_get_vm_start(vma), ka_mm_get_vm_end(vma));
        return -EFAULT;
    }

    query_size = svm_query_phys(vma, va, size, pa_seg, seg_num);
    if (query_size != size) {
        svm_err("Query pa failed. (va=0x%llx; size=0x%llx; query_size=0x%llx)\n", va, size, query_size);
        return -EFAULT;
    }

    return 0;
}

int svm_pmq_pa_get(int tgid, u64 va, u64 size, struct svm_pa_seg pa_seg[], u64 *seg_num)
{
    unsigned long stamp = (unsigned long)ka_jiffies;
    ka_mm_struct_t *mm = NULL;
    ka_page_t *pg = NULL;
    bool is_local = false;
    int ret;
    u64 i;

    mm = svm_mm_get(tgid, false);
    if (mm == NULL) {
        return -ENOSYS;
    }

    ret = _svm_pmq_pa_query(mm, va, size, pa_seg, seg_num);
    if (ret != 0) {
        svm_mm_put(mm, false);
        return ret;
    }

    is_local = ka_mm_page_is_ram(KA_MM_PFN_DOWN(pa_seg[0].pa));
    for (i = 0; i < *seg_num; i++) {
        ka_try_cond_resched(&stamp);
        if (is_local) {
            pg = ka_mm_pfn_to_page(KA_MM_PFN_DOWN(pa_seg[i].pa));
            ka_mm_get_page(pg);
        }
    }

    svm_mm_put(mm, false);
    return 0;
}

int svm_pmq_pa_query(int tgid, u64 va, u64 size, struct svm_pa_seg pa_seg[], u64 *seg_num)
{
    ka_mm_struct_t *mm = NULL;
    u64 max_size_by_seg_num = *seg_num * KA_MM_PAGE_SIZE;
    u64 query_size = (size < max_size_by_seg_num) ? size : max_size_by_seg_num;
    int ret;

    mm = svm_mm_get(tgid, false);
    if (mm == NULL) {
        return -ENOSYS;
    }

    ret = _svm_pmq_pa_query(mm, va, query_size, pa_seg, seg_num);
    svm_mm_put(mm, false);

    if (ret == 0) {
        *seg_num = svm_make_pa_continues(pa_seg, *seg_num);
    }

    return ret;
}

#if !defined(CFG_PLATFORM_SLT) && !defined(CFG_PLATFORM_RC)
int hal_kernel_svm_get_user_pages(int pid, u64 va, u32 nr_pages, void **pages, bool *is_remap_addr)
{
    int ret;

    if ((pages == NULL) || (is_remap_addr == NULL) || (nr_pages == 0)) {
        svm_err("Invalid paras. (pages_is_null=%d; nr_pages=%u)\n", (pages == NULL), nr_pages);
        return -EINVAL;
    }

    if (va != svm_align_down(va, KA_MM_PAGE_SIZE)) {
        svm_info("Va not align with PAGE_SIZE. (va=0%llx; size=%lu)\n", va, KA_MM_PAGE_SIZE * nr_pages);
        return -EINVAL;
    }

    ret = svm_pin_svm_range_uva_npages(pid, va, true, (ka_page_t **)pages, nr_pages);
    if (ret != 0) {
        svm_err("Svm_pin_svm_range_uva_npages failed. (pid=%d; va=0%llx; size=%llu)\n", pid, va, KA_MM_PAGE_SIZE * (u64)nr_pages);
        return ret;
    }
    *is_remap_addr = true;
    return ret;
}
KA_EXPORT_SYMBOL_GPL(hal_kernel_svm_get_user_pages);

void hal_kernel_svm_put_user_pages(void **pages, u32 nr_pages, bool is_remap_addr)
{
    if ((pages == NULL) || (is_remap_addr == false)) {
        return;
    }

    svm_unpin_svm_range_uva_npages((ka_page_t **)pages, nr_pages, nr_pages);
}
KA_EXPORT_SYMBOL_GPL(hal_kernel_svm_put_user_pages);
#else
#include "kernel_version_adapt.h"
#include "pbl/pbl_task_ctx.h"

static int _svm_get_user_pages(ka_task_struct_t *tsk, ka_mm_struct_t *mm, u64 va, u32 nr_pages,
    void **pages, bool *is_remap_addr)
{
    ka_vm_area_struct_t *vma = NULL;
    int ret;

    ka_task_down_read(get_mmap_sem(mm));
    vma = ka_mm_find_vma(mm, va);
    if ((vma == NULL) || (ka_mm_get_vm_start(vma) > va)) {
        svm_err("Invalid addr. (va=0x%llx)\n", va);
        ka_task_up_read(get_mmap_sem(mm));
        return -EINVAL;
    }

    if ((ka_mm_get_vm_flags(vma) & KA_VM_PFNMAP) != 0) {
        *is_remap_addr = true;
        ret = svm_pin_svm_npages(vma, va, nr_pages, true, (ka_page_t **)pages);
        ka_task_up_read(get_mmap_sem(mm));
    } else {
        ka_task_up_read(get_mmap_sem(mm));
        *is_remap_addr = false;
        ret = svm_pin_user_npages_remote(tsk, mm, va, nr_pages, (ka_page_t **)pages);
    }
    return ret;
}

static int svm_get_user_pages(int pid, u64 va, u32 nr_pages, void **pages, bool *is_remap_addr)
{
    ka_task_struct_t *tsk = NULL;
    ka_mm_struct_t *mm = NULL;
    int ret;

    tsk = task_get_by_tgid(pid);
    if (tsk == NULL) {
        return -ESRCH;
    }

    mm = ka_task_get_task_mm(tsk);
    if (mm == NULL) {
        ka_task_put_task_struct(tsk);
        return -ESRCH;
    }

    ret = _svm_get_user_pages(tsk, mm, va, nr_pages, pages, is_remap_addr);
    ka_mm_mmput(mm);
    ka_task_put_task_struct(tsk);
    return ret;
}

int hal_kernel_svm_get_user_pages(int pid, u64 va, u32 nr_pages, void **pages, bool *is_remap_addr)
{
    int ret;

    if ((pages == NULL) || (is_remap_addr == NULL) || (nr_pages == 0)) {
        svm_err("Invalid paras. (pages_is_null=%d; nr_pages=%u)\n", (pages == NULL), nr_pages);
        return -EINVAL;
    }

    if (va != svm_align_down(va, KA_MM_PAGE_SIZE)) {
        svm_info("Va not align with PAGE_SIZE. (va=0%llx; size=%lu)\n", va, KA_MM_PAGE_SIZE * nr_pages);
        return -EINVAL;
    }

    ret = svm_get_user_pages(pid, va, nr_pages, pages, is_remap_addr);
    if (ret != 0) {
        svm_err("Get user pages failed. (ret=%d; pid=%d; va=0%llx; size=%llu; is_pfn_map=%u)\n",
            ret, pid, va, KA_MM_PAGE_SIZE * (u64)nr_pages, (u32)*is_remap_addr);
    }

    return ret;
}
KA_EXPORT_SYMBOL_GPL(hal_kernel_svm_get_user_pages);

void hal_kernel_svm_put_user_pages(void **pages, u32 nr_pages, bool is_remap_addr)
{
    if (pages == NULL) {
        return;
    }

    if (is_remap_addr) {
        svm_unpin_svm_range_uva_npages((ka_page_t **)pages, nr_pages, nr_pages);
    } else {
        svm_unpin_user_npages((ka_page_t **)pages, nr_pages, nr_pages);
    }
}
KA_EXPORT_SYMBOL_GPL(hal_kernel_svm_put_user_pages);
#endif
