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

#ifndef UBMM_H
#define UBMM_H

#include <linux/types.h>

/*
    ubmm: unify bus memory map
    use a unique tid which configured by manager to map a user va to uba for remote access with ub memory decoder
*/
int ubmm_do_map(u32 udevid, int tgid, u64 va, u64 size, u64 *uba);
int ubmm_do_unmap(u32 udevid, int tgid, u64 va, u64 size);
int ubmm_query_uba_base(u32 udevid, u64 *uba_base);

#endif

