/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef BBOX_INT_H
#define BBOX_INT_H

#include "mmpa_api.h"
#include "bbox_proc_types.h"

typedef void arg_void;
typedef void buff;

#define BBOX_TRUE    1
#define BBOX_FALSE   0

/***********time*********************/
#define NSEC_PER_USEC 1000
#define MSEC_PER_SEC 1000
#define USEC_PER_SEC 1000000U
#define NSEC_PER_SEC 1000000000

typedef struct bbox_time {
    u64 tv_sec;
    u64 tv_nsec;
} bbox_time_t;


/************************************/
#ifndef STATIC
#if defined(BBOX_UT_TEST) || defined(BBOX_ST_TEST)
#define STATIC
#else
#define STATIC static
#endif
#endif

#ifndef STATIC_INLINE
#if defined(BBOX_UT_TEST) || defined(BBOX_ST_TEST)
#define STATIC_INLINE
#else
#define STATIC_INLINE static inline
#endif
#endif

#ifndef UNUSED
#define UNUSED(x)   do {(void)(x);} while (0)
#endif

#endif /* BBOX_INT_H */
