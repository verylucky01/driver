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
#ifndef SVM_MMAP_FOPS_H
#define SVM_MMAP_FOPS_H
#include "ka_memory_pub.h"

int svm_mmap_fops_init(void);
void svm_mmap_fops_uninit(void);

bool svm_is_svm_vma(ka_vm_area_struct_t *vma);

#endif
