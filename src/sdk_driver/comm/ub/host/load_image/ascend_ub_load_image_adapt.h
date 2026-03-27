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
#ifndef ASCEND_UB_LOAD_IMAGE_ADAPT_H
#define ASCEND_UB_LOAD_IMAGE_ADAPT_H

#include "cis.h"
#include "securec.h"

typedef struct ubdrv_uvb_msg_ctrl {
    u32 call_id;
    u32 receiver_id;
    int (*func)(struct cis_message *msg);
} ubdrv_uvb_msg_ctrl_t;

int ubdrv_load_device_init(void);
void ubdrv_load_device_uninit(void);

#endif