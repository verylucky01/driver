/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef BBOX_DRV_API_H
#define BBOX_DRV_API_H

#include "ascend_hal.h"
#include "bbox_int.h"
#include "bbox_print.h"
#include "bbox_common.h"

static inline bbox_status bbox_drv_get_dev_num(u32 *device_num)
{
    drvError_t err = drvGetDevNum(device_num);
    if (err != DRV_ERROR_NONE) {
        BBOX_ERR("call drvGetDevNum failed(%d).", (s32)err);
        return BBOX_FAILURE;
    }
    return BBOX_SUCCESS;
}

static inline bbox_status bbox_drv_get_dev_id_by_local_id(u32 local_id, u32 *dev_id)
{
    if (bbox_check_feature(FEATURE_ID_CONVERT) == false) {
        *dev_id = local_id;
        return BBOX_SUCCESS;
    }

    drvError_t err = drvGetDevIDByLocalDevID(local_id, dev_id);
    if (err != DRV_ERROR_NONE) {
        BBOX_WAR("call drvGetDevIDByLocalDevID unsuccessfully(%d).", err);
        return BBOX_FAILURE;
    }
    return BBOX_SUCCESS;
}

#endif /* BBOX_DRV_API_H */
