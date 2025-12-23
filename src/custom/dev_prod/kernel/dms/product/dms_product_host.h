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

#ifndef __DMS_PRODUCT_HOST_H__
#define __DMS_PRODUCT_HOST_H__

#include "comm_kernel_interface.h"

int devdrv_get_work_mode(void *feature, char *in, u32 in_len, char *out, u32 out_len);
#ifdef CFG_SOC_PLATFORM_CLOUD
extern int devdrv_manager_get_amp_smp_mode(u32 *amp_or_smp);
#endif

#endif