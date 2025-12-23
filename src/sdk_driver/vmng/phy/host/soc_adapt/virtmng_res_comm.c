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
#include <linux/delay.h>

#include "comm_kernel_interface.h"

#include "virtmng_public_def.h"
#include "virtmnghost_ctrl.h"
#include "virtmnghost_pci.h"
#include "virtmnghost_unit.h"
#include "virtmng_res_drv.h"
#include "virtmng_res_comm.h"

static int get_sriov_mode(u32 dev_id)
{
    struct vmngh_vdev_ctrl *ctrl = vmngh_get_ctrl(dev_id, 0);
    return ctrl->sriov_mode;
}

// Only create container vf need to alloc vfid, vascend has assign vfid when creating vm mdev
int vmngh_alloc_vfid_comm(u32 dev_id, u32 dtype, u32 *fid)
{
    int sriov_mode;
    int ret;

    if (is_sriov_enable(dev_id) == false) {
        vmng_err("Sriov disabled. (dev_id=%u)\n", dev_id);
        return VMNG_ERR;
    }

    sriov_mode = get_sriov_mode(dev_id);
    if (sriov_mode != DEVDRV_BOOT_ONLY_SRIOV) {
        vmng_err("Not support sriov mode, check operation. (dev_id=%u;sriov_mode=%d)\n", dev_id, sriov_mode);
        return -EOPNOTSUPP;
    }

    ret = vmngh_alloc_vfid(dev_id, fid);
    if (ret != 0) {
        vmng_err("Alloc vfid failed.(ret=%d;dev_id=%u;vfid=%u)\n", ret, dev_id, *fid);
        return ret;
    }

    return VMNG_OK;
}

STATIC int vmngh_alloc_vdev_ctrl_comm(u32 dev_id, u32 vfid, struct vmng_ctrl_msg_info *info)
{
    struct vmngh_vdev_ctrl *ctrl = vmngh_get_ctrl(dev_id, vfid);
    struct mutex *ctrl_mutex = vmngh_get_ctrl_mutex();
    struct vmng_vdev_ctrl *vdev_ctrl = &ctrl->vdev_ctrl;
    int ret;

    mutex_lock(ctrl_mutex);
    (void)devdrv_get_devid_by_pfvf_id(dev_id, vfid, &vdev_ctrl->dev_id);
    vdev_ctrl->vfid = 0;
    vdev_ctrl->dtype = info->dtype;
    vdev_ctrl->core_num = info->core_num;
    vdev_ctrl->total_core_num = info->total_core_num;
    if (memcpy_s(&vdev_ctrl->vf_cfg, sizeof(vmng_vf_cfg_t), &info->vf_cfg, sizeof(vmng_vf_cfg_t)) != EOK) {
        vmng_err("Call memcpy_s failed.\n");
        mutex_unlock(ctrl_mutex);
        return VMNG_ERR;
    }
    vdev_ctrl->status = VMNG_VDEV_STATUS_ALLOC;
    mutex_unlock(ctrl_mutex);

    info->dev_id = dev_id;
    info->vfid = vfid;
    ret = vmngh_vdev_msg_send(info, VMNG_CTRL_MSG_TYPE_ENQUIRE_VF);
    if (ret != 0) {
        vmng_err("Get vf info failed. (dev_id=%u;vfid=%u;ret=%d)\n", dev_id, vfid, ret);
        return ret;
    }
    ret = memcpy_s(&ctrl->memory, sizeof(struct vmng_vf_memory_info),
                   &info->enquire.each.memory, sizeof(struct vmng_vf_memory_info));
    if (ret != 0) {
        vmng_err("Memcpy failed. (ret=%d;dev_id=%u;vfid=%u)\n", ret, dev_id, vfid);
        return ret;
    }

    if (ctrl->memory.number > VMNG_NUMA_MAX_NUM) {
        vmng_err("Memory number is invalid. (number=%u;dev_id=%u;vfid=%u)\n", ctrl->memory.number, dev_id, vfid);
        return -EINVAL;
    }

    return VMNG_OK;
}

void vmngh_free_vfid_comm(u32 dev_id, u32 vfid)
{
    vmngh_free_vdev_ctrl(dev_id, vfid);
}

int vmngh_alloc_vf_comm(u32 dev_id, u32 *fid, u32 dtype, struct vmng_vf_res_info *vf_resource)
{
    struct vmng_ctrl_msg_info info;

    if (fid == NULL) {
        vmng_err("Invalid parameter.\n");
        return VMNG_ERR;
    }

    if (memset_s(&info, sizeof(info), 0, sizeof(info)) != EOK) {
        vmng_err("Call memset_s failed.\n");
        return VMNG_ERR;
    }

    info.dev_id = dev_id;
    info.vfid = *fid;
    info.dtype = dtype;
    if (memcpy_s(&info.vf_cfg, sizeof(struct vmng_vf_res_info), vf_resource, sizeof(struct vmng_vf_res_info)) != EOK) {
        vmng_err("Call memcpy_s failed.\n");
        return VMNG_ERR;
    }
    if (vmngh_vdev_msg_send(&info, VMNG_CTRL_MSG_TYPE_ALLOC_VNPU) != VMNG_OK) {
        return VMNG_ERR;
    }
    vf_resource->vfg.vfg_id = info.vf_cfg.vfg.vfg_id;

    if (vmngh_alloc_vdev_ctrl_comm(dev_id, *fid, &info) != VMNG_OK) {
        vmng_err("alloc_vdev_ctrl_cloud_v2 error. (dev_id=%u, vfid=%u, dtype=%u)\n", dev_id, *fid, dtype);
        goto exit;
    }

    return VMNG_OK;

exit:
    info.dev_id = dev_id;
    info.vfid = *fid;
    (void)vmngh_vdev_msg_send(&info, VMNG_CTRL_MSG_TYPE_FREE_VNPU);

    return VMNG_ERR;
}

int vmngh_free_vf_comm(u32 dev_id, u32 vfid)
{
    struct vmng_ctrl_msg_info info;

    if (vmngh_dev_id_check(dev_id, vfid) != 0) {
        vmng_err("Parameter check failed. (dev_id=%u; vfid=%u)\n", dev_id, vfid);
        return VMNG_ERR;
    }

    if (memset_s(&info, sizeof(struct vmng_ctrl_msg_info), 0, sizeof(struct vmng_ctrl_msg_info)) != EOK) {
        vmng_err("Call memset_s failed.\n");
        return VMNG_ERR;
    }

    info.dev_id = dev_id;
    info.vfid = vfid;
    if (vmngh_vdev_msg_send(&info, VMNG_CTRL_MSG_TYPE_FREE_VNPU) != VMNG_OK) {
        vmng_err("Send msg to device failed. (dev_id=%u;vfid=%u)\n", dev_id, vfid);
        return VMNG_ERR;
    }

    return VMNG_OK;
}

int vmngh_container_client_online_comm(u32 dev_id, u32 vfid)
{
    int ret;

    ret = vmngh_init_instance_client_device(dev_id, vfid);
    if (ret != VMNG_OK) {
        vmng_err("Init instance all client device failed. (dev_id=%u; vfid=%u)\n", dev_id, vfid);
        return ret;
    }

    ret = vmngh_ctrl_sriov_init_instance(dev_id, vfid);
    if (ret != VMNG_OK) {
        (void)vmngh_uninit_instance_client_device(dev_id, vfid);
        vmng_err("pci init instance error. (dev_id=%u;vfid=%u;dev_id=%u;ret=%d)\n", dev_id, vfid, dev_id, ret);
        return ret;
    }

    ret = vmngh_init_instance_all_client(dev_id, vfid, VMNGH_CONTAINER);
    if (ret != VMNG_OK) {
        (void)vmngh_ctrl_sriov_uninit_instance(dev_id, vfid);
        (void)vmngh_uninit_instance_client_device(dev_id, vfid);
        vmng_err("Init instance all client failed. (dev_id=%u; vfid=%u)\n", dev_id, vfid);
        return ret;
    }

    ret = vmngh_add_mia_dev(dev_id, vfid, 0);
    if (ret != 0) {
        (void)vmngh_uninit_instance_all_client(dev_id, vfid);
        (void)vmngh_ctrl_sriov_uninit_instance(dev_id, vfid);
        (void)vmngh_uninit_instance_client_device(dev_id, vfid);
        return ret;
    }

    return VMNG_OK;
}

int vmngh_container_client_offline_comm(u32 dev_id, u32 vfid)
{
    int ret;

    (void)vmngh_remove_mia_dev(dev_id, vfid, 0);

    ret = vmngh_uninit_instance_all_client(dev_id, vfid);
    if (ret != VMNG_OK) {
        vmng_err("Uninit instance all client failed. (dev_id=%u; vfid=%u)\n", dev_id, vfid);
    }

    if (vmngh_ctrl_sriov_uninit_instance(dev_id, vfid) != VMNG_OK) {
        (void)vmngh_init_instance_all_client(dev_id, vfid, VMNGH_CONTAINER);
        vmng_err("pci init instance error. (dev_id=%u, vfid=%u, dev_id=%u)\n", dev_id, vfid, dev_id);
        return VMNG_ERR;
    }

    ret = vmngh_uninit_instance_client_device(dev_id, vfid);
    if (ret != VMNG_OK) {
        vmng_err("Uninit instance all client device failed. (dev_id=%u; vfid=%u)\n", dev_id, vfid);
        (void)vmngh_ctrl_sriov_init_instance(dev_id, vfid);
        (void)vmngh_init_instance_all_client(dev_id, vfid, VMNGH_CONTAINER);
        return ret;
    }

    return VMNG_OK;
}

int vmngh_res_disable_sriov_comm(u32 dev_id, u32 boot_mode)
{
    struct vmng_ctrl_msg_info info;
    int ret;

    if (dev_id >= ASCEND_PDEV_MAX_NUM) {
        vmng_err("Input parameter is error. (dev_id=%u; MAX=%d)\n", dev_id, ASCEND_PDEV_MAX_NUM);
        return VMNG_ERR;
    }

    if (is_has_vf_running(dev_id)) {
        vmng_err("Has VF running. (dev_id=%u)\n", dev_id);
        return VMNG_ERR;
    }
    if (devdrv_get_connect_protocol(dev_id) == CONNECT_PROTOCOL_UB) {
        vmng_info("UB protocol, no need to close sriov. (dev_id=%u)\n", dev_id);
    } else {
        ret = devdrv_sriov_disable(dev_id, boot_mode);
        if (ret != VMNG_OK) {
            vmng_err("Sriov close failed. (dev_id=%u;ret=%d)\n", dev_id, ret);
            return ret;
        }
    }

    info.dev_id = dev_id;
    info.vfid = 0;
    info.sriov_status = VMNGH_PF_SRIOV_DISABLE;
    ret = vmngh_vdev_msg_send(&info, VMNG_CTRL_MSG_TYPE_SRIOV_INFO);
    if (ret != 0) {
        vmng_err("Send vdev msg failed, (dev_id=%u,ret=%d).\n", dev_id, ret);
        return ret;
    }

    return VMNG_OK;
}

int vmngh_res_enable_sriov_comm(u32 dev_id, u32 boot_mode)
{
    struct vmng_ctrl_msg_info info;
    int ret;

    if (dev_id >= ASCEND_PDEV_MAX_NUM) {
        vmng_err("Input parameter is error. (dev_id=%u; MAX=%d)\n", dev_id, ASCEND_PDEV_MAX_NUM);
        return VMNG_ERR;
    }

    if (devdrv_get_connect_protocol(dev_id) == CONNECT_PROTOCOL_UB){
        vmng_info("Connect protocol is UB, no need to enable sriov. (dev_id=%u)\n", dev_id);
    } else {
        if (devdrv_sriov_enable(dev_id, boot_mode) != VMNG_OK) {
            vmng_err("Sriov enable failed. (dev_id=%u)\n", dev_id);
            return VMNG_ERR;
        }
        ssleep(1);
    }

    info.dev_id = dev_id;
    info.vfid = 0;
    info.sriov_status = VMNGH_PF_SRIOV_ENABLE;
    ret = vmngh_vdev_msg_send(&info, VMNG_CTRL_MSG_TYPE_SRIOV_INFO);
    if (ret != 0) {
        vmng_err("Send vdev msg failed, (dev_id=%u, ret=%d).\n", dev_id, ret);
        (void)vmngh_res_disable_sriov_comm(dev_id, DEVDRV_BOOT_DEFAULT_MODE);
        return ret;
    }

    return VMNG_OK;
}

int vmngh_enquire_soc_resource_comm(u32 dev_id, u32 vfid, struct vmng_soc_resource_enquire *info)
{
    struct vmng_ctrl_msg_info info_msg;
    int ret;

    if (info == NULL) {
        vmng_err("Info NULL.\n");
        return VMNG_ERR;
    }

    if (memset_s(&info_msg, sizeof(struct vmng_ctrl_msg_info), 0, sizeof(struct vmng_ctrl_msg_info)) != EOK) {
        vmng_err("Call memset_s failed.\n");
        return VMNG_ERR;
    }

    info_msg.dev_id = dev_id;
    info_msg.vfid = vfid;
    ret = vmngh_vdev_msg_send(&info_msg, VMNG_CTRL_MSG_TYPE_ENQUIRE_VF);
    if (ret != VMNG_OK) {
        vmng_err("Send msg failed. (dev_id=%u; vfid=%u)\n", info_msg.dev_id, info_msg.vfid);
        return VMNG_ERR;
    }

    ret = memcpy_s(info, sizeof(struct vmng_soc_resource_enquire), &info_msg.enquire,
        sizeof(struct vmng_soc_resource_enquire));
    if (ret != EOK) {
        vmng_err("Call memcpy_s err.(dev_id=%u; vfid=%u)\n", info_msg.dev_id, info_msg.vfid);
        return VMNG_ERR;
    }

    return VMNG_OK;
}

int vmngh_refresh_vdev_resource_comm(u32 dev_id, u32 vfid, struct vmng_soc_resource_refresh *info)
{
    int ret;
    struct vmng_ctrl_msg_info info_msg;

    if (info == NULL) {
        vmng_err("Info NULL.\n");
        return VMNG_ERR;
    }

    if (dev_id >= ASCEND_PDEV_MAX_NUM || vfid > VMNG_VF_MAX_PER_PF) {
        vmng_err("Input parameter error. (dev_id=%u)\n", dev_id);
        return VMNG_ERR;
    }

    if (!is_sriov_enable(dev_id)) {
        vmng_err("Sriov not enable. (dev_id=%u)\n", dev_id);
        return VMNG_ERR;
    }

    if (memset_s(&info_msg, sizeof(struct vmng_ctrl_msg_info), 0, sizeof(struct vmng_ctrl_msg_info)) != EOK) {
        vmng_err("Call memset_s failed.\n");
        return VMNG_ERR;
    }

    ret = memcpy_s(&info_msg.refresh, sizeof(struct vmng_soc_resource_refresh), info,
        sizeof(struct vmng_soc_resource_refresh));
    if (ret != EOK) {
        vmng_err("Call memcpy_s err.(dev_id=%u; vfid=%u; vdev_id=%u)\n", dev_id, vfid, dev_id);
        return VMNG_ERR;
    }

    info_msg.dev_id = dev_id;
    info_msg.vfid = vfid;
    ret = vmngh_vdev_msg_send(&info_msg, VMNG_CTRL_MSG_TYPE_REFRESH_VF);
    if (ret != VMNG_OK) {
        vmng_err("Send msg failed. (dev_id=%u; vfid=%u;)\n", dev_id, vfid);
        return VMNG_ERR;
    }

    return VMNG_OK;
}

