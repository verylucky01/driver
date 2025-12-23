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
#ifndef TRS_SEC_EH_INIT_H
#define TRS_SEC_EH_INIT_H

#include <linux/types.h>
#include "trs_pub_def.h"

int trs_ts_hw_init(struct trs_id_inst *inst);
void trs_ts_hw_uninit(struct trs_id_inst *inst);
int trs_sec_eh_init(u32 devid);
void trs_sec_eh_unint(u32 devid);

#endif
