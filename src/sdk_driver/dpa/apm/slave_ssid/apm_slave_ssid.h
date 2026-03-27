/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025. All rights reserved.
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
#ifndef APM_SLAVE_SSID_H
#define APM_SLAVE_SSID_H

#include "apm_task_group_def.h"
#include "apm_msg.h"

int _apm_query_slave_ssid_by_master(u32 udevid, int master_tgid, processType_t proc_type, u32 *ssid);
int _apm_query_slave_ssid(u32 udevid, int slave_tgid, int *ssid);
int apm_query_local_slave_ssid_by_master(u32 udevid, int master_tgid, processType_t proc_type, int *ssid);
int apm_slave_ssid_init(void);

#endif
