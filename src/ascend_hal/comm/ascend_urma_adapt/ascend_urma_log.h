/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef ASCEND_URMA_LOG_H
#define ASCEND_URMA_LOG_H

#ifndef EMU_ST
#include "dmc_user_interface.h"
#else
#include "ascend_inpackage_hal.h"
#include "ut_log.h"
#endif

#ifndef EMU_ST
#define ascend_urma_err(fmt, ...)         DRV_ERR(HAL_MODULE_TYPE_ASCEND_URMA_ADAPT, "<errno:%d, %d> " fmt, errno, \
    errno_to_user_errno(errno), ##__VA_ARGS__)
#else
#define ascend_urma_err(fmt, ...)         DRV_ERR(HAL_MODULE_TYPE_ASCEND_URMA_ADAPT, fmt, ##__VA_ARGS__)
#endif

#define ascend_urma_warn(fmt, ...)        DRV_WARN(HAL_MODULE_TYPE_ASCEND_URMA_ADAPT, fmt, ##__VA_ARGS__)
#define ascend_urma_info(fmt, ...)        DRV_INFO(HAL_MODULE_TYPE_ASCEND_URMA_ADAPT, fmt, ##__VA_ARGS__)
#define ascend_urma_debug(fmt, ...)       DRV_DEBUG(HAL_MODULE_TYPE_ASCEND_URMA_ADAPT, fmt, ##__VA_ARGS__)
/* alarm event log, non-alarm events use debug or run log */
#define ascend_urma_event(fmt, ...)       DRV_EVENT(HAL_MODULE_TYPE_ASCEND_URMA_ADAPT, fmt, ##__VA_ARGS__)
/* infrequent log level */
#define ascend_urma_notice(fmt, ...)      DRV_NOTICE(HAL_MODULE_TYPE_ASCEND_URMA_ADAPT, fmt, ##__VA_ARGS__)
#define ascend_urma_run_info(fmt, ...)

#endif
