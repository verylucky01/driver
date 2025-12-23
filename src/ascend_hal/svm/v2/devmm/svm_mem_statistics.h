/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef SVM_MEM_STATISTICS_H
#define SVM_MEM_STATISTICS_H

#ifndef EMU_ST
#include "dmc_user_interface.h"
#else
#include "ut_log.h"
#endif
#include "drv_type.h"
#include "svm_ioctl.h"

#ifdef EMU_ST
#define THREAD __thread
#else
#define THREAD
#endif

#define MEM_STATS_DEV_TS_DDR_TYPE           2

uint64_t svm_get_mem_stats_va(uint32_t devid);
void svm_init_mem_stats_mng(uint32_t devid);
void svm_uninit_mem_stats_mng(uint32_t devid);

void svm_mem_stats_show_host(void);
void svm_mem_stats_show_device(uint32_t devid);
void svm_mem_stats_show_all_svm_mem(uint32_t devid);

void svm_module_alloced_size_inc(struct svm_mem_stats_type *type, uint32_t devid, uint32_t module_id, uint64_t size);
void svm_module_alloced_size_dec(struct svm_mem_stats_type *type, uint32_t devid, uint32_t module_id, uint64_t size);
void svm_mapped_size_inc(struct svm_mem_stats_type *type, uint32_t devid, uint64_t size);
void svm_mapped_size_dec(struct svm_mem_stats_type *type, uint32_t devid, uint64_t size);

#endif /* SVM_MEM_STATISTICS_H */

