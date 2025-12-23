/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef DMS_SOC_CPU_UTILIZATION_H
#define DMS_SOC_CPU_UTILIZATION_H

#ifdef STATIC_SKIP
#define STATIC
#else
#define STATIC static
#endif

#include "dms/dms_devdrv_info_comm.h"

int dms_get_aicpu_info(unsigned int dev_id, struct dmanage_aicpu_info_stru *aicpu_info);

int dms_get_ctlcpu_utilization(unsigned int dev_id, unsigned int *utilization);

#endif
