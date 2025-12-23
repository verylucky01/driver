/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */
 
#ifndef PBL_KA_TIMER_PUB_H
#define PBL_KA_TIMER_PUB_H
 
#include <linux/workqueue.h>
 
#define WORKQUEUE_NAME_LENGTH  32
#define S_TO_US_M           1000000
#define TIMER_STEP_MS       100
#define MAX_TASK_NUMS       1024
 
typedef enum {
    TIMER_IRQ = 0,
    COMMON_WORK,
    INDEPENDENCE_WORK,
    HIGH_PRI_WORK,
}timer_task_handler_mode;
 
struct ka_timer_task {
    u32 expire_ms;
    u32 count_ms;
    u64 user_data;
    u32 handler_mode;
    int (*exec_task)(u64 data);
};
 
int ka_timer_task_register(const struct ka_timer_task *task, u32 *node_id);
int ka_timer_task_unregister(u32 node_id);
 
#endif
