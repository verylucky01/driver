/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef QUE_TGT_PROC_H
#define QUE_TGT_PROC_H
#include "urma_api.h"
#include "ascend_hal_define.h"
#include "que_uma.h"
#include "que_comm_agent.h"

struct que_tgt_proc_attr {
    unsigned int devid;
    unsigned int qid;
};
int que_get_ini_log_level(void);
uint64_t que_get_tgt_basetime(unsigned int devid);
void que_update_tgt_basetime(unsigned int devid);
void que_tgt_time_stamp(struct que_tgt_proc *tgt_proc, QUE_TRACE_TGT_TIMESTAMP type);
struct que_tgt_proc *que_tgt_proc_create(struct que_tgt_proc_attr *attr);
void que_tgt_proc_destroy(struct que_tgt_proc *tgt_proc);
void que_tgt_pkt_proc(struct que_tgt_proc *tgt_proc, struct que_pkt *pkt);
void que_tgt_pkt_proc_ex(struct que_tgt_proc *tgt_proc, int cr_status);
int que_mbuf_ctx_init(struct que_tgt_proc *tgt_proc, struct que_rx *rx, struct que_pkt *pkt);
void que_mbuf_ctx_uninit(struct que_rx *rx);
int que_mbuf_ctx_read_post(struct que_tgt_proc *tgt_proc);
int que_mbuf_ctx_wr_fill(struct que_tgt_proc *tgt_proc, struct que_pkt *pkt);
int que_rx_send_ack_and_wait(struct que_tgt_proc *tgt_proc, unsigned long long imm_data, struct que_ack_jfs *ack_send_jfs, unsigned int d2d_flag);
#endif