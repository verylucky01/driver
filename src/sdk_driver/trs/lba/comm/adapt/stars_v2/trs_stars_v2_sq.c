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
#include "trs_stars_v2_sq.h"

struct rt_stars_sqe_Header_t {
    uint8_t type : 6;
    uint8_t l1_lock : 1;
    uint8_t l1_unlock : 1;

    uint8_t ie : 1;
    uint8_t pre_p : 1;
    uint8_t post_p : 1;
    uint8_t wr_cqe : 1;
    uint8_t pte_mode : 1;
    uint8_t rtt_mode : 1;
    uint8_t res0 : 2;

    uint16_t block_dim;  // block_dim or res

    uint16_t rt_stream_id;
    uint16_t task_id;
};

void trs_stars_v2_trace_sqe_fill(struct trs_id_inst *inst, struct trs_chan_sq_trace *sq_trace, void *sqe)
{
#ifndef EMU_ST
    struct rt_stars_sqe_Header_t *sqe_ = (struct rt_stars_sqe_Header_t *)sqe;
    sq_trace->type = sqe_->type;
    sq_trace->task_id = sqe_->task_id;
    sq_trace->stream_id = sqe_->rt_stream_id;
#endif
}

