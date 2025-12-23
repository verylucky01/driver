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

int devdrv_sync_msg_send_proc(void *msg_chan, void *data, u32 in_data_len, u32 out_data_len, u32 *real_out_len)
{
    int ret;
    struct devdrv_comm_dev_ops *dev_ops = devdrv_add_ops_ref();
    if (dev_ops == NULL) {
        devdrv_err("Device sync send msg fail.\n");
        return -EINVAL;
    }
    ret = dev_ops->ops.sync_msg_send(msg_chan, data, in_data_len, out_data_len, real_out_len);
    devdrv_sub_ops_ref(dev_ops);
    return ret;
}

int devdrv_get_msg_chan_devid_proc(void *msg_chan)
{
    int index_id;
    struct devdrv_comm_dev_ops *dev_ops = devdrv_add_ops_ref();
    if (dev_ops == NULL) {
        devdrv_err("Get msg chan dev_id fail.\n");
        return -EINVAL;
    }

    index_id = dev_ops->ops.get_msg_chan_devid(msg_chan);
    devdrv_sub_ops_ref(dev_ops);
    return index_id;
}

int devdrv_set_msg_chan_priv_proc(void *msg_chan, void *priv)
{
    int ret;
    struct devdrv_comm_dev_ops *dev_ops = devdrv_add_ops_ref();
    if (dev_ops == NULL) {
        devdrv_err("Set msg chan priv fail.\n");
        return -EINVAL;
    }
    ret = dev_ops->ops.set_msg_chan_priv(msg_chan, priv);
    devdrv_sub_ops_ref(dev_ops);
    return ret;
}

void *devdrv_get_msg_chan_priv_proc(void *msg_chan)
{
    void* ret;
    struct devdrv_comm_dev_ops *dev_ops = devdrv_add_ops_ref();
    if (dev_ops == NULL) {
        devdrv_err("Get msg chan priv fail.\n");
        return NULL;
    }
    ret = dev_ops->ops.get_msg_chan_priv(msg_chan);
    devdrv_sub_ops_ref(dev_ops);
    return ret;
}