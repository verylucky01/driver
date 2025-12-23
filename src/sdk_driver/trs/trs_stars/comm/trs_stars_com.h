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
#ifndef TRS_STARS_COM_H
#define TRS_STARS_COM_H

#include "pbl_uda.h"
#include "trs_pub_def.h"

struct trs_stars_ops {
    int (*res_id_ctrl)(struct trs_id_inst *inst, u32 type, u32 id, u32 cmd);
    int (*get_id_status)(struct trs_id_inst *inst, u32 type, u32 id, u32 *status);
    void (*set_sq_head)(struct trs_id_inst *inst, u32 sqid, u32 val);
    u32 (*get_sq_head)(struct trs_id_inst *inst, u32 sqid);
    void (*set_sq_tail)(struct trs_id_inst *inst, u32 sqid, u32 val);
    u32 (*get_sq_tail)(struct trs_id_inst *inst, u32 sqid);
    u32 (*get_sq_fsm_status)(struct trs_id_inst *inst, u32 sqid);
};
int trs_stars_notifier_func(u32 udevid, enum uda_notified_action action);
int trs_stars_soc_res_ctrl(struct trs_id_inst *inst, u32 type, u32 id, u32 cmd);
int trs_stars_soc_get_id_status(struct trs_id_inst *inst, u32 type, u32 id, u32 *status);
int trs_stars_soc_set_sq_head(struct trs_id_inst *inst, u32 sqid, u32 val);
int trs_stars_soc_set_sq_tail(struct trs_id_inst *inst, u32 sqid, u32 val);
int trs_stars_soc_get_sq_fsm_status(struct trs_id_inst *inst, u32 sqid, u32 *val);
int trs_stars_soc_init(u32 phy_devid);
void trs_stars_soc_uninit(u32 phy_devid);
int init_trs_stars(void);
void exit_trs_stars(void);
int trs_stars_soc_get_sq_head(struct trs_id_inst *inst, u32 sqid, u32 *val);
int trs_stars_soc_get_sq_tail(struct trs_id_inst *inst, u32 sqid, u32 *val);
#endif
