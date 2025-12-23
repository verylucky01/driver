/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef ATOMIC_REF_H
#define ATOMIC_REF_H

struct atomic_status_info {
    unsigned int valid : 1;
    unsigned int value : 31;
};

union atomic_status {
    struct atomic_status_info info;
    unsigned int status;
};

#define ATOMIC_VALID    1
#define ATOMIC_INVALID  0


void atomic_ref_self_healing(union atomic_status *status);
void atomic_ref_init(union atomic_status *status);
void atomic_ref_uninit(union atomic_status *status);
bool atomic_ref_try_init(union atomic_status *status);
bool atomic_ref_try_uninit(union atomic_status *status);

bool atomic_ref_inc(union atomic_status *status);
bool atomic_ref_dec(union atomic_status *status);
bool atomic_ref_is_valid(union atomic_status *status);
bool atomic_ref_is_ready_clear(union atomic_status *status);

void atomic_value_init(union atomic_status *status);
void atomic_value_set_invalid(union atomic_status *status);
bool atomic_value_set(union atomic_status *status, unsigned int value);
unsigned int atomic_value_get(union atomic_status *status);
bool atomic_value_is_valid(union atomic_status *status);

#endif
