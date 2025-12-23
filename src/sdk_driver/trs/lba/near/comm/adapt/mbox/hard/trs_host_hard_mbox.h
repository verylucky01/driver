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

#ifndef TRS_HOST_HARD_MBOX_H
#define TRS_HOST_HARD_MBOX_H
#include <linux/types.h>

#include "trs_pub_def.h"

int trs_mbox_config(struct trs_id_inst *inst);
void trs_mbox_deconfig(struct trs_id_inst *inst);

int devdrv_send_rdmainfo_to_ts(u32 devid, const u8 *buf, u32 len, int *result);
int trs_mbox_init(u32 ts_inst_id);
void trs_mbox_uninit(u32 ts_inst_id);
#endif /* TRS_HOST_HARD_MBOX_H */

