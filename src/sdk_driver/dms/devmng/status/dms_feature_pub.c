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

#include "dms_define.h"
#include "pbl/pbl_uda.h"

int dms_trans_and_check_id(u32 logical_dev_id, u32 *physical_dev_id, u32 *vfid)
{
    int ret;

    if (logical_dev_id >= ASCEND_DEV_MAX_NUM) {
        dms_err("Wrong logic id. (logic_id=%u)\n", logical_dev_id);
        return -EINVAL;
    }

    ret = uda_devid_to_phy_devid(logical_dev_id, physical_dev_id, vfid);
    if (ret != 0) {
        dms_err("Transfer logic id to phy id failed. (logic_id=%u; ret=%d)\n", logical_dev_id, ret);
        return ret;
    }

    if (*physical_dev_id >= ASCEND_DEV_MAX_NUM) {
        dms_err("wrong phy id. (logic_id=%u; phy_id=%u)\n", logical_dev_id, *physical_dev_id);
        return -EINVAL;
    }

    return 0;
}