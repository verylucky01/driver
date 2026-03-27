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

#ifndef SMM_UB_MEM_H
#define SMM_UB_MEM_H

#include <linux/types.h>

#define UB_MEM_ID_BIT_START 48
#define UB_MEM_VA_VALID_BIT 63

static inline int svm_ub_mem_id_and_offset_to_va(u32 mem_id, u64 offset, u64 *va)
{
    if ((offset >= (0x1ULL << UB_MEM_ID_BIT_START)) ||
        (mem_id >= (0x1ULL << (UB_MEM_VA_VALID_BIT - UB_MEM_ID_BIT_START)))) {
        return -EINVAL;
    }

    /* Considering that when the offset is 0 and memid is 0, the calculated va is 0,
       which is unfriendly for later, so a valid bit is added. */
    *va = ((u64)mem_id << UB_MEM_ID_BIT_START) | offset | (0x1ULL << UB_MEM_VA_VALID_BIT);
    return 0;
}

static inline void svm_ub_va_to_mem_id_and_offset(u64 va, u32 *mem_id, u64 *offset)
{
    u64 tmp_va = va & ((0x1ULL << UB_MEM_VA_VALID_BIT) - 1);
    *mem_id = (u32)(tmp_va >> UB_MEM_ID_BIT_START);
    *offset = tmp_va & ((0x1ULL << UB_MEM_ID_BIT_START) - 1);
}

#endif

