/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef CACHE_ALLOCATOR_H
#define CACHE_ALLOCATOR_H

#include <pthread.h>
#include <time.h>

#include "svm_pub.h"

struct cache_strategy {
    u64 alloc_thres;
    u64 shrink_thres_default;
    u64 shrink_thres_cur;
    u64 alloc_gran;
    u64 expand_granularity;
};

struct cache_stats {
    time_t peak_alloced_last_update_time;

    u64 total;
    u64 idle;
    u64 cur_alloced;
    u64 peak_alloced;   /* within CACHE_MALLOC_PEAK_ALLOCED_TIME_INTERVAL_S */
};

struct cache_allocator {
    u32 devid;
    u32 flag;
    void *ga_inst;

    pthread_rwlock_t rwlock;
    struct cache_stats stats;
    struct cache_strategy strategy;
};

int cache_allocator_create(u32 devid, u32 flag);
void cache_allocator_destroy(u32 devid, u32 flag);

struct cache_allocator *cache_get_allocator(u32 devid, u32 flag);

u32 cache_allocator_show(u32 devid, char *buf, u32 buf_len);

#endif
