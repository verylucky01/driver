/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "ascend_hal.h"
#include "atomic_ref.h"

#define ATOMIC_REF_STATUS_IDLE    { .info.valid = ATOMIC_INVALID, .info.value = 0 }
#define ATOMIC_REF_STATUS_REF     { .info.valid = ATOMIC_VALID, .info.value = 1 }
#define ATOMIC_REF_STATUS_NO_REF   { .info.valid = ATOMIC_VALID, .info.value = 0 }

void atomic_ref_init(union atomic_status *status)
{
    union atomic_status ref = ATOMIC_REF_STATUS_REF;
    (void)__sync_lock_test_and_set((volatile int *)&status->status, ref.status);
}

void atomic_ref_uninit(union atomic_status *status)
{
    union atomic_status idle = ATOMIC_REF_STATUS_IDLE;
    (void)__sync_lock_test_and_set((volatile int *)&status->status, idle.status);
}

bool atomic_ref_try_init(union atomic_status *status)
{
    union atomic_status ref = ATOMIC_REF_STATUS_REF;
    union atomic_status idle = ATOMIC_REF_STATUS_IDLE;
    return __sync_bool_compare_and_swap((volatile int *)&status->status, idle.status, ref.status);
}

bool atomic_ref_try_uninit(union atomic_status *status)
{
    union atomic_status no_ref = ATOMIC_REF_STATUS_NO_REF;
    union atomic_status idle = ATOMIC_REF_STATUS_IDLE;
    return __sync_bool_compare_and_swap((volatile int *)&status->status, no_ref.status, idle.status);
}

void atomic_ref_self_healing(union atomic_status *status)
{
    union atomic_status cur_status;
    union atomic_status idle = ATOMIC_REF_STATUS_IDLE;

    cur_status.status = status->status;
    if ((cur_status.info.valid == ATOMIC_INVALID) && (cur_status.info.value != 0)) {
        (void)__sync_bool_compare_and_swap((volatile int *)&status->status, cur_status.status, idle.status);
    }
}

static bool atomic_ref_set(union atomic_status *status, int ref)
{
    union atomic_status cur_status, new_status;

    do {
        cur_status.status = status->status;
        if (cur_status.info.valid != ATOMIC_VALID) {
            return false;
        }

        new_status.status = cur_status.status;
        new_status.info.value += ref;
    } while (!__sync_bool_compare_and_swap((volatile int *)&status->status, cur_status.status, new_status.status));

    return true;
}

bool atomic_ref_inc(union atomic_status *status)
{
    return atomic_ref_set(status, 1);
}

bool atomic_ref_dec(union atomic_status *status)
{
    return atomic_ref_set(status, -1);
}

bool atomic_ref_is_valid(union atomic_status *status)
{
    return (status->info.valid == ATOMIC_VALID);
}

bool atomic_ref_is_ready_clear(union atomic_status *status)
{
    union atomic_status no_ref = ATOMIC_REF_STATUS_NO_REF;
    return (status->status == no_ref.status);
}

void atomic_value_init(union atomic_status *status)
{
    union atomic_status init_status;

    init_status.info.valid = ATOMIC_VALID;
    init_status.info.value = 0;

    (void)__sync_lock_test_and_set((volatile int *)&status->status, init_status.status);
}

void atomic_value_set_invalid(union atomic_status *status)
{
    union atomic_status mask = { .info.valid = 0, .info.value = 0x7FFFFFFF }; // 0x7FFFFFFF is value mask
    (void)__sync_fetch_and_and((volatile int *)&status->status, mask.status);
}

bool atomic_value_set(union atomic_status *status, unsigned int value)
{
    union atomic_status cur_status, new_status;

    cur_status.status = status->status;
    if (cur_status.info.valid != ATOMIC_VALID) {
        return false;
    }

    new_status.status = cur_status.status;
    new_status.info.value = value;
    return __sync_bool_compare_and_swap((volatile int *)&status->status, cur_status.status, new_status.status);
}

unsigned int atomic_value_get(union atomic_status *status)
{
    return status->info.value;
}

bool atomic_value_is_valid(union atomic_status *status)
{
    return (status->info.valid == ATOMIC_VALID);
}
