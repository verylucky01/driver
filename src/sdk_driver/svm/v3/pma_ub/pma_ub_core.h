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

#ifndef PMA_UB_CORE_H
#define PMA_UB_CORE_H

#include <linux/types.h>

int pma_ub_register_seg(u32 udevid, int tgid, u64 start, u64 size, u32 token_id);
int pma_ub_unregister_seg(u32 udevid, int tgid, u64 start, u64 size);

int pma_ub_get_register_seg_info(u32 udevid, int tgid, u64 va, u64 *start, u64 *size, u32 *token_id);

int pma_ub_acquire_seg(u32 udevid, int tgid, u64 va, u64 size,
    int (*invalidate)(u64 invalidate_tag), u64 invalidate_tag, u32 *token_id);
int pma_ub_release_seg(u32 udevid, int tgid, u64 va, u64 size);

#endif

