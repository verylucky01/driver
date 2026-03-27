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
#include "ka_task_pub.h"
#include "ka_memory_pub.h"
#include "ka_dfx_pub.h"
#include "ka_common_pub.h"
#include "ka_system_pub.h"
#include "ka_sched_pub.h"

#include "pbl_feature_loader.h"
#include "pbl_task_ctx.h"
#include "dpa_kernel_interface.h"
#include "pbl_uda.h"

#include "svm_kern_log.h"
#include "svm_idr.h"
#include "svm_slab.h"
#include "framework_task.h"
#include "async_copy_task.h"
#include "async_copy_ctx.h"

#ifndef EMU_ST /* Simulation ST is required and cannot be deleted. */
#define SVM_ASYNC_TASK_MAX_ID       KA_UINT_MAX
#else
#define SVM_ASYNC_TASK_MAX_ID       8ULL
#endif

static u32 async_copy_feature_id;

void async_copy_set_feature_id(u32 id)
{
    async_copy_feature_id = id;
}

u32 async_copy_get_feature_id(void)
{
    return async_copy_feature_id;
}

struct async_copy_ctx *async_copy_ctx_get(u32 udevid, int tgid)
{
    struct async_copy_ctx *ctx = NULL;
    void *task_ctx = NULL;

    task_ctx = svm_task_ctx_get(udevid, tgid);
    if (task_ctx == NULL) {
        return NULL;
    }

    ctx = (struct async_copy_ctx *)svm_task_get_feature_priv(task_ctx, async_copy_feature_id);
    if (ctx == NULL) {
        svm_task_ctx_put(task_ctx);
    }

    return ctx;
}

void async_copy_ctx_put(struct async_copy_ctx *ctx)
{
    svm_task_ctx_put(ctx->task_ctx);
}

static void async_copy_ctx_init(struct async_copy_ctx *ctx)
{
    ka_task_init_rwsem(&ctx->rw_sem);
    svm_idr_init(SVM_ASYNC_TASK_MAX_ID, &ctx->idr);
}

static void async_copy_task_release(void *priv)
{
    struct svm_copy_task *copy_task = (struct svm_copy_task *)priv;

    (void)async_copy_task_destroy(copy_task);
}

static void async_copy_ctx_uninit(struct async_copy_ctx *ctx)
{
    ka_task_down_write(&ctx->rw_sem);
    svm_idr_uninit(&ctx->idr, async_copy_task_release, true);
    ka_task_up_write(&ctx->rw_sem);
}

static void async_copy_ctx_release(void *priv)
{
    struct async_copy_ctx *ctx = (struct async_copy_ctx *)priv;
    svm_kvfree(ctx);
}

int async_copy_ctx_create(u32 udevid, int tgid)
{
    struct async_copy_ctx *ctx = NULL;
    void *task_ctx = NULL;
    int ret;

    ctx = (struct async_copy_ctx *)svm_kvzalloc(sizeof(struct async_copy_ctx), KA_GFP_KERNEL | __KA_GFP_ACCOUNT);
    if (ctx == NULL) {
        svm_err("No mem. (size=%lu)\n", sizeof(struct async_copy_ctx));
        return -ENOMEM;
    }

    ctx->udevid = udevid;
    ctx->tgid = tgid;
    async_copy_ctx_init(ctx);

    task_ctx = svm_task_ctx_get(udevid, tgid);
    if (task_ctx == NULL) {
        svm_err("Invalid dev task. (udevid=%u; tgid=%d)\n", udevid, tgid);
        svm_kvfree(ctx);
        return -EINVAL;
    }

    ret = svm_task_set_feature_priv(task_ctx, async_copy_feature_id, "async_copy",
        (void *)ctx, async_copy_ctx_release);
    if (ret != 0) {
        svm_task_ctx_put(task_ctx);
        svm_kvfree(ctx);
        return ret;
    }

    ctx->task_ctx = task_ctx;
    return 0;
}

void async_copy_ctx_destroy(struct async_copy_ctx *ctx)
{
    svm_inst_trace("Destroy task. (udevid=%u; tgid=%d)\n", ctx->udevid, ctx->tgid);
    svm_task_set_feature_invalid(ctx->task_ctx, async_copy_feature_id);
    async_copy_ctx_uninit(ctx);
    svm_task_ctx_put(ctx->task_ctx); /* with init pair */
}

void async_copy_ctx_show(struct async_copy_ctx *ctx, ka_seq_file_t *seq)
{
}

int async_copy_init_task(u32 udevid, int tgid, void *start_time)
{
    int ret;

    ret = async_copy_ctx_create(udevid, tgid);

    svm_inst_trace("Init task. (udevid=%u; tgid=%d)\n", udevid, tgid);

    return ret;
}
DECLAER_FEATURE_AUTO_INIT_TASK(async_copy_init_task, FEATURE_LOADER_STAGE_6);

void async_copy_uninit_task(u32 udevid, int tgid, void *start_time)
{
    struct async_copy_ctx *ctx = NULL;

    ctx = async_copy_ctx_get(udevid, tgid);
    if (ctx == NULL) {
        return;
    }

    async_copy_ctx_put(ctx);

    if (!svm_task_is_exit_abort(ctx->task_ctx)) {
        async_copy_ctx_destroy(ctx);
        svm_inst_trace("Uninit task. (udevid=%u; tgid=%d)\n", udevid, tgid);
    }
}
DECLAER_FEATURE_AUTO_UNINIT_TASK(async_copy_uninit_task, FEATURE_LOADER_STAGE_6);

void async_copy_show_task(u32 udevid, int tgid, int feature_id, ka_seq_file_t *seq)
{
    struct async_copy_ctx *ctx = NULL;

    if (feature_id != async_copy_get_feature_id()) {
        return;
    }

    ctx = async_copy_ctx_get(udevid, tgid);
    if (ctx == NULL) {
        svm_err("Invalid dev task. (udevid=%u; tgid=%d)\n", udevid, tgid);
        return;
    }

    async_copy_ctx_show(ctx, seq);
    async_copy_ctx_put(ctx);
}
DECLAER_FEATURE_AUTO_SHOW_TASK(async_copy_show_task, FEATURE_LOADER_STAGE_6);

int async_copy_init(void)
{
    async_copy_set_feature_id(svm_task_obtain_feature_id());
    return 0;
}
DECLAER_FEATURE_AUTO_INIT(async_copy_init, FEATURE_LOADER_STAGE_6);

void async_copy_uninit(void)
{
}
DECLAER_FEATURE_AUTO_UNINIT(async_copy_uninit, FEATURE_LOADER_STAGE_6);

