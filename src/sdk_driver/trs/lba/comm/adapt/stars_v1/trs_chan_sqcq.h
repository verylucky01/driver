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

#ifndef TRS_CHAN_SQCQ_H
#define TRS_CHAN_SQCQ_H

#include <linux/types.h>
#include "trs_pub_def.h"

int trs_chan_ops_get_valid_cq_list(struct trs_id_inst *inst, u32 group, u32 cqid[], u32 cq_id_num, u32 *valid_cq_num);
void trs_chan_ops_intr_mask_config(struct trs_id_inst *inst, u32 group, u32 irq, int val);
void trs_chan_ops_get_sq_head_in_sqe(struct trs_id_inst *inst, void *sqe, u32 *sq_head);
bool trs_chan_ops_cqe_is_valid(struct trs_id_inst *inst, void *cqe, u32 loop);
int trs_stars_chan_ops_ctrl_sqcq(struct trs_id_inst *inst, u32 id, u32 cmd, u32 para);
int trs_stars_chan_ops_query_sqcq(struct trs_id_inst *inst, u32 id, u32 cmd, u64 *value);
void trs_chan_ops_get_sq_head_in_cqe(struct trs_id_inst *inst, void *cqe, u32 *sq_head);
#endif

