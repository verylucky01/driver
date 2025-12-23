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
#include "trs_tscpu_cq.h"
#include "trs_ts_db.h"
#include "trs_chip_def_comm.h"

/**
* @ingroup
* @brief the struct define of report msg when task is completed
*/
typedef struct tag_ts_task_report_msg {
    uint16_t sop : 1; /* start of packet, indicates this is the first 32bit return payload */
    uint16_t mop : 1; /* middle of packet, indicates the payload is a continuation of previous task return payload */
    uint16_t eop : 1; /* *<end of packet, indicates this is the last 32bit return payload.
                     SOP & EOP can appear in the same packet, MOP & EOP can also appear on the same packet. */
    uint16_t package_type : 3;
    uint16_t stream_id : 10;
    uint16_t task_id;
    uint32_t pay_load;
    uint16_t sq_id : 9;
    uint16_t reserved : 6;
    uint16_t phase : 1;
    uint16_t sq_head : 14;
    uint16_t stream_id_ex : 1; /* streamID high bit */
    uint16_t fault_stream_id_ex : 1; /* fault streamID high bit */
} rt_task_report_t; /* Non stars Cq report */

int trs_tscpu_cqe_get_streamid(struct trs_id_inst *inst, void *cqe, u32 *stream_id)
{
    *stream_id = ((rt_task_report_t *)cqe)->stream_id;
    return 0;
}

bool trs_tscpu_cqe_is_valid(struct trs_id_inst *inst, void *cqe, u32 loop)
{
    return (((rt_task_report_t *)cqe)->phase != (loop & 0x1));
}

void trs_tscpu_cqe_get_sq_id(struct trs_id_inst *inst, void *cqe, u32 *sqid)
{
    *sqid = ((rt_task_report_t *)cqe)->sq_id;
}

void trs_tscpu_cqe_get_sq_head(struct trs_id_inst *inst, void *cqe, u32 *sq_head)
{
    *sq_head = ((rt_task_report_t *)cqe)->sq_head;
}

int trs_tscpu_cq_head_update(struct trs_id_inst *inst, u32 cqid, u32 head)
{
    return trs_ring_ts_db(inst, TRS_DB_TASK_CQ, cqid, head);
}

