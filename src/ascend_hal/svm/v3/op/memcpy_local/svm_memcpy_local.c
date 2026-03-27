/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include <errno.h>
#include <securec.h>

#include "ascend_hal.h"

#include "svm_user_adapt.h"
#include "svm_pub.h"
#include "svm_log.h"

#ifndef EMU_ST /* for emu st, do not delete */
#define SDMA_MAX_COPY_SIZE (4ULL * SVM_BYTES_PER_GB)
#else
#define SDMA_MAX_COPY_SIZE (1ULL * SVM_BYTES_PER_MB)
#endif

static int _svm_memcpy_local(u64 dst, u64 dst_max, u64 src, u64 count)
{
    int ret;

    ret = halSdmaCopy(dst, dst_max, src, count);
    if (ret != 0) {
        ret = svm_memcpy_s((void *)(uintptr_t)dst, dst_max, (void *)(uintptr_t)src, count);
    }

    return ret;
}

static bool is_memcpy_overlap(u64 dst, u64 src, u64 count)
{
    u64 src_end = src + count;
    u64 dst_end = dst + count;
    return (((src >= dst) && (src < dst_end)) || ((dst >= src) && (dst < src_end)));
}

int svm_memcpy_local(u64 dst, u64 dst_max, u64 src, u64 count)
{
    size_t split_size = svm_min(SECUREC_MEM_MAX_LEN, SDMA_MAX_COPY_SIZE);
    size_t rest_count, per_count;
    u64 tmp_dst = dst;
    u64 tmp_src = src;
    int ret;

    if (dst_max < count) {
        svm_err("Count bigger than dstMax. (dst=0x%llx; src=0x%llx; count=%lu; dstMax=%lu)\n",
            tmp_dst, tmp_src, count, dst_max);
        return DRV_ERROR_INVALID_VALUE;
    }

    if (is_memcpy_overlap(tmp_dst, tmp_src, count)) {
        svm_err("Memcpy overlap. (dst=0x%llx; src=0x%llx; count=%lu; dstMax=%lu)\n",
            tmp_dst, tmp_src, count, dst_max);
        return DRV_ERROR_INVALID_VALUE;
    }

    if (count <= split_size) {
        ret = _svm_memcpy_local(tmp_dst, count, tmp_src, count);
        if (ret != 0) {
            svm_err("Memcpy error. (dst=0x%llx; dstMax=%llu; src=0x%llx; count=%llu; ret=%d)\n",
                tmp_dst, dst_max, tmp_src, count, ret);
            return DRV_ERROR_INVALID_VALUE;
        }
    } else {
        for (rest_count = count; rest_count > 0;) {
            per_count = rest_count > split_size ? split_size : rest_count;
            ret = _svm_memcpy_local(tmp_dst, per_count, tmp_src, per_count);
            if (ret != 0) {
                svm_err("Memcpy error. (dst=0x%llx; src=0x%llx; per_count=%lu; ret=%d)\n",
                    tmp_dst, tmp_src, per_count, ret);
                return DRV_ERROR_INVALID_VALUE;
            }
            tmp_dst = tmp_dst + per_count;
            tmp_src = tmp_src + per_count;
            rest_count = rest_count - per_count;
        }
    }

    return DRV_ERROR_NONE;
}
