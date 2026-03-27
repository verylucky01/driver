/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include <securec.h>

#include "ascend_hal.h"

#include "svm_pub.h"
#include "svm_user_adapt.h"
#include "svm_log.h"
#include "svm_memset.h"

/* because memset_s 2nd para must less than(or equal to) 2G, devmm will set big memory by loop */
int svm_memset(u64 dst, u64 dst_max, u8 value, u64 count)
{
    size_t rest_count, per_count;
    u64 tmp_dst = dst;
    int ret;

    if (count <= SECUREC_MEM_MAX_LEN && dst_max <= SECUREC_MEM_MAX_LEN) {
        ret = svm_memset_s((void *)(uintptr_t)tmp_dst, dst_max, value, count);
        if (ret != 0) {
            svm_err("Memset error. (dst=0x%llx; dstMax=%llu; value=%u; count=%llu; ret=%d)\n",
                tmp_dst, dst_max, value, count, ret);
            return DRV_ERROR_INVALID_VALUE;
        }
    } else if (count <= SECUREC_MEM_MAX_LEN && dst_max > SECUREC_MEM_MAX_LEN) {
        ret = svm_memset_s((void *)(uintptr_t)tmp_dst, SECUREC_MEM_MAX_LEN, value, count);
        if (ret != 0) {
            svm_err("Memset error. (dst=0x%llx; dstMax=%llu; value=%u; count=%llu; ret=%d)\n",
                tmp_dst, SECUREC_MEM_MAX_LEN, value, count, ret);
            return DRV_ERROR_INVALID_VALUE;
        }
    } else if (count > SECUREC_MEM_MAX_LEN) {
        for (rest_count = count; rest_count > 0;) {
            per_count = rest_count > SECUREC_MEM_MAX_LEN ? SECUREC_MEM_MAX_LEN : rest_count;
            ret = svm_memset_s((void *)(uintptr_t)tmp_dst, per_count, value, per_count);
            if (ret != 0) {
                svm_err("Memset error. (dst=0x%llx; value=%u; per_count=%lu; ret=%d)\n",
                    tmp_dst, value, per_count, ret);
                return DRV_ERROR_INVALID_VALUE;
            }
            tmp_dst = tmp_dst + per_count;
            rest_count = rest_count - per_count;
        }
    }

    return DRV_ERROR_NONE;
}
