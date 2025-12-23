/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef UREF_H
#define UREF_H

#include <stdbool.h>

struct uref {
    int refcount;
};

static inline void uref_init(struct uref *uref)
{
    uref->refcount = 1;
    __sync_synchronize();
}

static inline void uref_get(struct uref *uref)
{
    (void)__sync_fetch_and_add(&uref->refcount, 1);
}

static inline int uref_put(struct uref *uref, void (*release)(struct uref *uref))
{
    if (__sync_sub_and_fetch(&uref->refcount, 1) == 0) {
        release(uref);
        return 1;
    }
    return 0;
}

static inline int uref_read(struct uref *uref)
{
    __sync_synchronize();
    return uref->refcount;
}

static inline bool uref_try_cmpxchg(struct uref *uref, int *old_val, int new_val)
{
    int r, o = *old_val;

    r = __sync_val_compare_and_swap(&uref->refcount, o, new_val);
    if (r != o) {
        *old_val = r;
    }
    return (r == o);
}

static inline int uref_get_unless_zero(struct uref *uref)
{
    int old = __sync_fetch_and_add(&uref->refcount, 0);

    do {
        if (old == 0) {
            break;
        }
    } while (!uref_try_cmpxchg(uref, &old, old + 1));

    return old;
}

#endif
