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

#include <linux/slab.h>
#include <linux/uaccess.h>
#include "pbl/pbl_uda.h"
#include "securec.h"
#include "dms_define.h"
#include "devdrv_user_common.h"
#include "devdrv_common.h"
#include "ascend_kernel_hal.h"

#ifdef CFG_HOST_ENV
int hal_kernel_get_device_chip_die_id(u32 dev_id, u32 *chip_id, u32 *die_id)
{
    struct devdrv_info *dev_info = NULL;

    if ((chip_id == NULL) ||
        (die_id == NULL) ||
        (!uda_is_udevid_exist(dev_id))) {
        dms_err("Invalid parameter. (dev_id=%u; chip_id=%s; die_id=%s)\n",
            dev_id, (chip_id == NULL) ? "NULL" : "OK", (die_id == NULL) ? "NULL" : "OK");
        return -EINVAL;
    }

    dev_info = devdrv_manager_get_devdrv_info(dev_id);
    if (dev_info == NULL) {
        dms_err("Device is not initialized. (dev_id=%u)\n", dev_id);
        return -EINVAL;
    }

    *chip_id = dev_info->chip_id;
    *die_id = dev_info->die_id;
    return 0;
}
EXPORT_SYMBOL_GPL(hal_kernel_get_device_chip_die_id);

int hal_kernel_get_soc_type(u32 dev_id, u32 *soc_type)
{
    u32 chip_type;
    struct devdrv_info *dev_info = NULL;
    /*
     * soc type map from uda definition to hal kernel
     ****   uda chip type ******* hal soc type ******
     * HISI_MINI_V1  (0)          SOC_TYPE_MINI
     * HISI_CLOUD_V1 (1)          SOC_TYPE_CLOUD
     * HISI_MINI_V2  (2)          SOC_TYPE_DC
     * HISI_CLOUD_V2 (3)          SOC_TYPE_CLOUD_V2 or SOC_TYPE_CLOUD_V3
     * HISI_MINI_V3  (4)          SOC_TYPE_MINI_V3
     * HISI_CLOUD_V4 (6)          SOC_TYPE_CLOUD_V4
     * HISI_CLOUD_V5 (7)          SOC_TYPE_CLOUD_V5
     * HISI_CHIP_NUM (9)          SOC_TYPE_MAX
     * HISI_CHIP_UNKNOWN (10)      SOC_TYPE_MAX
     */
    u32 chip_type_map[HISI_CHIP_NUM] = {SOC_TYPE_MINI, SOC_TYPE_CLOUD, SOC_TYPE_DC, 
        SOC_TYPE_CLOUD_V2, SOC_TYPE_MINI_V3, SOC_TYPE_RSVD_4, SOC_TYPE_CLOUD_V4, SOC_TYPE_CLOUD_V5, SOC_TYPE_RSVD_6};

    if ((soc_type == NULL) ||
        (!uda_is_udevid_exist(dev_id))) {
        dms_err("Invalid parameter. (dev_id=%u; soc_type=%s)\n",
            dev_id, (soc_type == NULL) ? "NULL" : "OK");
        return -EINVAL;
    }

    chip_type = uda_get_chip_type(dev_id);
    dev_info = devdrv_manager_get_devdrv_info(dev_id);
    if (dev_info == NULL) {
        dms_err("Device is not initialized. (dev_id=%u)\n", dev_id);
        return -EINVAL;
    }

    if (chip_type >= HISI_CHIP_NUM) {
        *soc_type = SOC_TYPE_MAX;
    } else if (chip_type == HISI_CLOUD_V2) {
        *soc_type = (dev_info->multi_die == 1) ? SOC_TYPE_CLOUD_V3 : CHIP_CLOUD_V2;
    } else {
        *soc_type = chip_type_map[chip_type];
    }

    return 0;
}
EXPORT_SYMBOL_GPL(hal_kernel_get_soc_type);
#else
int hal_kernel_get_device_chip_die_id(u32 dev_id, u32 *chip_id, u32 *die_id)
{
    if ((chip_id == NULL) ||
        (die_id == NULL) ||
        (!uda_is_udevid_exist(dev_id))) {
        dms_err("Invalid parameter. (dev_id=%u; chip_id=%s; die_id=%s)\n",
            dev_id, (chip_id == NULL) ? "NULL" : "OK", (die_id == NULL) ? "NULL" : "OK");
        return -EINVAL;
    }

    return devdrv_get_chip_die_id(dev_id, chip_id, die_id);
}
EXPORT_SYMBOL_GPL(hal_kernel_get_device_chip_die_id);
#endif

int hal_kernel_get_device_addr_mode(u32 dev_id, HAL_KERNEL_ADDR_MODE *addr_mode)
{
    struct devdrv_info *dev_info = NULL;

    if ((addr_mode == NULL) ||
        (!uda_is_udevid_exist(dev_id))) {
        dms_err("Invalid parameter. (dev_id=%u; addr_mode=%s)\n",
            dev_id, (addr_mode == NULL) ? "NULL" : "OK");
        return -EINVAL;
    }

    dev_info = devdrv_manager_get_devdrv_info(dev_id);
    if (dev_info == NULL) {
        dms_err("Device is not initialized. (dev_id=%u)\n", dev_id);
        return -EINVAL;
    }

    *addr_mode = (HAL_KERNEL_ADDR_MODE)dev_info->addr_mode;
    return 0;
}
EXPORT_SYMBOL_GPL(hal_kernel_get_device_addr_mode);

int __attribute__((weak)) hal_kernel_get_d2d_topology_type(u32 dev_id1, u32 dev_id2, HAL_KERNEL_TOPOLOGY_TYPE *topology_type)
{
    return -EOPNOTSUPP;
}

int __attribute__((weak)) hal_kernel_get_spod_node_status(u32 dev_id, u32 sdid, u32 *status)
{
    (void) dev_id;
    (void) sdid;
    (void) status;
    return -EOPNOTSUPP;
}
EXPORT_SYMBOL_GPL(hal_kernel_get_spod_node_status);