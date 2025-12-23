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
#ifndef RMO_FOPS_H
#define RMO_FOPS_H

#include <linux/types.h>
#include <linux/ioctl.h>

#include "rmo_ioctl.h"
#include "rmo_kern_log.h"

#define RMO_VMA_MAP_MAGIC_VERIFY 0x5a5aa5a55a5aa5a5

void rmo_register_ioctl_cmd_func(int nr, int (*fn)(u32 cmd, unsigned long arg));

int rmo_fops_init(void);
void rmo_fops_uninit(void);

#endif
