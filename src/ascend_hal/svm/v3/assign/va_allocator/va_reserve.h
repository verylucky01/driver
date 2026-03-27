/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef VA_RESERVE_H
#define VA_RESERVE_H

#include <stdbool.h>

#include "svm_pub.h"

#define SVM_VA_RESERVE_FLAG_WITH_MASTER     (1U << 0U)
#define SVM_VA_RESERVE_FLAG_WITH_CUSTOM_CP  (1U << 1U)
#define SVM_VA_RESERVE_FLAG_WITH_HCCP       (1U << 2U)
#define SVM_VA_RESERVE_FLAG_MASTER_ONLY     (1U << 4U)
#define SVM_VA_RESERVE_FLAG_PRIVATE         (1U << 8U)

#define SVM_VA_RESERVE_MAX_SIZE (136ULL * SVM_BYTES_PER_TB)

/* va is not 0, specified va */
int svm_reserve_va(u64 va, u64 size, u32 flag, u64 *start);
int svm_release_va(u64 start);

int svm_reserve_master_only_va(u64 va, u64 size);
int svm_release_master_only_va(u64 start);

int va_reserve_add_dev(u32 devid);
void va_reserve_del_dev(u32 devid);
bool va_reserve_has_dev(void);

int svm_check_reserve_range(u64 va, u64 size, bool *is_reserved);

#endif
