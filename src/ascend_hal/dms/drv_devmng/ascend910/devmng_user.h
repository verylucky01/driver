/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef DEVDRV_MANAGER_H
#define DEVDRV_MANAGER_H

#define CAP_BIT_CLEAR 0

drvError_t drv_device_memory_dump(uint32_t devId, uint64_t bbox_addr_offset, uint32_t size, void *buffer);
drvError_t drv_vmcore_dump(uint32_t devId, uint64_t bbox_addr_offset, uint32_t size, void *buffer);
drvError_t drv_reg_ddr_read(uint32_t devId, uint64_t reg_addr_offset, uint32_t size, void *buffer);
drvError_t drv_ts_log_dump(uint32_t devid, uint64_t bbox_addr_offset, uint32_t size, void *buffer);
int drv_dev_log_dump(uint32_t devid, uint64_t bbox_addr_offset, uint32_t size, void *buffer, uint32_t log_type);
#endif
