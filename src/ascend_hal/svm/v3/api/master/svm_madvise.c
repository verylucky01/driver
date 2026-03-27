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

#include "svm_log.h"
#include "svm_pagesize.h"
#include "svm_dbi.h"
#include "malloc_mng.h"
#include "madvise_client.h"

static u32 advise_type_to_advise_flag(u32 type)
{
    u32 flag = 0;

    flag |= (type == ADVISE_ACCESS_READONLY) ? SVM_MADVISE_ACCESS_READ : 0;
    flag |= (type == ADVISE_ACCESS_READWRITE) ? (SVM_MADVISE_ACCESS_READ | SVM_MADVISE_ACCESS_WRITE) : 0;

    return flag;
}

static int mem_advise_check(u32 devid, u64 va, u64 size, u64 *aligned_size)
{
    struct svm_prop prop;
    int ret;

    ret = svm_get_prop(va, &prop);
    if (ret != 0) {
        svm_err("Get prop failed. (va=0x%llx)\n", va);
        return ret;
    }

    if (devid != prop.devid) {
        svm_err("Devid is invalid. (va=0x%llx; size=%llu; devid=%u; prop start=0x%llx; size=%llu; devid=%u)\n",
            va, size, devid, prop.start, prop.size, prop.devid);
        return DRV_ERROR_INVALID_DEVICE;
    }

    ret = svm_get_aligned_size(devid, prop.flag, size, aligned_size);
    if (ret != 0) {
        svm_err("Get aligned size fail. (va=0x%llx; size=%llu; flag=0x%llx)\n", va, size, prop.flag);
        return DRV_ERROR_INVALID_VALUE;
    }

    if ((va != prop.start) || !svm_is_page_align(devid, prop.flag, va) || (*aligned_size != prop.aligned_size)) {
        svm_err("Va or size is invalid. (va=0x%llx; size=%llu; aligned_size=%llu; prop start=0x%llx; "
            "aligned_size=%llu)\n", va, size, *aligned_size, prop.start, prop.aligned_size);
        return DRV_ERROR_INVALID_VALUE;
    }

    if (!svm_flag_cap_is_support_madvise(prop.flag)) {
        svm_run_info("Addr cap is not support madvise. (va=0x%llx; flag=0x%llx)\n", va, prop.flag);
        return DRV_ERROR_NOT_SUPPORT;
    }

    if (svm_get_host_devid() == prop.devid) {
        svm_run_info("Not support host va. (va=0x%llx; size=%llu; devid=%u)\n", va, size, prop.devid);
        return DRV_ERROR_NOT_SUPPORT;
    }
    return DRV_ERROR_NONE;
}

static int mem_advise(u32 devid, u64 va, u64 size, u32 type)
{
    u32 flag = advise_type_to_advise_flag(type);
    u64 aligned_size;
    int ret;

    ret = mem_advise_check(devid, va, size, &aligned_size);
    if (ret != 0) {
        return ret;
    }
    return svm_madvise_client(devid, va, aligned_size, flag);
}

typedef int (*svm_advise_policy)(u32 devid, u64 va, u64 size, u32 type);
drvError_t halMemAdvise(DVdeviceptr ptr, size_t count, unsigned int type, DVdevice device)
{
    static const svm_advise_policy advise_policy[ADVISE_TYPE_MAX] = {
        [ADVISE_ACCESS_READONLY] = mem_advise,
        [ADVISE_ACCESS_READWRITE] = mem_advise,
    };

    if ((type >= ADVISE_TYPE_MAX) || (advise_policy[type] == NULL)) {
        svm_run_info("Advise not support this type. (type=%u)\n", type);
        return DRV_ERROR_NOT_SUPPORT;
    }
    svm_debug("halMemAdvise. (ptr=0x%llx; count=%lu; type=%u; devid=%u)\n", ptr, count, type, device);
    return advise_policy[type](device, ptr, count, type);
}
