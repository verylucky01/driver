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

#ifndef DPA_DP_PROC_MNG_PID_MAPS_H
#define DPA_DP_PROC_MNG_PID_MAPS_H

#include "dpa_pids_map.h"

typedef int (*query_process_by_host_pid_kernel_func)(unsigned int host_pid,
    unsigned int chip_id, enum devdrv_process_type cp_type, unsigned int vfid, int *pid);
typedef int (*query_process_by_host_pid_func)(unsigned int host_pid,
    unsigned int chip_id, enum devdrv_process_type cp_type, unsigned int vfid, int *pid);
typedef int (*query_process_host_pid_func)(int pid, unsigned int *chip_id, unsigned int *vfid, unsigned int *host_pid,
    enum devdrv_process_type *cp_type);
typedef int (*query_master_location_func)(const devdrv_pid_map_info_t *q_info, unsigned int *location);
typedef int (*query_master_info_by_slave_func)(int slave_tgid, int *master_tgid, u32 *udevid, int *mode, u32 *proc_type_bitmap);
typedef int (*query_slave_tgid_by_master_func)(int master_tgid, u32 udevid, processType_t proc_type, int *slave_tgid);
typedef int (*query_master_pid_by_device_slave_func)(u32 udevid, int slave_pid, u32 *master_pid);
typedef int (*query_master_pid_by_host_slave_func)(int slave_pid, u32 *master_pid);
typedef int (*query_process_host_pids_by_pid_func)(int pid, devdrv_host_pids_info_t *host_pids_info);
typedef int (*check_hostpid_func)(pid_t hostpid, unsigned int chip_id, unsigned int vfid);
typedef int (*check_sign_func)(pid_t hostpid, const char *sign, u32 len);
typedef int (*get_dev_process_func)(pid_t);
typedef void (*put_dev_process_func)(pid_t);

struct pid_maps_ops {
    query_process_by_host_pid_kernel_func query_process_by_host_pid_kernel;
    query_process_by_host_pid_func query_process_by_host_pid;
    query_process_host_pid_func query_process_host_pid;
    query_master_location_func query_master_location;
    query_master_info_by_slave_func query_master_info_by_slave;
    query_slave_tgid_by_master_func query_slave_tgid_by_master;
    query_master_pid_by_device_slave_func query_master_pid_by_device_slave;
    query_master_pid_by_host_slave_func query_master_pid_by_host_slave;
    query_process_host_pids_by_pid_func query_process_host_pids_by_pid;
    check_hostpid_func check_hostpid;
    check_sign_func check_sign;
    get_dev_process_func get_dev_process;
    put_dev_process_func put_dev_process;
};

void dp_pid_maps_ops_regester(struct pid_maps_ops *maps_ops);
void dp_pid_maps_ops_unregester(void);
int hal_kernel_devdrv_query_process_by_host_pid_kernel(unsigned int host_pid,
    unsigned int chip_id, enum devdrv_process_type cp_type, unsigned int vfid, int *pid);
int apm_query_master_info_by_slave(int slave_tgid, int *master_tgid, u32 *udevid, int *mode, u32 *proc_type_bitmap);
int hal_kernel_apm_query_slave_tgid_by_master(int master_tgid, u32 udevid, processType_t proc_type, int *slave_tgid);
int devdrv_get_dev_process(pid_t devpid);
void devdrv_put_dev_process(pid_t devpid);
void dp_pid_maps_init(void);

#endif