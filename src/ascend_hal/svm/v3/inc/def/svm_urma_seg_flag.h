/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef SVM_URMA_SEG_FLAG_H
#define SVM_URMA_SEG_FLAG_H

#include <stdbool.h>

#include "svm_pub.h"

#define SVM_URMA_SEG_FLAG_ACCESS_WRITE       (1U << 0U)
#define SVM_URMA_SEG_FLAG_PIN                (1U << 1U)
#define SVM_URMA_SEG_FLAG_SELF_USER          (1U << 2U)

static inline bool svm_urma_seg_flag_is_access_write(u32 seg_flag)
{
    return ((seg_flag & SVM_URMA_SEG_FLAG_ACCESS_WRITE) != 0);
}

static inline bool svm_urma_seg_flag_is_pin(u32 seg_flag)
{
    return ((seg_flag & SVM_URMA_SEG_FLAG_PIN) != 0);
}

static inline bool svm_urma_seg_flag_is_self_user(u32 seg_flag)
{
    return ((seg_flag & SVM_URMA_SEG_FLAG_SELF_USER) != 0);
}

#endif
