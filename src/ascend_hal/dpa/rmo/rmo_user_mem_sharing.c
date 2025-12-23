/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include <sys/ioctl.h>

#include "securec.h"
#include "ascend_hal.h"
#include "davinci_interface.h"
#include "rmo_ioctl.h"
#include "rmo.h"

static drvError_t rmo_mem_sharing_para_check(struct drvMemSharingPara *para)
{
    if (para == NULL) {
        rmo_err("Para is NULL.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    if ((para->ptr == NULL) || (para->size == 0)) {
        rmo_err("Invalid para. (ptr=0x%llx; size=%llu)\n", (uint64_t)(uintptr_t)para->ptr, para->size);
        return DRV_ERROR_INVALID_VALUE;
    }

    if ((para->accessor < 0) || (para->accessor >= ACCESSOR_MAX)) {
        rmo_err("Invalid accessor. (accessor=%d)\n", para->accessor);
        return DRV_ERROR_INVALID_VALUE;
    }

    if (para->side != MEM_HOST_SIDE) {
        return DRV_ERROR_NOT_SUPPORT;
    }

    if ((para->enable_flag != 0) && (para->enable_flag != 1)) {
        rmo_err("Invalid enable_flag. (enable_flag=%d)\n", para->enable_flag);
        return DRV_ERROR_INVALID_VALUE;
    }

    return DRV_ERROR_NONE;
}

drvError_t halSetMemSharing(struct drvMemSharingPara *para)
{
    struct rmo_cmd_mem_sharing arg = {0};
    drvError_t ret;

    ret = rmo_mem_sharing_para_check(para);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    arg.devid = para->id;
    arg.side = para->side;
    arg.ptr = para->ptr;
    arg.size = para->size;
    arg.accessor = para->accessor;
    arg.enable_flag = para->enable_flag;
    arg.pg_prot = para->pg_prot;
    ret = rmo_cmd_ioctl(RMO_MEM_SHARING, &arg);
    if (ret != DRV_ERROR_NONE) {
        rmo_err("Dispatch memory failed. (ret=%d; id=%u; side=%d; accessor=%u; va=0x%llx; size=%llu)\n",
            ret, para->id, para->side, para->accessor, (uint64_t)(uintptr_t)para->ptr, para->size);
        return ret;
    }
    rmo_debug("Dispatch memory success. (id=%u; side=%d; accessor=%u; va=0x%llx; size=%llu)\n",
        para->id, para->side, para->accessor, (uint64_t)(uintptr_t)para->ptr, para->size);
    return DRV_ERROR_NONE;
}
