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
#ifndef APM_RES_MAP_H
#define APM_RES_MAP_H

#include "ka_base_pub.h"
#include "ka_common_pub.h"

#include "ascend_hal_define.h"
#include "apm_res_map_ctx.h"

#define APM_OP_MAP 0
#define APM_OP_UNMAP 1

struct apm_task_res_map_ops {
    int (*res_map)(struct apm_res_map_info *para);
    int (*res_unmap)(struct apm_res_map_info *para);
    int (*res_map_query)(struct apm_res_map_info *para);
};

void apm_task_res_map_ops_register(struct apm_task_res_map_ops *ops);
int apm_fops_res_info_check(struct res_map_info_in *res_info);
int apm_res_addr_map(u32 udevid, struct res_map_info_in *res_info, u64 *va, u32 *len);
int apm_res_addr_unmap(u32 udevid, struct res_map_info_in *res_info);
int apm_res_addr_query(u32 udevid, struct res_map_info_in *res_info, u64 *va, u32 *len);
void apm_res_map_domain_task_exit(u32 udevid, int tgid, struct task_start_time *start_time);
void apm_res_map_domain_task_show(u32 udevid, int tgid, int feature_id, ka_seq_file_t *seq);
int apm_res_map_init(void);
void apm_res_map_uninit(void);

#endif
