/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef SVM_URMA_SEG_LOCAL_H
#define SVM_URMA_SEG_LOCAL_H

#include "svm_pub.h"
#include "svm_urma_def.h"
#include "svm_urma_seg_flag.h"

struct svm_urma_seg_info {
    u64 start;
    u64 size;
    u32 token_id;
    u32 token_val;
    u32 seg_flag;
    urma_target_seg_t *tseg;
};

int svm_urma_seg_local_dev_init(u32 devid);
int svm_urma_seg_local_dev_uninit(u32 devid);

int svm_urma_seg_local_register(u32 devid, u64 start, u64 size, u32 seg_flag);
int svm_urma_seg_local_unregister(u32 devid, u64 start, u64 size, u32 seg_flag);

int svm_urma_seg_local_get_info(u32 devid, u64 va, u32 seg_flag, struct svm_urma_seg_info *seg_info);

#endif
