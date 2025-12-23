/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef USER_LOG_H
#define USER_LOG_H

#ifndef CONFIG_LLT
#include <unistd.h>
#include <sys/syscall.h>
#include "slog.h"

#define CHK_PRT_RETURN(result, exeLog, ret) \
    do {                                      \
        if (result) {                         \
            exeLog;                           \
            return (ret);                       \
        }                                     \
    } while (0)

#ifdef DRV_HOST
#include "dmc_log_user.h"
#define roce_err(fmt, ...)    DRV_ERR(HAL_MODULE_TYPE_NET, fmt, ##__VA_ARGS__)
#define roce_warn(fmt, ...)   DRV_WARN(HAL_MODULE_TYPE_NET, fmt, ##__VA_ARGS__)
#define roce_info(fmt, ...)   DRV_INFO(HAL_MODULE_TYPE_NET, fmt, ##__VA_ARGS__)
#define roce_dbg(fmt, ...)    DRV_DEBUG(HAL_MODULE_TYPE_NET, fmt, ##__VA_ARGS__)
#define roce_run_warn(fmt, ...)  DRV_RUN_WARN(HAL_MODULE_TYPE_NET, fmt, ##__VA_ARGS__)
#define roce_run_info(fmt, ...)  DRV_NOTICE(HAL_MODULE_TYPE_NET, fmt, ##__VA_ARGS__)

#else
#ifdef LOG_HOST

#define DEBUG_LEVEL 0
#define INFO_LEVEL 1
#define WARN_LEVEL 2
#define ERROR_LEVEL 3
#define EVENT_LEVEL 16

/* HCCP module */
#define hccp_err(fmt, args...)  DlogForC(HCCP, ERROR_LEVEL, "tid:%d,%s(%d) : " fmt, \
    syscall(__NR_gettid), __func__, __LINE__, ##args)
#define hccp_warn(fmt, args...) DlogForC(HCCP, WARN_LEVEL, "tid:%d,%s(%d) : " fmt, \
    syscall(__NR_gettid), __func__, __LINE__, ##args)
#define hccp_info(fmt, args...) DlogForC(HCCP, INFO_LEVEL, "tid:%d,%s(%d) : " fmt, \
    syscall(__NR_gettid), __func__, __LINE__, ##args)
#define hccp_dbg(fmt, args...)  DlogForC(HCCP, DEBUG_LEVEL, "tid:%d,%s(%d) : " fmt, \
    syscall(__NR_gettid), __func__, __LINE__, ##args)
 
#define hccp_run_err(fmt, args...) DlogForC(HCCP | RUN_LOG_MASK, ERROR_LEVEL, "tid:%d,%s(%d) : " fmt, \
    syscall(__NR_gettid), __func__, __LINE__, ##args)
#define hccp_run_warn(fmt, args...) DlogForC(HCCP | RUN_LOG_MASK, WARN_LEVEL, "tid:%d,%s(%d) : " fmt, \
    syscall(__NR_gettid), __func__, __LINE__, ##args)
#define hccp_run_info(fmt, args...) DlogForC(HCCP | RUN_LOG_MASK, INFO_LEVEL, "tid:%d,%s(%d) : " fmt, \
    syscall(__NR_gettid), __func__, __LINE__, ##args)
#define hccp_run_dbg(fmt, args...)  DlogForC(HCCP | RUN_LOG_MASK, DEBUG_LEVEL, "tid:%d,%s(%d) : " fmt, \
    syscall(__NR_gettid), __func__, __LINE__, ##args)
 
#define hccp_event(fmt, args...)  DlogForC(HCCP, EVENT_LEVEL, "tid:%d,%s(%d) : " fmt, \
    syscall(__NR_gettid), __func__, __LINE__, ##args)

/* ROCE module */
#define roce_err(fmt, args...)    DlogForC(ROCE, ERROR_LEVEL, "%s(%d) : " fmt, __func__, __LINE__, ##args)
#define roce_warn(fmt, args...)   DlogForC(ROCE, WARN_LEVEL, "%s(%d) : " fmt, __func__, __LINE__, ##args)
#define roce_info(fmt, args...)   DlogForC(ROCE, INFO_LEVEL, "%s(%d) : " fmt, __func__, __LINE__, ##args)
#define roce_dbg(fmt, args...)    DlogForC(ROCE, DEBUG_LEVEL, "%s(%d) : " fmt, __func__, __LINE__, ##args)
#define roce_event(fmt, args...)  DlogForC(ROCE, EVENT_LEVEL, "%s(%d) : " fmt, __func__, __LINE__, ##args)

#define roce_run_err(fmt, args...)    DlogForC(ROCE | RUN_LOG_MASK, ERROR_LEVEL, "%s(%d) : " fmt, \
    __func__, __LINE__, ##args)
#define roce_run_warn(fmt, args...)   DlogForC(ROCE | RUN_LOG_MASK, WARN_LEVEL, "%s(%d) : " fmt, \
    __func__, __LINE__, ##args)
#define roce_run_info(fmt, args...)   DlogForC(ROCE | RUN_LOG_MASK, INFO_LEVEL, "%s(%d) : " fmt, \
    __func__, __LINE__, ##args)
#define roce_run_dbg(fmt, args...)    DlogForC(ROCE | RUN_LOG_MASK, DEBUG_LEVEL, "%s(%d) : " fmt, \
    __func__, __LINE__, ##args)

#define roce_event(fmt, args...)  DlogForC(ROCE, EVENT_LEVEL, "%s(%d) : " fmt, __func__, __LINE__, ##args)

#else
/* HCCP module */
#define hccp_err(fmt, args...)  dlog_error(HCCP, "tid:%d,%s(%d) : " fmt, \
    syscall(__NR_gettid), __func__, __LINE__, ##args)
#define hccp_warn(fmt, args...) dlog_warn(HCCP, "tid:%d,%s(%d) : " fmt, \
    syscall(__NR_gettid), __func__, __LINE__, ##args)
#define hccp_info(fmt, args...) dlog_info(HCCP, "tid:%d,%s(%d) : " fmt, \
    syscall(__NR_gettid), __func__, __LINE__, ##args)
#define hccp_dbg(fmt, args...)  dlog_debug(HCCP, "tid:%d,%s(%d) : " fmt, \
    syscall(__NR_gettid), __func__, __LINE__, ##args)
 
#define hccp_run_err(fmt, args...)  dlog_error(HCCP | RUN_LOG_MASK, "tid:%d,%s(%d) : " fmt, \
    syscall(__NR_gettid), __func__, __LINE__, ##args)
#define hccp_run_warn(fmt, args...) dlog_warn(HCCP | RUN_LOG_MASK, "tid:%d,%s(%d) : " fmt, \
    syscall(__NR_gettid), __func__, __LINE__, ##args)
#define hccp_run_info(fmt, args...) dlog_info(HCCP | RUN_LOG_MASK, "tid:%d,%s(%d) : " fmt, \
    syscall(__NR_gettid), __func__, __LINE__, ##args)
#define hccp_run_dbg(fmt, args...)  dlog_debug(HCCP | RUN_LOG_MASK, "tid:%d,%s(%d) : " fmt, \
    syscall(__NR_gettid), __func__, __LINE__, ##args)
 
#define hccp_event(fmt, args...)  dlog_event(HCCP, "tid:%d,%s(%d) : " fmt, \
    syscall(__NR_gettid), __func__, __LINE__, ##args)

/* ROCE module */
#define roce_err(fmt, args...)    dlog_error(ROCE, "%s(%d) : " fmt, __func__, __LINE__, ##args)
#define roce_warn(fmt, args...)   dlog_warn(ROCE, "%s(%d) : " fmt, __func__, __LINE__, ##args)
#define roce_info(fmt, args...)   dlog_info(ROCE, "%s(%d) : " fmt, __func__, __LINE__, ##args)
#define roce_dbg(fmt, args...)    dlog_debug(ROCE, "%s(%d) : " fmt, __func__, __LINE__, ##args)
#define roce_event(fmt, args...)  dlog_event(ROCE, "%s(%d) : " fmt, __func__, __LINE__, ##args)

#define roce_run_err(fmt, args...)    dlog_error(ROCE | RUN_LOG_MASK, "%s(%d) : " fmt, __func__, __LINE__, ##args)
#define roce_run_warn(fmt, args...)   dlog_warn(ROCE | RUN_LOG_MASK, "%s(%d) : " fmt, __func__, __LINE__, ##args)
#define roce_run_info(fmt, args...)   dlog_info(ROCE | RUN_LOG_MASK, "%s(%d) : " fmt, __func__, __LINE__, ##args)
#define roce_run_dbg(fmt, args...)    dlog_debug(ROCE | RUN_LOG_MASK, "%s(%d) : " fmt, __func__, __LINE__, ##args)

#define roce_event(fmt, args...)  dlog_event(ROCE, "%s(%d) : " fmt, __func__, __LINE__, ##args)
#endif
#endif

#else
static inline void printf_stub(char *format, ...) {
}
#define hccp_err  printf
#define hccp_warn  printf
#define hccp_info  printf
#define hccp_dbg  printf
#define hccp_event  printf
#define roce_err  printf
#define roce_warn  printf
#define roce_info  printf
#define roce_dbg  printf
#define roce_event  printf
#define DEBUG_LEVEL 0
#define INFO_LEVEL 1
#define WARN_LEVEL 2
#define ERROR_LEVEL 3
#define EVENT_LEVEL 16
#endif

#endif
