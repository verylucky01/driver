/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <pthread.h>

#include "ascend_hal_error.h"

#include "svm_get_mem_token_info.h"
#include "svm_get_mem_token_info_ops_register.h"

static pthread_rwlock_t g_rwlock = PTHREAD_RWLOCK_INITIALIZER;
static get_mem_token_info_func g_get_mem_token_info[SVM_MAX_DEV_NUM] = {NULL};

void svm_get_mem_token_info_ops_register(u32 devid, get_mem_token_info_func func)
{
    (void)pthread_rwlock_wrlock(&g_rwlock);
    g_get_mem_token_info[devid] = func;
    (void)pthread_rwlock_unlock(&g_rwlock);
}

int svm_get_mem_token_info(u32 devid, u64 va, u64 size, u32 *token_id, u32 *token_val)
{
    int ret;

    (void)pthread_rwlock_wrlock(&g_rwlock);
    if (g_get_mem_token_info[devid] == NULL) {
        (void)pthread_rwlock_unlock(&g_rwlock);
        return DRV_ERROR_UNINIT;
    }

    ret = g_get_mem_token_info[devid](devid, va, size, token_id, token_val);
    (void)pthread_rwlock_unlock(&g_rwlock);

    return ret;
}
