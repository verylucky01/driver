/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef VA_RESERVE_MSG_H
#define VA_RESERVE_MSG_H

#include "svm_pub.h"

#define SVM_MMAP_FLAG_PRIVATE           (1U << 0U)

#define VA_RESERVE_STATUS_OK 1
#define VA_RESERVE_STATUS_FAIL 2
#define VA_RESERVE_STATUS_FAIL_WITH_SUGGEST 3

/* SVM_VA_RESERVE_EVENT */
struct svm_va_reserve_msg {
    int op; /* 1. mmap, 0. munmap */
    u32 flag;
    u64 va;
    u64 size;
    int status;
};

#endif
