/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef TRS_STL_H__
#define TRS_STL_H__
#include <stddef.h>
#include "ascend_hal_define.h"

inline static drvError_t trs_bind_stl(uint32_t dev_id)
{
    (void)dev_id;
    return DRV_ERROR_NOT_SUPPORT;
}
inline static drvError_t trs_launch_stl(uint32_t dev_id, void *param, size_t paramSize)
{
    (void)dev_id;
    (void)param;
    (void)paramSize;
    return DRV_ERROR_NOT_SUPPORT;
}
inline static drvError_t trs_query_stl(uint32_t dev_id, void *out, size_t *outSize)
{
    (void)dev_id;
    (void)out;
    (void)outSize;
    return DRV_ERROR_NOT_SUPPORT;
}

#endif
