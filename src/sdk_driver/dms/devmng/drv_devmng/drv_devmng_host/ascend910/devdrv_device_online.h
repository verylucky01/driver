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

#ifndef DEVDRV_DEVICE_ONLINE_H
#define DEVDRV_DEVICE_ONLINE_H
#include "devdrv_manager_common.h"

void devdrv_manager_online_devid_update(u32 dev_id);
int devdrv_manager_online_get_devids(struct file *filep, unsigned int cmd, unsigned long arg);
u32 devdrv_manager_poll(struct file *filep, struct poll_table_struct *wait);
int devdrv_manager_online_kfifo_alloc(void);
void devdrv_manager_online_kfifo_free(void);
void devdrv_manager_online_del_devids(u32 dev_id);

#endif
