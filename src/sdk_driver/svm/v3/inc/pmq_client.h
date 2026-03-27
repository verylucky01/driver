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

#ifndef PMQ_CLIENT_H
#define PMQ_CLIENT_H

#include "svm_pub.h"
#include "svm_addr_desc.h"

int svm_pmq_client_pa_query(u32 local_udevid, struct svm_global_va *src_info,
    struct svm_pa_seg pa_seg[], u64 *seg_num);
int svm_pmq_client_host_bar_query(u32 local_udevid, struct svm_global_va *src_info,
    struct svm_pa_seg pa_seg[], u64 *seg_num);
int hal_kernel_svm_dev_va_to_dma_addr(int pid, u32 udevid, u64 va, u64 *dma_addr);

int pmq_client_host_init_dev(u32 udevid);
void pmq_client_host_uninit_dev(u32 udevid);
int pmq_client_host_init(void);
void pmq_client_host_uninit(void);
#endif

