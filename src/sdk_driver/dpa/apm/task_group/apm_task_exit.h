/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025. All rights reserved.
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
#ifndef APM_TASK_EXIT_H
#define APM_TASK_EXIT_H

#include "ka_dfx_pub.h"
#include "ka_task_pub.h"

#include "dpa/dpa_apm_kernel.h"

#define EXIT_CHECK_SCOPE_STR    "EXIT_CHECK_SCOPE"

enum {
    EXIT_CHECK_SCOPE,
};

void apm_task_exit(int tgid, ka_blocking_notifier_head_t *nh,
    bool (*is_tasks_in_group_exit_synchronized)(int tgid, enum apm_exit_stage stage));

static inline unsigned long apm_get_exit_val(int stage, int tgid, bool is_force_exit)
{
    return ((((unsigned long)is_force_exit) << APM_FORCE_EXIT_FLAG_OFFSET) |
            ((unsigned long)(stage) << APM_STAGE_OFFSET) | ((tgid) & 0xFFFFFFFF));
}

bool apm_is_all_task_exit_finish(void);
void apm_task_exit_check_work(ka_work_struct_t *p_work);
int apm_task_exit_init(void);
void apm_task_exit_uninit(void);

#endif
