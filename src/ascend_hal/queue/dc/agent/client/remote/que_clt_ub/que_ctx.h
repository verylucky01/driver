/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef QUE_CTX_H
#define QUE_CTX_H

#include "urma_api.h"
#include "ascend_hal_define.h"
#include "ascend_hal_external.h"
#include "que_ub_msg.h"
#include "que_jetty.h"
#include "que_comm_ctx.h"
#include "que_comm_event.h"

struct queue_sub_flag {
    bool en_que;
    bool f2nf;
};

int que_ctx_create(unsigned int devid, pid_t hostpid, pid_t devpid);
void que_ctx_destroy(unsigned int devid);

int que_ctx_get_pids(unsigned int devid, pid_t *hostpid, pid_t *devpid);

int que_ctx_enque(unsigned int devid, unsigned int qid, struct buff_iovec *vector, int timeout);
int que_ctx_deque(unsigned int devid, unsigned int qid, struct buff_iovec *vector, int timeout);
drvError_t que_h2d_info_fill(unsigned int dev_id, struct que_event_attr *attr, struct que_init_in_msg *in);
drvError_t que_ctx_h2d_init(unsigned int devid, urma_jfr_id_t *tjfr_id, urma_token_t *token);
bool que_is_need_sync_wait(unsigned int devid, unsigned int qid, unsigned int subevent_id);
int que_sub_status_set(struct QueueSubPara *sub_para, unsigned int devid, unsigned int qid);
int que_unsub_status_set(struct QueueUnsubPara *unsub_para, unsigned int devid, unsigned int qid);

int queue_clt_init_check(unsigned int devid);
#endif