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

#ifndef DMA_DESC_CTX_H
#define DMA_DESC_CTX_H

#include "ka_common_pub.h"

#include "pbl_range_rbtree.h"

#include "svm_idr.h"

struct dma_desc_ctx {
    u32 udevid;
    int tgid;
    void *task_ctx;

    ka_rw_semaphore_t rw_sem;
    struct range_rbtree root;
};

void dma_desc_set_feature_id(u32 id);
u32 dma_desc_get_feature_id(void);

int dma_desc_ctx_create(u32 udevid, int tgid);
void dma_desc_ctx_destroy(struct dma_desc_ctx *ctx);

struct dma_desc_ctx *dma_desc_ctx_get(u32 udevid, int tgid);
void dma_desc_ctx_put(struct dma_desc_ctx *ctx);

void dma_desc_ctx_show(struct dma_desc_ctx *ctx, ka_seq_file_t *seq);

int dma_desc_init_task(u32 udevid, int tgid, void *start_time);
void dma_desc_uninit_task(u32 udevid, int tgid, void *start_time);
void dma_desc_show_task(u32 udevid, int tgid, int feature_id, ka_seq_file_t *seq);
int dma_desc_init(void);
void dma_desc_uninit(void);

#endif
