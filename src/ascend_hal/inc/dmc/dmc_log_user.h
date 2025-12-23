/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef DMC_LOG_USER_H
#define DMC_LOG_USER_H

#include <syslog.h>
#include <time.h>
#include <sys/time.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/syscall.h>

#include "ascend_hal_define.h"
#include "slog.h"
#include "dmc_share_log.h"

#ifdef __cplusplus
extern "C" {
#endif
const char *drv_log_get_module_str(enum devdrv_module_type module);

int32_t errno_to_user_errno(int32_t kern_err_no);
drvError_t drvMngGetConsoleLogLevel(unsigned int *logLevel);// This function is defined in devdrv_manage.
int32_t drv_log_out_handle_register(struct log_out_handle *handle, size_t input_size, uint32_t flag);
int32_t drv_log_out_handle_unregister(void);
int32_t is_run_log(void);

uint32_t get_con_log_level(void);
const char *get_log_get_level_string(uint32_t level);
const char *get_log_get_print_time(void);
uint32_t get_log_level_shift(uint32_t level);
void (*get_log_Print(void))(int32_t, int32_t, const char *, ...);

#ifdef DRV_HOST
#define DRV_SYSLOG_BASE(LEVEL, module, mask, actual_print_level, fmt, ...) do { \
        if ((uint32_t)LEVEL <= (uint32_t)actual_print_level) { \
            (*get_log_Print())(mask, (int32_t)get_log_level_shift((uint32_t)LEVEL), \
                "%s%s[%s:%d][ascend][curpid:%d,%d][drv][%s][%s]" fmt, \
                get_log_get_level_string((uint32_t)LEVEL), get_log_get_print_time(), \
                __FILE__, __LINE__, getpid(), syscall(__NR_gettid), \
                drv_log_get_module_str(module), __func__, ##__VA_ARGS__); \
        } \
    } while (0)

#define DRV_SYSLOG(LEVEL, module, fmt, ...) DRV_SYSLOG_BASE(LEVEL, module, DRV, \
    get_con_log_level(), fmt, ##__VA_ARGS__)
#define DRV_SYSLOG_RUN(LEVEL, module, fmt, ...) DRV_SYSLOG_BASE(LEVEL, module, (DRV | RUN_LOG_MASK), \
    LOG_INFO, fmt, ##__VA_ARGS__)

/* debug log, the default log level is LOG_ERR. */
#define DRV_ERR(module, fmt, ...) DRV_SYSLOG(LOG_ERR, module, fmt, ##__VA_ARGS__)
#define DRV_WARN(module, fmt, ...) DRV_SYSLOG(LOG_WARNING, module, fmt, ##__VA_ARGS__)
#define DRV_INFO(module, fmt, ...) DRV_SYSLOG(LOG_INFO, module, fmt, ##__VA_ARGS__)
#define DRV_DEBUG(module, fmt, ...) DRV_SYSLOG(LOG_DEBUG, module, fmt, ##__VA_ARGS__)

/* alarm event log, non-alarm events use debug or run log */
#define DRV_CRIT(module, fmt, ...) DRV_SYSLOG(LOG_CRIT, module, fmt, ##__VA_ARGS__)
#define DRV_EVENT(module, fmt, ...) DRV_CRIT(module, fmt, ##__VA_ARGS__)

/* infrequent log level */
#define DRV_EMERG(module, fmt, ...) DRV_SYSLOG(LOG_EMERG, module, fmt, ##__VA_ARGS__)
#define DRV_ALERT(module, fmt, ...) DRV_SYSLOG(LOG_ALERT, module, fmt, ##__VA_ARGS__)
#define DRV_NOTICE(module, fmt, ...) DRV_SYSLOG(LOG_NOTICE, module, fmt, ##__VA_ARGS__)

#define DRV_LOG_CMPT(LEVEL, module, fmt, ...) do { \
    if (is_run_log()) {                                       \
        DRV_SYSLOG_RUN(LEVEL, module, fmt, ##__VA_ARGS__); \
    } else {                                                  \
        DRV_SYSLOG(LOG_CRIT, module, fmt, ##__VA_ARGS__);     \
    }                                                         \
} while (0)

/* run log, the default log level is LOG_INFO. */
#define DRV_RUN_ERR(module, fmt, ...) DRV_LOG_CMPT(LOG_ERR, module, fmt, ##__VA_ARGS__)
#define DRV_RUN_WARN(module, fmt, ...) DRV_LOG_CMPT(LOG_WARNING, module, fmt, ##__VA_ARGS__)
#define DRV_RUN_INFO(module, fmt, ...) DRV_LOG_CMPT(LOG_INFO, module, fmt, ##__VA_ARGS__)
#define DRV_RUN_DEBUG(module, fmt, ...) DRV_LOG_CMPT(LOG_DEBUG, module, fmt, ##__VA_ARGS__)

#else
/* debug log, the default log level is LOG_DEBUG. */
#define DRV_EMERG(module, fmt, ...) \
    dlog_error(DRV, "[%s] [%s] " fmt, drv_log_get_module_str(module), __func__, ##__VA_ARGS__)
#define DRV_ALERT(module, fmt, ...) \
    dlog_error(DRV, "[%s] [%s] " fmt, drv_log_get_module_str(module), __func__, ##__VA_ARGS__)
#define DRV_ERR(module, fmt, ...) \
    dlog_error(DRV, "[%s] [%s] " fmt, drv_log_get_module_str(module), __func__, ##__VA_ARGS__)
#define DRV_WARN(module, fmt, ...) \
    dlog_warn(DRV, "[%s] [%s] " fmt, drv_log_get_module_str(module), __func__, ##__VA_ARGS__)
#define DRV_INFO(module, fmt, ...) \
    dlog_info(DRV, "[%s] [%s] " fmt, drv_log_get_module_str(module), __func__, ##__VA_ARGS__)
#define DRV_DEBUG(module, fmt, ...) \
    dlog_debug(DRV, "[%s] [%s] " fmt, drv_log_get_module_str(module), __func__, ##__VA_ARGS__)

/* alarm event log, non-alarm events use debug or run log */
#define DRV_CRIT(module, fmt, ...) \
    dlog_event(DRV, "[%s] [%s] " fmt, drv_log_get_module_str(module), __func__, ##__VA_ARGS__)
#define DRV_NOTICE(module, fmt, ...) \
    dlog_event(DRV, "[%s] [%s] " fmt, drv_log_get_module_str(module), __func__, ##__VA_ARGS__)
#define DRV_EVENT(module, fmt, ...) DRV_CRIT(module, fmt, ##__VA_ARGS__)

/* run log, the default log level is LOG_INFO. */
#define DRV_RUN_INFO(module, fmt, ...) \
    dlog_info((DRV | RUN_LOG_MASK), "[%s] [%s] " fmt, \
              drv_log_get_module_str(module), __func__, ##__VA_ARGS__)
#endif

#ifdef __cplusplus
}
#endif
#endif /* _DRV_SYSLOG_USER_H_ */