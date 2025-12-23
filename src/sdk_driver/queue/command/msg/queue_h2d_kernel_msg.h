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
#ifndef QUEUE_H2D_KERNEL_MSG_H
#define QUEUE_H2D_KERNEL_MSG_H

#include "queue_command_base.h"

#define REPLY_MSG_QUEUE_STATUS_TYPE_NUM (DEV_END_REPLY - DEV_QUEUE_STATUS_RECORE_START)
#define REPLY_MSG_TIME_RECORD_SIZE (sizeof(long long int) * REPLY_MSG_QUEUE_STATUS_TYPE_NUM)
#define QUEUE_CTRL_MSG_DATA_LEN  (4 * 1024)

/* h2d */
typedef enum {
    QUEUE_CTRL_MSG = 0,
    QUEUE_DATA_MSG,
    QUEUE_CHAN_MSG_MAX,
} QUEUE_CHAN_MSG_TYPE;

struct queue_chan_head {
    u32 devid;
    int hostpid;
    QUEUE_CHAN_MSG_TYPE msg_type;
    struct sched_published_event_info event_info;
    char msg[EVENT_MAX_MSG_LEN];
};

struct queue_chan_dma_info {
    int hostpid;
    u64 serial_num;
    u64 que_chan_addr;
};

struct queue_reply_complete_msg {
    struct queue_chan_dma_info dma_info;
    long long int time_record[REPLY_MSG_QUEUE_STATUS_TYPE_NUM];
    unsigned int qid;
    unsigned int dma_node_num;
};

struct queue_chan_ctrl_msg_mng {
    struct queue_chan_head head;
    char ctrl_data[QUEUE_CTRL_MSG_DATA_LEN];
    u32 ctrl_data_len;
    u32 host_timestamp;
};

#endif
