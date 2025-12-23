/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef _DSMI_DEVICE_PCIE_DOMAIN_H_
#define _DSMI_DEVICE_PCIE_DOMAIN_H_
#include "dsmi_inner_interface.h"
#include "ascend_hal_error.h"

#undef DEVDRV_MAX_DAVINCI_NUM
#define DEVDRV_MAX_DAVINCI_NUM  64
#define DMS_SUBCMD_GET_PCIE_ID_INFO_ALL 0

#ifdef CONFIG_LLT
static inline void printf_stub(char *format, ...) {
}
#define DEV_MON_ERR          printf_stub
#define DEV_MON_WARNING      printf_stub
#define DEV_MON_INFO         printf_stub
#define DEV_MON_DEBUG        printf_stub
#define DEV_MON_EVENT        printf_stub
#define DEV_MON_PRINT        printf_stub
#endif
struct dmanage_pcie_id_info_all {
    unsigned int venderid;    /* 厂商id */
    unsigned int subvenderid; /* 厂商子id */
    unsigned int deviceid;    /* 设备id */
    unsigned int subdeviceid; /* 设备子id */
    int domain;               /* pcie域 */
    unsigned int bus;         /* 总线号 */
    unsigned int device;      /* 设备物理号 */
    unsigned int fn;          /* 设备功能号 */
    unsigned int davinci_id;  /* device id */
    unsigned char reserve[28];
};

drvError_t drvDeviceGetPcieIdInfoAll(unsigned int devId, struct tag_pcie_idinfo_all *pcie_idinfo);

#endif
