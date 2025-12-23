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
#ifndef TRS_STARS_FUNC_ADAPT_H
#define TRS_STARS_FUNC_ADAPT_H

#include <linux/types.h>
#include "trs_pub_def.h"

int trs_init_cnt_notify_tbl(struct trs_id_inst *inst);
void trs_uninit_cnt_notify_tbl(struct trs_id_inst *inst);
int trs_stars_func_cnt_notify_id_ctrl(struct trs_id_inst *inst, u32 id, u32 cmd);
int trs_stars_func_res_id_ctrl(struct trs_id_inst *inst, u32 type, u32 id, u32 cmd);
int trs_stars_ops_func_res_id_ctrl(struct trs_id_inst *inst, u32 type, u32 id, u32 cmd);
int trs_stars_notifier_register(void);
void trs_stars_notifier_unregister(void);
struct trs_stars_ops *trs_stars_func_op_get(void);
int trs_stars_func_notify_id_reset(struct trs_id_inst *inst, u32 id);
#endif
