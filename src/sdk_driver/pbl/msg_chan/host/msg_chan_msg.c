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

void *devdrv_pcimsg_alloc_non_trans_queue_inner_msg(u32 index_id, struct devdrv_non_trans_msg_chan_info *chan_info)
{
    void* ret;
    struct devdrv_comm_dev_ops *dev_ops = NULL;

    dev_ops = devdrv_add_ops_ref();
    if (dev_ops == NULL) {
        devdrv_err("Host alloc non trans chan fail.\n");
        return NULL;
    }
    ret = dev_ops->ops.alloc_non_trans(index_id, chan_info);
    devdrv_sub_ops_ref(dev_ops);
    return ret;
}
EXPORT_SYMBOL(devdrv_pcimsg_alloc_non_trans_queue_inner_msg);

void *devdrv_pcimsg_alloc_non_trans_queue(u32 dev_id, struct devdrv_non_trans_msg_chan_info *chan_info)
{
    u32 index_id;

    (void)uda_udevid_to_add_id(dev_id, &index_id);
    return devdrv_pcimsg_alloc_non_trans_queue_inner_msg(index_id, chan_info);
}
EXPORT_SYMBOL(devdrv_pcimsg_alloc_non_trans_queue);

int devdrv_pcimsg_free_non_trans_queue(void *msg_chan)
{
    int ret;
    struct devdrv_comm_dev_ops *dev_ops = devdrv_add_ops_ref();
    if (dev_ops == NULL) {
        devdrv_err("Host free non trans chan fail.\n");
        return -EINVAL;
    }
    ret = dev_ops->ops.free_non_trans(msg_chan);
    devdrv_sub_ops_ref(dev_ops);
    return ret;
}
EXPORT_SYMBOL(devdrv_pcimsg_free_non_trans_queue);

int devdrv_sync_msg_send(void *msg_chan, void *data, u32 in_data_len, u32 out_data_len, u32 *real_out_len)
{
    return devdrv_sync_msg_send_proc(msg_chan, data, in_data_len, out_data_len, real_out_len);
}
EXPORT_SYMBOL(devdrv_sync_msg_send);

int devdrv_get_msg_chan_devid_inner(void *msg_chan)
{
    return devdrv_get_msg_chan_devid_proc(msg_chan);
}
EXPORT_SYMBOL(devdrv_get_msg_chan_devid_inner);

int devdrv_get_msg_chan_devid(void *msg_chan)
{
    int index_id;
    u32 udevid;

    index_id = devdrv_get_msg_chan_devid_proc(msg_chan);
    if (index_id == -EINVAL) {
        return -EINVAL;
    }
    uda_udevid_to_add_id((u32)index_id, &udevid);
    return (int)udevid;
}
EXPORT_SYMBOL(devdrv_get_msg_chan_devid);

int devdrv_set_msg_chan_priv(void *msg_chan, void *priv)
{
    return devdrv_set_msg_chan_priv_proc(msg_chan, priv);
}
EXPORT_SYMBOL(devdrv_set_msg_chan_priv);

void *devdrv_get_msg_chan_priv(void *msg_chan)
{
    return devdrv_get_msg_chan_priv_proc(msg_chan);
}
EXPORT_SYMBOL(devdrv_get_msg_chan_priv);