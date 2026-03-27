/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef SVM_ALLOC_H
#define SVM_ALLOC_H

#include "svm_pub.h"

int svm_module_mem_malloc(u32 devid, u32 numa_id, u64 flag, u64 *start, u64 size, u32 module_id);
int svm_module_mem_free(u32 devid, u64 flag, u64 start, u64 size, u32 module_id);

#endif

