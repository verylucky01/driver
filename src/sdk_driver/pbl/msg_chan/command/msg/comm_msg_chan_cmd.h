/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025. All rights reserved.
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

#ifndef __COMM_MSG_CHAN_CMD_H__
#define __COMM_MSG_CHAN_CMD_H__

/* host/device common msg */
enum devdrv_common_msg_type {
    DEVDRV_COMMON_MSG_PCIVNIC = 0,
    DEVDRV_COMMON_MSG_SMMU,
    DEVDRV_COMMON_MSG_DEVMM,
    DEVDRV_COMMON_MSG_VMNG,
    DEVDRV_COMMON_MSG_PROFILE = 4,
    DEVDRV_COMMON_MSG_DEVDRV_MANAGER,
    DEVDRV_COMMON_MSG_DEVDRV_TSDRV,
    DEVDRV_COMMON_MSG_HDC,
    DEVDRV_COMMON_MSG_SYSFS,
    DEVDRV_COMMON_MSG_ESCHED,
    DEVDRV_COMMON_MSG_DP_PROC_MNG,
    DEVDRV_COMMON_MSG_TEST,
    DEVDRV_COMMON_MSG_UDIS,
    DEVDRV_COMMON_MSG_BBOX,
    DEVDRV_COMMON_MSG_TYPE_MAX
};

#endif