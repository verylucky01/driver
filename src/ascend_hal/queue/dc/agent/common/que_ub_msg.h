/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef QUE_UB_MSG_H
#define QUE_UB_MSG_H

#include "urma_api.h"

#include "ascend_hal_define.h"
#include "ascend_hal_external.h"
#include "queue_ioctl.h"
#include "que_uma.h"
#include "ascend_hal_external.h"
#include "que_comm_agent.h"
#include "queue_h2d_user_ub_msg.h"

static inline unsigned int _que_get_node_num(void)
{
    return (QUE_UMA_MAX_SEND_SIZE - sizeof(struct que_pkt)) / sizeof(struct que_node);
}

static inline unsigned long long _que_get_pkt_size(unsigned long long iovec_num)
{
    return sizeof(struct que_pkt) + iovec_num * sizeof(struct que_node);
}

#define QUE_ACK_PKT_SIZE  4096
#define QUE_SEND_PKT_DEPTH  64
#define QUE_ACK_PKT_DEPTH  64
#define QUE_PKT_SEND_JETTY_POOL_DEPTH 16

struct que_peek_out_msg {
    unsigned long long buf_len;
};

struct que_tgt_proc_ack_msg { /* max 8 byte */
    int qid       : 16;
    int result    : 16;
    int sn        : 8;
    int tgt_time  : 24; 
};

typedef union {
    unsigned long long imm_data;
    struct  que_tgt_proc_ack_msg ack_msg;
} que_ack_data;

#endif
