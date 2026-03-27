/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef VA_DEV_DEFAULT_ALLOCATOR_H
#define VA_DEV_DEFAULT_ALLOCATOR_H

#include "svm_pub.h"

int va_dev_default_allocator_dev_init(u32 devid);
void va_dev_default_allocator_dev_uninit(u32 devid);

u64 va_get_dev_default_alloc_max_size(u32 devid);

int va_dev_default_alloc(u32 devid, u64 align, u64 size, u64 *va);
int va_dev_default_free(u32 devid, u64 va, u64 size, u64 align);

#endif
