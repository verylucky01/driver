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

#ifndef DPA_RMO_KERNEL_H
#define DPA_RMO_KERNEL_H
#include <linux/types.h>
#include "ascend_hal_define.h"

typedef int (*mem_sharing_func)(u32 devid, u64 addr, u64 len);
void rmo_mem_sharing_register(mem_sharing_func handle, accessMember_t accessor);
void rmo_mem_sharing_unregister(accessMember_t accessor);
#endif
