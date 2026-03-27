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
#ifndef SVM_MMU_NOTIFIER_H
#define SVM_MMU_NOTIFIER_H

#include "ka_memory_pub.h"

#include "svm_pub.h"

struct svm_mmu_notifier_ctx {
    ka_mmu_notifier_t mn;
    ka_vm_area_struct_t *vma;
    int tgid;
    int status;
};

/* return -EINTR means interrupted by signals, should retry. */
int svm_mmu_notifier_register(ka_mm_struct_t *mm, ka_vm_area_struct_t *vma, struct svm_mmu_notifier_ctx **mn_ctx);
void svm_mmu_notifier_unregister(struct svm_mmu_notifier_ctx *mn_ctx);
#endif
