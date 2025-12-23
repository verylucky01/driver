/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "ascend_hal.h"
#include "uda_user_base.h"

drvError_t drvGetDevNum(uint32_t *devNum)
{
    return uda_user_get_dev_num(devNum);
}

drvError_t drvGetDevIDs(uint32_t *devices, uint32_t len)
{
    return uda_user_get_dev_ids(devices, len);
}

drvError_t halGetDevNumEx(uint32_t hw_type, uint32_t *devNum)
{
    return uda_user_get_dev_num_ex(hw_type, devNum);
}

drvError_t halGetDevIDsEx(uint32_t hw_type, uint32_t *devices, uint32_t len)
{
    return uda_user_get_dev_ids_ex(hw_type, devices, len);
}

drvError_t halGetVdevNum(uint32_t *devNum)
{
    return uda_user_get_vdev_num(devNum);
}

drvError_t halGetVdevIDs(uint32_t *devices, uint32_t len)
{
    return uda_user_get_vdev_ids(devices, len);
}

drvError_t drvDeviceGetPhyIdByIndex(uint32_t devid, uint32_t *phyId)
{
    return uda_user_get_phy_id_by_index(devid, phyId);
}

drvError_t drvDeviceGetIndexByPhyId(uint32_t phyId, uint32_t *devid)
{
    return uda_user_get_index_by_phy_id(phyId, devid);
}

drvError_t drvGetDeviceLocalIDs(uint32_t *devices, uint32_t len)
{
    return uda_user_get_device_local_ids(devices, len);
}

drvError_t drvGetDevIDByLocalDevID(uint32_t local_devid, uint32_t *remote_udevid)
{
    return uda_user_get_devid_by_local_devid(local_devid, remote_udevid);
}

drvError_t drvGetLocalDevIDByHostDevID(uint32_t remote_udevid, uint32_t *local_devid)
{
    return uda_user_get_local_devid_by_host_devid(remote_udevid, local_devid);
}

drvError_t halGetPhyDevIdByLogicDevId(unsigned int dev_id, unsigned int *phy_dev_id)
{
    return uda_user_get_phy_devid_by_logic_devid(dev_id, phy_dev_id);
}

drvError_t halGetHostID(uint32_t *host_id)
{
    return uda_user_get_host_id(host_id);
}