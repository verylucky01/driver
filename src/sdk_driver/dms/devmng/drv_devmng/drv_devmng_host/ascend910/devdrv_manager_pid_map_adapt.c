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

#include "ka_kernel_def_pub.h"
#include "devdrv_manager_pid_map.h"
#ifndef CFG_FEATURE_NO_DP_PROC
#include "dpa_kernel_interface.h"

void devdrv_pid_map_init(void)
{
#ifndef CFG_FEATURE_APM_SUPP_PID
    struct pid_maps_ops maps_ops = {
        .query_process_by_host_pid_kernel = hal_kernel_devdrv_query_process_by_host_pid_kernel,
        .query_process_by_host_pid = devdrv_query_process_by_host_pid,
        .query_process_host_pid = hal_kernel_devdrv_query_process_host_pid,
        .query_master_location = devdrv_query_master_location,
        .query_master_info_by_slave = apm_query_master_info_by_slave,
        .query_slave_tgid_by_master = hal_kernel_apm_query_slave_tgid_by_master,
#ifdef CFG_HOST_ENV
        .query_master_pid_by_device_slave = devdrv_query_master_pid_by_device_slave,
#else
        .query_master_pid_by_host_slave = devdrv_query_master_pid_by_host_slave,
#endif
        .query_process_host_pids_by_pid = devdrv_query_process_host_pids_by_pid,
        .check_hostpid = devdrv_check_hostpid,
        .check_sign = devdrv_check_sign,
        .get_dev_process = devdrv_get_dev_process,
        .put_dev_process = devdrv_put_dev_process,
    };

    dp_pid_maps_ops_regester(&maps_ops);
#endif
}

void devdrv_pid_map_uninit(void)
{
#ifndef CFG_FEATURE_APM_SUPP_PID
    dp_pid_maps_ops_unregester();
#endif
}

#else
void devdrv_pid_map_init(void)
{
    return;
}

void devdrv_pid_map_uninit(void)
{
    return;
}

KA_EXPORT_SYMBOL_GPL(hal_kernel_devdrv_query_process_by_host_pid_kernel);
KA_EXPORT_SYMBOL_GPL(devdrv_query_process_by_host_pid);
KA_EXPORT_SYMBOL(hal_kernel_devdrv_query_process_host_pid);
KA_EXPORT_SYMBOL(devdrv_query_master_location);
#ifdef CFG_HOST_ENV
KA_EXPORT_SYMBOL(devdrv_query_master_pid_by_device_slave);
#else
KA_EXPORT_SYMBOL_GPL(devdrv_query_master_pid_by_host_slave);
#endif
KA_EXPORT_SYMBOL(devdrv_query_process_host_pids_by_pid);
KA_EXPORT_SYMBOL(devdrv_check_hostpid);
KA_EXPORT_SYMBOL(devdrv_check_sign);
KA_EXPORT_SYMBOL(devdrv_get_dev_process);
KA_EXPORT_SYMBOL(devdrv_put_dev_process);
#endif
