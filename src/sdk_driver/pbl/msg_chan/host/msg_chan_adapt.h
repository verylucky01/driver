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
 
#ifndef MSG_CHAN_ADAPT_H
#define MSG_CHAN_ADAPT_H
#include "comm_kernel_interface.h"

struct devdrv_msg_client {
    const struct devdrv_common_msg_client* comm[DEVDRV_COMMON_MSG_TYPE_MAX];
    struct mutex lock;
};

struct devdrv_msg_client *devdrv_get_msg_client(void);

#endif