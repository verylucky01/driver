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

#ifndef __TRS_SEC_EH_AGENT_DEV_INIT_H__
#define __TRS_SEC_EH_AGENT_DEV_INIT_H__

#include "trs_pub_def.h"

int trs_sec_eh_ts_init(struct trs_id_inst *inst);
void trs_sec_eh_ts_uninit(struct trs_id_inst *inst);

#endif

