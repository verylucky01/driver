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
#include "ka_compiler_pub.h"
#include "ka_sched_pub.h"

#include "kernel_version_adapt.h"

#include "pbl_ka_mem_query.h"
#include "svm_kernel_interface.h"
#include "svm_kern_log.h"
#include "svm_pgtable.h"
#include "svm_mm.h"
#include "svm_gup.h"

static void svm_unpin_svm_npages(ka_page_t **pages, u64 page_num, u64 unpin_num)
{
    unsigned long stamp;
    u64 i;

    if ((unpin_num > page_num) || (pages == NULL)) {
        return;
    }

    stamp = ka_jiffies;
    for (i = 0; i < unpin_num; i++) {
        if (pages[i] != NULL) {
            ka_mm_put_page(pages[i]);
            pages[i] = NULL;
        }
        ka_try_cond_resched(&stamp);
    }
}

void svm_unpin_user_npages(ka_page_t **pages, u64 page_num, u64 unpin_num)
{
    unsigned long stamp;
    u64 i;

    if (unpin_num > page_num || pages == NULL) {
        return;
    }

    stamp = ka_jiffies;
    for (i = 0; i < unpin_num; i++) {
        ka_mm_unpin_user_page(pages[i]);
        ka_try_cond_resched(&stamp);
    }
}

#define SVM_PIN_512_PAGE_NUM    512ull
int svm_pin_user_npages_fast(u64 va, u64 total_num, bool write, ka_page_t **pages)
{
    u64 got_num, remained_num, tmp_va;
    int tmp_num, expected_num;
    unsigned long stamp;

    stamp = ka_jiffies;
    for (got_num = 0; got_num < total_num;) {
        tmp_va = va + got_num * KA_MM_PAGE_SIZE;
        remained_num = total_num - got_num;
        expected_num = (remained_num > SVM_PIN_512_PAGE_NUM) ? SVM_PIN_512_PAGE_NUM : (int)remained_num;
        tmp_num = ka_mm_pin_user_pages_fast(tmp_va, expected_num, write ? KA_FOLL_WRITE : 0, &pages[got_num]);
        got_num += (tmp_num > 0) ? (u32)tmp_num : 0;
        if (tmp_num != expected_num) {
            svm_err("Get_user_pages_fast fail. (va=0x%llx; expected_page_num=%d; real_got_page_num=%d)\n",
                tmp_va, expected_num, tmp_num);
            goto page_err;
        }
        ka_try_cond_resched(&stamp);
    }

    return 0;

page_err:
    svm_unpin_user_npages(pages, total_num, got_num);

    return -EINVAL;
}

static long _svm_pin_user_npages_remote(ka_task_struct_t *tsk, ka_mm_struct_t *mm,
    u64 va, u32 num, ka_page_t **pages)
{
    long got_num;
    ka_task_down_read(get_mmap_sem(mm));
    got_num = ka_mm_get_user_pages_remote(tsk, mm, va, KA_FOLL_WRITE, num, pages);
    ka_task_up_read(get_mmap_sem(mm));

    return got_num;
}

int svm_pin_user_npages_remote(ka_task_struct_t *tsk, ka_mm_struct_t *mm,
    u64 va, u32 num, ka_page_t **pages)
{
    long got_num;

    got_num = _svm_pin_user_npages_remote(tsk, mm, va, num, pages);
    if (got_num != num) {
        svm_unpin_user_npages(pages, num, (u64)got_num);
        return -EFAULT;
    }
    return 0;
}

#ifndef EMU_ST /* Simulation ST ignore pte */
struct svm_pgwalk_data_of_pin_npages {
    bool check_local;
    u64 got_num;
    u64 page_num;
    ka_page_t **pages;
};

static int svm_pte_entry_of_pin_npages(ka_pte_t *pte, u64 addr, u64 next,
    enum ka_pte_level level, struct ka_pgwalk *walk)
{
    struct svm_pgwalk_data_of_pin_npages *data = (struct svm_pgwalk_data_of_pin_npages *)walk->priv;
    ka_page_t *page = NULL;
    u64 pfn, pa, page_size, i;
    u64 pg_num;

    page_size = ka_pte_level_to_page_size(level);
    if (ka_unlikely(page_size == 0ULL)) {
        svm_unpin_svm_npages(data->pages, data->page_num, data->got_num);
        return -EFAULT;
    }

    if (svm_pte_to_pfn(pte, &pfn) != 0) {
        svm_unpin_svm_npages(data->pages, data->page_num, data->got_num);
        return -EFAULT;
    }

    pa = (unsigned long)KA_MM_PFN_PHYS(pfn);
    pa += (addr - ka_base_round_down(addr, page_size));

    if (data->check_local && !svm_pa_is_local_mem(KA_MM_PFN_PHYS(pfn))) {
        svm_err("Isn't local mem. (addr=0x%llx)\n", addr);
        svm_unpin_svm_npages(data->pages, data->page_num, data->got_num);
        return -EFAULT;
    }

    page = svm_pa_to_page(pa);
    pg_num = (ka_base_round_up(next, KA_MM_PAGE_SIZE) - ka_base_round_down(addr, KA_MM_PAGE_SIZE)) / KA_MM_PAGE_SIZE;

    for (i = 0; (i < pg_num) && (data->got_num < data->page_num);
        i++, page++, data->got_num++) {
        ka_mm_get_page(page);
        data->pages[data->got_num] = page;
    }

    return 0;
}

static int svm_pte_hole_of_pin_npages(u64 addr, u64 next,
    enum ka_pte_level level, struct ka_pgwalk *walk)
{
    struct svm_pgwalk_data_of_pin_npages *data = (struct svm_pgwalk_data_of_pin_npages *)walk->priv;

    svm_err("Va to page failed. (addr=0x%llx; next=0x%llx; level=%u)\n", addr, next, level);

    svm_unpin_svm_npages(data->pages, data->page_num, data->got_num);
    return -EFAULT;
}

int svm_pin_svm_npages(ka_vm_area_struct_t *vma, u64 va, u64 page_num, bool check_local,
    ka_page_t **pages)
{
    struct svm_pgwalk_data_of_pin_npages data = {0};
    struct ka_pgwalk_ops ops = {NULL};
    u64 start = va;
    u64 end = va + (page_num * KA_MM_PAGE_SIZE);

    data.check_local = check_local;
    data.page_num = page_num;
    data.pages = pages;
    ops.pte_hole = svm_pte_hole_of_pin_npages;
    ops.pte_entry = svm_pte_entry_of_pin_npages;

    return ka_walk_page_range(vma, start, end, &ops, (void *)&data);
}
#endif

int svm_pin_uva_npages(u64 va, u64 page_num, u32 flag, ka_page_t **pages, bool *is_pfn_map)
{
    ka_vm_area_struct_t *vma = NULL;
    bool is_write = ((flag & SVM_GUP_FLAG_ACCESS_WRITE) != 0);
    bool check_local = ((flag & SVM_GUP_FLAG_CHECK_PA_LOCAL) != 0);
    int ret;

    ka_task_down_read(get_mmap_sem(ka_task_get_current_mm()));
    vma = ka_mm_find_vma(ka_task_get_current_mm(), va);
    if (vma == NULL) {
        svm_err("Invalid addr. (va=0x%llx)\n", va);
        ka_task_up_read(get_mmap_sem(ka_task_get_current_mm()));
        return -EINVAL;
    }

    if ((ka_mm_get_vm_flags(vma) & KA_VM_PFNMAP) != 0) {
        *is_pfn_map = true;
        ret = svm_pin_svm_npages(vma, va, page_num, check_local, pages);
        ka_task_up_read(get_mmap_sem(ka_task_get_current_mm()));
    } else {
        *is_pfn_map = false;
        ka_task_up_read(get_mmap_sem(ka_task_get_current_mm()));
        ret = svm_pin_user_npages_fast(va, page_num, is_write, pages);
    }

    return ret;
}

void svm_unpin_uva_npages(bool is_pfn_map, ka_page_t **pages, u64 page_num, u64 unpin_num)
{
    if (is_pfn_map) {
        svm_unpin_svm_npages(pages, page_num, unpin_num);
    } else {
        svm_unpin_user_npages(pages, page_num, unpin_num);
    }
}

int svm_pin_svm_range_uva_npages(int tgid, u64 va, bool is_write, ka_page_t **pages, u64 page_num)
{
    ka_vm_area_struct_t *vma = NULL;
    ka_mm_struct_t *mm = NULL;
    int ret;

    mm = svm_mm_get(tgid, false);
    if (mm == NULL) {
        return -ESRCH;
    }

    vma = ka_mm_find_vma(mm, va);
    if (vma == NULL) {
        svm_err("Invalid addr. (va=0x%llx)\n", va);
        svm_mm_put(mm, false);
        return -EINVAL;
    }

    ret = svm_pin_svm_npages(vma, va, page_num, true, pages);
    if (ret != 0) {
        svm_err("Svm_pin_svm_npages failed. (tgid=%d; va=0x%llx; page_num=%llu)\n", tgid, va, page_num);
    }

    svm_mm_put(mm, false);
    return ret;
}

void svm_unpin_svm_range_uva_npages(ka_page_t **pages, u64 page_num, u64 unpin_num)
{
    svm_unpin_svm_npages(pages, page_num, unpin_num);
}

/* For queue */
int devmm_get_pages_list(ka_mm_struct_t *mm, u64 va, u64 num, ka_page_t **pages)
{
    ka_vm_area_struct_t *vma = NULL;
    int ret;

    if ((mm == NULL) || (pages == NULL)) {
        svm_err("Invalid para.\n");
        return -EINVAL;
    }

    ka_task_down_read(get_mmap_sem(mm));
    vma = ka_mm_find_vma(mm, va);
    if (vma == NULL) {
        ka_task_up_read(get_mmap_sem(mm));
        svm_err("Invalid addr. (va=0x%llx)\n", va);
        return -EINVAL;
    }

    ret = svm_pin_svm_npages(vma, va, num, true, pages);
    ka_task_up_read(get_mmap_sem(mm));
    return ret;
}
KA_EXPORT_SYMBOL_GPL(devmm_get_pages_list);
