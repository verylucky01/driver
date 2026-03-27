/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef CACHE_RECYCLE_SEG_H
#define CACHE_RECYCLE_SEG_H

#include <stdint.h>

#include "svm_pub.h"

void cache_recycle_add_seg(u64 start, u64 size, u64 align, u32 devid, u32 flag);
u32 cache_recycle_seg_show(u32 devid, char *buf, u32 buf_len);

#endif
