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
#ifndef TOPIC_SCHED_DRV_H
#define TOPIC_SCHED_DRV_H

#include <linux/types.h>
#include "ascend_kernel_hal.h"
#include "topic_sched_drv_interface.h"

/* For details about the maximum specifications, see tsdrv_kernel_common.h
 * (DEVDER_MAX_SQ_DEPTH or DEVDER_MAX_CQ_DEPTH) */
#define TOPIC_SCHED_TASK_SUBMIT_SQ_DEPTH (2 * 1024 - 7) // stars restrictions (depth - 1) % 2^3 = 0
#define TOPIC_SCHED_TASK_SUBMIT_CQ_DEPTH (1024 - 7) // stars restrictions (depth - 1) % 2^3 = 0

#define TOPIC_SCHED_MAX_CHAN_NUM 64
#define TOPIC_SCHED_HOST_AICPU_CHAN_NUM 64

#define TOPIC_SCHED_QOS_COMPRESS_RATE 2

struct topic_sched_cqe {
    u16 status;
    u16 err_code; /* 0: normal; 1:exception. */

    u32 res0;
    u32 res1;
    u32 res2;
};

#endif
