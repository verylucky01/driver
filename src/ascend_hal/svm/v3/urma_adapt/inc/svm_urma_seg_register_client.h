/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef SVM_URMA_SEG_REGISTER_CLIENT_H
#define SVM_URMA_SEG_REGISTER_CLIENT_H

#include "svm_pub.h"
#include "svm_urma_def.h"
#include "svm_addr_desc.h"
#include "svm_urma_seg_mng.h"

int svm_urma_register_seg_client(u32 user_devid, struct svm_dst_va *dst_va,
    struct svm_urma_client_seg *client_seg, u32 seg_flag);
int svm_urma_unregister_seg_client(u32 user_devid, struct svm_dst_va *dst_va,
    struct svm_urma_client_seg *client_seg, u32 seg_flag);

#endif
