/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef QUE_MEM_MERGE_H
#define QUE_MEM_MERGE_H

#include <stdlib.h>
#include "urma_api.h"
#include "queue_h2d_user_ub_msg.h"

struct que_mem_node {
    struct rbtree_node node;

    unsigned long long va;
    unsigned long long size;

    urma_target_seg_t *tseg;
};

void que_mem_ctx_init(struct que_mem_merge_ctx *mem_ctx);
int que_mem_merge(struct que_mem_merge_ctx *mem_ctx, unsigned int devid, unsigned long long va, unsigned long long size);
urma_target_seg_t *que_mem_seg_register(struct que_mem_merge_ctx *mem_ctx, unsigned int d2d_flag, struct que_urma_token *token,
    unsigned int pin_flg, unsigned int devid, unsigned long long va, unsigned int access);
void que_mem_seg_unregister(struct que_mem_merge_ctx *mem_ctx, unsigned int devid, unsigned long long va);
void que_mem_erase_all_node(struct que_mem_merge_ctx *mem_ctx);

#endif
