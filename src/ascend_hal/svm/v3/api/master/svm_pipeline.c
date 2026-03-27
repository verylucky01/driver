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
#include <time.h>

#include "svm_atomic.h"

static pthread_rwlock_t svm_pipeline_rwlock = PTHREAD_RWLOCK_INITIALIZER;

/* If multiple threads concurrently obtain read locks, write locks may starve. 
 * In this case, a write request counter needs to be added to ensure that write locks are prioritized and prevent write lock starvation.
 */
static u32 svm_occupy_req = 0;

void svm_use_pipeline(void)
{
    struct timespec ts;

    ts.tv_sec = 0;
    ts.tv_nsec = 100; // sleep 100 ns
    while (svm_occupy_req != 0) {
        nanosleep(&ts, NULL);
    }

    (void)pthread_rwlock_rdlock(&svm_pipeline_rwlock);
}

void svm_unuse_pipeline(void)
{
    (void)pthread_rwlock_unlock(&svm_pipeline_rwlock);
}

void svm_occupy_pipeline(void)
{
    svm_atomic_inc(&svm_occupy_req);
    (void)pthread_rwlock_wrlock(&svm_pipeline_rwlock);
    svm_atomic_dec(&svm_occupy_req);
}

void svm_release_pipeline(void)
{
    (void)pthread_rwlock_unlock(&svm_pipeline_rwlock);
}

