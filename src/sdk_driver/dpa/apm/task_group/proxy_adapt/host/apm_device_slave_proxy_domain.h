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
#ifndef APM_DEVICE_SLAVE_PROXY_DOMAIN_H
#define APM_DEVICE_SLAVE_PROXY_DOMAIN_H

#include "ka_common_pub.h"

#include "apm_slave.h"

int apm_device_slave_proxy_domain_bind(int slave_tgid, int master_tgid, struct apm_cmd_bind *para);
int apm_device_slave_proxy_domain_unbind(int slave_tgid, struct apm_cmd_bind *para);
void apm_device_slave_proxy_domain_task_exit(u32 udevid, int tgid);
void apm_device_slave_proxy_domain_master_exit(int master_tgid);
int apm_query_master_tgid_by_device_slave(u32 udevid, int slave_tgid, int *master_tgid);
int apm_device_slave_proxy_domain_set_slave_status(u32 udevid, int slave_tgid, int status_type, int status);
void apm_device_slave_proxy_domain_task_show(u32 udevid, int tgid, int feature_id, ka_seq_file_t *seq);
int apm_device_slave_proxy_domain_init(void);
void apm_device_slave_proxy_domain_uninit(void);

#endif
