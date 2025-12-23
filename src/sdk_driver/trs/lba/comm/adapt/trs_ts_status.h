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
#ifndef TRS_TS_STATUS_H
#define TRS_TS_STATUS_H

#include <linux/types.h>

#include "trs_pub_def.h"
int trs_set_ts_status(struct trs_id_inst *inst, u32 status);
int trs_get_ts_status(struct trs_id_inst *inst, u32 *status);
void trs_ts_status_mng_init(struct trs_id_inst *inst);
void trs_ts_status_mng_exit(struct trs_id_inst *inst);
int trs_get_ts_status_ioctl(void *feature, char *in, u32 in_len, char *out, u32 out_len);
int trs_ts_status_init(u32 ts_inst_id);
void trs_ts_status_uninit(u32 ts_inst_id);

#endif
