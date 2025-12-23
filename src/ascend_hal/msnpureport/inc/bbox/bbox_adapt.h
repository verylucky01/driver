/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef BBOX_ADAPT_H
#define BBOX_ADAPT_H

#include <bbox_int.h>

#define FEATURE_CLOCK                      (1ULL << 0)     // clock config
#define FEATURE_TMSTMP                     (1ULL << 1)     // log with timestamp
#define FEATURE_ID_CONVERT                 (1ULL << 2)     // is need to convert devid
#define FEATURE_MKDIR_RECUR                (1ULL << 3)     // mkdir recursively

u32 bbox_get_feature(void);
#endif // BBOX_ADAPT_H
