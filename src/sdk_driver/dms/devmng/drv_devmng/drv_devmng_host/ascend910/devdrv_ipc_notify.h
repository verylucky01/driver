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

#ifndef DEVDRV_IPC_NOTIFY_H
#define DEVDRV_IPC_NOTIFY_H

#include "devdrv_manager.h"

int devdrv_manager_ipc_notify_init(struct devdrv_manager_context *dev_manager_context);
void devdrv_manager_ipc_notify_uninit(struct devdrv_manager_context *dev_manager_context);
void devdrv_manager_resource_recycle(struct devdrv_manager_context *dev_manager_context);
#ifndef CFG_FEATURE_REFACTOR
int devdrv_manager_ipc_notify_ioctl(ka_file_t *filep, unsigned int cmd, unsigned long arg);
#endif

#endif /* DEVDRV_IPC_NOTIFY_H */
