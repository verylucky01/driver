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

#ifndef FRAMEWORK_VMA_H
#define FRAMEWORK_VMA_H

#include "ka_memory_pub.h"

#include "svm_pub.h"
#include "va_mng.h"

int svm_check_vma(ka_vm_area_struct_t *vma, u64 va, u64 size);
int svm_get_current_task_va_type(u64 va, u64 size, int *va_type);

/* func will be called within mmap lock */
void svm_mem_recycle_register(u64 (*func)(ka_vm_area_struct_t *vma, int tgid));

#define VMA_STATUS_IDLE 0
#define VMA_STATUS_NORMAL_OP 1
void svm_set_vma_status(ka_vm_area_struct_t *vma, int status);

#define SVM_FAULT_ERROR KA_MM_FAULT_ERROR

#define SVM_FAULT_RETRY VM_FAULT_RETRY
#define SVM_FAULT_OK VM_FAULT_NOPAGE

static inline bool svm_is_write_fault(ka_vm_fault_struct_t *vmf)
{
    return ((vmf->flags & FAULT_FLAG_WRITE) != 0);
}

typedef ka_vm_fault_t (*svm_fault_handle)(ka_vm_area_struct_t *vma, ka_vm_fault_struct_t *vmf, int huge_fault_flag);
void svm_register_vma_fault_handle(svm_fault_handle handle);

#endif
