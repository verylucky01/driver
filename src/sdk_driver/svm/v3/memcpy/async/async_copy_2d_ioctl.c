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

static inline int async_copy_2d_para_check(struct svm_async_copy_submit_2d_para *para)
{
    if ((para->width > para->dpitch) || (para->width > para->spitch)) {
        svm_err("Dpitch and spitch should both larger than width. (dpitch=%llu; spitch=%llu; width=%llu)\n",
            para->dpitch, para->spitch, para->width);
        return -EINVAL;
    }

    if ((para->width == 0) || (para->height == 0)) {
        svm_err("Width & height para invalid. (width=%llu; height=%llu)\n", para->width, para->height);
        return -EINVAL;
    }

    return 0;
}

static int async_copy_extract_va_info_from_ioctl_2d_para(struct svm_async_copy_submit_2d_para *para,
    struct copy_2d_va_info *info)
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
    info->spitch = para->spitch;
    info->dpitch = para->dpitch;
    info->width = para->width;
    info->height = para->height;

    return 0;
}

static int _async_copy_ioctl_submit_2d(struct async_copy_ctx *ctx, unsigned long __ka_user arg)
{
    struct svm_async_copy_submit_2d_para para;
    struct copy_2d_va_info info;
    int ret;

    ret = ka_base_copy_from_user(&para, (void __ka_user *)(uintptr_t)arg, sizeof(struct svm_async_copy_submit_2d_para));
    if (ret != 0) {
        svm_err("Copy_from_user fail.\n");
        return -EINVAL;
    }

    ret = async_copy_2d_para_check(&para);
    if (ret != 0) {
        return ret;
    }

    ret = async_copy_extract_va_info_from_ioctl_2d_para(&para, &info);
    if (ret != 0) {
        return ret;
    }

    ret = async_copy_task_submit_2d(ctx, &info, &para.id);
    if (ret != 0) {
        return ret;
    }

    ret = ka_base_copy_to_user((void *)(uintptr_t)arg, (void *)&para, sizeof(struct svm_async_copy_submit_2d_para));
    if (ret != 0) {
        svm_err("Copy to user failed. (ret=%d)\n", ret);
        async_copy_task_wait(ctx, para.id);
        return -EINVAL;
    }

    return 0;
}

static int async_copy_ioctl_submit_2d(u32 udevid, u32 cmd, unsigned long arg)
{
    struct async_copy_ctx *ctx = NULL;
    int ret;

    ctx = async_copy_ctx_get(udevid, ka_task_get_current_tgid());
    if (ctx == NULL) {
        return -EINVAL;
    }

    ret = _async_copy_ioctl_submit_2d(ctx, arg);
    async_copy_ctx_put(ctx);
    return ret;
}

int async_copy_2d_ioctl_init(void)
{
    svm_register_ioctl_cmd_handle(_IOC_NR(SVM_ASYNC_COPY_SUBMIT_2D), async_copy_ioctl_submit_2d);
    return 0;
}
DECLAER_FEATURE_AUTO_INIT(async_copy_2d_ioctl_init, FEATURE_LOADER_STAGE_6);
