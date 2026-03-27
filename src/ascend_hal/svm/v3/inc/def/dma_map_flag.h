/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef DMA_MAP_FLAG_H
#define DMA_MAP_FLAG_H

#include <stdbool.h>

#include "svm_pub.h"

#define SVM_DMA_MAP_ACCESS_WRITE            (1u << 0u)
#define SVM_DMA_MAP_VA_IO_MAP               (1u << 1u)

static inline bool svm_dma_map_flag_is_access_write(u32 flag)
{
    return ((flag & SVM_DMA_MAP_ACCESS_WRITE) != 0);
}

static inline bool svm_dma_map_flag_is_va_io_map(u32 flag)
{
    return ((flag & SVM_DMA_MAP_VA_IO_MAP) != 0);
}

#endif
