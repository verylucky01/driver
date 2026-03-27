/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef QUE_COMM_THREAD_H
#define QUE_COMM_THREAD_H

#include <stdlib.h>

#include "securec.h"
#include "ascend_hal.h"
#include "ascend_hal_error.h"

typedef enum que_thread_type {
    QUEUE_POLL,
    QUEUE_POLL_D2D,
    QUEUE_WAIT_F2NF,
    QUEUE_RECYCLE,
    QUEUE_THREAD_BUTT,
} QUEUE_THREAD_TYPE;

int que_create_poll_thread(unsigned int devid);
int que_create_wait_f2nf_thread(unsigned int devid);
int que_create_recycle_thread(unsigned int devid);
void que_thread_cancle(unsigned int devid);
int que_create_poll_d2d_thread(unsigned int devid);
void que_cancle_poll_d2d_thread(unsigned int devid);
#endif
