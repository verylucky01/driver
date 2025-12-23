/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "devdrv_alarm.h"
#include "devmng_common.h"
#include "ascend_hal_error.h"
#include "devdrv_user_common.h"

int devdrv_format_error_code(unsigned int dev_id, unsigned int *p_error_code,
                             int error_code_cnt, struct dsmi_alarm_info_stru *p_alarm_info)
{
    int err_cnt;
    unsigned int alarm_idx;
    int alarm_cnt = 0;

    struct dsmi_alarm_info_stru alarm_info_table[] = {
        {0, MAJOR, RESET_CLEAR, HDC_NONEXIST_CODE, HDC_NONEXIST_STR,
            HDC_NONEXIST_EXINFO, HDC_NONEXIST_REASON, HDC_NONEXIST_REPAIR},
        {0, CRITICAL, RESET_CLEAR, KERNEL_PANIC_CODE, KERNEL_PANIC_STR,
            KERNEL_PANIC_EXINFO, KERNEL_PANIC_REASON, KERNEL_PANIC_REPAIR},
        {0, MINOR, RELATED_CLEAR, KERNEL_OOM_CODE, KERNEL_OOM_STR,
            KERNEL_OOM_EXINFO, KERNEL_OOM_REASON, KERNEL_OOM_REPAIR},
    };

    if (dev_id >= DEVDRV_MAX_DAVINCI_NUM) {
        DEVDRV_DRV_ERR("Invalid device id. (id=%u)\n", dev_id);
        return DRV_ERROR_INVALID_DEVICE;
    }

    if ((p_error_code == NULL) || (p_alarm_info == NULL) || (error_code_cnt > ALARM_INFO_MAX_NUM)) {
        DEVDRV_DRV_ERR("Invalid input handler.\n");
        return DRV_ERROR_PARA_ERROR;
    }

    for (err_cnt = 0; err_cnt < error_code_cnt; err_cnt++) {
        for (alarm_idx = 0; alarm_idx < (sizeof(alarm_info_table) / sizeof(struct dsmi_alarm_info_stru)); alarm_idx++) {
            if (alarm_info_table[alarm_idx].moi == p_error_code[err_cnt]) {
                p_alarm_info[alarm_cnt++] = alarm_info_table[alarm_idx];
            }
        }
    }

    return alarm_cnt;
}
