/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef __DRV_LOG_USER_KERNEL_API_H
#define __DRV_LOG_USER_KERNEL_API_H

#include <syslog.h>
#include <time.h>
#include <sys/time.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>

#include "ascend_hal_define.h"
#include "slog.h"

#ifdef __cplusplus
extern "C" {
#endif
const char *drv_log_get_module_str_inner(enum devdrv_module_type module);


int32_t errno_to_user_errno_inner(int32_t kern_err_no);
int32_t drv_log_out_handle_register_inner(struct log_out_handle *handle, size_t input_size, uint32_t flag);
int32_t drv_log_out_handle_unregister_inner(void);
int32_t is_run_log_inner(void);

#ifdef DRV_HOST
void drv_log_rsyslog_console_level_set(uint32_t level);
#endif

uint32_t get_con_log_level_inner(void);
const char *get_log_get_level_string_inner(uint32_t level);
const char *get_log_get_print_time_inner(void);
uint32_t get_log_level_shift_inner(uint32_t level);
void (*get_log_print_inner(void))(int32_t, int32_t, const char *, ...);

#ifdef __cplusplus
}
#endif
#endif /* _DRV_SYSLOG_USER_H_ */
