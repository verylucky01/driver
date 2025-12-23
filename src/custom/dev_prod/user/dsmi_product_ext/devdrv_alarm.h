/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef _DEVDRV_ALARM_H_
#define _DEVDRV_ALARM_H_

#include "dsmi_common_interface_custom.h"

#define ALARM_INFO_MAX_NUM	128

enum alarm_level {
    CRITICAL = 0,
    MAJOR,
    MINOR,
    WARN,
    INDETERMINATE,
    CLEAERED,
    UNDEF_LEVEL
};

enum alarm_clr_type {
    NORMAL_CLEAR = 0,
    RESET_CLEAR,
    MANUL_CLEAR,
    RELATED_CLEAR,
    INHERIT_CLEAR,
    UNDEF_TYPE
};


/* 0xA6040001 */
#define HDC_NONEXIST_CODE  0xA6040001
#define HDC_NONEXIST_STR   "hdc exit"
#define HDC_NONEXIST_EXINFO ""
#define HDC_NONEXIST_REASON "hdcd exit/linkdown"
#define HDC_NONEXIST_REPAIR "self-repair"

/* 0xA8040001 */
#define KERNEL_PANIC_CODE 0xA8040001
#define KERNEL_PANIC_STR "kernel panic"
#define KERNEL_PANIC_EXINFO ""
#define KERNEL_PANIC_REASON "invalid addr access in kernel"
#define KERNEL_PANIC_REPAIR "save bbox info and reboot"

/* 0xA4040001 */
#define KERNEL_OOM_CODE  0xA4040001
#define KERNEL_OOM_STR  "kernel OOM"
#define KERNEL_OOM_EXINFO ""
#define KERNEL_OOM_REASON "no memory"
#define KERNEL_OOM_REPAIR "release memory"


int devdrv_format_error_code(unsigned int dev_id, unsigned int *p_error_code, int error_code_cnt,
                             struct dsmi_alarm_info_stru *p_alarm_info);

#endif
