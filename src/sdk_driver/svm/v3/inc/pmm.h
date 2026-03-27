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
#ifndef PMM_H
#define PMM_H

#include <linux/types.h>

/*
* pmm: private memory management
* Record the address mapped by current process.
*/

#define PMM_SEG_WITH_PA     (1U << 0U)

int pmm_add_seg(u32 udevid, u64 va, u64 size, u32 flag);
int pmm_del_seg(u32 udevid, u64 va, u64 size, u32 flag);

/*
* Traverse all address segments that may be mapped.
* Attention: 
* 1. Can't add or del seg in func.
* 2. Might sleep.
*/
int pmm_for_each_seg(u32 udevid, int tgid, int (*func)(void *priv, u64 va, u64 size), void *priv);

#endif
