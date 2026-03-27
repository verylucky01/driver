/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include <stdlib.h>
#include <stdint.h>
#include <securec.h>

#include "urma_api.h"
#include "urma_types.h"

#include "ascend_hal.h"
#include "ascend_hal_error.h"
#include "dms_user_interface.h"
#include "drv_user_common.h"

#include "ascend_urma_log.h"
#include "ascend_urma_dev.h"

#define ASCEND_MAX_URMA_DEV_NAME_LEN       DMS_URMA_NAME_MAX_LEN

static urma_device_t *ascend_urma_get_device_when_hd_ub_conn(u32 devid, u32 *eid_index)
{
    struct dms_ub_dev_info eid_info = {{0}};
    int num, ret;

    ret = dms_get_ub_dev_info(devid, &eid_info, &num);
    if ((ret != DRV_ERROR_NONE) || (num == 0)) {
        ascend_urma_info("dms_get_ub_dev_info check. (ret=%d)\n", ret);
        return NULL;
    }

    *eid_index = eid_info.eid_index[0];
    return (urma_device_t *)eid_info.urma_dev[0];
}

static urma_device_t *ascend_urma_get_device_when_hd_others_conn(u32 devid, u32 *eid_index)
{
    urma_eid_info_t *eid_info = NULL;
    urma_device_t **urma_dev_list = NULL;
    urma_device_t *urma_dev = NULL;
    int i, num_devices;
    uint32_t cnt;

    urma_dev_list = urma_get_device_list(&num_devices);
    if (urma_dev_list == NULL) {
        ascend_urma_info("urma_get_device_list failed.\n");
        return NULL;
    }

    for (i = 0; i < num_devices; i++) {
        eid_info = urma_get_eid_list(urma_dev_list[i], &cnt);
        if ((eid_info != NULL) && (cnt != 0)) {
            *eid_index = eid_info[0].eid_index;
            urma_dev = urma_dev_list[i];
            urma_free_eid_list(eid_info);
            break;
        }
        urma_free_eid_list(eid_info);
    }
    urma_free_device_list(urma_dev_list);

    return urma_dev;
}

urma_device_t *ascend_urma_get_device(u32 devid, u32 *eid_index)
{
    int64_t hd_connect_type;
    int ret;

    ret = halGetDeviceInfo(devid, MODULE_TYPE_SYSTEM, INFO_TYPE_HD_CONNECT_TYPE, &hd_connect_type);
    if (ret != DRV_ERROR_NONE) {
        ascend_urma_info("halGetDeviceInfo check. (devid=%u; ret=%d)\n", devid, ret);
        return NULL;
    }

    if (hd_connect_type == HOST_DEVICE_CONNECT_TYPE_UB) {
        return ascend_urma_get_device_when_hd_ub_conn(devid, eid_index);
    } else {
        return ascend_urma_get_device_when_hd_others_conn(devid, eid_index);
    }
}

bool ascend_urma_dev_is_exist(uint32_t devid)
{
    u32 eid_index;
    return (ascend_urma_get_device(devid, &eid_index) != NULL);
}
