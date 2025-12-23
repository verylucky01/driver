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

int devdrv_register_rao_client(u32 dev_id, enum devdrv_rao_client_type type, u64 va, u64 len,
    enum devdrv_rao_permission_type perm)
{
    int ret;
    u32 index_id;
    struct devdrv_comm_dev_ops *dev_ops = devdrv_add_ops_ref();
    if (dev_ops == NULL) {
        devdrv_err("Get rao dev_ops fail. (dev_id=%u; type=%d)\n", dev_id, type);
        return -EINVAL;
    }
    (void)uda_udevid_to_add_id(dev_id, &index_id);
    ret = dev_ops->ops.register_rao_client(index_id, type, va, len, perm);
    devdrv_sub_ops_ref(dev_ops);
    return ret;
}
EXPORT_SYMBOL(devdrv_register_rao_client);

int devdrv_unregister_rao_client(u32 dev_id, enum devdrv_rao_client_type type)
{
    int ret;
    u32 index_id;
    struct devdrv_comm_dev_ops *dev_ops = devdrv_add_ops_ref();
    if (dev_ops == NULL) {
        devdrv_err("Get rao dev_ops fail. (dev_id=%u; type=%d)\n", dev_id, type);
        return -EINVAL;
    }
    (void)uda_udevid_to_add_id(dev_id, &index_id);

    ret = dev_ops->ops.unregister_rao_client(index_id, type);
    devdrv_sub_ops_ref(dev_ops);
    return ret;
}
EXPORT_SYMBOL(devdrv_unregister_rao_client);