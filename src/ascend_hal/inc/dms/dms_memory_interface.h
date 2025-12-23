/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef __DMS_MEMORY_INTERFACE_H
#define __DMS_MEMORY_INTERFACE_H

#include "ascend_hal_error.h"

drvError_t dms_memory_get_ddr_bw_util_rate(unsigned int dev_id, unsigned int *rate);
drvError_t dms_memory_get_ecc_statistics(unsigned int dev_id, unsigned int dev_type, unsigned int error_type,
    unsigned int *value, unsigned int len);
drvError_t dms_memory_get_hbm_bw_util_rate(unsigned int dev_id, unsigned int *value);
drvError_t dms_memory_get_ddr_freq(unsigned int dev_id, unsigned int *frequency);
drvError_t dms_memory_get_hbm_temperature(unsigned int dev_id, unsigned int *temperature);
drvError_t dms_memory_get_hbm_ecc_syscnt(unsigned int dev_id, void *buf, unsigned int *size);
drvError_t dms_memory_get_hbm_freq(unsigned int dev_id, unsigned int *frequency);
drvError_t dms_memory_get_hbm_medium_capacity(unsigned int dev_id, unsigned long long  *total_size);

#endif
