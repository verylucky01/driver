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

#ifndef VIRTMNG_RES_COMM_H
#define VIRTMNG_RES_COMM_H

#include "virtmnghost_ctrl.h"

int vmngh_alloc_vfid_comm(u32 dev_id, u32 dtype, u32 *fid);
void vmngh_free_vfid_comm(u32 dev_id, u32 vfid);

int vmngh_alloc_vf_comm(u32 dev_id, u32 *fid, u32 dtype, struct vmng_vf_res_info *vf_resource);
int vmngh_free_vf_comm(u32 dev_id, u32 vfid);
int vmngh_container_client_online_comm(u32 dev_id, u32 vfid);
int vmngh_container_client_offline_comm(u32 dev_id, u32 vfid);
int vmngh_res_enable_sriov_comm(u32 dev_id, u32 boot_mode);
int vmngh_res_disable_sriov_comm(u32 dev_id, u32 boot_mode);
int vmngh_enquire_soc_resource_comm(u32 dev_id, u32 vfid, struct vmng_soc_resource_enquire *info);
int vmngh_refresh_vdev_resource_comm(u32 dev_id, u32 vfid, struct vmng_soc_resource_refresh *info);

#endif
