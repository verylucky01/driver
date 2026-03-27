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

#ifndef FRAMEWORK_CMD_H
#define FRAMEWORK_CMD_H

#include "svm_pub.h"

void svm_register_ioctl_cmd_handle(int nr, int (*fn)(u32 udevid, u32 cmd, unsigned long arg));

/* para is kernel space addr */
void svm_register_ioctl_cmd_pre_handle(int nr, int (*fn)(u32 udevid, u32 cmd, void *para));
void svm_register_ioctl_cmd_post_handle(int nr, int (*fn)(u32 udevid, u32 cmd, void *para));
void svm_register_ioctl_cmd_pre_cancle_handle(int nr, void (*fn)(u32 udevid, u32 cmd, void *para));

int svm_call_ioctl_pre_handler(u32 udevid, u32 cmd, void *para);
int svm_call_ioctl_post_handler(u32 udevid, u32 cmd, void *para);
void svm_call_ioctl_pre_cancle_handler(u32 udevid, u32 cmd, void *para);

#endif
