/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef SVM_ATOMIC_H
#define SVM_ATOMIC_H

#include <stdbool.h>

#include "svm_pub.h"

static inline void svm_atomic_dec(u32 *val)
{
    __sync_fetch_and_sub(val, 1);
}

static inline void svm_atomic_inc(u32 *val)
{
    __sync_fetch_and_add(val, 1);
}

static inline u32 svm_atomic_read(u32 *val)
{
    u32 cur, next;

    cur = *val;
    do {
        next = cur;
        cur = __sync_val_compare_and_swap((volatile u32 *)val, next, next);
    } while (cur != next);

    return cur;
}

static inline void svm_atomic64_inc(u64 *val)
{
    (void)__sync_add_and_fetch(val, 1ULL);
}

static inline void svm_atomic64_dec(u64 *val)
{
    (void)__sync_sub_and_fetch(val, 1ULL);
}

static inline u64 svm_atomic64_add(u64 *val, u64 i)
{
    return (u64)__sync_add_and_fetch(val, i);
}

static inline u64 svm_atomic64_sub(u64 *val, u64 i)
{
    return (u64)__sync_sub_and_fetch(val, i);
}

static inline bool svm_atomic64_compare_and_swap(u64 *val, u64 old, u64 new)
{
    return __sync_bool_compare_and_swap(val, old, new);
}
#endif
