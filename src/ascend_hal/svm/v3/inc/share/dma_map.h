/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef DMA_MAP_H
#define DMA_MAP_H

#include "svm_pub.h"
#include "svm_addr_desc.h"
#include "dma_map_flag.h"

/*
    map dst va to dma addr which device use, and save the dma addr for later query
    case1: dst va is device addr, user devid is also same device, send msg to the device do dma map,
           then return back the dma addr(is device smmu disable, only query pa)
    case2: dst va is host addr, user devid is device, map dma addr use this device
    case3: dst va is device addr, user devid is another device, query the bar addr, then use case2
*/

int svm_dma_map(u32 user_devid, struct svm_dst_va *dst_va, u32 flag);
int svm_dma_unmap(u32 user_devid, struct svm_dst_va *dst_va);

#endif

