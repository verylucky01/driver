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
#ifndef TRS_CHAN_STARS_V2_OPS_H
#define TRS_CHAN_STARS_V2_OPS_H

#include "trs_pub_def.h"
#include "trs_chan.h"

struct trs_chan_adapt_ops *trs_chan_get_stars_v2_adapt_ops(void);

int trs_chan_stars_v2_ops_init(struct trs_id_inst *inst);
void trs_chan_stars_v2_ops_uninit(struct trs_id_inst *inst);

int trs_chan_ops_agent_init(struct trs_id_inst *inst);
void trs_chan_ops_agent_uninit(struct trs_id_inst *inst);

int trs_stars_v2_chan_ops_request_irq(struct trs_id_inst *inst, u32 irq_type, int irq_index, void *para,
                                      int (*handler)(int irq_type, int irq_index, void *para, u32 cqid[], u32 cq_num));
int trs_stars_v2_chan_ops_free_irq(struct trs_id_inst *inst, int irq_type, int irq_index, void *para);
int trs_chan_stars_v2_ops_ctrl_sqcq(struct trs_id_inst *inst, struct trs_chan_type *types, u32 id, u32 cmd, u32 para);
int trs_chan_stars_v2_ops_query_sqcq(struct trs_id_inst *inst, struct trs_chan_type *types, u32 id, u32 cmd,
                                     u64 *value);
void trs_chan_stars_v2_update_ssid(struct trs_id_inst *inst, struct trs_chan_info *chan_info);
struct trs_chan_adapt_ops *trs_chan_get_stars_v2_adapt_ops(void);
int trs_chan_stars_v2_ops_init(struct trs_id_inst *inst);
void trs_chan_stars_v2_ops_uninit(struct trs_id_inst *inst);
#endif /* TRS_CHAN_STARS_V2_OPS_H */

