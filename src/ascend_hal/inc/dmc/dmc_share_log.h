/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

// As a common header file related to shared logs, it is used together with the 2nd and 2.5th packages.
#ifndef DMC_SHARE_LOG_H
#define DMC_SHARE_LOG_H

#include "ascend_hal_define.h"

#define TSDRV_SHARE_LOG_START   (0xE0000000000ULL)
#define DEVMM_SHARE_LOG_START   (0xE0000080000ULL)
#define DEVMNG_SHARE_LOG_START  (0xE0000100000ULL)
#define HDC_SHARE_LOG_START     (0xE0000180000ULL)
#define ESCHED_SHARE_LOG_START  (0xE0000200000ULL)
#define XSMEM_SHARE_LOG_START   (0xE0000280000ULL)
#define QUEUE_SHARE_LOG_START   (0xE0000300000ULL)
#define COMMON_SHARE_LOG_START  (0xE0000380000ULL)

#define TSDRV_SHARE_LOG_RUNINFO_START  (0xE0000040000ULL)
#define DEVMM_SHARE_LOG_RUNINFO_START  (0xE00000C0000ULL)
#define DEVMNG_SHARE_LOG_RUNINFO_START (0xE0000140000ULL)
#define HDC_SHARE_LOG_RUNINFO_START    (0xE00001C0000ULL)
#define ESCHED_SHARE_LOG_RUNINFO_START (0xE0000240000ULL)
#define XSMEM_SHARE_LOG_RUNINFO_START  (0xE00002C0000ULL)
#define QUEUE_SHARE_LOG_RUNINFO_START  (0xE0000340000ULL)
#define COMMON_SHARE_LOG_RUNINFO_START (0xE00003C0000ULL)

#define SHARE_LOG_MAX_SIZE (4 * 1024)

enum share_log_type_enum {
    SHARE_LOG_ERR = 0,
    SHARE_LOG_RUN_INFO,
    SHARE_LOG_TYPE_MAX,
};

void share_log_create(enum devdrv_module_type module_type, uint32_t size);
void share_log_destroy(enum devdrv_module_type type);
void share_log_read(enum devdrv_module_type module_type);
void share_log_read_err(enum devdrv_module_type module_type);
void share_log_read_run_info(enum devdrv_module_type module_type);

#endif