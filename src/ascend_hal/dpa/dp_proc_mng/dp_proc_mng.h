/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef DP_PROC_MNG_H
#define DP_PROC_MNG_H

#ifdef EMU_ST
#define THREAD __thread
#else
#define THREAD
#endif

#ifndef EMU_ST
#include "dmc_user_interface.h"
#else
#include "ut_log.h"
#endif

#include "ascend_inpackage_hal.h"
#include "ascend_hal_error.h"

#ifndef EMU_ST
#ifdef CFG_FEATURE_SYSLOG
#include <syslog.h>
#define DP_PROC_MNG_ERR(fmt, ...) syslog(LOG_ERR, "[%s %d] " fmt, __func__, __LINE__, ##__VA_ARGS__)
#define DP_PROC_MNG_WARN(fmt, ...) syslog(LOG_WARNING, "[%s %d] " fmt, __func__, __LINE__, ##__VA_ARGS__)
#define DP_PROC_MNG_INFO(fmt, ...) syslog(LOG_INFO, "[%s %d] " fmt, __func__, __LINE__, ##__VA_ARGS__)
#define DP_PROC_MNG_DEBUG(fmt, ...)
#define DP_PROC_MNG_EVENT(fmt, ...) syslog(LOG_NOTICE, "[%s %d] " fmt, __func__, __LINE__, ##__VA_ARGS__)
#else
#define DP_PROC_MNG_ERR(format, ...) do { \
    DRV_ERR(HAL_MODULE_TYPE_DP_PROC_MNG, format, ##__VA_ARGS__); \
} while (0)

#define DP_PROC_MNG_WARN(format, ...) do { \
    DRV_WARN(HAL_MODULE_TYPE_DP_PROC_MNG, format, ##__VA_ARGS__); \
} while (0)

#define DP_PROC_MNG_INFO(format, ...) do { \
    DRV_INFO(HAL_MODULE_TYPE_DP_PROC_MNG, format, ##__VA_ARGS__); \
} while (0)

#define DP_PROC_MNG_DEBUG(format, ...) do { \
    DRV_DEBUG(HAL_MODULE_TYPE_DP_PROC_MNG, format, ##__VA_ARGS__); \
} while (0)

/* infrequent log level */
#define DP_PROC_MNG_EVENT(format, ...) do { \
    DRV_NOTICE(HAL_MODULE_TYPE_DP_PROC_MNG, format, ##__VA_ARGS__); \
} while (0)
#endif
#else
#define DP_PROC_MNG_ERR(fmt, ...)
#define DP_PROC_MNG_WARN(fmt, ...)
#define DP_PROC_MNG_INFO(fmt, ...)
#define DP_PROC_MNG_DEBUG(fmt, ...)
#define DP_PROC_MNG_EVENT(fmt, ...)
#endif

/* This struct comes from the profile tool;
 * directory: collector/dvvp/driver/inc/ai_drv_prof_api.h;
 * definition: struct TagMemProfileConfig;
 */
struct msprof_config {
    uint32_t period;
    uint32_t timestamp_mode; /* 0 monotonic : others cpu_cycle */
    uint32_t event;
    uint32_t res;
};

int dp_proc_mng_prof_start_fun(struct prof_sample_start_para *para);
int dp_proc_mng_prof_stop_fun(struct prof_sample_stop_para *para);
int dp_proc_mng_prof_sample_fun(struct prof_sample_para *para);

drvError_t dp_proc_mng_ioctl(unsigned int cmd, void *para);
int dp_proc_mng_get_fd_inner(void);

#endif

