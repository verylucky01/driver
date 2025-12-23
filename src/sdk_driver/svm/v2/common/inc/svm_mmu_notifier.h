/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
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

#include "devmm_proc_info.h"

int devmm_mmu_notifier_register(struct devmm_svm_process *svm_proc);
void devmm_svm_mmu_notifier_unreg(struct devmm_svm_process *svm_proc);
void devmm_mmu_notifier_unregister_no_release(struct devmm_svm_process *svm_proc);
bool devmm_mem_is_in_vma_range(u64 start, u64 end);

#endif /* __SVM_MMU_NOTIFIER_H__ */
