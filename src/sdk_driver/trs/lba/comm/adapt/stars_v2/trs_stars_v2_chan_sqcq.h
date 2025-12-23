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

#ifndef TRS_STARS_V2_CHAN_SQCQ_H
#define TRS_STARS_V2_CHAN_SQCQ_H

#include <linux/types.h>
#include "trs_pub_def.h"

bool trs_stars_v2_chan_ops_cqe_is_valid(struct trs_id_inst *inst, void *cqe, u32 loop);
void trs_stars_v2_chan_ops_get_sq_head_in_cqe(struct trs_id_inst *inst, void *cqe, u32 *sq_head);
int trs_stars_v2_chan_ops_ctrl_sqcq(struct trs_id_inst *inst, u32 id, u32 cmd, u32 para);
int trs_stars_v2_chan_ops_query_sqcq(struct trs_id_inst *inst, u32 id, u32 cmd, u64 *value);
int trs_stars_chan_ops_ctrl_sqcq(struct trs_id_inst *inst, u32 id, u32 cmd, u32 para);
int trs_stars_chan_ops_query_sqcq(struct trs_id_inst *inst, u32 id, u32 cmd, u64 *value);
int trs_stars_v2_chan_ops_get_valid_cq_list(struct trs_id_inst *inst, u32 group, u32 cqid[], u32 cq_id_num,
    u32 *valid_cq_num);
void trs_stars_v2_chan_ops_intr_mask_config(struct trs_id_inst *inst, u32 group, u32 irq, int val);
int trs_stars_v2_chan_ops_get_sq_head_paddr(struct trs_id_inst *inst, u32 sqid, u64 *paddr);
int trs_stars_v2_chan_ops_get_sq_tail_paddr(struct trs_id_inst *inst, u32 sqid, u64 *paddr);
#endif

