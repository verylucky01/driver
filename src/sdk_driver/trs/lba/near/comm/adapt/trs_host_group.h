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
#ifndef TRS_HOST_GROUP_H
#define TRS_HOST_GROUP_H
#include "trs_pub_def.h"

int trs_add_group(struct trs_id_inst *inst, u32 grp_id);
int trs_delete_group(struct trs_id_inst *inst, u32 grp_id);
bool trs_get_sqcq_change_flag(u32 devid, u32 slice);
int trs_sqcq_change_flag_init(struct trs_id_inst *inst, u32 grp_id, u32 vfid);
int trs_host_group_init(u32 ts_inst_id);
void trs_host_group_uninit(u32 ts_inst_id);

int trs_get_vfid_by_grp_id(u32 devid, u32 grp_id, u32 *vfid);
void trs_sqcq_change_flag_uninit(struct trs_id_inst *inst, u32 grp_id, u32 vfid);
int trs_stars_init_with_group(struct trs_id_inst *inst, u32 vfid);
void trs_stars_uninit_with_group(struct trs_id_inst *inst, u32 vfid);
#endif