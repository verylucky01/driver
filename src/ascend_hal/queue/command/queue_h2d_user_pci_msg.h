/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef QUEUE_H2D_USER_PCI_MSG_H
#define QUEUE_H2D_USER_PCI_MSG_H

#include "ascend_hal.h"
#include "queue_ioctl.h"

struct event_queue_query_result {
    unsigned int last_host_enqueue_time;
    unsigned int last_host_dequeue_time;
};

struct queue_init_msg_para {
    struct queue_common_para comm;
    int hostpid;
};

struct queue_enqueue_para {
    struct queue_common_para comm;
    unsigned int type : 2;
    unsigned int copy_flag : 1;
    unsigned int hostpid : 29;
    unsigned long long buf_len;
    unsigned long long bare_buff;
};

struct queue_sub_para {
    struct queue_common_para comm;
    int pid;
    unsigned int gid;
    unsigned int tid;
    unsigned int event_id       :16;
    unsigned int inner_sub_flag :16;
    unsigned int dst_phy_devid;
};

struct queue_get_status_para {
    struct queue_common_para comm;
    QUEUE_QUERY_ITEM query_item;
    unsigned int out_len;
};

struct queue_finish_callback_para {
    struct queue_common_para comm;
    unsigned int grp_id;
    unsigned int event_id;
};

#endif
