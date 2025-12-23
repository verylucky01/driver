/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef DEVDRV_PCIE_H
#define DEVDRV_PCIE_H

int drvGetDeviceDevIDByHostDevID(uint32_t host_dev_id, uint32_t *local_dev_id);

#ifdef CFG_FEATURE_SUPPORT_DEVMNG_BBOX
#ifdef CFG_FEATURE_IMU_DDR_READ
extern drvError_t drvPcieIMUDDRRead(uint32_t devId, uint32_t offset, uint8_t *value, uint32_t len);
#endif

drvError_t drv_pcie_sram_readead(uint32_t devId, uint32_t offset, uint8_t *value, uint32_t len);
drvError_t drv_pcie_ddr_readead(uint32_t devId, uint32_t offset, uint8_t *value, uint32_t len);
drvError_t drv_pcie_bbox_hdr_readread(uint32_t devId, uint32_t offset, uint8_t *value, uint32_t len);
drvError_t drv_reg_sram_readead(uint32_t devId, uint32_t offset, uint8_t *value, uint32_t len);
#endif
#endif
