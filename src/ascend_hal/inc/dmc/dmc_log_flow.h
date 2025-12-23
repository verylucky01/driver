/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef __DMC_LOG_FLOW_H__
#define __DMC_LOG_FLOW_H__
#include <sys/types.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/time.h>
#include <time.h>

static unsigned long g_log_last_jiffies = 0;  /* ms */
static unsigned long g_log_count = 0;

#define LOG_MS_PER_SECOND 1000
#define LOG_NSEC_PER_MSECOND 1000000
#define LOG_LIMIT_RATE 30

/*lint -e551*/
static inline bool print_timed_ratelimit(unsigned long *caller_jiffies, unsigned long interval_msecs)
{
    unsigned long timer, time_now;
    struct timespec now;

    (void)clock_gettime(CLOCK_MONOTONIC, &now);
    time_now = (unsigned long)(now.tv_sec * LOG_MS_PER_SECOND + now.tv_nsec / LOG_NSEC_PER_MSECOND);
    timer =  time_now - *caller_jiffies;
    if ((*caller_jiffies != 0) && (timer <= interval_msecs)) {
        return false;
    }
    *caller_jiffies = time_now;
    return true;
}

/*lint +e551*/
static inline bool drv_log_rate_limit(int *count, int branch_rate, unsigned long limit_time)
{
    if (print_timed_ratelimit(&g_log_last_jiffies, limit_time)) {
        *count = 0;
        g_log_count = 0;
        return false;
    } else {
        (*count)++;
        g_log_count++;
        return  ((*count >= branch_rate)  || (g_log_count  >=  LOG_LIMIT_RATE));
    }
}

static inline bool drv_log_rate_limit_ex(int *count, unsigned long *log_last_jiffies, int branch_rate,
    unsigned long limit_time)
{
    if (print_timed_ratelimit(log_last_jiffies, limit_time)) {
        *count = 0;
        return false;
    } else {
        (*count)++;
        return  (*count >= branch_rate);
    }
}
#endif
