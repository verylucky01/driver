/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
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

#ifndef _RES_DRV_CLOUD_V4_H_
#define _RES_DRV_CLOUD_V4_H_

#include "devdrv_pci.h"

#define DEVDRV_S2S_MAX_CHIP_NUM 8
#define DEVDRV_CLOUD_V4_SLOT_ID_MAX 8U
#define DEVDRV_CLOUD_V4_MODULE_ID_MAX 8U
#define UB_REMOTE_PORT_NUM 3U
#define UB_REMOTE_DEVICE_NUM 5U
#define UB_HW_INFO_RSV2_NUM 10U

int devdrv_cloud_v4_res_init(struct devdrv_pci_ctrl *pci_ctrl);

#endif
