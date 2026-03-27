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

#ifndef __SVM_KERNEL_INTERFACE_H__
#define __SVM_KERNEL_INTERFACE_H__

#ifdef __KERNEL__
#include <linux/types.h>
#include "ka_memory_pub.h"
#include "ka_list_pub.h"
#include "pbl_ka_mem_query.h"

struct devmm_pfn_owner_info {
    ka_list_head_t list;
    int dev_pid;
    u16 size_shift;
    u64 va;
};

/*
 * The caller needs to ensure the mutual exclusivity of the list and the memory release of the list node.
 * hal_kernel_svm_query_pfn_owner_info : head should be an empty list, otherwise the query fails.
 * hal_kernel_svm_clear_pfn_owner_info_list : head should be the value returned by hal_kernel_svm_query_pfn_owner_info.
 */
int hal_kernel_svm_query_pfn_owner_info(u32 devid, u64 pfn, ka_list_head_t *head);
void hal_kernel_svm_clear_pfn_owner_info_list(ka_list_head_t *head);

int devmm_get_pages_list(ka_mm_struct_t *mm, u64 va, u64 num, ka_page_t **pages);
int svm_get_pages_list(ka_mm_struct_t *mm, u64 va, u64 num, ka_page_t **pages);

/* pin normal mem which can be shared and op */
int svm_smp_pin_mem(u32 udevid, int tgid, u64 va, u64 size);
int svm_smp_unpin_mem(u32 udevid, int tgid, u64 va, u64 size);


/* pin device cp only mem */
int svm_smp_pin_dev_cp_only_mem(u32 udevid, int tgid, u64 va, u64 size);
int svm_smp_unpin_dev_cp_only_mem(u32 udevid, int tgid, u64 va, u64 size);

typedef struct ka_pa_wraper svm_pa_seg_wraper_t;
int svm_pmq_client_cp_pa_query(u32 udevid, u64 va, u64 size, svm_pa_seg_wraper_t pa_seg[], u64 *seg_num);
#endif

struct devmm_set_convert_len_para {
    unsigned long long total_convert_len;
};

struct devmm_get_convert_len_para {
    unsigned long long total_convert_len;
};

int halMemGet(unsigned long long addr, unsigned long long size);
int halMemPut(unsigned long long addr, unsigned long long size);

#endif