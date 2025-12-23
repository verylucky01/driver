/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef QUE_COMPILER_H
#define QUE_COMPILER_H

#define que_align_up(val, al)       (((val) + ((typeof(val))(al) - 1)) & ~((typeof(val))(al) - 1))
#define que_align_down(val, al)     ((val) & ~((typeof(val))(al) - 1))

#define que_likely(x)           __builtin_expect(!!(x), 1)
#define que_unlikely(x)         __builtin_expect(!!(x), 0)

#define que_min(x, y)           (((x) > (y)) ? (y) : (x))
#define que_max(x, y)           (((x) > (y)) ? (x) : (y))

#endif
