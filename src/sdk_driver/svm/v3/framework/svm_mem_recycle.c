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
#include "framework_vma.h"
#include "svm_mem_recycle.h"

static u64 (*svm_mem_recycle_func)(ka_vm_area_struct_t *vma, int tgid) = NULL;

void svm_mem_recycle_register(u64 (*func)(ka_vm_area_struct_t *vma, int tgid))
{
    svm_mem_recycle_func = func;
}

u64 svm_mem_recycle(ka_vm_area_struct_t *vma, int tgid)
{
    if (svm_mem_recycle_func != NULL) {
        return svm_mem_recycle_func(vma, tgid);
    }
    return 0ULL;
}
