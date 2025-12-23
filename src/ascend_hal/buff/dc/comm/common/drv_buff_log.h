/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef DRV_BUFF_LOG_H
#define DRV_BUFF_LOG_H

#include "securec.h"
#include <stdio.h>

#ifdef CFG_FEATURE_SYSLOG
#include <syslog.h>
#define buff_err(fmt, ...) syslog(LOG_ERR, "[%s %d] " fmt, __func__, __LINE__, ##__VA_ARGS__)
#define buff_warn(fmt, ...) syslog(LOG_WARNING, "[%s %d] " fmt, __func__, __LINE__, ##__VA_ARGS__)
#define buff_info(fmt, ...) syslog(LOG_INFO, "[%s %d] " fmt, __func__, __LINE__, ##__VA_ARGS__)
#define buff_debug(fmt, ...) syslog(LOG_DEBUG, "[%s %d] " fmt, __func__, __LINE__, ##__VA_ARGS__)
#define buff_event(fmt, ...) syslog(LOG_NOTICE, "[%s %d] " fmt, __func__, __LINE__, ##__VA_ARGS__)
#define buff_show(fmt, ...) syslog(LOG_INFO, fmt, ##__VA_ARGS__)
#else
#ifndef EMU_ST
#include "dmc_user_interface.h"
#define buff_err(fmt, ...)   DRV_ERR(HAL_MODULE_TYPE_BUF_MANAGER, "<errno:%d, %d> " fmt, errno, \
    errno_to_user_errno(errno), ##__VA_ARGS__)
#define buff_warn(fmt, ...)  DRV_WARN(HAL_MODULE_TYPE_BUF_MANAGER, fmt, ##__VA_ARGS__)
#define buff_info(fmt, ...)  DRV_INFO(HAL_MODULE_TYPE_BUF_MANAGER, fmt, ##__VA_ARGS__)
#define buff_debug(fmt, ...) DRV_DEBUG(HAL_MODULE_TYPE_BUF_MANAGER, fmt, ##__VA_ARGS__)
/* run log, the default log level is LOG_INFO. */
#define buff_event(fmt, ...) DRV_RUN_INFO(HAL_MODULE_TYPE_BUF_MANAGER, fmt, ##__VA_ARGS__)
#define buff_show(fmt, ...)  DRV_RUN_INFO(HAL_MODULE_TYPE_BUF_MANAGER, fmt, ##__VA_ARGS__)
#else
#define buff_err(fmt, ...) printf("[%s %d] %d " fmt, __func__, __LINE__, buff_get_current_pid(), ##__VA_ARGS__)
#define buff_warn(fmt, ...) printf("[%s %d] %d " fmt, __func__, __LINE__, buff_get_current_pid(), ##__VA_ARGS__)
#define buff_info(fmt, ...)  printf("[%s %d] %d " fmt, __func__, __LINE__, buff_get_current_pid(), ##__VA_ARGS__)
#define buff_debug(fmt, ...) printf("[%s %d] %d " fmt, __func__, __LINE__, buff_get_current_pid(), ##__VA_ARGS__)
#define buff_event(fmt, ...) printf("[%s %d] %d " fmt, __func__, __LINE__, buff_get_current_pid(), ##__VA_ARGS__)
#define buff_show(fmt, ...) printf("[%s %d] %d " fmt, __func__, __LINE__, buff_get_current_pid(), ##__VA_ARGS__)
#endif
#endif

#endif