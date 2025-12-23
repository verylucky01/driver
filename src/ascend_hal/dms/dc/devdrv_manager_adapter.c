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
#include "devmng_common.h"
#include "dms_device_info.h"
#include "ascend_dev_num.h"
#include "dms_drv_internal.h"

drvError_t halGetSocVersion(uint32_t devId, char *socVersion, uint32_t len)
{
#ifdef CFG_FEATURE_SOC_VERSION
    int ret;
    struct devdrv_device_info info = {0};
    int64_t value;

    if (socVersion == NULL || len < SOC_VERSION_LEN || devId >= ASCEND_DEV_MAX_NUM) {
        DEVDRV_DRV_ERR("Invalid parameter. (devId=%u, len=%u)\n", devId, len);
        return DRV_ERROR_INVALID_VALUE;
    }

    ret = halGetDeviceInfo(devId, MODULE_TYPE_SYSTEM, INFO_TYPE_VERSION, &value);
    if (ret != 0) {
        DEVDRV_DRV_ERR("Hal get device info failed. (dev_id=%u; ret=%d)\n", devId, ret);
        return ret;
    }

    ret = drvGetDevInfo(devId, &info);
    if (ret != 0) {
        DEVDRV_DRV_ERR("Drv get device info failed. (dev_id=%u; ret=%d)\n", devId, ret);
        return ret;
    }

    ret = strncpy_s(socVersion, SOC_VERSION_LEN, info.soc_version, SOC_VERSION_LEN - 1);
    if (ret != 0) {
        DEVDRV_DRV_ERR("strncpy_s failed. (ret=%d)\n", ret);
        return DRV_ERROR_MEMORY_OPT_FAIL;
    }

    return DRV_ERROR_NONE;
#else
    (void)devId;
    (void)socVersion;
    (void)len;
    return DRV_ERROR_NOT_SUPPORT;
#endif
}