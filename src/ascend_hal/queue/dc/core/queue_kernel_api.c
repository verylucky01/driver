/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "queue_kernel_api.h"

#define MAX_PRINT_CNT   5U        /* 10min max print 5 time */
#define QUEUE_OVERTIME_PRINT_CHECK_PERIOD   6000    // 10min

#ifdef EMU_ST
#include <sys/syscall.h>
#ifndef THREAD
#define THREAD __thread
#endif
#else
#ifndef THREAD
#define THREAD
#endif
#endif


static THREAD unsigned int g_recycle_proc_count = 0;
static THREAD unsigned long g_run_info_flow = 0;

void queue_run_log_flow_ctrl_cnt_clear(void)
{
    unsigned int clear_flag = g_recycle_proc_count % (unsigned int)QUEUE_OVERTIME_PRINT_CHECK_PERIOD;

    g_recycle_proc_count++;
    if (clear_flag == 0) {
        ATOMIC_SET((volatile long long *)&g_run_info_flow, 0); /* Counts are cleared every 10 minutes. */
        return;
    }
}
bool queue_proc_log_flow_ctrl_cnt_check(void)
{
    if (g_run_info_flow < MAX_PRINT_CNT) {
        ATOMIC_INC((volatile long long *)&g_run_info_flow);
        return true;
    }
    return false;
}