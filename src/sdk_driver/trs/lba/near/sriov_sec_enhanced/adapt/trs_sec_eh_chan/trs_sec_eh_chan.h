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
#ifndef TRS_SEC_EH_CHAN_H
#define TRS_SEC_EH_CHAN_H

#include "trs_pub_def.h"

int trs_chan_config(struct trs_id_inst *inst);
void trs_chan_deconfig(struct trs_id_inst *inst);
int trs_chan_init(u32 ts_inst_id);
void trs_chan_uninit(u32 ts_inst_id);

int trs_sec_eh_chan_sqe_update(struct trs_id_inst *inst, struct trs_sqe_update_info *update_info);
int trs_sec_eh_chan_stars_ops_query_sqcq(struct trs_id_inst *inst, struct trs_chan_type *types, u32 id,
                                         u32 cmd, u64 *value);
#endif