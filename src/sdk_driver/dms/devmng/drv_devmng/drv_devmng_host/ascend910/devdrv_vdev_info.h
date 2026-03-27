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

#ifndef DEVMNG_VDEV_INFO_H
#define DEVMNG_VDEV_INFO_H

#include "ka_common_pub.h"

int devdrv_manager_ioctl_set_vdevinfo(ka_file_t *filep, unsigned int cmd, unsigned long arg);
int devdrv_manager_ioctl_get_svm_vdevinfo(ka_file_t *filep, unsigned int cmd, unsigned long arg);
#ifdef CFG_FEATURE_VASCEND
int devdrv_manager_ioctl_set_vdevmode(ka_file_t *filep, unsigned int cmd, unsigned long arg);
int devdrv_manager_ioctl_get_vdevmode(ka_file_t *filep, unsigned int cmd, unsigned long arg);
#endif

#endif /* DEVMNG_VDEV_INFO_H */
