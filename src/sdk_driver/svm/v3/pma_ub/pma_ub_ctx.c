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
#include "ka_memory_pub.h"
#include "ka_system_pub.h"
#include "ka_dfx_pub.h"
#include "ka_sched_pub.h"

#include "pbl_feature_loader.h"
#include "pbl_task_ctx.h"
#include "dpa_kernel_interface.h"
#include "pbl_uda.h"

#include "svm_kern_log.h"
#include "framework_task.h"
#include "svm_slab.h"
#include "pma_ub_ctx.h"

u32 pma_ub_feature_id;

struct pma_ub_ctx *pma_ub_ctx_get(u32 udevid, int tgid)
{
    struct pma_ub_ctx *pma_ub_ctx = NULL;
    void *task_ctx = NULL;

    task_ctx = svm_task_ctx_get(udevid, tgid);
    if (task_ctx == NULL) {
        return NULL;
    }

    pma_ub_ctx = (struct pma_ub_ctx *)svm_task_get_feature_priv(task_ctx, pma_ub_feature_id);
    if (pma_ub_ctx == NULL) {
        svm_task_ctx_put(task_ctx);
    }

    return pma_ub_ctx;
}

void pma_ub_ctx_put(struct pma_ub_ctx *ctx)
{
    svm_task_ctx_put(ctx->task_ctx);
}

static void pma_ub_ctx_release(void *priv)
{
    struct pma_ub_ctx *ctx = (struct pma_ub_ctx *)priv;
    svm_kvfree(ctx);
}

int pma_ub_init_task(u32 udevid, int tgid, void *start_time)
{
    struct pma_ub_ctx *pma_ub_ctx = NULL;
    void *task_ctx = NULL;
    int ret;

    if (udevid == uda_get_host_id()) {
        return 0;
    }

    pma_ub_ctx = (struct pma_ub_ctx *)svm_kvzalloc(sizeof(struct pma_ub_ctx), KA_GFP_KERNEL | __KA_GFP_ACCOUNT);
    if (pma_ub_ctx == NULL) {
        svm_err("No mem. (udevid=%u; tgid=%d)\n", udevid, tgid);
        return -ENOMEM;
    }

    pma_ub_ctx->udevid = udevid;
    pma_ub_ctx->tgid = tgid;
    pma_ub_seg_mng_init(&pma_ub_ctx->seg_mng, udevid, tgid);

    task_ctx = svm_task_ctx_get(udevid, tgid);
    if (task_ctx == NULL) {
        svm_kvfree(pma_ub_ctx);
        svm_err("Invalid dev task. (udevid=%u; tgid=%d)\n", udevid, tgid);
        return -EINVAL;
    }

    ret = svm_task_set_feature_priv(task_ctx, pma_ub_feature_id, "pma_ub",
        (void *)pma_ub_ctx, pma_ub_ctx_release);
    if (ret != 0) {
        svm_task_ctx_put(task_ctx);
        svm_kvfree(pma_ub_ctx);
        return ret;
    }

    pma_ub_ctx->task_ctx = task_ctx;

    svm_inst_trace("Init task. (udevid=%u; tgid=%d)\n", udevid, tgid);

    return 0;
}
DECLAER_FEATURE_AUTO_INIT_TASK(pma_ub_init_task, FEATURE_LOADER_STAGE_6);

static void pma_ub_destroy_task(struct pma_ub_ctx *pma_ub_ctx)
{
    svm_inst_trace("Destroy task. (udevid=%u; tgid=%d)\n", pma_ub_ctx->udevid, pma_ub_ctx->tgid);
    svm_task_set_feature_invalid(pma_ub_ctx->task_ctx, pma_ub_feature_id);
    pma_ub_seg_mng_uninit(&pma_ub_ctx->seg_mng);
    svm_task_ctx_put(pma_ub_ctx->task_ctx); /* with init pair */
}

void pma_ub_uninit_task(u32 udevid, int tgid, void *start_time)
{
    struct pma_ub_ctx *pma_ub_ctx = NULL;

    if (udevid == uda_get_host_id()) {
        return;
    }

    pma_ub_ctx = pma_ub_ctx_get(udevid, tgid);
    if (pma_ub_ctx == NULL) {
        return ;
    }

    pma_ub_ctx_put(pma_ub_ctx);

    if (!svm_task_is_exit_abort(pma_ub_ctx->task_ctx)) {
        pma_ub_destroy_task(pma_ub_ctx);
        svm_inst_trace("Uninit task. (udevid=%u; tgid=%d)\n", udevid, tgid);
    }
}
DECLAER_FEATURE_AUTO_UNINIT_TASK(pma_ub_uninit_task, FEATURE_LOADER_STAGE_6);

void pma_ub_show_task(u32 udevid, int tgid, int feature_id, ka_seq_file_t *seq)
{
    struct pma_ub_ctx *pma_ub_ctx = NULL;

    if (udevid == uda_get_host_id()) {
        return;
    }

    if (feature_id != (int)pma_ub_feature_id) {
        return;
    }

    pma_ub_ctx = pma_ub_ctx_get(udevid, tgid);
    if (pma_ub_ctx == NULL) {
        svm_err("Invalid dev task. (udevid=%u; tgid=%d)\n", udevid, tgid);
        return;
    }

    pma_ub_seg_mng_show(&pma_ub_ctx->seg_mng, seq);

    pma_ub_ctx_put(pma_ub_ctx);
}
DECLAER_FEATURE_AUTO_SHOW_TASK(pma_ub_show_task, FEATURE_LOADER_STAGE_6);

int pma_ub_ctx_init(void)
{
    pma_ub_feature_id = svm_task_obtain_feature_id();
    return 0;
}

void pma_ub_ctx_uninit(void)
{
}
