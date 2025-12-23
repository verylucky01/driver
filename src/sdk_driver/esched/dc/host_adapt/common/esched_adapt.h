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

#ifndef ESCHED_ADAPT_H
#define ESCHED_ADAPT_H

#include <linux/types.h>
#include <stdbool.h>
#include <linux/kallsyms.h>
#include "esched_log.h"
#include "esched.h"
#include "esched_kernel_interface.h"
#include "pbl/pbl_user_cfg_interface.h"

int32_t sched_fop_query_sync_msg_trace(u32 devid, unsigned long arg);
int sched_check_cp2_dest_pid(struct sched_published_event_info *event_info);
int32_t sched_node_sched_cpu_init(u32 dev_id);

int sched_query_sync_event_trace(unsigned int chip_id,
    unsigned int dev_pid, unsigned int gid, unsigned int tid, struct sched_sync_event_trace *trace_result);

int32_t sched_node_sched_cpu_uda_init(u32 dev_id);
int32_t sched_node_sched_cpu_module_init(u32 dev_id);
bool esched_is_support_uda_online(void);
int esched_pm_shutdown(u32 chip_id);
int dev_get_cpudomain_number(dev_cpu_nums_cfg_t *cpu_nums_cfg);
void sched_put_thread_map(struct sched_event_thread_map *thread_map);
void sched_get_thread_map(struct sched_event_thread_map *thread_map);
int esched_ts_platform_init(void);
void esched_ts_platform_uninit(void);
int esched_drv_init_cpu_port_adapt(u32 chip_id, u32 start_id, u32 chan_num);
void esched_drv_uninit_cpu_port_adapt(u32 chip_id, u32 start_id, u32 chan_num);
int esched_drv_init_msgq_config_adapt(struct sched_numa_node *node, u32 start_id, u32 aicpu_chan_num, u32 comcpu_chan_num);

#endif

