/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef QUE_COMM_CHAN_H
#define QUE_COMM_CHAN_H

#include "securec.h"
#include "ascend_hal.h"
#include "ascend_hal_error.h"
#include "uref.h"
#include "que_compiler.h"
#include "queue.h"
#include "que_uma.h"
#include "que_comm_event.h"
#include "que_ini_proc.h"
#include "que_tgt_proc.h"
#include "queue_h2d_user_ub_msg.h"

struct que_chan *_que_chan_create(unsigned int devid, unsigned int qid, QUEUE_CHAN_TYPE chan_type, unsigned long create_time);
void _que_chan_destroy(struct que_chan *chan);
int que_chan_tgt_init(struct que_chan *chan);
void que_chan_tgt_uninit(struct que_chan *chan);
void que_chan_uninit(struct que_chan *chan);
int que_chan_create_check(unsigned int devid, unsigned int qid, unsigned long create_time);
int que_chan_create(unsigned int devid, unsigned int qid, QUEUE_CHAN_TYPE chan_type, unsigned long create_time, unsigned int d2d_flag);
int que_chan_destroy(unsigned int devid, unsigned int qid);
int que_chan_update_jfs_info(unsigned int devid, unsigned int qid, struct que_jfs_pool_info *jfs_pool,
    struct que_jfr *qjfr, urma_jfs_id_t *tjfr_id, urma_token_t token);
void que_get_d2d_flag(unsigned int devid, unsigned int peer_devid, unsigned int *d2d_flag);
int que_qjfs_alloc(struct que_jfs_pool_info *jfs_pool, int timeout, int *idx, unsigned int d2d_flag);
void que_qjfs_free(struct que_jfs_pool_info *jfs_pool, int idx);
int que_qjfr_alloc(struct que_jfr_pool_info *jfr_pool, int timeout, int *idx);
void que_qjfr_free(struct que_jfr_pool_info *jfr_pool, int idx);
int que_chan_done(unsigned int devid, unsigned int qid, QUEUE_AGENT_TYPE que_type);
int que_chan_tgt_recv(unsigned int urma_devid, struct que_jfs *qjfs, struct que_pkt *pkt, unsigned int d2d_flag);
int que_chan_tgt_data_read_and_ack(urma_cr_t *cr);
int que_chan_async_pre_proc(unsigned int devid, unsigned int qid, void *mbuf);
bool que_chan_update_ini_status(unsigned int devid, unsigned int qid, ASYNC_QUE_INI_EVENT event);
int que_chan_inter_dev_ini_proc(unsigned int devid, unsigned int qid);
int que_chan_inter_dev_clear_mbuf(unsigned int devid, unsigned int qid);
#endif