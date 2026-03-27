/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
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
#include "ka_ioctl_pub.h"
#include "ka_base_pub.h"
#include "ka_compiler_pub.h"

#include "pbl_feature_loader.h"
#include "pbl_task_ctx.h"
#include "pbl_uda.h"

#include "svm_pub.h"
#include "svm_kern_log.h"
#include "copy_pub.h"
#include "svm_ioctl_ex.h"
#include "framework_cmd.h"
#include "async_copy_task.h"
#include "async_copy_ctx.h"
#include "async_copy_core.h"

static int async_copy_extract_va_info_from_ioctl_para(struct svm_async_copy_submit_para *para,
    struct copy_va_info *info)
{
    int ret;

    ret = uda_devid_to_udevid_ex(para->src_devid, &info->src_udevid);
    if (ret != 0) {
        svm_err("uda_devid_to_udevid_ex failed. (ret=%d; src_devid=%u)\n", ret, para->src_devid);
        return ret;
    }

    ret = uda_devid_to_udevid_ex(para->dst_devid, &info->dst_udevid);
    if (ret != 0) {
        svm_err("uda_devid_to_udevid_ex failed. (ret=%d; dst_devid=%u)\n", ret, para->dst_devid);
        return ret;
    }

    info->src_va = para->src_va;
    info->dst_va = para->dst_va;
    info->size = para->size;
    info->src_host_tgid = para->src_host_tgid;
    info->dst_host_tgid = para->dst_host_tgid;

    return 0;
}

static int _async_copy_ioctl_submit(struct async_copy_ctx *ctx, unsigned long __ka_user arg)
{
    struct svm_async_copy_submit_para para;
    struct copy_va_info info;
    int ret;

    ret = (int)ka_base_copy_from_user(&para, (void __ka_user *)(uintptr_t)arg, sizeof(struct svm_async_copy_submit_para));
    if (ret != 0) {
        svm_err("Copy_from_user fail.\n");
        return -EINVAL;
    }

    ret = async_copy_extract_va_info_from_ioctl_para(&para, &info);
    if (ret != 0) {
        return ret;
    }

    ret = copy_va_info_check(copy_va_info_get_exec_udevid(&info), &info);
    if (ret != 0) {
        return ret;
    }

    ret = async_copy_task_submit(ctx, &info, &para.id);
    if (ret != 0) {
        return ret;
    }

    ret = ka_base_copy_to_user((void *)(uintptr_t)arg, (void *)&para, sizeof(struct svm_async_copy_submit_para));
    if (ret != 0) {
        svm_err("Copy to user failed. (ret=%d)\n", ret);
        async_copy_task_wait(ctx, para.id);
        return -EINVAL;
    }

    return 0;
}

static int _async_copy_ioctl_wait(struct async_copy_ctx *ctx, unsigned long __ka_user arg)
{
    struct svm_async_copy_wait_para para;
    int ret;

    ret = ka_base_copy_from_user(&para, (void __ka_user *)(uintptr_t)arg, sizeof(struct svm_async_copy_wait_para));
    if (ret != 0) {
        svm_err("Copy_from_user fail.\n");
        return -EINVAL;
    }

    return async_copy_task_wait(ctx, para.id);
}

static int async_copy_ioctl_submit(u32 udevid, u32 cmd, unsigned long arg)
{
    struct async_copy_ctx *ctx = NULL;
    int ret;

    ctx = async_copy_ctx_get(udevid, ka_task_get_current_tgid());
    if (ctx == NULL) {
        svm_err("async_copy_ctx_get fail. (devid=%u; tgid=%d)\n", udevid, ka_task_get_current_tgid());
        return -EINVAL;
    }

    ret = _async_copy_ioctl_submit(ctx, arg);
    async_copy_ctx_put(ctx);
    return ret;
}

static int async_copy_ioctl_wait(u32 udevid, u32 cmd, unsigned long arg)
{
    struct async_copy_ctx *ctx = NULL;
    int ret;

    ctx = async_copy_ctx_get(udevid, ka_task_get_current_tgid());
    if (ctx == NULL) {
        return -EINVAL;
    }

    ret = _async_copy_ioctl_wait(ctx, arg);
    async_copy_ctx_put(ctx);
    return ret;
}

int async_copy_ioctl_init(void)
{
    svm_register_ioctl_cmd_handle(_IOC_NR(SVM_ASYNC_COPY_SUBMIT), async_copy_ioctl_submit);
    svm_register_ioctl_cmd_handle(_IOC_NR(SVM_ASYNC_COPY_WAIT), async_copy_ioctl_wait);

    return 0;
}
DECLAER_FEATURE_AUTO_INIT(async_copy_ioctl_init, FEATURE_LOADER_STAGE_6);
