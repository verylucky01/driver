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
#ifndef TRS_STARS_V2_CQ_H
#define TRS_STARS_V2_CQ_H

#include <linux/types.h>

#include "trs_pub_def.h"
#include "trs_core.h"
#include "trs_chan.h"

#define TRS_STARS_CQE_HOLDER    3
#define TRS_STARS_CQE_RECORED   4

typedef struct {
    uint16_t task_id;
    uint16_t stream_id : 12; /* 0~4096 for stream_id */
    uint16_t result : 1;
    uint16_t rsv : 3;
} rt_stars_model_execute_error_t;

typedef union {
    rt_stars_model_execute_error_t model_exec;
    uint32_t value;
} rt_stars_cqe_sw_status_t;

typedef struct tag_stars_cqe_error_info {
    uint8_t error_code;
    uint8_t notify_attached : 1;
    uint8_t next_task_vld : 1;
    uint8_t sqe_type : 6;
    uint16_t sqe_index;
    rt_stars_cqe_sw_status_t sq_sw_status;
} rt_stars_cqe_error_info_t;

/**
* @ingroup
* @brief the struct define of cqe when task is completed
*/
typedef struct tag_stars_cqe_sys_cnt {
    uint32_t syscnt_low;
    uint32_t syscnt_high;
} rt_stars_cqe_sys_cnt_t;

typedef struct tag_stars_cqe_dvpp_info {
    uint8_t error_code;
    uint8_t rsv0 : 1;
    uint8_t rsv1 : 1;
    uint8_t sqe_type : 6;
    uint16_t rsv2;

    uint16_t status;
    uint16_t cmdlst_id;
    uint32_t int_err;
} rt_stars_cqe_dvpp_info_t;

typedef union tag_stars_cqe_status {
    rt_stars_cqe_sys_cnt_t sys_cnt;
    rt_stars_cqe_error_info_t error_info;
    rt_stars_cqe_dvpp_info_t dvpp_info;
} rt_stars_cqe_status_t;

typedef struct tag_stars_cqe {
    uint16_t phase : 1;
    uint16_t warn : 1;          /* process warning */
    uint16_t evt : 1;           /* event record flag */
    uint16_t place_hold : 1;
    uint16_t sq_id : 11;
    uint16_t error_bit : 1;
    uint16_t sq_head;

    uint16_t stream_id;
    uint16_t task_id;

    rt_stars_cqe_status_t cqe_status;
} rt_stars_cqe_t;

int trs_stars_v2_cq_get_valid_list(struct trs_id_inst *inst, u32 group, u32 cqid[], u32 num, u32 *valid_num);
int trs_stars_v2_cqe_get_streamid(struct trs_id_inst *inst, void *cqe, u32 *stream_id);
bool trs_stars_v2_cqe_is_valid(struct trs_id_inst *inst, void *cqe, u32 loop);
void trs_stars_v2_cqe_get_sqid(struct trs_id_inst *inst, void *cqe, u32 *sqid);
void trs_stars_v2_cqe_get_sq_head(struct trs_id_inst *inst, void *cqe, u32 *sq_head);
void trs_stars_v2_cqe_to_logic_cqe(void *hw_cqe, struct trs_logic_cqe *logic_cqe);
void trs_stars_v2_trace_cqe_fill(struct trs_id_inst *inst, struct trs_chan_cq_trace *cq_trace, void *cqe);
u16 trs_stars_v2_cqe_get_match_flag(rt_stars_cqe_t *cqe);
#endif /* TRS_STARS_V2_CQ_H */

