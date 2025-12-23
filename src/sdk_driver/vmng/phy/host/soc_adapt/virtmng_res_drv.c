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

#include "virtmng_res_drv.h"
#include "virtmng_res_mini_v2.h"
#include "virtmng_res_cloud_v2.h"
#include "virtmng_res_cloud_v4.h"
#include "virtmng_res_cloud_v1.h"
#include "virtmng_public_def.h"
#include "comm_kernel_interface.h"

bool is_has_vf_running(u32 dev_id)
{
    struct vmngh_vdev_ctrl *ctrl = NULL;
    u32 i;
    int cnt = 0;

    for (i = VMNG_VDEV_FIRST_VFID; i < VMNG_VDEV_MAX_PER_PDEV; i++) {
        ctrl = vmngh_get_ctrl(dev_id, i);
        if (ctrl->vdev_ctrl.status != VMNG_VDEV_STATUS_FREE) {
            cnt++;
        }
    }
    return ((cnt != 0) ? true : false);
}

STATIC int (*vmngh_res_init_func[HISI_CHIP_NUM])(u32 devid, struct vmngh_ctrl_ops *ops) = {
    [HISI_MINI_V1] = NULL,
    [HISI_CLOUD_V1] = vmngh_res_init_cloud_v1,
    [HISI_MINI_V2] = vmngh_res_init_mini_v2,
    [HISI_CLOUD_V2] = vmngh_res_init_cloud_v2,
    [HISI_MINI_V3] = NULL,
    [HISI_CLOUD_V4] = vmngh_res_init_cloud_v4,
    [HISI_CLOUD_V5] = vmngh_res_init_cloud_v4,
};

int vmngh_res_drv_init(u32 dev_id, struct vmngh_ctrl_ops *ops)
{
    u32 chip_type;

    chip_type = uda_get_chip_type(dev_id);
    if ((chip_type < HISI_CHIP_NUM) && (vmngh_res_init_func[chip_type] != NULL)) {
        return vmngh_res_init_func[chip_type](dev_id, ops);
    }

    return VMNG_OK;
}

void vmngh_res_drv_uninit(struct vmngh_ctrl_ops *ops)
{
    if (ops != NULL) {
        (void)memset_s(ops, sizeof(struct vmngh_ctrl_ops), 0, sizeof(struct vmngh_ctrl_ops));
    }
}
