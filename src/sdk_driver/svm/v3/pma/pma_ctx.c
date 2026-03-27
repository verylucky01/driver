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
#include "ka_common_pub.h"
#include "ka_dfx_pub.h"
#include "ka_memory_pub.h"
#include "ka_system_pub.h"
#include "ka_sched_pub.h"

#include "pbl_feature_loader.h"
#include "pbl_task_ctx.h"
#include "dpa_kernel_interface.h"
#include "pbl_uda.h"

#include "svm_kern_log.h"
#include "framework_task.h"
#include "svm_slab.h"
#include "pma_core.h"
#include "pma_ctx.h"

u32 pma_feature_id;

static void pma_init_pipeline(struct pma_ctx *pma_ctx)
{
    ka_task_init_rwsem(&pma_ctx->pipeline_rw_sem);
}

void pma_use_pipeline(struct pma_ctx *pma_ctx)
{
    ka_task_down_read(&pma_ctx->pipeline_rw_sem);
}

void pma_unuse_pipeline(struct pma_ctx *pma_ctx)
{
    ka_task_up_read(&pma_ctx->pipeline_rw_sem);
}

void pma_occupy_pipeline(struct pma_ctx *pma_ctx)
{
    ka_task_down_write(&pma_ctx->pipeline_rw_sem);
}

void pma_release_pipeline(struct pma_ctx *pma_ctx)
{
    ka_task_up_write(&pma_ctx->pipeline_rw_sem);
}

struct pma_ctx *pma_ctx_get(u32 udevid, int tgid)
{
    struct pma_ctx *pma_ctx = NULL;
    void *task_ctx = NULL;

    task_ctx = svm_task_ctx_get(udevid, tgid);
    if (task_ctx == NULL) {
        return NULL;
    }

    pma_ctx = (struct pma_ctx *)svm_task_get_feature_priv(task_ctx, pma_feature_id);
    if (pma_ctx == NULL) {
        svm_task_ctx_put(task_ctx);
    }

    return pma_ctx;
}

void pma_ctx_put(struct pma_ctx *pma_ctx)
{
    svm_task_ctx_put(pma_ctx->task_ctx);
}

static void pma_ctx_init(struct pma_ctx *pma_ctx)
{
    pma_init_pipeline(pma_ctx);

    ka_task_init_rwsem(&pma_ctx->rw_sem);
    KA_INIT_LIST_HEAD(&pma_ctx->head);
    pma_ctx->mem_node_num = 0ULL;
    pma_ctx->get_cnt = 0ULL;
    pma_ctx->put_cnt = 0ULL;
}

static void pma_ctx_release(void *priv)
{
    struct pma_ctx *ctx = (struct pma_ctx *)priv;
    svm_kvfree(ctx);
}

int pma_init_task(u32 udevid, int tgid, void *start_time)
{
    struct pma_ctx *pma_ctx = NULL;
    void *task_ctx = NULL;
    int ret;

    pma_ctx = (struct pma_ctx *)svm_kvzalloc(sizeof(struct pma_ctx), KA_GFP_KERNEL | __KA_GFP_ACCOUNT);
    if (pma_ctx == NULL) {
        svm_err("No mem. (udevid=%u; tgid=%d)\n", udevid, tgid);
        return -ENOMEM;
    }

    pma_ctx->udevid = udevid;
    pma_ctx->tgid = tgid;
    pma_ctx_init(pma_ctx);

    task_ctx = svm_task_ctx_get(udevid, tgid);
    if (task_ctx == NULL) {
        svm_kvfree(pma_ctx);
        svm_err("Invalid dev task. (udevid=%u; tgid=%d)\n", udevid, tgid);
        return -EINVAL;
    }

    ret = svm_task_set_feature_priv(task_ctx, pma_feature_id, "pma",
        (void *)pma_ctx, pma_ctx_release);
    if (ret != 0) {
        svm_task_ctx_put(task_ctx);
        svm_kvfree(pma_ctx);
        return ret;
    }

    pma_ctx->task_ctx = task_ctx;

    svm_inst_trace("Init task. (udevid=%u; tgid=%d)\n", udevid, tgid);

    return 0;
}
DECLAER_FEATURE_AUTO_INIT_TASK(pma_init_task, FEATURE_LOADER_STAGE_6);

static void pma_destroy_task(struct pma_ctx *pma_ctx)
{
    svm_inst_trace("Destroy task. (udevid=%u; tgid=%d)\n", pma_ctx->udevid, pma_ctx->tgid);
    svm_task_set_feature_invalid(pma_ctx->task_ctx, pma_feature_id);
    pma_mem_recycle(pma_ctx);
    svm_task_ctx_put(pma_ctx->task_ctx); /* with init pair */
}

void pma_uninit_task(u32 udevid, int tgid, void *start_time)
{
    struct pma_ctx *pma_ctx = NULL;

    pma_ctx = pma_ctx_get(udevid, tgid);
    if (pma_ctx == NULL) {
        return ;
    }

    pma_ctx_put(pma_ctx);

    if (!svm_task_is_exit_abort(pma_ctx->task_ctx)) {
        pma_destroy_task(pma_ctx);
        svm_inst_trace("Uninit task. (udevid=%u; tgid=%d)\n", udevid, tgid);
    }
}
DECLAER_FEATURE_AUTO_UNINIT_TASK(pma_uninit_task, FEATURE_LOADER_STAGE_6);

void pma_show_task(u32 udevid, int tgid, int feature_id, ka_seq_file_t *seq)
{
    struct pma_ctx *pma_ctx = NULL;

    if (feature_id != (int)pma_feature_id) {
        return;
    }

    pma_ctx = pma_ctx_get(udevid, tgid);
    if (pma_ctx == NULL) {
        svm_err("Invalid dev task. (udevid=%u; tgid=%d)\n", udevid, tgid);
        return;
    }

    pma_mem_show(pma_ctx, seq);

    pma_ctx_put(pma_ctx);
}
DECLAER_FEATURE_AUTO_SHOW_TASK(pma_show_task, FEATURE_LOADER_STAGE_6);

int pma_init(void)
{
    pma_feature_id = svm_task_obtain_feature_id();
    return 0;
}
DECLAER_FEATURE_AUTO_INIT(pma_init, FEATURE_LOADER_STAGE_6);

void pma_uninit(void)
{
}
DECLAER_FEATURE_AUTO_UNINIT(pma_uninit, FEATURE_LOADER_STAGE_6);

