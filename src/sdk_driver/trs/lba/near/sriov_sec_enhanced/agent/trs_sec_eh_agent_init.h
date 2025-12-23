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

#ifndef TRS_SEC_EH_AGENT_INIT_H
#define TRS_SEC_EH_AGENT_INIT_H

#include <linux/types.h>
#include "trs_pub_def.h"

void trs_sec_get_cq_update_irq_num(u32 devid, u32 *irq_num);
int init_sec_eh_trs_agent(void);
void exit_sec_eh_trs_agent(void);
#endif
