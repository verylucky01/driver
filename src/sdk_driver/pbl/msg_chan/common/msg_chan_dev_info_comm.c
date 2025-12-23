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

int devdrv_get_connect_protocol_inner(u32 index_id)
{
    int ret;
    struct devdrv_comm_dev_ops *dev_ops = NULL;

    dev_ops = devdrv_add_ops_ref();
    if (dev_ops == NULL) {
        devdrv_err("Get connect type fail.\n");
        return -EINVAL;
    }

    ret = dev_ops->ops.get_connect_type(index_id);
    devdrv_sub_ops_ref(dev_ops);
    return ret;
}
EXPORT_SYMBOL(devdrv_get_connect_protocol_inner);

int devdrv_get_connect_protocol(u32 dev_id)
{
    u32 index_id;

    index_id = devdrv_get_index_id_by_devid(dev_id);
    return devdrv_get_connect_protocol_inner(index_id);
}
EXPORT_SYMBOL(devdrv_get_connect_protocol);

int devdrv_get_pfvf_type_by_devid_inner(u32 index_id)
{
    int ret;
    struct devdrv_comm_dev_ops *dev_ops = devdrv_add_ops_ref();
    if (dev_ops == NULL) {
        devdrv_err("Get pfvf type fail.\n");
        return -EINVAL;
    }
    ret = dev_ops->ops.get_pfvf_type_by_devid(index_id);
    devdrv_sub_ops_ref(dev_ops);
    return ret;
}
EXPORT_SYMBOL(devdrv_get_pfvf_type_by_devid_inner);

int devdrv_get_pfvf_type_by_devid(u32 dev_id)
{
    u32 index_id;

    index_id = devdrv_get_index_id_by_devid(dev_id);
    return devdrv_get_pfvf_type_by_devid_inner(index_id);
}
EXPORT_SYMBOL(devdrv_get_pfvf_type_by_devid);

bool devdrv_is_mdev_vm_boot_mode_inner(u32 index_id)
{
    int ret;
    struct devdrv_comm_dev_ops *dev_ops = devdrv_add_ops_ref();
    if (dev_ops == NULL) {
        devdrv_err("Get vm boot mode fail.\n");
        return false;
    }
    ret = dev_ops->ops.mdev_vm_boot_mode(index_id);
    devdrv_sub_ops_ref(dev_ops);
    return ret;
}
EXPORT_SYMBOL(devdrv_is_mdev_vm_boot_mode_inner);

bool devdrv_is_mdev_vm_boot_mode(u32 dev_id)
{
    u32 index_id;

    index_id = devdrv_get_index_id_by_devid(dev_id);
    return devdrv_is_mdev_vm_boot_mode_inner(index_id);
}
EXPORT_SYMBOL(devdrv_is_mdev_vm_boot_mode);

bool devdrv_is_sriov_support(u32 dev_id)
{
    int ret;
    u32 index_id;
    struct devdrv_comm_dev_ops *dev_ops = devdrv_add_ops_ref();
    if (dev_ops == NULL) {
        devdrv_err("Get sriov support fail.\n");
        return false;
    }
    index_id = devdrv_get_index_id_by_devid(dev_id);
    ret = dev_ops->ops.sriov_support(index_id);
    devdrv_sub_ops_ref(dev_ops);
    return ret;
}
EXPORT_SYMBOL(devdrv_is_sriov_support);

int devdrv_sriov_enable(u32 dev_id, u32 boot_mode)
{
    int ret = 0;
    u32 index_id;
    struct devdrv_comm_dev_ops *dev_ops = devdrv_add_ops_ref();
    if (dev_ops == NULL) {
        devdrv_err("Get sriov enable fail.\n");
        return -EINVAL;
    }
    index_id = devdrv_get_index_id_by_devid(dev_id);

    if (dev_ops->ops.sriov_enable != NULL) {
        ret = dev_ops->ops.sriov_enable(index_id, boot_mode);
    }
    devdrv_sub_ops_ref(dev_ops);
    return ret;
}
EXPORT_SYMBOL(devdrv_sriov_enable);

int devdrv_sriov_disable(u32 dev_id, u32 boot_mode)
{
    int ret = 0;
    u32 index_id;
    struct devdrv_comm_dev_ops *dev_ops = devdrv_add_ops_ref();
    if (dev_ops == NULL) {
        devdrv_err("Get sriov disable fail.\n");
        return -EINVAL;
    }
    index_id = devdrv_get_index_id_by_devid(dev_id);

    if (dev_ops->ops.sriov_disable != NULL) {
        ret = dev_ops->ops.sriov_disable(index_id, boot_mode);
    }
    devdrv_sub_ops_ref(dev_ops);
    return ret;
}
EXPORT_SYMBOL(devdrv_sriov_disable);

int devdrv_get_ub_urma_info_by_udevid(u32 udevid, struct ascend_urma_dev_info *urma_info)
{
    int ret = 0;
    u32 index_id;
    struct devdrv_comm_dev_ops *dev_ops = devdrv_add_ops_ref();
    if (dev_ops == NULL) {
        devdrv_err("Get sriov disable fail.\n");
        return -EINVAL;
    }
    index_id = devdrv_get_index_id_by_devid(udevid);

    if (dev_ops->ops.get_urma_info_by_eid != NULL) {
        ret = dev_ops->ops.get_urma_info_by_eid(index_id, urma_info);
    }else {
        devdrv_err("Get_urma_info_by_eid is not regeistered.\n");
        devdrv_sub_ops_ref(dev_ops);
        return -EINVAL;
    }
    devdrv_sub_ops_ref(dev_ops);
    return ret;
}
EXPORT_SYMBOL(devdrv_get_ub_urma_info_by_udevid);

int devdrv_get_ub_dev_info(u32 dev_id, struct devdrv_ub_dev_info *eid_info, int *num)
{
    int ret = -EOPNOTSUPP;
    u32 index_id;
    struct devdrv_comm_dev_ops *dev_ops = devdrv_add_ops_ref();
    if (dev_ops == NULL) {
        devdrv_err("Get ub dev info fail.(devid=%d)\n", dev_id);
        return -EINVAL;
    }
    index_id = devdrv_get_index_id_by_devid(dev_id);

    if (dev_ops->ops.get_ub_dev_info != NULL) {
        ret = dev_ops->ops.get_ub_dev_info(index_id, eid_info, num);
    }
    devdrv_sub_ops_ref(dev_ops);
    return ret;
}
EXPORT_SYMBOL(devdrv_get_ub_dev_info);

int devdrv_addr_trans_p2p_peer_to_local(u32 udevid, u32 peer_udevid, struct devdrv_addr_desc *addr_desc,
    u64 *trans_addr)
{
    int ret = -EOPNOTSUPP;
    u32 index_id, peer_index_id;
    struct devdrv_comm_dev_ops *dev_ops = devdrv_add_ops_ref_by_type(DEVDRV_COMMNS_UB);
    if (dev_ops == NULL) {
        devdrv_err("trans p2p peer to local fail.(devid=%d)\n", udevid);
        return -EINVAL;
    }
    index_id = devdrv_get_index_id_by_devid(udevid);
    peer_index_id = devdrv_get_index_id_by_devid(peer_udevid);

    if (dev_ops->ops.addr_trans_p2p != NULL) {
        ret = dev_ops->ops.addr_trans_p2p(index_id, peer_index_id, addr_desc, trans_addr);
    }
    devdrv_sub_ops_ref_by_type(dev_ops);
    return ret;
}
EXPORT_SYMBOL(devdrv_addr_trans_p2p_peer_to_local);

int devdrv_addr_trans_cs_p2p_peer_to_local(u32 udevid, u32 peer_sdid,
    struct devdrv_addr_desc *addr_desc, u64 *trans_addr)
{
    return -EOPNOTSUPP;
}
EXPORT_SYMBOL(devdrv_addr_trans_cs_p2p_peer_to_local);

int devdrv_get_token_val(u32 dev_id, u32 *token_val)
{
    int ret = -EOPNOTSUPP;
    u32 index_id;
    struct devdrv_comm_dev_ops *dev_ops = devdrv_get_token_val_ops();
    if (dev_ops == NULL) {
        devdrv_err("Get ub token_val fail.(devid=%d)\n", dev_id);
        return -EINVAL;
    }
    index_id = devdrv_get_index_id_by_devid(dev_id);

    if (dev_ops->ops.get_token_val != NULL) {
        ret = dev_ops->ops.get_token_val(index_id, token_val);
    }
    devdrv_sub_ops_ref(dev_ops);
    return ret;
}
EXPORT_SYMBOL(devdrv_get_token_val);

int devdrv_process_pasid_add(u32 dev_id, u64 pasid)
{
    int ret = -EOPNOTSUPP;
    u32 index_id;
    struct devdrv_comm_dev_ops *dev_ops = devdrv_add_ops_ref();
    if (dev_ops == NULL) {
        devdrv_err("Get ub dev info fail.(devid=%d)\n", dev_id);
        return -EINVAL;
    }
    index_id = devdrv_get_index_id_by_devid(dev_id);

    if (dev_ops->ops.add_pasid != NULL) {
        ret = dev_ops->ops.add_pasid(index_id, pasid);
    }
    devdrv_sub_ops_ref(dev_ops);
    return ret;
}
EXPORT_SYMBOL(devdrv_process_pasid_add);

int devdrv_process_pasid_del(u32 dev_id, u64 pasid)
{
    int ret = -EOPNOTSUPP;
    u32 index_id;
    struct devdrv_comm_dev_ops *dev_ops = devdrv_add_ops_ref();
    if (dev_ops == NULL) {
        devdrv_err("Get ub dev info fail.(devid=%d)\n", dev_id);
        return -EINVAL;
    }
    index_id = devdrv_get_index_id_by_devid(dev_id);

    if (dev_ops->ops.del_pasid != NULL) {
        ret = dev_ops->ops.del_pasid(index_id, pasid);
    }
    devdrv_sub_ops_ref(dev_ops);
    return ret;
}
EXPORT_SYMBOL(devdrv_process_pasid_del);