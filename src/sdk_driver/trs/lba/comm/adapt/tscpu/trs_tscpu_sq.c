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
#include "trs_tscpu_sq.h"
#include "trs_ts_db.h"
#include "trs_chip_def_comm.h"
/**
 * @ingroup
 * @brief the struct define of task
 */
typedef struct tag_ts_command_head {
    uint16_t stream_id;      /* offset 0 */
    uint16_t task_id;        /* offset 2 */
    uint16_t next_task_idx;   /* offset 4 */
    uint16_t type;          /* offset 6 */
    uint16_t next_stream_idx; /* offset 8 */
    uint16_t task_state;     /* 10 */
    uint8_t task_prof_en : 7;     /* offset 12 */
    uint8_t isctrl : 1;
    uint8_t task_info_flag;   /* bit 0: is need send cq, bit 2: endgraph dump, bit 3: sink flag */
    uint8_t reserved[2];
} rt_command_head_t;

int trs_tscpu_sq_tail_update(struct trs_id_inst *inst, u32 sqid, u32 tail)
{
    return trs_ring_ts_db(inst, TRS_DB_TASK_SQ, sqid, tail);
}

int trs_tscpu_get_sq_db_paddr(struct trs_id_inst *inst, u32 sqid, u64 *paddr)
{
    return trs_get_ts_db_paddr(inst, TRS_DB_TASK_SQ, sqid, paddr);
}

int trs_tscpu_get_sq_tail(struct trs_id_inst *inst, u32 sqid, u32 *tail)
{
    return trs_get_ts_db_val(inst, TRS_DB_TASK_SQ, sqid, tail);
}

