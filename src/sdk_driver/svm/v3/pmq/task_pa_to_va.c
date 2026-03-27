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
#include "ka_sched_pub.h"

#include "svm_kern_log.h"
#include "svm_pgtable.h"
#include "svm_mm.h"
#include "pmq.h"

static int _svm_pmq_query_va(ka_vm_area_struct_t *vma, u64 pa, u64 start_va, u64 end_va, u64 *matched_va)
{
    u64 va, tmp_pa, page_size, va_offset, pa_offset;
    unsigned long stamp = (unsigned long)ka_jiffies;

    for (va = start_va; va < end_va; va += (page_size - va_offset)) {
        ka_try_cond_resched(&stamp);
        if (svm_va_to_pa(vma, va, &tmp_pa, &page_size) != 0) {
            page_size = KA_MM_PAGE_SIZE;
            va_offset = va - ka_base_round_down(va, page_size);
            continue;
        }

        va_offset = va - ka_base_round_down(va, page_size); /* Resolve start_va misalignment scene. */
        if ((pa >= tmp_pa) && (pa < (tmp_pa + (page_size - va_offset)))) {
            pa_offset = pa - tmp_pa;
            *matched_va = va + pa_offset;
            return 0;
        }
    }

    return -ESRCH; /* Only return this err code when va not found */
}

int svm_pmq_query_va(int tgid, u64 pa, u64 start_va, u64 end_va, u64 *matched_va)
{
    ka_vm_area_struct_t *vma = NULL;
    ka_mm_struct_t *mm = NULL;
    int ret;

    mm = svm_mm_get(tgid, false);
    if (mm == NULL) {
        return -ENOSYS;
    }

    vma = ka_mm_find_vma(mm, start_va);
    if (vma == NULL) {
        svm_mm_put(mm, false);
        return -EFAULT;
    }

    ret = _svm_pmq_query_va(vma, pa, start_va, end_va, matched_va);
    svm_mm_put(mm, false);
    return ret;
}
