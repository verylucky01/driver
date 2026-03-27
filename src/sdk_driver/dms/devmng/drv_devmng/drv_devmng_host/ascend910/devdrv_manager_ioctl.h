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

#ifndef DEVMNG_IOCTL_H
#define DEVMNG_IOCTL_H

#include <linux/fs.h>
#include <linux/types.h>

int devdrv_host_query_devpid(ka_file_t *filep, unsigned int cmd, unsigned long arg);
int devdrv_manager_ioctl_get_dev_resource_info(ka_file_t *filep, unsigned int cmd, unsigned long arg);
#ifdef CFG_FEATURE_CHIP_DIE
int devdrv_manager_ioctl_get_chip_count(ka_file_t *filep, unsigned int cmd, unsigned long arg);
int devdrv_manager_ioctl_get_chip_list(ka_file_t *filep, unsigned int cmd, unsigned long arg);
int devdrv_manager_ioctl_get_device_from_chip(ka_file_t *filep, unsigned int cmd, unsigned long arg);
int devdrv_manager_ioctl_get_chip_from_device(ka_file_t *filep, unsigned int cmd, unsigned long arg);
#endif
long devdrv_manager_ioctl(ka_file_t *filep, unsigned int cmd, unsigned long arg);
int devdrv_manager_trans_and_check_id(u32 logical_dev_id, u32 *physical_dev_id, u32 *vfid);

#endif /* DEVMNG_IOCTL_H */