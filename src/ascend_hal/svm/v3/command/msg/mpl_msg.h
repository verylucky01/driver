/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef MPL_MSG_H
#define MPL_MSG_H

#include "svm_pub.h"

struct svm_mpl_populate_msg {
    u64 va;
    u64 size;
    u32 flag;
    u64 rsv;  /* reserve */
};

struct svm_mpl_depopulate_msg {
    u64 va;
    u64 size;
    int is_busy;
    u64 rsv;  /* reserve */
};

#endif

