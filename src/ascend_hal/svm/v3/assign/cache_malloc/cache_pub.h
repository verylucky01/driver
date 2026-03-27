/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef CACHE_PUB_H
#define CACHE_PUB_H

#include "svm_pub.h"
#include "va_allocator.h"
#include "cache_malloc.h"
#include "normal_malloc.h"

#define CACHE_TYPE_MAX_BIT          4U
#define CACHE_TYPE_BIT_MASK         ((1U << CACHE_TYPE_MAX_BIT) - 1U)
#define CACHE_TYPE_MAX              (CACHE_TYPE_BIT_MASK + 1U)

static inline u32 cache_flag_to_cache_type(u32 devid, u32 cache_flag)
{
    if (devid < SVM_MAX_AGENT_NUM) {
        return (cache_flag & CACHE_TYPE_BIT_MASK);
    } else {
        if ((cache_flag & SVM_CACHE_MALLOC_FLAG_MASTER_UVA) == 0) {
            return 0;
        }

        return (svm_is_support_pcie_th()) ? 1 : 0;
    }
}

static inline u32 cache_flag_to_normal_flag(u32 cache_flag)
{
    u32 normal_flag = SVM_NORMAL_MALLOC_FLAG_CAP_COPY | SVM_NORMAL_MALLOC_FLAG_VA_WITH_MASTER;

    normal_flag |= ((cache_flag & SVM_CACHE_MALLOC_FLAG_PA_HPAGE) != 0) ? SVM_NORMAL_MALLOC_FLAG_PA_HPAGE : 0;
    normal_flag |= ((cache_flag & SVM_CACHE_MALLOC_FLAG_PA_P2P) != 0) ? SVM_NORMAL_MALLOC_FLAG_PA_P2P : 0;
    normal_flag |= ((cache_flag & SVM_CACHE_MALLOC_FLAG_MASTER_UVA) != 0) ? SVM_NORMAL_MALLOC_FLAG_MASTER_UVA : 0;

    return normal_flag;
}

#endif
