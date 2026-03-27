/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef QUE_COMM_CTX_H
#define QUE_COMM_CTX_H

#include "uref.h"
#include "que_ub_msg.h"
#include "esched_user_interface.h"
#include "que_comm_event.h"

enum ctx_cnt_type {
    RECV_CQE_TOTAL,
    RECV_CQE_FAIL,
    WAIT_JFC_SUCCESS,
    WAIT_JFC_FAIL,
    POLL_SEND_JFC_SUCCESS,
    POLL_SEND_JFC_FAIL,
    CTX_CNT_MAX,
};

struct que_ctx {
    unsigned int devid;
    unsigned int res_bitmap;

    pid_t devpid;  /* host h2d only */
    pid_t hostpid; /* host h2d only */
    urma_target_jetty_t *tjetty; /* host h2d only */
    struct queue_sub_flag *sub_flag; /* host h2d only */
    urma_token_t token[TRANS_TYPE_MAX];

    struct que_f2nf_res f2nf_res; /* async que only */

    /* The following part is public */
    struct que_jfs_pool_info jfs_pool[TRANS_TYPE_MAX][QUE_PKT_SEND_JETTY_POOL_DEPTH];
    struct que_jfr_pool_info jfr_pool[TRANS_TYPE_MAX][QUE_PKT_SEND_JETTY_POOL_DEPTH];
    struct que_jfs *ack_send_jetty[TRANS_TYPE_MAX];
    struct que_jfr *pkt_recv_jetty[TRANS_TYPE_MAX];
    struct que_recv_para *recv_para[TRANS_TYPE_MAX];
    unsigned int cnt[CTX_CNT_MAX];
    urma_jfc_t *jfc[TRANS_TYPE_MAX];
    urma_cr_t *cr[TRANS_TYPE_MAX];
    struct uref ref;
};

int que_ctx_init(struct que_ctx *ctx);
void que_ctx_uninit(struct que_ctx *ctx);
void que_ctx_chan_recycle(unsigned int devid, struct que_query_alive_msg *qid_list);
int que_ctx_chan_check(unsigned int devid, unsigned int qid, unsigned long create_time);
int que_ctx_chan_create(unsigned int devid, unsigned int qid, QUEUE_CHAN_TYPE chan_type, unsigned long create_time, unsigned int d2d_flag);
int que_ctx_chan_destroy(unsigned int devid, unsigned int qid);
int que_ctx_chan_update(unsigned int devid, unsigned int peer_devid, unsigned int qid, urma_jfr_id_t *tjfr_id, urma_token_t *token);
int que_ctx_get_f2nf_res(unsigned int devid, unsigned int qid, struct que_f2nf_res *f2nf_res);
void que_ctx_poll(unsigned int devid, unsigned int d2d_flag);
int que_ctx_async_ini(unsigned int devid, unsigned int qid, void *mbuf);
void que_ctx_wait_f2nf(unsigned int devid);
int que_ctx_bulid_f2nf_event(unsigned int devid);
drvError_t que_clt_query_que_alive(unsigned int dev_id, struct que_query_alive_msg *qid_list);

#endif
