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

#ifndef URD_INIT_H
#define URD_INIT_H

#include <linux/atomic.h>
#include <linux/types.h>
#include "dms/dms_cmd_def.h"

struct urd_file_private_stru {
    pid_t owner_pid;
    atomic_t work_count;
};

#define MSG_FROM_USER           0
#define MSG_FROM_KERNEL         1
#define MSG_FROM_USER_REST_ACC  2   /* restrict access */
#define MSG_FROM_TYPE_MAX       3

int urd_init(void);
void urd_exit(void);

#endif
