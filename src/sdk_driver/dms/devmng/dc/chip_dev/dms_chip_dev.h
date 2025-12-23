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
#ifndef __DMS_CHIP_DEV_H__
#define __DMS_CHIP_DEV_H__

#include "urd_define.h"
#include "urd_feature.h"
#include "urd_acc_ctrl.h"

#define DMS_CHIP_DEV_CMD_NAME "DMS_CHIP_DEV"

int dms_ioctl_get_chip_count(void *feature, char *in, u32 in_len, char *out, u32 out_len);
int dms_ioctl_get_chip_list(void *feature, char *in, u32 in_len, char *out, u32 out_len);
int dms_ioctl_get_device_from_chip(void *feature, char *in, u32 in_len, char *out, u32 out_len);
int dms_ioctl_get_chip_from_device(void *feature, char *in, u32 in_len, char *out, u32 out_len);

int dms_chip_dev_init(void);
void dms_chip_dev_uninit(void);

#endif