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
#ifndef TRS_UB_INIT_H
#define TRS_UB_INIT_H

#include "trs_pub_def.h"
#include "trs_msg.h"
#ifdef TRS_HOST
#include "trs_chan_irq.h"

int trs_ub_get_sq_head_paddr(struct trs_id_inst *inst, u32 sq_id, u64 *paddr);
int trs_ub_get_sq_tail_paddr(struct trs_id_inst *inst, u32 sq_id, u64 *paddr);
int trs_ub_query_reset_sq_head_paddr(struct trs_id_inst *inst, u32 sq_id, u64 *paddr);
int trs_ub_query_reset_sq_tail_paddr(struct trs_id_inst *inst, u32 sq_id, u64 *paddr);
int trs_ub_get_sq_head(struct trs_id_inst *inst, u32 sq_id, u32 *head);
int trs_ub_get_sq_tail(struct trs_id_inst *inst, u32 sq_id, u32 *tail);
int trs_ub_get_cq_head(struct trs_id_inst *inst, u32 cq_id, u32 *head);
int trs_ub_get_cq_tail(struct trs_id_inst *inst, u32 cq_id, u32 *tail);
int trs_ub_set_cq_head(struct trs_id_inst *inst, u32 cq_id, u32 head);
int trs_ub_cq_reset(struct trs_id_inst *inst, u32 cq_id);
int trs_ub_sq_reset(struct trs_id_inst *inst, u32 sq_id);
int trs_ub_cq_pause(struct trs_id_inst *inst, u32 cq_id);
int trs_ub_cq_resume(struct trs_id_inst *inst, u32 cq_id);
int trs_ub_sqcq_info_update(struct trs_id_inst *inst, struct trs_chan_info *chan_info);
int trs_ub_request_cq_update_irq(struct trs_id_inst *inst, int irq_type, int irq_index, struct trs_chan_irq_attr *attr);
int trs_ub_free_cq_update_irq(struct trs_id_inst *inst, int irq_type, int irq_index, void *para);
#else
int trs_agent_ub_jetty_init(u32 devid, struct trs_msg_data *msg);
int trs_agent_ub_jetty_uninit(u32 devid, struct trs_msg_data *msg);
#endif

#endif /* TRS_UB_INIT_H */

