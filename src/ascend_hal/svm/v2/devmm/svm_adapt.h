/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef DEV_SVM_ADAPT_H
#define DEV_SVM_ADAPT_H

#include <stdint.h>
#include "ascend_hal.h"
#include "svm_ioctl.h"

bool drv_mem_support_prof_sample(void);
bool drv_mem_support_alloc_cnt_stats(void);
void drv_mem_current_alloc_size_stats(struct svm_mem_stats *mem_stats, uint32_t module_id_tmp, uint64_t size);

#endif
