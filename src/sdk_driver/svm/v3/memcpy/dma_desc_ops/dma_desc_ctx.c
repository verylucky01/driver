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
#include "ka_common_pub.h"
#include "ka_task_pub.h"
#include "ka_memory_pub.h"
#include "ka_dfx_pub.h"
#include "ka_system_pub.h"
#include "ka_sched_pub.h"

#include "pbl_feature_loader.h"
#include "pbl_task_ctx.h"
#include "dpa_kernel_interface.h"
#include "pbl_uda.h"

#include "svm_kern_log.h"
#include "svm_slab.h"
#include "copy_task.h"
#include "framework_task.h"
#include "dma_desc_node.h"
#include "dma_desc_ctx.h"

u32 dma_desc_feature_id;

struct dma_desc_ctx *dma_desc_ctx_get(u32 udevid, int tgid)
{
    struct dma_desc_ctx *ctx = NULL;
    void *task_ctx = NULL;

    task_ctx = svm_task_ctx_get(udevid, tgid);
    if (task_ctx == NULL) {
        return NULL;
    }

    ctx = (struct dma_desc_ctx *)svm_task_get_feature_priv(task_ctx, dma_desc_feature_id);
    if (ctx == NULL) {
        svm_task_ctx_put(task_ctx);
    }

    return ctx;
}

void dma_desc_ctx_put(struct dma_desc_ctx *ctx)
{
    svm_task_ctx_put(ctx->task_ctx);
}

static void dma_desc_ctx_init(struct dma_desc_ctx *ctx)
{
    ka_task_init_rwsem(&ctx->rw_sem);
    range_rbtree_init(&ctx->root);
}

static void dma_desc_ctx_uninit(struct dma_desc_ctx *ctx)
{
    struct svm_copy_task *copy_task = NULL;
    struct dma_desc_node *node = NULL;
    struct range_rbtree_node *range_node = NULL;
    int recycle_num = 0;
    unsigned long stamp;

    do {
        ka_task_down_write(&ctx->rw_sem);
        range_node = range_rbtree_erase_one(&ctx->root);
        ka_task_up_write(&ctx->rw_sem);
        if (range_node == NULL) {
            break;
        }

        recycle_num++;
        node = ka_container_of(range_node, struct dma_desc_node, node);
        copy_task = node->copy_task;
        (void)svm_copy_task_wait(copy_task);
        dma_desc_node_destroy(ctx, node);
        svm_copy_task_destroy(copy_task);
        ka_try_cond_resched(&stamp);
    } while (1);

    if (recycle_num > 0) {
        svm_warn("Recycle dma desc node. (udevid=%u; tgid=%d; recycle_num=%d)\n",
            ctx->udevid, ctx->tgid, recycle_num);
    }
}

static void dma_desc_ctx_release(void *priv)
{
    struct dma_desc_ctx *ctx = (struct dma_desc_ctx *)priv;
    svm_kvfree(ctx);
}

int dma_desc_ctx_create(u32 udevid, int tgid)
{
    struct dma_desc_ctx *ctx = NULL;
    void *task_ctx = NULL;
    int ret;

    ctx = (struct dma_desc_ctx *)svm_kvzalloc(sizeof(struct dma_desc_ctx), KA_GFP_KERNEL | __KA_GFP_ACCOUNT);
    if (ctx == NULL) {
        svm_err("No mem. (size=%lu)\n", sizeof(struct dma_desc_ctx));
        return -ENOMEM;
    }

    ctx->udevid = udevid;
    ctx->tgid = tgid;
    dma_desc_ctx_init(ctx);

    task_ctx = svm_task_ctx_get(udevid, tgid);
    if (task_ctx == NULL) {
        svm_err("Invalid dev task. (udevid=%u; tgid=%d)\n", udevid, tgid);
        svm_kvfree(ctx);
        return -EINVAL;
    }

    ret = svm_task_set_feature_priv(task_ctx, dma_desc_feature_id, "dma_desc",
        (void *)ctx, dma_desc_ctx_release);
    if (ret != 0) {
        svm_task_ctx_put(task_ctx);
        svm_kvfree(ctx);
        return ret;
    }

    ctx->task_ctx = task_ctx;
    return 0;
}

void dma_desc_ctx_destroy(struct dma_desc_ctx *ctx)
{
    svm_inst_trace("Destroy task. (udevid=%u; tgid=%d)\n", ctx->udevid, ctx->tgid);
    svm_task_set_feature_invalid(ctx->task_ctx, dma_desc_feature_id);
    dma_desc_ctx_uninit(ctx);
    svm_task_ctx_put(ctx->task_ctx); /* with init pair */
}

void dma_desc_ctx_show(struct dma_desc_ctx *ctx, ka_seq_file_t *seq)
{
}

int dma_desc_init_task(u32 udevid, int tgid, void *start_time)
{
    int ret;

    ret = dma_desc_ctx_create(udevid, tgid);

    svm_inst_trace("Init task. (udevid=%u; tgid=%d)\n", udevid, tgid);

    return ret;
}
DECLAER_FEATURE_AUTO_INIT_TASK(dma_desc_init_task, FEATURE_LOADER_STAGE_6);

void dma_desc_uninit_task(u32 udevid, int tgid, void *start_time)
{
    struct dma_desc_ctx *ctx = NULL;

    ctx = dma_desc_ctx_get(udevid, tgid);
    if (ctx == NULL) {
        return;
    }

    dma_desc_ctx_put(ctx);

    if (!svm_task_is_exit_abort(ctx->task_ctx)) {
        dma_desc_ctx_destroy(ctx);
        svm_inst_trace("Uninit task. (udevid=%u; tgid=%d)\n", udevid, tgid);
    }
}
DECLAER_FEATURE_AUTO_UNINIT_TASK(dma_desc_uninit_task, FEATURE_LOADER_STAGE_6);

void dma_desc_show_task(u32 udevid, int tgid, int feature_id, ka_seq_file_t *seq)
{
    struct dma_desc_ctx *ctx = NULL;

    if (feature_id != dma_desc_feature_id) {
        return;
    }

    ctx = dma_desc_ctx_get(udevid, tgid);
    if (ctx == NULL) {
        svm_err("Invalid dev task. (udevid=%u; tgid=%d)\n", udevid, tgid);
        return;
    }

    dma_desc_ctx_show(ctx, seq);
    dma_desc_ctx_put(ctx);
}
DECLAER_FEATURE_AUTO_SHOW_TASK(dma_desc_show_task, FEATURE_LOADER_STAGE_6);

int dma_desc_init(void)
{
    dma_desc_feature_id = svm_task_obtain_feature_id();
    return 0;
}
DECLAER_FEATURE_AUTO_INIT(dma_desc_init, FEATURE_LOADER_STAGE_6);

void dma_desc_uninit(void)
{
}
DECLAER_FEATURE_AUTO_UNINIT(dma_desc_uninit, FEATURE_LOADER_STAGE_6);
