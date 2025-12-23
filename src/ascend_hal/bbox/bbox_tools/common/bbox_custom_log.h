/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef BBOX_CUSTOM_LOG_H
#define BBOX_CUSTOM_LOG_H

#include <sys/syslog.h>
#include <sys/syscall.h>
#include "bbox_tool_log.h"

#define PRINT_SYSLOG 0
#define PRINT_STDOUT 1
#define BBOX_MODULE "bbox"

#define BBOX_CUSTOM_LOG_INF(msg, ...) do {                                                                      \
    if (bbox_print_get_log_level() >= LOG_INFO) {                                                                   \
        if (bbox_print_get_log_mode() == PRINT_SYSLOG) {                                                            \
            syslog(LOG_INFO, "[%s:%d][ascend][curpid:%d,%ld][drv][%s][%s]" msg "\n",                            \
                   __FILE__, __LINE__, getpid(), syscall(__NR_gettid), BBOX_MODULE, __func__,##__VA_ARGS__);    \
        } else {                                                                                                \
            LOG_FPRINTF("[BBOX][INFO] " msg "\n", ##__VA_ARGS__);                                               \
        }                                                                                                       \
    }                                                                                                           \
} while (0)

#define BBOX_CUSTOM_LOG_WAR(msg, ...) do {                                                                      \
    if (bbox_print_get_log_level() >= LOG_WARNING) {                                                                \
        if (bbox_print_get_log_mode() == PRINT_SYSLOG) {                                                            \
            syslog(LOG_WARNING, "[%s:%d][ascend][curpid:%d,%ld][drv][%s][%s]" msg "\n",                         \
                   __FILE__, __LINE__, getpid(), syscall(__NR_gettid), BBOX_MODULE, __func__,##__VA_ARGS__);    \
        } else {                                                                                                \
            LOG_FPRINTF("[BBOX][WARNING] " msg "\n", ##__VA_ARGS__);                                            \
        }                                                                                                       \
    }                                                                                                           \
} while (0)

#define BBOX_CUSTOM_LOG_ERR(msg, ...) do {                                                                      \
    if (bbox_print_get_log_level() >= LOG_ERR) {                                                                    \
        if (bbox_print_get_log_mode() == PRINT_SYSLOG) {                                                            \
            syslog(LOG_ERR, "[%s:%d][ascend][curpid:%d,%ld][drv][%s][%s]" msg "\n",                             \
                   __FILE__, __LINE__, getpid(), syscall(__NR_gettid), BBOX_MODULE, __func__,##__VA_ARGS__);    \
        } else {                                                                                                \
            LOG_FPRINTF("[BBOX][ERROR] " msg "\n", ##__VA_ARGS__);                                              \
        }                                                                                                       \
    }                                                                                                           \
} while (0)

#define BBOX_CUSTOM_LOG_DBG(msg, ...) do {                                                                      \
    if (bbox_print_get_log_level() >= LOG_DEBUG) {                                                                  \
        if (bbox_print_get_log_mode() == PRINT_SYSLOG) {                                                            \
            syslog(LOG_DEBUG, "[%s:%d][ascend][curpid:%d,%ld][drv][%s][%s]" msg "\n",                           \
                   __FILE__, __LINE__, getpid(), syscall(__NR_gettid), BBOX_MODULE, __func__,##__VA_ARGS__);    \
        } else {                                                                                                \
            LOG_FPRINTF("[BBOX][DEBUG] " msg "\n", ##__VA_ARGS__);                                              \
        }                                                                                                       \
    }                                                                                                           \
} while (0)

#endif /* BBOX_CUSTOM_LOG_H */