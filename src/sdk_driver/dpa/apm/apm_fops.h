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
#ifndef APM_FOPS_H
#define APM_FOPS_H

#include "apm_kernel_ioctl.h"
#include "apm_kern_log.h"

void apm_register_ioctl_cmd_func(int nr, int (*fn)(u32 cmd, unsigned long arg));

int apm_fops_init(void);
void apm_fops_uninit(void);

#endif
