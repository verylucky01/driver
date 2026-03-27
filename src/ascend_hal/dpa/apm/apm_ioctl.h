/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef APM_IOCTL_H
#define APM_IOCTL_H

#include <sys/types.h>
#include <fcntl.h>
#include <stdint.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "apm_kernel_ioctl.h"

#ifndef EMU_ST
#include "dmc_user_interface.h"
#include "ascend_inpackage_hal.h"
#include "ascend_hal.h"
#else
#include <pthread.h>

#include "ascend_inpackage_hal.h"
#include "ut_log.h"
#endif

#define apm_err(fmt, ...) DRV_ERR(HAL_MODULE_TYPE_APM, fmt, ##__VA_ARGS__)
#define apm_warn(fmt, ...) DRV_WARN(HAL_MODULE_TYPE_APM, fmt, ##__VA_ARGS__)
#define apm_info(fmt, ...) DRV_INFO(HAL_MODULE_TYPE_APM, fmt, ##__VA_ARGS__)
#define apm_debug(fmt, ...) DRV_DEBUG(HAL_MODULE_TYPE_APM, fmt, ##__VA_ARGS__)
#define apm_event(fmt, ...) DRV_NOTICE(HAL_MODULE_TYPE_APM, fmt, ##__VA_ARGS__)

#ifdef EMU_ST
#define THREAD__  __thread
#else
#define THREAD__
#endif

int apm_cmd_ioctl(unsigned long cmd, void *para);

#endif
