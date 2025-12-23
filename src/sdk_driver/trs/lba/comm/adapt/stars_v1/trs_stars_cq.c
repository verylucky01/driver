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
#include "trs_stars.h"
#include "trs_timestamp.h"
#include "trs_stars_cq.h"

#define LOGIC_CQE_STREAM_ID_MASK 0x7fff

int trs_stars_cqe_get_streamid(struct trs_id_inst *inst, void *cqe, u32 *stream_id)
{
    *stream_id = ((rt_stars_cqe_t *)cqe)->stream_id & LOGIC_CQE_STREAM_ID_MASK;
    return 0;
}

bool trs_stars_cqe_is_valid(struct trs_id_inst *inst, void *cqe, u32 loop)
{
    return (((rt_stars_cqe_t *)cqe)->phase != (loop & 0x1));
}

void trs_stars_trace_cqe_fill(struct trs_id_inst *inst, struct trs_chan_cq_trace *cq_trace, void *cqe)
{
    rt_stars_cqe_t *cqe_ = (rt_stars_cqe_t *)cqe;
    cq_trace->task_id = cqe_->task_id;
    cq_trace->stream_id = cqe_->stream_id;
    cq_trace->sq_id = cqe_->sq_id;
    cq_trace->sq_head = cqe_->sq_head;
}

void trs_stars_cqe_get_sqid(struct trs_id_inst *inst, void *cqe, u32 *sqid)
{
    *sqid = ((rt_stars_cqe_t *)cqe)->sq_id;
}

void trs_stars_cqe_get_sq_head(struct trs_id_inst *inst, void *cqe, u32 *sq_head)
{
    *sq_head = ((rt_stars_cqe_t *)cqe)->sq_head;
}

int trs_stars_cq_head_update(struct trs_id_inst *inst, u32 cqid, u32 head)
{
    return trs_stars_set_cq_head(inst, cqid, head);
}

int trs_stars_cq_get_valid_list(struct trs_id_inst *inst, u32 group, u32 cqid[], u32 num, u32 *valid_num)
{
    return trs_stars_get_valid_cq_list(inst, group, cqid, num, valid_num);
}

static inline u64 trs_stars_cq_get_timestamp(rt_stars_cqe_t *cqe)
{
    u64 timestamp;

    timestamp = (u64)(((u64)cqe->cqe_status.sys_cnt.syscnt_high) <<
        (sizeof(cqe->cqe_status.sys_cnt.syscnt_low) * BITS_PER_BYTE));
    timestamp |= (u64)cqe->cqe_status.sys_cnt.syscnt_low;

    return timestamp;
}

static inline u8 trs_stars_cq_get_sqe_type(rt_stars_cqe_t *cqe)
{
    return cqe->cqe_status.error_info.sqe_type;
}

static inline u8 trs_stars_cq_get_warn_type(rt_stars_cqe_t *cqe)
{
    return (u8)(cqe->warn << 6); /* cqe warn offset: 6 */
}

static inline u8 trs_stars_cq_get_err_type(rt_stars_cqe_t *cqe)
{
    return (u8)((cqe->cqe_status.sys_cnt.syscnt_low & 0x3F) | trs_stars_cq_get_warn_type(cqe));
}

static inline u8 trs_stars_cq_get_drop_flag(rt_stars_cqe_t *cqe)
{
    return cqe->cqe_status.error_info.drop_flag;
}

static inline u32 trs_stars_cq_get_err_code(rt_stars_cqe_t *cqe)
{
    return cqe->cqe_status.sys_cnt.syscnt_high;
}

static inline u16 trs_stars_cq_get_acc_error(rt_stars_cqe_t *cqe)
{
    return cqe->cqe_status.error_info.acc_error;
}

static inline u16 trs_stars_cq_get_sqe_index(rt_stars_cqe_t *cqe)
{
    return cqe->cqe_status.error_info.sqe_index;
}

static inline bool trs_stars_cq_has_syscnt(rt_stars_cqe_t *cqe)
{
    return (((cqe->evt == 1) || (cqe->place_hold == 1)) && (cqe->error_bit == 0));
}

u16 trs_stars_cqe_get_match_flag(rt_stars_cqe_t *cqe)
{
    /* The bit15 is 1 means match_type cqe */
    return (cqe->stream_id & 0x8000) == 0x8000 ? 1 : 0; /* 0x8000 */
}

void trs_stars_cqe_to_logic_cqe(void *hw_cqe, struct trs_logic_cqe *logic_cqe)
{
    rt_stars_cqe_t *cqe = (rt_stars_cqe_t *)hw_cqe;
    logic_cqe->drop_flag = 0;

    if (trs_stars_cq_has_syscnt(cqe)) {
        logic_cqe->timestamp = trs_stars_cq_get_timestamp(cqe);
        logic_cqe->sqe_type = (cqe->evt != 0) ? TRS_STARS_CQE_RECORED : TRS_STARS_CQE_HOLDER;
        logic_cqe->error_code = 0;
        /* No error, get warning */
        logic_cqe->error_type = trs_stars_cq_get_warn_type(cqe);
    } else {
        logic_cqe->sqe_index = trs_stars_cq_get_sqe_index(cqe);
        logic_cqe->sqe_type = trs_stars_cq_get_sqe_type(cqe);
        logic_cqe->error_code = trs_stars_cq_get_err_code(cqe);
        logic_cqe->error_type = trs_stars_cq_get_err_type(cqe);
        logic_cqe->drop_flag = trs_stars_cq_get_drop_flag(cqe);
    }
    logic_cqe->stream_id = cqe->stream_id & LOGIC_CQE_STREAM_ID_MASK;
    logic_cqe->match_flag = trs_stars_cqe_get_match_flag(cqe);
    logic_cqe->task_id = cqe->task_id;
    logic_cqe->sq_id = cqe->sq_id;
    logic_cqe->sq_head = cqe->sq_head;
    logic_cqe->error_bit = cqe->error_bit;
    logic_cqe->acc_error = trs_stars_cq_get_acc_error(cqe);
    if (logic_cqe->match_flag != 0) {
        logic_cqe->enque_timestamp = trs_get_s_timestamp();
    }
}

