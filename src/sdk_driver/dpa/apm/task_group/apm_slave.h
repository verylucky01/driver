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
#ifndef APM_SLAVE_H
#define APM_SLAVE_H

#include "ka_base_pub.h"
#include "ka_common_pub.h"

#include "ascend_hal_define.h"
#include "apm_task_group_def.h"

#define APM_SLAVE_CHECK_EXIT_NONE                   0U
#define APM_SLAVE_CHECK_EXIT_FROM_EXTERNAL          1U
#define APM_SLAVE_CHECK_EXIT_FROM_APM               2U

struct slave_ctx {
    ka_proc_dir_entry_t *entry;
    int slave_pid;
    int master_pid;
    int master_tgid;
    int mode;
    int ssid;
    u32 udevid;
    u32 num;
    u32 exit_check_src_flag;
    int valid[APM_PROC_TYPE_NUM];
};

int apm_slave_ctx_create(struct task_ctx_domain *domain, int slave_tgid, int slave_pid);
void apm_slave_ctx_destroy(struct task_ctx_domain *domain, int slave_tgid);
int apm_slave_add_master(struct task_ctx_domain *domain, int slave_tgid, int master_tgid, struct apm_cmd_bind *para);
int apm_slave_del_master(struct task_ctx_domain *domain, int slave_tgid, struct apm_cmd_bind *para);
int apm_slave_query_master(struct task_ctx_domain *domain, int slave_tgid, struct apm_cmd_query_master_info *para);
int apm_slave_query_master_self_exit_check(struct task_ctx_domain *domain, int slave_tgid,
    struct apm_cmd_query_master_info *para);
int apm_slave_tgid_to_pid(struct task_ctx_domain *domain, int slave_tgid, int *slave_pid);
int apm_slave_get_ssid(struct task_ctx_domain *domain, int slave_tgid, struct apm_cmd_slave_ssid *para);
int apm_slave_set_ssid(struct task_ctx_domain *domain, int slave_tgid, struct apm_cmd_slave_ssid *para);
void apm_slave_ctx_destroy_by_master_tgid(struct task_ctx_domain *domain, int master_tgid);
void apm_slave_ctx_show(struct task_ctx_domain *domain, int slave_tgid, ka_seq_file_t *seq);
int apm_slave_check_set_exit_status(struct task_ctx_domain *domain, int slave_tgid, struct task_start_time *time,
    u32 check_src);

#endif
