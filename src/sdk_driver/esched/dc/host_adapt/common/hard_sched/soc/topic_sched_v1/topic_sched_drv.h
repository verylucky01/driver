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
#include "topic_sched_v1/topic_sched_drv_shmem.h"

#define TOPIC_SCHED_SQE_TYPE 1

#define TOPIC_SCHED_ESCHED_COMM_PID 0

/* For details about the maximum specifications, see tsdrv_kernel_common.h
 * (DEVDER_MAX_SQ_DEPTH or DEVDER_MAX_CQ_DEPTH) */
#define TOPIC_SCHED_TASK_SUBMIT_SQ_DEPTH (2 * 1024)
/* In HCCS connection, virtual machine, cq size should be less or equal 4096 to ensure phy mem is contiguous */
#define TOPIC_SCHED_TASK_SUBMIT_CQ_DEPTH (256)
#define TOPIC_SCHED_TASK_SQE_SIZE (64)
#define TOPIC_SCHED_TASK_CQE_SIZE (16)

/* CLOUDV2 host chan num: AICPU 64, CCPU 16, total 80
   CLOUDV2 device chan num: AICPU 8, CCPU 1, total 9
   MINIV3 host chan num:  AICPU 0, CCPU 1, total 1
   MINIV3 device chan num:  AICPU 4, CCPU 1, total 5 */
#define TOPIC_SCHED_MAX_CHAN_NUM 80
#define TOPIC_SCHED_HOST_AICPU_CHAN_NUM 64

#define TOPIC_SCHED_QOS_COMPRESS_RATE 1

#endif

