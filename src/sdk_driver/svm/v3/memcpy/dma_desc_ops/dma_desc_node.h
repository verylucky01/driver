/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#ifndef DMA_DESC_NODE_H
#define DMA_DESC_NODE_H

#include "ka_base_pub.h"
#include "ka_task_pub.h"

#include "ascend_hal_define.h"
#include "comm_kernel_interface.h"
#include "pbl_range_rbtree.h"

#include "svm_pub.h"
#include "copy_task.h"
#include "dma_desc_ctx.h"

enum dma_desc_node_state {
    DMA_DESC_NODE_IDLE = 0U,
    DMA_DESC_NODE_SUBMITING,
    DMA_DESC_NODE_COPYING,
    DMA_DESC_NODE_WAITING,
    DMA_DESC_NODE_FREEING
};

struct dma_desc_node {
    struct range_rbtree_node node;
    ka_kref_t ref;

    ka_rwlock_t rwlock;
    enum dma_desc_node_state state;

    u64 handle;

    struct svm_copy_task *copy_task;

    u64 fixed_size;
    u64 fixed_dma_node_num;
    struct devdrv_dma_prepare dma_prepare;
};

int dma_desc_node_create(struct dma_desc_ctx *ctx,
    struct svm_copy_task *copy_task, struct DMA_ADDR *dma_desc);
void dma_desc_node_destroy(struct dma_desc_ctx *ctx, struct dma_desc_node *node);
struct dma_desc_node *dma_desc_node_get(struct dma_desc_ctx *ctx, u64 handle);
void dma_desc_node_put(struct dma_desc_node *node);
int dma_desc_node_state_trans(struct dma_desc_node *node, int src_state, int dst_state);

#endif

