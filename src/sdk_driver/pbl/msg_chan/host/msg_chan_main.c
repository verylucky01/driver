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

#include "msg_chan_main.h"

int devdrv_check_communication_api(struct devdrv_comm_ops *ops)
{
    if ((ops->alloc_non_trans == NULL) || (ops->free_non_trans == NULL) || (ops->get_dev_topology == NULL)) {
        devdrv_err("Invalid ops, alloc free ops is null. (type=%u)\n", ops->comm_type);
        return -EINVAL;
    }
    return devdrv_check_communication_api_proc(ops);
}

int devdrv_check_dev_manager_api(struct devdrv_comm_ops *ops)
{
    if (ops->get_boot_status == NULL) {
        devdrv_err("Invalid ops, get boot status is null. (type=%u)\n", ops->comm_type);
        return -EINVAL;
    }
    if ((ops->get_host_phy_mach_flag == NULL) || (ops->get_env_boot_type == NULL)) {
        devdrv_err("Invalid ops, host flag ops is null. (type=%u)\n", ops->comm_type);
        return -EINVAL;
    }
    return devdrv_check_dev_manager_api_proc(ops);
}

void devdrv_register_save_client_info(struct devdrv_comm_dev_ops *dev_ops)
{
    struct devdrv_msg_client *g_client_info = devdrv_get_msg_client();
    devdrv_register_save_client_info_proc(dev_ops);
    mutex_unlock(&g_client_info->lock);
}