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

#ifndef __DMS_NOTIFIER_H__
#define __DMS_NOTIFIER_H__
#include <linux/notifier.h>
#include <linux/types.h>

/* dms notifier chain */
typedef enum {
    DMS_DEVICE_NOTIFIER_MIN = 0,
    DMS_DEVICE_REBOOT = 0x0003,
    DMS_DRIVER_REMOVE = 0x0004,
    DMS_DEVICE_SUSPEND = 0x0005,
    DMS_DEVICE_RESUME = 0x0006,
    DMS_DEVICE_PRE_HOTRESET = 0x0007,
    DMS_DEVICE_HOTRESET_CANCEL = 0x0008,
    DMS_H2D_EVENT = 0x0009,

    DMS_DEVICE_UP0 = 0x0010,
    DMS_DEVICE_UP1 = 0x0011,
    DMS_DEVICE_UP2 = 0x0012,
    DMS_DEVICE_UP3 = 0x0013,

    DMS_DEVICE_DOWN0 = 0x00020,
    DMS_DEVICE_DOWN1 = 0x00021,
    DMS_DEVICE_DOWN2 = 0x00022,
    DMS_DEVICE_DOWN3 = 0x00023,

    DMS_DEVICE_SET_AICPU_NUM = 0x000FE,
    DMS_DEVICE_NOTIFIER_MAX
} DMS_DEVICE_NOTIFIER_ENUM;

int dms_notifyer_call(u64 mode, void *v);
int dms_register_notifier(struct notifier_block *nb);
int dms_unregister_notifier(struct notifier_block *nb);

#endif
