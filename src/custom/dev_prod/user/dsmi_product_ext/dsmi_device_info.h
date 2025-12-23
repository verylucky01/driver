/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef _DSMI_DEVICE_INFO_H_
#define _DSMI_DEVICE_INFO_H_
#include "dsmi_inner_interface.h"

#define DMS_SUBCMD_GET_WORK_MODE 1
#define DMS_SUBCMD_GET_VRD_INFO 2
#define DMS_SUBCMD_GET_MAINBOARD_ID 5

#define VRD_STATUS_MAX_NUM  8
typedef struct vrd_status_info {
    uint8_t valid;
    uint8_t vrdType;            /* vrd电源类型 */
    uint16_t slaveAddr;         /* vrd电源地址 */
    uint16_t version;           /* 当前固件版本 */
    uint8_t upgradeRemainCnt;   /* 剩余升级次数 */
    uint8_t reserve[29];        /* 预留使用 */
} VrdStatusInfo;

typedef struct device_vrd_status_info {
    uint32_t num;               /* vrd电源数量 */
    VrdStatusInfo vrdInfo[VRD_STATUS_MAX_NUM];
} DeviceVrdStatusInfo;

int dms_get_work_mode(unsigned int *work_mode);
int dms_get_mainboard_id(unsigned int device_id, unsigned int *mainboard_id);
#endif