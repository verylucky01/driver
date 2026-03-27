/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "dms_device_info.h"
#include "dms_user_common.h"
#include "dms_p2p_com.h"
#include "ascend_dev_num.h"

typedef struct {
    int cnt[ASCEND_PDEV_MAX_NUM][ASCEND_PDEV_MAX_NUM];      // [dev_id][peer_phy_id] = enable P2P count
    unsigned int p2p_type[ASCEND_PDEV_MAX_NUM][ASCEND_PDEV_MAX_NUM]; // [dev_id][peer_phy_id] = enable P2P type: mem or notify
    bool flag; // 0: on, 1:off;
}p2p_restore_context;

p2p_restore_context g_p2p_restore_info = {0};

int dms_set_p2p_com_info(unsigned int dev_id, unsigned int sub_cmd, void *buf, unsigned int size)
{
    return DmsSetDeviceInfo(dev_id, (DSMI_MAIN_CMD)DMS_MAIN_CMD_P2P_COM, sub_cmd, buf, size);
}

int dms_get_p2p_com_info(unsigned int dev_id, unsigned int vfid, unsigned int sub_cmd, void *buf, unsigned int *size)
{
    (void)vfid;
    return DmsGetDeviceInfo(dev_id, (DSMI_MAIN_CMD)DMS_MAIN_CMD_P2P_COM, sub_cmd, buf, size);
}

static inline void dms_atomic_inc(int *val)
{
    __sync_fetch_and_add(val, 1);
}

static inline void dms_atomic_dec(int *val)
{
    __sync_fetch_and_sub(val, 1);
}

void dms_set_p2p_restore_info_flag(bool flag)
{
    g_p2p_restore_info.flag = flag;
}

drvError_t dms_set_p2p_restore_info(u32 dev_id, u32 peer_phy_id, u32 p2p_type, enum urd_devdrv_p2p_attr_op opcode)
{
    if ((dev_id >= ASCEND_PDEV_MAX_NUM) || (peer_phy_id >= ASCEND_PDEV_MAX_NUM)) {
        DMS_ERR("Invalid parameter. (dev_id=%u; peer_phy_id=%u)\n", dev_id, peer_phy_id);
        return DRV_ERROR_INVALID_DEVICE;
    }

    if (g_p2p_restore_info.flag == DMS_RES_FLAG_ENABLE) {
        if (opcode == DEVDRV_P2P_ADD) {
            dms_atomic_inc(&g_p2p_restore_info.cnt[dev_id][peer_phy_id]);
            g_p2p_restore_info.p2p_type[dev_id][peer_phy_id] = p2p_type;
            return DRV_ERROR_NONE;
        } else if (opcode == DEVDRV_P2P_DEL) {
            if (g_p2p_restore_info.cnt[dev_id][peer_phy_id] > 0) {
                dms_atomic_dec(&g_p2p_restore_info.cnt[dev_id][peer_phy_id]);
            }
            return DRV_ERROR_NONE;
        } else {
            DMS_ERR("Invalid parameter. (dev_id=%u; peer_phy_id=%u; opcode=%u)\n", dev_id, peer_phy_id, opcode);
            return DRV_ERROR_INVALID_VALUE;
        }
    }

    return DRV_ERROR_NONE;
}

drvError_t dms_get_p2p_restore_info(u32 dev_id, u32 peer_phy_id, u32 *cnt, u32 *p2p_type)
{
    if ((dev_id >= ASCEND_PDEV_MAX_NUM) || (peer_phy_id >= ASCEND_PDEV_MAX_NUM)) {
        DMS_ERR("Invalid parameter. (dev_id=%u; peer_phy_id=%u)\n", dev_id, peer_phy_id);
        return DRV_ERROR_INVALID_DEVICE;
    }

    if (cnt == NULL) {
        DMS_ERR("para is NULL (dev_id=%u; peer_phy_id=%u)\n", dev_id, peer_phy_id);
        return DRV_ERROR_INVALID_VALUE;
    }

    *cnt = (unsigned int)g_p2p_restore_info.cnt[dev_id][peer_phy_id];
    *p2p_type = g_p2p_restore_info.p2p_type[dev_id][peer_phy_id];
    return DRV_ERROR_NONE;
}