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

#include "svm_slab.h"
#include "svm_kern_log.h"
#include "svm_ioctl_ex.h"
#include "framework_task.h"
#include "framework_cmd.h"
#include "mms_ctx.h"
#include "mms_core.h"

u32 mms_feature_id;

u32 mms_get_feature_id(void)
{
    return mms_feature_id;
}

struct mms_ctx *mms_ctx_get(u32 udevid, int tgid)
{
    struct mms_ctx *mms_ctx = NULL;
    void *task_ctx = NULL;

    task_ctx = svm_task_ctx_get(udevid, tgid);
    if (task_ctx == NULL) {
        return NULL;
    }

    mms_ctx = (struct mms_ctx *)svm_task_get_feature_priv(task_ctx, mms_feature_id);
    if (mms_ctx == NULL) {
        svm_task_ctx_put(task_ctx);
    }

    return mms_ctx;
}

void mms_ctx_put(struct mms_ctx *ctx)
{
    svm_task_ctx_put(ctx->task_ctx);
}

static void mms_ctx_init(struct mms_ctx *mms_ctx)
{
    ka_task_init_rwsem(&mms_ctx->rw_sem);
    mms_ctx->stats = NULL;
    mms_ctx->uva = 0ULL;
    mms_ctx->npage_num = 0ULL;
    mms_ctx->pages = NULL;
    mms_ctx->task_ctx = NULL;
    mms_ctx->is_pfn_map = false;
}

static void mms_ctx_release(void *priv)
{
    struct mms_ctx *ctx = (struct mms_ctx *)priv;
    svm_kvfree(ctx);
}

int mms_init_task(u32 udevid, int tgid, void *start_time)
{
    struct mms_ctx *mms_ctx = NULL;
    void *task_ctx = NULL;
    int ret;

    mms_ctx = (struct mms_ctx *)svm_kvzalloc(sizeof(struct mms_ctx), KA_GFP_KERNEL | __KA_GFP_ACCOUNT);
    if (mms_ctx == NULL) {
        svm_err("No mem. (udevid=%u; tgid=%d)\n", udevid, tgid);
        return -ENOMEM;
    }

    mms_ctx->udevid = udevid;
    mms_ctx->tgid = tgid;
    mms_ctx_init(mms_ctx);

    task_ctx = svm_task_ctx_get(udevid, tgid);
    if (task_ctx == NULL) {
        svm_kvfree(mms_ctx);
        svm_err("Invalid dev task. (udevid=%u; tgid=%d)\n", udevid, tgid);
        return -EINVAL;
    }

    ret = svm_task_set_feature_priv(task_ctx, mms_feature_id, "mms", (void *)mms_ctx, mms_ctx_release);
    if (ret != 0) {
        svm_task_ctx_put(task_ctx);
        svm_kvfree(mms_ctx);
        return ret;
    }

    mms_ctx->task_ctx = task_ctx;

    svm_inst_trace("Init task. (udevid=%u; tgid=%d)\n", udevid, tgid);

    return 0;
}
DECLAER_FEATURE_AUTO_INIT_TASK(mms_init_task, FEATURE_LOADER_STAGE_0);

static void mms_destroy_task(struct mms_ctx *mms_ctx)
{
    svm_inst_trace("Destroy task. (udevid=%u; tgid=%d)\n", mms_ctx->udevid, mms_ctx->tgid);
    svm_task_set_feature_invalid(mms_ctx->task_ctx, mms_feature_id);
    svm_task_ctx_put(mms_ctx->task_ctx); /* with init pair */
    mms_stats_mem_decfg(mms_ctx);
}

void mms_uninit_task(u32 udevid, int tgid, void *start_time)
{
    struct mms_ctx *mms_ctx = NULL;

    mms_ctx = mms_ctx_get(udevid, tgid);
    if (mms_ctx == NULL) {
        svm_err("Invalid dev task. (udevid=%u; tgid=%d)\n", udevid, tgid);
        return;
    }

    mms_ctx_put(mms_ctx);
    if (!svm_task_is_exit_abort(mms_ctx->task_ctx)) {
        mms_destroy_task(mms_ctx);
        svm_inst_trace("Uninit task. (udevid=%u; tgid=%d)\n", udevid, tgid);
    }
}
DECLAER_FEATURE_AUTO_UNINIT_TASK(mms_uninit_task, FEATURE_LOADER_STAGE_0);

void mms_show_task(u32 udevid, int tgid, int feature_id, ka_seq_file_t *seq)
{
    struct mms_ctx *mms_ctx = NULL;
    struct mms_stats *mms_stats = NULL;

    if (feature_id != (int)mms_feature_id) {
        return;
    }

    mms_ctx = mms_ctx_get(udevid, tgid);
    if (mms_ctx == NULL) {
        svm_err("Invalid dev task. (udevid=%u; tgid=%d)\n", udevid, tgid);
        return;
    }

    mms_stats = (struct mms_stats *)mms_ctx->stats;

    mms_mem_task_show(udevid, mms_stats, seq);

    mms_ctx_put(mms_ctx);
}
DECLAER_FEATURE_AUTO_SHOW_TASK(mms_show_task, FEATURE_LOADER_STAGE_0);

static int mms_ioctl_stats_mem_cfg(u32 udevid, u32 cmd, unsigned long arg)
{
    struct mms_ctx *mms_ctx = NULL;
    u64 stats_mem_uva = (u64)arg;
    int ret;

    mms_ctx = mms_ctx_get(udevid, ka_task_get_current_tgid());
    if (mms_ctx == NULL) {
        svm_err("Invalid dev task. (udevid=%u; tgid=%d)\n", udevid, ka_task_get_current_tgid());
        return -EINVAL;
    }

    ret = mms_stats_mem_cfg(mms_ctx, stats_mem_uva);
    mms_ctx_put(mms_ctx);
    return ret;
}

int mms_kern_init(void)
{
    mms_feature_id = svm_task_obtain_feature_id();
    svm_register_ioctl_cmd_handle(_IOC_NR(SVM_MMS_STATS_MEM_CFG), mms_ioctl_stats_mem_cfg);
    return 0;
}
DECLAER_FEATURE_AUTO_INIT(mms_kern_init, FEATURE_LOADER_STAGE_0);

void mms_kern_uninit(void)
{
}
DECLAER_FEATURE_AUTO_UNINIT(mms_kern_uninit, FEATURE_LOADER_STAGE_0);
