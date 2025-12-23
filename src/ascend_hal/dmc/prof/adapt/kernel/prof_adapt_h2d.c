/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef UT_TEST
#include "prof_adapt_hdc.h"
#include "prof_adapt_kernel.h"
#include "prof_adapt_h2d.h"

static struct prof_h2d_ops g_prof_urma_ops = {NULL};

void prof_h2d_regiser_urma_ops(struct prof_h2d_ops *ops)
{
    g_prof_urma_ops.get_channels = ops->get_channels;
    g_prof_urma_ops.get_chan_ops = ops->get_chan_ops;
}

STATIC struct prof_h2d_ops *prof_h2d_get_urma_ops(void)
{
    return &g_prof_urma_ops;
}

drvError_t prof_kernel_get_channels(uint32_t dev_id, struct prof_channel_list *channels)
{
    struct prof_h2d_ops *urma_ops = prof_h2d_get_urma_ops();
    struct process_sign sign_info = {0};
    int64_t h2d_type;
    drvError_t ret;

    ret = drvGetProcessSign(&sign_info);
    if (ret != DRV_ERROR_NONE) {
        PROF_ERR("Failed get process sign. (devid=%u, ret=%d).\n", dev_id, (int)ret);
        return ret;
    }

    ret = halGetDeviceInfo(dev_id, MODULE_TYPE_SYSTEM, INFO_TYPE_HD_CONNECT_TYPE, &h2d_type);
    if (ret != DRV_ERROR_NONE) {
        PROF_ERR("Failed to get h2d_type. (dev_id=%u, ret=%d)\n", dev_id, (int)ret);
        return ret;
    }

    if (h2d_type == HOST_DEVICE_CONNECT_TYPE_UB) {
        if (urma_ops->get_channels == NULL) {
            PROF_ERR("Failed to get urma ops. (dev_id=%u)\n", dev_id);
            return DRV_ERROR_INNER_ERR;
        } else {
            return urma_ops->get_channels(dev_id, channels);
        }
    } else {
        return prof_hdc_kernel_get_channels(dev_id, channels);
    }

    return DRV_ERROR_NONE;
}

drvError_t prof_kernel_get_chan_ops(uint32_t dev_id, struct prof_chan_ops **ops)
{
    struct prof_h2d_ops *urma_ops = prof_h2d_get_urma_ops();
    int64_t h2d_type;
    drvError_t ret;

    ret = halGetDeviceInfo(dev_id, MODULE_TYPE_SYSTEM, INFO_TYPE_HD_CONNECT_TYPE, &h2d_type);
    if (ret != DRV_ERROR_NONE) {
        PROF_ERR("Failed to get h2d_type. (dev_id=%u, ret=%d)\n", dev_id, (int)ret);
        return ret;
    }

    if (h2d_type == HOST_DEVICE_CONNECT_TYPE_UB) {
        if (urma_ops->get_chan_ops == NULL) {
            PROF_ERR("Failed to get urma ops. (dev_id=%u)\n", dev_id);
            return DRV_ERROR_INNER_ERR;
        } else {
            return urma_ops->get_chan_ops(ops);
        }
    } else {
        return prof_hdc_get_chan_ops(ops);
    }

    return DRV_ERROR_NONE;
}

#else
int prof_adapt_h2d_ut_test(void)
{
    return 0;
}
#endif
