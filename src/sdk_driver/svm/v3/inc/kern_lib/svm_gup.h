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
#ifndef SVM_GUP_H
#define SVM_GUP_H

#include "ka_memory_pub.h"
#include "ka_common_pub.h"

#define SVM_GUP_FLAG_ACCESS_WRITE   (1U << 0U)
#define SVM_GUP_FLAG_CHECK_PA_LOCAL (1U << 1U)

int svm_pin_user_npages_fast(u64 va, u64 total_num, bool write, ka_page_t **pages);
int svm_pin_user_npages_remote(ka_task_struct_t *tsk, ka_mm_struct_t *mm,
    u64 va, u32 num, ka_page_t **pages);
void svm_unpin_user_npages(ka_page_t **pages, u64 page_num, u64 unpin_num);
int svm_pin_svm_npages(ka_vm_area_struct_t *vma, u64 va, u64 page_num, bool check_local, ka_page_t **pages);

int svm_pin_uva_npages(u64 va, u64 page_num, u32 flag, ka_page_t **pages, bool *is_pfn_map);
void svm_unpin_uva_npages(bool is_pfn_map, ka_page_t **pages, u64 page_num, u64 unpin_num);
int svm_pin_svm_range_uva_npages(int tgid, u64 va, bool is_write, ka_page_t **pages, u64 page_num);
void svm_unpin_svm_range_uva_npages(ka_page_t **pages, u64 page_num, u64 unpin_num);

#endif
