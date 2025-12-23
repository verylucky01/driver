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
#ifndef TRS_CHAN_UPDATE_H
#define TRS_CHAN_UPDATE_H

#include "trs_pub_def.h"
#include "trs_res_id_def.h"
#include "trs_adapt.h"

struct trs_res_ops {
    bool (*res_belong_proc[TRS_MAX_ID_TYPE])(struct trs_id_inst *inst, int pid, int res_type, u32 res_id);
    int (*res_get_info[TRS_MAX_ID_TYPE])(struct trs_id_inst *inst, int res_type, u32 res_id, void *info);
};

void trs_res_ops_register(u32 devid, struct trs_res_ops *ops);
void trs_res_ops_unregister(u32 devid);

int trs_mb_update(struct trs_id_inst *inst, int pid, void *data, u32 size);
int trs_chan_ops_sqe_update(struct trs_id_inst *inst, struct trs_sqe_update_info *update_info);
int trs_chan_ops_cqe_update(struct trs_id_inst *inst, int pid, u32 cqid, void *cqe);
int trs_chan_ops_sqe_update_src_check(struct trs_id_inst *inst, struct trs_sqe_update_info *update_info);
int trs_chan_ops_agent_init(struct trs_id_inst *inst);
void trs_chan_ops_agent_uninit(struct trs_id_inst *inst);

int trs_get_res_info(struct trs_id_inst *inst, int res_type, u32 res_id, void *info);
int trs_chan_get_ts_sram_addr(struct trs_id_inst *inst, phys_addr_t *paddr, size_t *size);
#endif
