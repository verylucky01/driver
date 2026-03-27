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
#include "svm_pub.h"
#include "svm_kern_log.h"
#include "copy_pub.h"
#include "async_copy_ctx.h"
#include "async_copy_task.h"
#include "async_copy_core.h"

int async_copy_task_submit(struct async_copy_ctx *ctx, struct copy_va_info *info, int *id)
{
    struct svm_copy_task *copy_task = NULL;
    int ret;

    copy_task = async_copy_task_create(ctx, info);
    if (copy_task == NULL) {
        return -EINVAL;
    }

    ret = async_copy_task_save(ctx, copy_task, id);
    if (ret != 0) {
        svm_err("Save copy task failed. ret=%d\n", ret);
        (void)async_copy_task_destroy(copy_task);
        return ret;
    }

    return 0;
}

int async_copy_task_submit_2d(struct async_copy_ctx *ctx, struct copy_2d_va_info *info, int *id)
{
    struct svm_copy_task *copy_task = NULL;
    int ret;

    copy_task = async_copy_task_create_2d(ctx, info);
    if (copy_task == NULL) {
        return -EINVAL;
    }

    ret = async_copy_task_save(ctx, copy_task, id);
    if (ret != 0) {
        svm_err("Save copy task failed. ret=%d\n", ret);
        (void)async_copy_task_destroy(copy_task);
        return ret;
    }

    return 0;
}

int async_copy_task_submit_batch(struct async_copy_ctx *ctx, struct copy_batch_va_info *info, int *id)
{
    struct svm_copy_task *copy_task = NULL;
    int ret;

    copy_task = async_copy_task_create_batch(ctx, info);
    if (copy_task == NULL) {
        return -EINVAL;
    }

    ret = async_copy_task_save(ctx, copy_task, id);
    if (ret != 0) {
        svm_err("Save copy task failed. ret=%d\n", ret);
        (void)async_copy_task_destroy(copy_task);
        return ret;
    }

    return 0;
}

int async_copy_task_wait(struct async_copy_ctx *ctx, int id)
{
    struct svm_copy_task *copy_task = NULL;
    int ret;

    copy_task = async_copy_task_remove(ctx, id);
    if (copy_task == NULL) {
        svm_err("Invalid id. (id=%d)\n", id);
        return -EINVAL;
    }

    ret = async_copy_task_destroy(copy_task);
    return ret;
}
