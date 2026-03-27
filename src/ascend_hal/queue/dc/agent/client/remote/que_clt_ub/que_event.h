/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef QUE_EVENT_H
#define QUE_EVENT_H

#include <sys/types.h>
#include "que_comm_event.h"

int que_event_send(struct que_event_attr *attr, struct que_event_msg *msg, int timeout_ms);
int que_event_send_ex(unsigned int devid, unsigned int retry_flg, unsigned int sub_event, struct que_event_msg *msg, int timeout_ms);
int que_clt_send_event_with_wait(unsigned int devid, unsigned int qid, unsigned int sub_event,
    struct que_event_msg *msg, int timeout_ms);
int que_event_sum_init(struct que_event_attr *attr, struct que_event_msg *msg, struct event_summary *event_sum);
void que_event_sum_uninit(struct event_summary *event);
void que_init_grpid(void);
void que_init_grpid_by_dev(int devid);
#endif
