/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include <pthread.h>
#include <sys/mman.h>
#include "securec.h"
#include "dmc/dmc_log_user.h"
#include "drv_log_user_kernel_api.h"

// this file is a common.c

int32_t errno_to_user_errno(int32_t kern_err_no)
{
    return errno_to_user_errno_inner(kern_err_no);
}

const char *drv_log_get_module_str(enum devdrv_module_type module)
{
    return drv_log_get_module_str_inner(module);
}

int32_t drv_log_out_handle_register(struct log_out_handle *handle, size_t input_size, uint32_t flag)
{
    return drv_log_out_handle_register_inner(handle, input_size, flag);
}

/* compatibility */
int32_t is_run_log(void)
{
    return is_run_log_inner();
}

int32_t drv_log_out_handle_unregister(void)
{
    return drv_log_out_handle_unregister_inner();
}

uint32_t get_con_log_level(void)
{
    return get_con_log_level_inner();
}

const char *get_log_get_level_string(uint32_t level)
{
    return get_log_get_level_string_inner(level);
}

const char *get_log_get_print_time(void)
{
    return get_log_get_print_time_inner();
}

uint32_t get_log_level_shift(uint32_t level)
{
    return get_log_level_shift_inner(level);
}

void (*get_log_Print(void))(int32_t, int32_t, const char *, ...)
{
    return get_log_print_inner();
}