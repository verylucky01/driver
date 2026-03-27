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

#ifndef DEVDRV_DEVICE_STATUS_H
#define DEVDRV_DEVICE_STATUS_H

#include "ka_fs_pub.h"

void devdrv_manager_device_status_init(u32 dev_id);
int devdrv_manager_get_device_status(ka_file_t *filep, unsigned int cmd, unsigned long arg);
int devdrv_manager_get_device_health_status(ka_file_t *filep, unsigned int cmd, unsigned long arg);
int devdrv_manager_get_device_startup_status(ka_file_t *filep, unsigned int cmd, unsigned long arg);

#endif