/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef DPA_APM_H
#define DPA_APM_H

#include "ascend_hal_define.h"

typedef enum tagProcMemType {
    PROC_MEM_TYPE_ALL = 0,
    PROC_MEM_TYPE_VMRSS,
    PROC_MEM_TYPE_SP,
    PROC_MEM_MAX
} processMemType_t;

drvError_t halQuerySlaveProcMeminfo(int master_pid, uint32_t devid, processType_t processType,
    processMemType_t memType, unsigned long long *size);

int halQueryMasterPidByDeviceSlave(unsigned int devid, int slave_pid, unsigned int *master_pid, unsigned int *proc_type);
#endif
