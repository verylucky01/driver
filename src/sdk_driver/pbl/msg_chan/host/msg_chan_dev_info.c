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

struct device *devdrv_base_comm_get_device(u32 devid, u32 vfid, u32 udevid);
int devdrv_get_device_boot_status_inner(u32 index_id, u32 *boot_status)
{
    int ret;
    struct devdrv_comm_dev_ops *dev_ops = devdrv_add_ops_ref();
    if (dev_ops == NULL) {
        devdrv_err("Get boot status fail.\n");
        return -EINVAL;
    }
    ret = dev_ops->ops.get_boot_status(index_id, boot_status);
    devdrv_sub_ops_ref(dev_ops);
    return ret;
}
EXPORT_SYMBOL(devdrv_get_device_boot_status_inner);

int devdrv_get_device_boot_status(u32 devid, u32 *boot_status)
{
    u32 index_id;

    (void)uda_udevid_to_add_id(devid, &index_id);
    return devdrv_get_device_boot_status_inner(index_id, boot_status);
}
EXPORT_SYMBOL(devdrv_get_device_boot_status);

int devdrv_get_host_phy_mach_flag(u32 devid, u32 *host_flag)
{
    int ret;
    u32 index_id;
    struct devdrv_comm_dev_ops *dev_ops = devdrv_add_ops_ref();
    if (dev_ops == NULL) {
        devdrv_err("Get host phy mach flag fail.\n");
        return -EINVAL;
    }
    (void)uda_udevid_to_add_id(devid, &index_id);
    ret = dev_ops->ops.get_host_phy_mach_flag(index_id, host_flag);
    devdrv_sub_ops_ref(dev_ops);
    return ret;
}
EXPORT_SYMBOL(devdrv_get_host_phy_mach_flag);

int devdrv_get_env_boot_type_inner(u32 index_id)
{
    int ret;
    struct devdrv_comm_dev_ops *dev_ops = devdrv_add_ops_ref();
    if (dev_ops == NULL) {
        devdrv_err("Get env boot type fail.\n");
        return -EINVAL;
    }
    ret = dev_ops->ops.get_env_boot_type(index_id);
    devdrv_sub_ops_ref(dev_ops);
    return ret;
}
EXPORT_SYMBOL(devdrv_get_env_boot_type_inner);

int devdrv_get_env_boot_type(u32 dev_id)
{
    u32 index_id;
    (void)uda_udevid_to_add_id(dev_id, &index_id);
    return devdrv_get_env_boot_type_inner(index_id);
}
EXPORT_SYMBOL(devdrv_get_env_boot_type);

struct device *devdrv_base_comm_get_device(u32 devid, u32 vfid, u32 udevid)
{
    struct device *dev = NULL;
    u32 index_id;
    struct devdrv_comm_dev_ops *dev_ops = devdrv_add_ops_ref();
    if (dev_ops == NULL) {
        devdrv_err("Get base communication dev fail.\n");
        return dev;
    }
    (void)uda_udevid_to_add_id(udevid, &index_id);

    if (dev_ops->ops.get_device != NULL) {
        dev = dev_ops->ops.get_device(devid, vfid, index_id);
    }
    devdrv_sub_ops_ref(dev_ops);
    return dev;
}
EXPORT_SYMBOL(devdrv_base_comm_get_device);

int devdrv_get_dev_topology(u32 devid, u32 peer_devid, int *topo_type)
{
    int ret;
    u32 index_id, peer_index_id;
    struct devdrv_comm_dev_ops *dev_ops = devdrv_add_ops_ref();
    if (dev_ops == NULL) {
        devdrv_err("Get dev topology fail.\n");
        return -EINVAL;
    }
    (void)uda_udevid_to_add_id(devid, &index_id);
    (void)uda_udevid_to_add_id(peer_devid, &peer_index_id);

    ret = dev_ops->ops.get_dev_topology(index_id, peer_index_id, topo_type);
    devdrv_sub_ops_ref(dev_ops);
    return ret;
}
EXPORT_SYMBOL(devdrv_get_dev_topology);

int devdrv_get_device_info(u32 devid, struct devdrv_base_device_info *device_info)
{
    struct devdrv_comm_dev_ops *dev_ops = devdrv_add_ops_ref();
    int ret = -EOPNOTSUPP;
    u32 index_id;
    if (dev_ops == NULL) {
        devdrv_err("Get dev info fail.\n");
        return -EINVAL;
    }
    (void)uda_udevid_to_add_id(devid, &index_id);

    if (dev_ops->ops.get_device_info != NULL) {
        ret = dev_ops->ops.get_device_info(index_id, device_info);
    }
    devdrv_sub_ops_ref(dev_ops);
    return ret;
}

int devdrv_hot_pre_reset(u32 dev_id)
{
    struct devdrv_comm_dev_ops *dev_ops = devdrv_add_ops_ref();
    int ret = -EOPNOTSUPP;
    u32 index_id;
    if (dev_ops == NULL) {
        devdrv_err("Get pre reset fail.\n");
        return -EINVAL;
    }
    (void)uda_udevid_to_add_id(dev_id, &index_id);

    if (dev_ops->ops.prereset_assemble != NULL) {
        ret = dev_ops->ops.prereset_assemble(index_id);
    }
    devdrv_sub_ops_ref(dev_ops);
    return ret;
}
EXPORT_SYMBOL(devdrv_hot_pre_reset);

int devdrv_hot_reset_device(u32 dev_id)
{
    struct devdrv_comm_dev_ops *dev_ops = devdrv_add_ops_ref();
    int ret = -EOPNOTSUPP;
    u32 index_id;
    if (dev_ops == NULL) {
        devdrv_err("Get reset fail.\n");
        return -EINVAL;
    }
    (void)uda_udevid_to_add_id(dev_id, &index_id);

    if (dev_ops->ops.hotreset_assemble != NULL) {
        ret = dev_ops->ops.hotreset_assemble(index_id);
    }
    devdrv_sub_ops_ref(dev_ops);
    return ret;
}
EXPORT_SYMBOL(devdrv_hot_reset_device);

int devdrv_hotreset_atomic_rescan(u32 dev_id)
{
    struct devdrv_comm_dev_ops *dev_ops = devdrv_add_ops_ref_after_unbind();
    int ret = -EOPNOTSUPP;
    u32 index_id;
    
    if (dev_ops == NULL) {
        devdrv_err("Get rescan fail.\n");
        return -EINVAL;
    }
    (void)uda_udevid_to_add_id(dev_id, &index_id);
    if (dev_ops->ops.rescan_atomic != NULL) {
        ret = dev_ops->ops.rescan_atomic(index_id);
    }
    devdrv_sub_ops_ref(dev_ops);
    return ret;
}
EXPORT_SYMBOL(devdrv_hotreset_atomic_rescan);

int devdrv_p2p_attr_op(struct devdrv_base_comm_p2p_attr *attr)
{
    struct devdrv_comm_dev_ops *dev_ops = devdrv_add_ops_ref();
    int ret = -EOPNOTSUPP;

    if (dev_ops == NULL) {
        devdrv_err("Get p2p attr fail.\n");
        return -EINVAL;
    }

    if (dev_ops->ops.p2p_attr_op != NULL) {
        ret = dev_ops->ops.p2p_attr_op(attr);
    }
    devdrv_sub_ops_ref(dev_ops);
    return ret;
}

u32 devdrv_get_index_id_by_devid(u32 dev_id)
{
    u32 index_id;
    (void)uda_udevid_to_add_id(dev_id, &index_id);
    return index_id;
}

struct devdrv_comm_dev_ops *devdrv_get_token_val_ops()
{
    return devdrv_add_ops_ref();
}

int devdrv_hotreset_atomic_unbind(u32 dev_id)
{
    struct devdrv_comm_dev_ops *dev_ops = devdrv_add_ops_ref();
    int ret = -EOPNOTSUPP;
    u32 index_id;
    if (dev_ops == NULL) {
        devdrv_err("Get unbind fail.\n");
        return -EINVAL;
    }
    (void)uda_udevid_to_add_id(dev_id, &index_id);

    if (dev_ops->ops.unbind_atomic != NULL) {
        ret = dev_ops->ops.unbind_atomic(index_id);
    }
    devdrv_sub_ops_ref(dev_ops);
    return ret;
}
EXPORT_SYMBOL(devdrv_hotreset_atomic_unbind);

int devdrv_hotreset_atomic_reset(u32 dev_id)
{
    struct devdrv_comm_dev_ops *dev_ops = devdrv_add_ops_ref_after_unbind();
    int ret = -EOPNOTSUPP;
    u32 index_id;
    if (dev_ops == NULL) {
        devdrv_err("Get devdrv_hotreset_atomic_reset fail.\n");
        return -EINVAL;
    }
    (void)uda_udevid_to_add_id(dev_id, &index_id);

    if (dev_ops->ops.reset_atomic != NULL) {
        ret = dev_ops->ops.reset_atomic(index_id);
    }
    devdrv_sub_ops_ref(dev_ops);
    return ret;
}
EXPORT_SYMBOL(devdrv_hotreset_atomic_reset);

int devdrv_hotreset_atomic_remove(u32 dev_id)
{
    struct devdrv_comm_dev_ops *dev_ops = devdrv_add_ops_ref_after_unbind();
    int ret = -EOPNOTSUPP;
    u32 index_id;
    if (dev_ops == NULL) {
        devdrv_err("Get remove fail.\n");
        return -EINVAL;
    }
    (void)uda_udevid_to_add_id(dev_id, &index_id);

    if (dev_ops->ops.remove_atomic != NULL) {
        ret = dev_ops->ops.remove_atomic(index_id);
    }
    devdrv_sub_ops_ref(dev_ops);
    return ret;
}
EXPORT_SYMBOL(devdrv_hotreset_atomic_remove);