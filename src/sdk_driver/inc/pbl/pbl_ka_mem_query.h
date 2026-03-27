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
#ifndef PBL_KA_MEM_QUERY_H
#define PBL_KA_MEM_QUERY_H

#include "ascend_kernel_hal.h"

struct ka_pa_wraper {
    u64 pa;
    u64 size;
};

struct ka_mem_attr {
    u64 addr;
    u64 size;
    bool cp_only_flag;
    bool raw_pa_flag; /* only return device pa if device addr, not convert to host bar addr */
};
struct svm_mem_query_ops {
    int (*get_svm_mem_pa)(u32 devid, int tgid, struct ka_mem_attr *mem, u64 *pa_num, struct ka_pa_wraper *pa_list);
    int (*put_svm_mem_pa)(u32 devid, int tgid, struct ka_mem_attr *mem, u64 pa_num, struct ka_pa_wraper *pa_list);
    u32 (*get_svm_mem_page_size)(u32 devid, int tgid, struct ka_mem_attr *mem);
};

/* might sleep */
int hal_kernel_get_mem_pa_list(u32 devid, int tgid, struct ka_mem_attr *mem, u64 *pa_num, struct ka_pa_wraper *pa_list);
int hal_kernel_put_mem_pa_list(u32 devid, int tgid, struct ka_mem_attr *mem, u64 pa_num, struct ka_pa_wraper *pa_list);
u32 hal_kernel_get_mem_page_size(u32 devid, int tgid, struct ka_mem_attr *mem);
int hal_kernel_register_mem_query_ops(struct svm_mem_query_ops *query_ops);
int hal_kernel_unregister_mem_query_ops(void);
int hal_kernel_get_vdev_hbm_info(u32 devid, u32 vfid, u32 huge_flag, u64 *free_size, u64 *total_size);

#endif
