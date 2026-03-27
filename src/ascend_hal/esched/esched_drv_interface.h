/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef ESCHED_DRV_INTERFACE_H
#define ESCHED_DRV_INTERFACE_H

/* 0xFF means task will not timeout and STARS don't report timeout interrupt to TSFW */
#define TOPIC_SCHED_SQE_KERNEL_CREDIT 100
 
/* custom process kernel type config */
#define TOPIC_SCHED_CUSTOM_KERNEL_TYPE     4
#define TOPIC_SCHED_CUSTOM_KFC_KERNEL_TYPE 6
#define TOPIC_SCHED_DEFAULT_KERNEL_TYPE    127

/* The RTSQ priority ranges from 0 to 7, 0 is the highest priority. */
#define TOPIC_SCHED_RTSQ_PRI          4

#define TOPIC_SCHED_ACPU_POOL_ID 0

enum esched_topic_type {
    TOPIC_TYPE_AICPU_DEVICE_ONLY = 0,
    TOPIC_TYPE_AICPU_DEVICE_FIRST = 1,
    TOPIC_TYPE_AICPU_HOST_ONLY = 2,
    TOPIC_TYPE_AICPU_HOST_FIRST = 3,
    TOPIC_TYPE_CCPU_HOST = 4,
    TOPIC_TYPE_DCPU_DEVICE = 5,
    TOPIC_TYPE_CCPU_DEVICE = 6,
    TOPIC_TYPE_TSCPU = 7,
    TOPIC_TYPE_DVPPCPU = 8,
    TOPIC_TYPE_AICPU_DEVICE_SRC_PID = 10, /* device aicpu task with src_pid */
    TOPIC_TYPE_MAX
};

struct topic_trs_chan_ext_info {
    u8  mb_spec;
    u8  mb_specid;
};

struct sched_trs_chan_ext_msg {
    struct trs_ext_info_header msg_header;
    struct topic_trs_chan_ext_info topic_ext_info;
};

#endif
