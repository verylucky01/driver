/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef SVM_URMA_SEG_MSG_H
#define SVM_URMA_SEG_MSG_H

#include "urma_seg_msg_head.h"
#include "svm_urma_def.h"
#include "svm_pub.h"

/* SVM_URMA_SEG_REGISTER_EVENT */
struct svm_urma_seg_register_msg {
    struct svm_urma_seg_msg_head head;
    int valid;
    u32 token_val;
    urma_seg_t seg;
    u64 rsv;  /* reserve */
};

/* SVM_URMA_SEG_UNREGISTER_EVENT */
struct svm_urma_seg_unregister_msg {
    struct svm_urma_seg_msg_head head;
    u64 rsv;  /* reserve */
};

#endif

