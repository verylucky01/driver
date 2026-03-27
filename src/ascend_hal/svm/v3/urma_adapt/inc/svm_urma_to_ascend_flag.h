/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef SVM_URMA_TO_ASCEND_FLAG_H
#define SVM_URMA_TO_ASCEND_FLAG_H

#include "comm_user_interface.h"

#include "svm_urma_seg_flag.h"

static inline u32 svm_urma_to_ascend_seg_flag(u32 flag)
{
    u32 ascend_seg_flag = 0;

    ascend_seg_flag |= svm_urma_seg_flag_is_access_write(flag) ? ASCEND_URMA_SEG_FLAG_ACCESS_WRITE : 0;
    ascend_seg_flag |= svm_urma_seg_flag_is_pin(flag) ? ASCEND_URMA_SEG_FLAG_PIN : 0;
    ascend_seg_flag |= svm_urma_seg_flag_is_self_user(flag) ? ASCEND_URMA_SEG_FLAG_WITHOUT_TOKEN_VAL : 0;

    return ascend_seg_flag;
}

#endif

