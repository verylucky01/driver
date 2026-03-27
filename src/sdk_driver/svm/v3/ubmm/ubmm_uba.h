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

#ifndef UBMM_UBA_H
#define UBMM_UBA_H

#include <linux/types.h>

int ubmm_uba_pool_create(u64 uba_base, u64 uba_size);
void ubmm_uba_pool_destroy(void);
int ubmm_alloc_uba(u64 size, u64 *uba);
int ubmm_free_uba(u64 uba, u64 size);
int ubmm_get_uba_pool(u64 *uba_base, u64 *total_size, u64 *avail_size);

#endif

