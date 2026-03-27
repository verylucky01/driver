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

#ifndef ASYNC_COPY_CTX_H
#define ASYNC_COPY_CTX_H

#include "ka_memory_pub.h"
#include "ka_common_pub.h"

#include "svm_idr.h"

struct async_copy_ctx {
    u32 udevid;
    int tgid;
    void *task_ctx;

    ka_rw_semaphore_t rw_sem;
    struct svm_idr idr;
};

void async_copy_set_feature_id(u32 id);
u32 async_copy_get_feature_id(void);

int async_copy_ctx_create(u32 udevid, int tgid);
void async_copy_ctx_destroy(struct async_copy_ctx *ctx);

struct async_copy_ctx *async_copy_ctx_get(u32 udevid, int tgid);
void async_copy_ctx_put(struct async_copy_ctx *ctx);

void async_copy_ctx_show(struct async_copy_ctx *ctx, ka_seq_file_t *seq);

int async_copy_init_task(u32 udevid, int tgid, void *start_time);
void async_copy_uninit_task(u32 udevid, int tgid, void *start_time);
void async_copy_show_task(u32 udevid, int tgid, int feature_id, ka_seq_file_t *seq);
int async_copy_init(void);
void async_copy_uninit(void);

#endif
