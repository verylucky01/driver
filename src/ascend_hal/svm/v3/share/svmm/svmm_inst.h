/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef SVMM_INST_H
#define SVMM_INST_H

#include <pthread.h>
#include <semaphore.h>

#include "rbtree.h"
#include "drv_user_common.h" /* user list */

#include "svm_pub.h"
#include "svmm.h"

struct svmm_inst {
    enum svmm_overlap_type overlap_type;
    u64 svmma_start;
    u64 svmma_size;
    u64 svm_flag;
    u64 seg_num;
    pthread_rwlock_t pipeline_rwlock;

    pthread_rwlock_t rwlock;
    union {
        struct rbtree_root root;
        struct rbtree_root dev_root[SVM_MAX_DEV_NUM];
        struct list_head head; /* reserve */
    };
};

#endif

