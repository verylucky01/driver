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
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/rwsem.h>

#include "dpa/dpa_dp_proc_mng_pid_maps.h"

struct rw_semaphore g_dp_pid_map_lock;
struct pid_maps_ops g_pid_maps_ops;
#ifndef EMU_ST
void dp_pid_maps_ops_regester(struct pid_maps_ops *maps_ops)
{
    down_write(&g_dp_pid_map_lock);
    g_pid_maps_ops.query_process_by_host_pid_kernel = maps_ops->query_process_by_host_pid_kernel;
    g_pid_maps_ops.query_process_by_host_pid = maps_ops->query_process_by_host_pid;
    g_pid_maps_ops.query_process_host_pid = maps_ops->query_process_host_pid;
    g_pid_maps_ops.query_master_location = maps_ops->query_master_location;
    g_pid_maps_ops.query_master_info_by_slave = maps_ops->query_master_info_by_slave;
    g_pid_maps_ops.query_slave_tgid_by_master = maps_ops->query_slave_tgid_by_master;
    g_pid_maps_ops.query_master_pid_by_device_slave = maps_ops->query_master_pid_by_device_slave;
    g_pid_maps_ops.query_master_pid_by_host_slave = maps_ops->query_master_pid_by_host_slave;
    g_pid_maps_ops.query_process_host_pids_by_pid = maps_ops->query_process_host_pids_by_pid;
    g_pid_maps_ops.check_hostpid = maps_ops->check_hostpid;
    g_pid_maps_ops.check_sign = maps_ops->check_sign;
    g_pid_maps_ops.get_dev_process = maps_ops->get_dev_process;
    g_pid_maps_ops.put_dev_process = maps_ops->put_dev_process;
    up_write(&g_dp_pid_map_lock);
}
EXPORT_SYMBOL_GPL(dp_pid_maps_ops_regester);

void dp_pid_maps_ops_unregester(void)
{
    down_write(&g_dp_pid_map_lock);
    g_pid_maps_ops.query_process_by_host_pid_kernel = NULL;
    g_pid_maps_ops.query_process_by_host_pid = NULL;
    g_pid_maps_ops.query_process_host_pid = NULL;
    g_pid_maps_ops.query_master_location = NULL;
    g_pid_maps_ops.query_master_info_by_slave = NULL;
    g_pid_maps_ops.query_slave_tgid_by_master = NULL;
    g_pid_maps_ops.query_master_pid_by_device_slave = NULL;
    g_pid_maps_ops.query_master_pid_by_host_slave = NULL;
    g_pid_maps_ops.query_process_host_pids_by_pid = NULL;
    g_pid_maps_ops.check_hostpid = NULL;
    g_pid_maps_ops.check_sign = NULL;
    g_pid_maps_ops.get_dev_process = NULL;
    g_pid_maps_ops.put_dev_process = NULL;
    up_write(&g_dp_pid_map_lock);
}
EXPORT_SYMBOL_GPL(dp_pid_maps_ops_unregester);

#ifndef DCFG_FEATURE_APM_SUPP_PID
int hal_kernel_devdrv_query_process_by_host_pid_kernel(unsigned int host_pid,
    unsigned int chip_id, enum devdrv_process_type cp_type, unsigned int vfid, int *pid)
{
    int ret = -ENODATA;
    down_read(&g_dp_pid_map_lock);
    if (g_pid_maps_ops.query_process_by_host_pid_kernel != NULL) {
        ret = g_pid_maps_ops.query_process_by_host_pid_kernel(host_pid, chip_id, cp_type, vfid, pid);
    }
    up_read(&g_dp_pid_map_lock);
    return ret;
}
EXPORT_SYMBOL_GPL(hal_kernel_devdrv_query_process_by_host_pid_kernel);

int devdrv_query_process_by_host_pid(unsigned int host_pid,
    unsigned int chip_id, enum devdrv_process_type cp_type, unsigned int vfid, int *pid)
{
    int ret = -ENODATA;
    down_read(&g_dp_pid_map_lock);
    if (g_pid_maps_ops.query_process_by_host_pid != NULL) {
        ret = g_pid_maps_ops.query_process_by_host_pid(host_pid, chip_id, cp_type, vfid, pid);
    }
    up_read(&g_dp_pid_map_lock);
    return ret;
}
EXPORT_SYMBOL_GPL(devdrv_query_process_by_host_pid);

int hal_kernel_devdrv_query_process_host_pid(int pid, unsigned int *chip_id, unsigned int *vfid, unsigned int *host_pid,
    enum devdrv_process_type *cp_type)
{
    int ret = -ENODATA;
    down_read(&g_dp_pid_map_lock);
    if (g_pid_maps_ops.query_process_host_pid != NULL) {
        ret = g_pid_maps_ops.query_process_host_pid(pid, chip_id, vfid, host_pid, cp_type);
    }
    up_read(&g_dp_pid_map_lock);
    return ret;
}
EXPORT_SYMBOL_GPL(hal_kernel_devdrv_query_process_host_pid);

int devdrv_query_master_location(const devdrv_pid_map_info_t *q_info, unsigned int *location)
{
    int ret = -ENODATA;
    down_read(&g_dp_pid_map_lock);
    if (g_pid_maps_ops.query_master_location != NULL) {
        ret = g_pid_maps_ops.query_master_location(q_info, location);
    }
    up_read(&g_dp_pid_map_lock);
    return ret;
}
EXPORT_SYMBOL_GPL(devdrv_query_master_location);

int apm_query_master_info_by_slave(int slave_tgid, int *master_tgid, u32 *udevid, int *mode, u32 *proc_type_bitmap)
{
    int ret = -ENODATA;
    down_read(&g_dp_pid_map_lock);
    if (g_pid_maps_ops.query_master_info_by_slave != NULL) {
        ret = g_pid_maps_ops.query_master_info_by_slave(slave_tgid, master_tgid, udevid, mode, proc_type_bitmap);
    }
    up_read(&g_dp_pid_map_lock);
    return ret;
}
EXPORT_SYMBOL_GPL(apm_query_master_info_by_slave);

int hal_kernel_apm_query_slave_tgid_by_master(int master_tgid, u32 udevid, processType_t proc_type, int *slave_tgid)
{
    int ret = -ENODATA;
    down_read(&g_dp_pid_map_lock);
    if (g_pid_maps_ops.query_slave_tgid_by_master != NULL) {
        ret = g_pid_maps_ops.query_slave_tgid_by_master(master_tgid, udevid, proc_type, slave_tgid);
    }
    up_read(&g_dp_pid_map_lock);
    return ret;
}
EXPORT_SYMBOL_GPL(hal_kernel_apm_query_slave_tgid_by_master);

int devdrv_query_master_pid_by_device_slave(u32 udevid, int slave_pid, u32 *master_pid)
{
    int ret = -ENODATA;
    down_read(&g_dp_pid_map_lock);
    if (g_pid_maps_ops.query_master_pid_by_device_slave != NULL) {
        ret = g_pid_maps_ops.query_master_pid_by_device_slave(udevid, slave_pid, master_pid);
    }
    up_read(&g_dp_pid_map_lock);
    return ret;
}
EXPORT_SYMBOL_GPL(devdrv_query_master_pid_by_device_slave);

int devdrv_query_master_pid_by_host_slave(int slave_pid, u32 *master_pid)
{
    int ret = -ENODATA;
    down_read(&g_dp_pid_map_lock);
    if (g_pid_maps_ops.query_master_pid_by_host_slave != NULL) {
        ret = g_pid_maps_ops.query_master_pid_by_host_slave(slave_pid, master_pid);
    }
    up_read(&g_dp_pid_map_lock);
    return ret;
}
EXPORT_SYMBOL_GPL(devdrv_query_master_pid_by_host_slave);

int devdrv_query_process_host_pids_by_pid(int pid, devdrv_host_pids_info_t *host_pids_info)
{
    int ret = -ENODATA;
    down_read(&g_dp_pid_map_lock);
    if (g_pid_maps_ops.query_process_host_pids_by_pid != NULL) {
        ret = g_pid_maps_ops.query_process_host_pids_by_pid(pid, host_pids_info);
    }
    up_read(&g_dp_pid_map_lock);
    return ret;
}
EXPORT_SYMBOL_GPL(devdrv_query_process_host_pids_by_pid);

int devdrv_check_hostpid(pid_t hostpid, unsigned int chip_id, unsigned int vfid)
{
    int ret = -ENODATA;
    down_read(&g_dp_pid_map_lock);
    if (g_pid_maps_ops.check_hostpid != NULL) {
        ret = g_pid_maps_ops.check_hostpid(hostpid, chip_id, vfid);
    }
    up_read(&g_dp_pid_map_lock);
    return ret;
}
EXPORT_SYMBOL_GPL(devdrv_check_hostpid);

int devdrv_check_sign(pid_t hostpid, const char *sign, u32 len)
{
    int ret = -ENODATA;
    down_read(&g_dp_pid_map_lock);
    if (g_pid_maps_ops.check_sign != NULL) {
        ret = g_pid_maps_ops.check_sign(hostpid, sign, len);
    }
    up_read(&g_dp_pid_map_lock);
    return ret;
}
EXPORT_SYMBOL_GPL(devdrv_check_sign);

int devdrv_get_dev_process(pid_t devpid)
{
    int ret = -ENODATA;
    down_read(&g_dp_pid_map_lock);
    if (g_pid_maps_ops.get_dev_process != NULL) {
        ret = g_pid_maps_ops.get_dev_process(devpid);
    }
    up_read(&g_dp_pid_map_lock);
    return ret;
}
EXPORT_SYMBOL_GPL(devdrv_get_dev_process);

void devdrv_put_dev_process(pid_t devpid)
{
    down_read(&g_dp_pid_map_lock);
    if (g_pid_maps_ops.put_dev_process != NULL) {
        g_pid_maps_ops.put_dev_process(devpid);
    }
    up_read(&g_dp_pid_map_lock);
}
EXPORT_SYMBOL_GPL(devdrv_put_dev_process);
#endif
#endif

void dp_pid_maps_init(void)
{
    init_rwsem(&g_dp_pid_map_lock);
}
