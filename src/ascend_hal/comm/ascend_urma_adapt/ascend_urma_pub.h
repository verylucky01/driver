/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef ASCEND_URMA_PUB_H
#define ASCEND_URMA_PUB_H

#ifndef u64
typedef unsigned long long u64;
#endif

#ifndef u32
typedef unsigned int u32;
#endif

#ifndef u16
typedef unsigned short u16;
#endif

#ifndef u8
typedef unsigned char u8;
#endif

static inline u64 ascend_urma_adapt_align_up(u64 value, u64 align)
{
    if (align == 0) {
        return value;
    }

    return (value + (align - 1ULL)) & ~(align - 1ULL);
}

static inline u64 ascend_urma_adapt_align_down(u64 value, u64 align)
{
    if (align == 0) {
        return value;
    }

    return value & ~(align - 1ULL);
}

#endif
