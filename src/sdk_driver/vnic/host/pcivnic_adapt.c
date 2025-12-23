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
#include <linux/types.h>
#include "pbl_dev_identity.h"
#include "comm/comm_pcie.h"
#include "pcivnic_main.h"

bool pcivnic_is_register_netdev(u32 dev_id)
{
    u32 chip_type = HISI_CHIP_NUM;

    chip_type = devdrv_get_dev_chip_type(dev_id);
    if (chip_type == HISI_CHIP_UNKNOWN) {
        return false;
    }

    /* CLOUD_V1 and CLOUD_V2's host not support vnic in release run pkg */
    if ((chip_type == HISI_CLOUD_V1) || (chip_type == HISI_CLOUD_V2)) {
        return false;
    } else {
        return true;
    }
}