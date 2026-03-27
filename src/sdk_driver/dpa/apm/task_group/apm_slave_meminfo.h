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
#ifndef APM_SLAVE_MEMINFO_H
#define APM_SLAVE_MEMINFO_H

#include "ascend_hal_define.h"
#include "apm_kernel_ioctl.h"

int apm_get_slave_meminfo(int slave_tgid, processMemType_t type, u64 *size);

#endif
