/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef PROF_EVENT_COMM_H
#define PROF_EVENT_COMM_H
#include "prof_common.h"

struct prof_event_msg {
    char *msg;
    size_t msg_len;
};

struct prof_event_para {
    uint32_t dev_id;
    uint32_t event_id;
    uint32_t subevent_id;
    uint32_t remote_pid;
    struct prof_event_msg msg_send;
    struct prof_event_msg msg_recv;
};

STATIC_INLINE void prof_event_event_para_comm_pack(struct prof_event_para *event_para, 
    uint32_t dev_id, uint32_t event_id, uint32_t subevent_id, uint32_t remote_pid)
{
    event_para->dev_id = dev_id;
    event_para->event_id = event_id;
    event_para->subevent_id = subevent_id;
    event_para->remote_pid = remote_pid;
}

STATIC_INLINE void prof_event_event_para_msg_send_pack(struct prof_event_para *event_para,
    char *msg, size_t msg_len)
{
    event_para->msg_send.msg = msg;
    event_para->msg_send.msg_len = msg_len;
}

STATIC_INLINE void prof_event_event_para_msg_recv_pack(struct prof_event_para *event_para,
    char *msg, size_t msg_len)
{
    event_para->msg_recv.msg = msg;
    event_para->msg_recv.msg_len = msg_len;
}

drvError_t prof_event_submit_event_sync(struct prof_event_para *para);

#endif