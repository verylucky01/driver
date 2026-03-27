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

#ifndef TOPIC_SCHED_COMMON_H
#define TOPIC_SCHED_COMMON_H

#include "ascend_hal_define.h"
#include "esched.h"

#define ESCHED_DRV_REASSIGN_VFID(vf_id)  (((vf_id) == 0) ? (vf_id) : ((vf_id) - 1))

STATIC inline void esched_drv_reg_wr(void __ka_mm_iomem *io_base, u32 offset, u32 val)
{
    ka_mm_writel(val, io_base + offset);
    sched_debug("Show details. (offset=%x; data=%x)\n", offset, val);
}

STATIC inline void esched_drv_reg_rd(const void __ka_mm_iomem *io_base, u32 offset, u32 *val)
{
    *val = ka_mm_readl(io_base + offset);

    if (*val != 0) {
        sched_debug("Show details. (offset=%x; data=%x)\n", offset, *val);
    }
}

STATIC inline void esched_drv_reg_mem_wr(const void __ka_mm_iomem *io_base, u32 offset, void* val, u32 size)
{
    sched_debug("Memory write. (offset=%x; size=%x)\n", offset, size);
#ifdef CFG_PLATFORM_ESL
    ka_mm_memcpy_toio((void *)(io_base + offset), val, size);
#else
    *((u64 *)(uintptr_t)(io_base + offset)) = *((u64 *)val); // atomic write
#endif
}

#endif
