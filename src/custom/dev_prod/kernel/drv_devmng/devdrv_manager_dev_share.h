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

#ifndef __DEVDRV_MANAGER_DEV_SHARE_H__
#define __DEVDRV_MANAGER_DEV_SHARE_H__

#define DEVICE_UNSHARE 0
#define DEVICE_SHARE   1

#ifdef CFG_FEATURE_DEVICE_SHARE
int set_device_share_flag(unsigned int device_id, unsigned value);
#else
void set_device_share_flag(unsigned int device_id, unsigned value);
#endif
int get_device_share_flag(unsigned int device_id);
int devdrv_manager_config_device_share(struct file *filep, unsigned int cmd, unsigned long arg);

#endif
