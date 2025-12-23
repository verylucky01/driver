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

#ifndef DPA_PIDS_MAP_H
#define DPA_PIDS_MAP_H

#include <linux/pid.h>
#include "ascend_hal_define.h"

typedef struct {
    unsigned int vaild_num;
    unsigned int chip_id[DEVDRV_PROCESS_CPTYPE_MAX];
    unsigned int vfid[DEVDRV_PROCESS_CPTYPE_MAX];
    pid_t host_pids[DEVDRV_PROCESS_CPTYPE_MAX];
    unsigned int cp_type[DEVDRV_PROCESS_CPTYPE_MAX];
} devdrv_host_pids_info_t;

typedef struct devdrv_pid_map_info {
    pid_t master_pid;
    pid_t slave_pid;
    unsigned int dev_id;
    unsigned int vf_id;
    unsigned int cp_type;
} devdrv_pid_map_info_t;

int apm_query_slave_all_meminfo_by_master(int master_tgid, unsigned int udevid, processType_t process_type,
    unsigned long long *size);
int apm_get_all_master_tgids(int *tgids, unsigned int len, unsigned int *cnt);

int devdrv_check_hostpid(pid_t hostpid, unsigned int chip_id, unsigned int vfid);
int devdrv_check_sign(pid_t hostpid, const char *sign, u32 len);
int devdrv_query_process_by_host_pid(unsigned int host_pid,
    unsigned int chip_id, enum devdrv_process_type cp_type, unsigned int vfid, int *pid);
int hal_kernel_devdrv_query_process_by_host_pid_kernel(unsigned int host_pid,
    unsigned int chip_id, enum devdrv_process_type cp_type, unsigned int vfid, int *pid);
int hal_kernel_devdrv_query_process_host_pid(int pid, unsigned int *chip_id, unsigned int *vfid, unsigned int *host_pid,
    enum devdrv_process_type *cp_type);
int devdrv_query_process_host_pids_by_pid(int pid, devdrv_host_pids_info_t *host_pids_info);
/* call on device side, only surport user proc */
int devdrv_query_master_pid_by_host_slave(int slave_pid, u32 *master_pid);
/* call on host side, only surport cp proc */
int devdrv_query_master_pid_by_device_slave(u32 udevid, int slave_pid, u32 *master_pid);
int devdrv_query_process_by_host_pid_user(unsigned int host_pid,
    unsigned int chip_id, enum devdrv_process_type cp_type, unsigned int vfid, int *pid);
int devdrv_query_master_location(const devdrv_pid_map_info_t *q_info, unsigned int *location);

#endif /* __DPA_PIDS_MAP_H */