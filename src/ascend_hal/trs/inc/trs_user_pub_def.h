/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef TRS_USER_PUB_DEF_H__
#define TRS_USER_PUB_DEF_H__

#include "ascend_hal.h"

#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

#include "securec.h"


#define TRS_DEV_NUM 1124
#define TRS_DEVICE_DEV_NUM 64
#define TRS_TS_NUM 2

#define TRS_MC2_FEATURE 0x072313
#define TRS_MC2_BIND_LOGICCQ_FEATURE 0x072317
#define TRS_STREAM_EXPEND_FEATURE 0x072418

#ifdef CFG_FEATURE_SYSLOG

#include <syslog.h>
#define trs_err(fmt, ...) syslog(LOG_ERR, "[%s %d] " fmt, __func__, __LINE__, ##__VA_ARGS__)
#define trs_warn(fmt, ...) syslog(LOG_WARNING, "[%s %d] " fmt, __func__, __LINE__, ##__VA_ARGS__)
#define trs_info(fmt, ...) syslog(LOG_INFO, "[%s %d] " fmt, __func__, __LINE__, ##__VA_ARGS__)
#define trs_debug(fmt, ...) syslog(LOG_DEBUG, "[%s %d] " fmt, __func__, __LINE__, ##__VA_ARGS__)
#define trs_warn_limit(fmt, ...) syslog(LOG_WARNING, "[%s %d] " fmt, __func__, __LINE__, ##__VA_ARGS__)
#else

#ifndef EMU_ST
#include "dmc_user_interface.h"
#else
#include "ascend_inpackage_hal.h"
#include "ut_log.h"
#define drv_log_rate_limit_ex(cnt, jiffies, rate, time) false
#endif

#define trs_err(fmt, ...) DRV_ERR(HAL_MODULE_TYPE_TS_DRIVER, fmt, ##__VA_ARGS__)
#define trs_warn(fmt, ...) DRV_WARN(HAL_MODULE_TYPE_TS_DRIVER, fmt, ##__VA_ARGS__)
#define trs_info(fmt, ...) DRV_INFO(HAL_MODULE_TYPE_TS_DRIVER, fmt, ##__VA_ARGS__)
#define trs_debug(fmt, ...) DRV_DEBUG(HAL_MODULE_TYPE_TS_DRIVER, fmt, ##__VA_ARGS__)

#define TRS_LOG_LIMIT_TIME          72000 /* 72s */
#define TRS_LOG_LIMIT_BRANCH_RATE   1 /* print 1 counts per 60s */

#define trs_warn_limit(fmt, ...)                                                                                    \
    do {                                                                                                            \
        static unsigned long log_last_jiffies = 0;  /* ms */                                                        \
        static int _log_cnt = 0;                                                                                    \
        if (!drv_log_rate_limit_ex(&_log_cnt, &log_last_jiffies, TRS_LOG_LIMIT_BRANCH_RATE, TRS_LOG_LIMIT_TIME)) {  \
            DRV_WARN(HAL_MODULE_TYPE_TS_DRIVER, fmt, ##__VA_ARGS__);                                                \
        }                                                                                                           \
    } while (0)

#endif

#define TRS_NSEC_PER_SEC        1000000000U
#define TRS_LOG_PRINT_LIMIT     ((3600ULL / 50) * TRS_NSEC_PER_SEC) // max: 50 times per hour, in ns
#define TRS_TIMESTAMP_BUFF_LEN  100U

static inline unsigned long long trs_get_clock_time(void)
{
    struct timespec timestamp;

    (void)clock_gettime(CLOCK_MONOTONIC, &timestamp);
    return (unsigned long long)((timestamp.tv_sec * TRS_NSEC_PER_SEC) + timestamp.tv_nsec);
}

static inline void trs_print_time_consume_warning_log(int timeout, unsigned long long *time_stamp, const int len)
{
    char time_str[TRS_TIMESTAMP_BUFF_LEN] = {0};
    int index = 0;
    int ret, i;

    for (i = 1; i < len; i++) {
        ret = sprintf_s(time_str + index, (TRS_TIMESTAMP_BUFF_LEN - (unsigned int)index), "%lld ",
            (long long)time_stamp[i] - (long long)time_stamp[i - 1]);
        if (ret > 0) {
            index += ret;
        }
    }

    trs_warn("Time consume warning. (start_time=%lluns; total_time=%lluns; limit=%uns; time_interval=%s(ns))\n",
        time_stamp[0], time_stamp[len -1] - time_stamp[0], timeout, time_str);
}

#define TRS_TRACE_TIME_CONSUME_START(is_tracing_on, time_stamp, len, time_limit)        \
    static long long trace_last_warning_time = -(long long)TRS_LOG_PRINT_LIMIT;                   \
    unsigned long long *trace_time = time_stamp;                                      \
    int trace_len = len;                                                             \
    unsigned long long trace_time_out = time_limit;                                                   \
    bool is_tracing = is_tracing_on;                                                   \
    if (is_tracing) {                                                                \
        trace_time[0] = trs_get_clock_time();                                           \
    }

#define TRS_TRACE_RECORD_TIMESTAMP(is_tracing_on, time_stamp) do {                     \
    if (is_tracing_on) {                                                              \
        *(time_stamp) = trs_get_clock_time();                                           \
    }                                                                               \
} while(0)

#define TRS_TRACE_TIME_CONSUME_END do {                                             \
    if (is_tracing) {                                                                \
        trace_time[trace_len - 1] = trs_get_clock_time();                                \
        if (((trace_time[trace_len - 1] - trace_time[0]) > trace_time_out) &&            \
            ((unsigned long long)(trace_time[trace_len - 1] - (unsigned long long)trace_last_warning_time) >= TRS_LOG_PRINT_LIMIT)) { \
            trace_last_warning_time = (long long)trace_time[trace_len - 1];              \
            trs_print_time_consume_warning_log((int)trace_time_out, trace_time, (int)trace_len);       \
        }                                                                           \
    }                                                                               \
} while(0)

#endif
