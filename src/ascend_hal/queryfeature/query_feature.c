/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <stdbool.h>

#include "ascend_hal.h"
#include "queryfeature_usr_pub_def.h"
#include "query_feature.h"
#ifndef CFG_FEATURE_PROF_AICPU_CHAN_DEFUALT
#include "dms_user_interface.h"
#endif
#include "svm_user_interface.h"

typedef bool (*feature_support_handle)(uint32_t devId);

static bool featureSupportTsDrvSqDevmemPrio(uint32_t devId)
{
    (void)devId;
    return TRSDRV_SQ_DEVICE_MEM_PRIORITY_SUPPORT;
}

static bool featureSupportTsDrvSqDanamicBind(uint32_t devId)
{
    uint32_t mode;
    int ret;

    ret = halGetDeviceSplitMode(devId, &mode);
    if (ret != DRV_ERROR_NONE) {
        return false;
    }

    if (mode == VMNG_NORMAL_NONE_SPLIT_MODE) {
        return true;
    }
    return false;
}

static bool featureTsDrvIsSupportSqDanamicBindVersion(uint32_t devId)
{
    (void)devId;
    return true;
}

static bool featureSupportProfAicpuChan(uint32_t devId)
{
    (void)devId;
#ifdef CFG_FEATURE_PROF_AICPU_CHAN_DEFUALT
    #ifdef CFG_FEATURE_PROF_AICPU_CHAN_NOT_SUPPORT
        return false;
    #else
        return true;
    #endif
#else
    int ret;
    unsigned int mode;

    ret = halGetDeviceSplitMode(devId, &mode);
    if (ret != DRV_ERROR_NONE) {
        return false;
    }

    return (mode == VMNG_NORMAL_NONE_SPLIT_MODE);
#endif
}

static const feature_support_handle g_feature_support[FEATURE_MAX] = {
    [FEATURE_TRSDRV_SQ_DEVICE_MEM_PRIORITY] = featureSupportTsDrvSqDevmemPrio,
    [FEATURE_PROF_AICPU_CHAN] = featureSupportProfAicpuChan,
    [FEATURE_SVM_GET_USER_MALLOC_ATTR] = svm_support_get_user_malloc_attr,
    [FEATURE_TRSDRV_SQ_SUPPORT_DYNAMIC_BIND] = featureSupportTsDrvSqDanamicBind,
    [FEATURE_SVM_VMM_NORMAL_GRANULARITY] = svm_support_vmm_normal_granularity,
    [FEATURE_TRSDRV_IS_SQ_SUPPORT_DYNAMIC_BIND_VERSION] = featureTsDrvIsSupportSqDanamicBindVersion,
};

bool halSupportFeature(uint32_t devId, drvFeature_t type)
{
    if (type < 0 || type >= FEATURE_MAX) {
        return false;
    }

    if (g_feature_support[type] != NULL) {
        return (g_feature_support[type])(devId);
    } else {
        return false;
    }
}
