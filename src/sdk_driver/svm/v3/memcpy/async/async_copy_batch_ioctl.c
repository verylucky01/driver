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
#include "ka_memory_pub.h"
#include "ka_base_pub.h"
#include "ka_compiler_pub.h"

#include "pbl_feature_loader.h"
#include "pbl_task_ctx.h"

#include "svm_pub.h"
#include "svm_kern_log.h"
#include "copy_pub.h"
#include "svm_ioctl_ex.h"
#include "framework_cmd.h"
#include "svm_slab.h"
#include "async_copy_task.h"
#include "async_copy_ctx.h"
#include "async_copy_core.h"

#define SVM_CPY_BATCH_MAX_COUNT (4ULL * SVM_BYTES_PER_KB)
static int async_copy_batch_para_check(struct svm_async_copy_submit_batch_para *para)
{
    if ((para->src_va == NULL) || (para->dst_va == NULL) || (para->size == NULL) || (para->count == 0)) {
        svm_err("Ioctl input para invalid. (src_va_is_null=%d; dst_va_is_null=%d; size_is_null=%d; "
            "count=%llu)\n", (para->src_va == NULL), (para->dst_va == NULL), (para->size == NULL), para->count);
        return -EINVAL;
    }

    if (para->count > SVM_CPY_BATCH_MAX_COUNT) {
        return -EOPNOTSUPP;
    }

    return 0;
}

static int async_cpy_batch_addr_array_alloc(struct svm_async_copy_submit_batch_para *para, struct copy_batch_va_info *info)
{
    u64 array_size = para->count * sizeof(u64);

    info->src_va = svm_kvmalloc(array_size, KA_GFP_KERNEL | __KA_GFP_ACCOUNT);
    if (info->src_va == NULL) {
        svm_err("Alloc for src_va failed. (size=%llu)\n", array_size);
        return -ENOMEM;
    }

    info->dst_va = svm_kvmalloc(array_size, KA_GFP_KERNEL | __KA_GFP_ACCOUNT);
    if (info->dst_va == NULL) {
        svm_err("Alloc for dst_va failed. (size=%llu)\n", array_size);
        goto free_src_va;
    }

    info->size = svm_kvmalloc(array_size, KA_GFP_KERNEL | __KA_GFP_ACCOUNT);
    if (info->size == NULL) {
        svm_err("Alloc for size failed. (size=%llu)\n", array_size);
        goto free_dst_va;
    }

    return 0;

free_dst_va:
    svm_kvfree(info->dst_va);
    info->dst_va = NULL;
free_src_va:
    svm_kvfree(info->src_va);
    info->src_va = NULL;
    return -ENOMEM;
}

static void async_cpy_batch_addr_array_free(struct copy_batch_va_info *info)
{
    svm_kvfree(info->src_va);
    info->src_va = NULL;
    svm_kvfree(info->dst_va);
    info->dst_va = NULL;
    svm_kvfree(info->size);
    info->size = NULL;
}

static int async_cpy_batch_addr_array_cpy(struct svm_async_copy_submit_batch_para *para, struct copy_batch_va_info *info)
{
    u64 array_size = para->count * sizeof(u64);

    if (ka_base_copy_from_user(info->src_va, (void __ka_user *)(uintptr_t)(para->src_va), array_size) != 0) {
        svm_err("Copy from user dst args fail. (array_size=%llu; src_va=0x%llx)\n", array_size, (u64)(uintptr_t)para->src_va);
        return -EINVAL;
    }

    if (ka_base_copy_from_user(info->dst_va, (void __ka_user *)(uintptr_t)(para->dst_va), array_size) != 0) {
        svm_err("Copy from user dst_va fail. (array_size=%llu; dst_va=0x%llx)\n", array_size, (u64)(uintptr_t)para->dst_va);
        return -EINVAL;
    }

    if (ka_base_copy_from_user(info->size, (void __ka_user *)(uintptr_t)(para->size), array_size) != 0) {
        svm_err("Copy from user size_addr fail. (array_size=%llu; size_addr=0x%llx)\n", array_size, (u64)(uintptr_t)para->size);
        return -EINVAL;
    }

    return 0;
}

static int _async_cpy_batch_addr_info_init(struct svm_async_copy_submit_batch_para *para, struct copy_batch_va_info *info)
{
    int ret;

    ret = async_cpy_batch_addr_array_alloc(para, info);
    if (ret != 0) {
        return ret;
    }
    ret = async_cpy_batch_addr_array_cpy(para, info);
    if (ret != 0) {
        async_cpy_batch_addr_array_free(info);
        return ret;
    }

    info->count = para->count;

    return 0;
}

static int async_cpy_batch_addr_info_init(struct svm_async_copy_submit_batch_para *para, struct copy_batch_va_info *info)
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

    return _async_cpy_batch_addr_info_init(para, info);
}

static void async_cpy_batch_addr_info_uninit(struct copy_batch_va_info *info)
{
    async_cpy_batch_addr_array_free(info);
}

static int _async_copy_submit_batch(struct async_copy_ctx *ctx, struct svm_async_copy_submit_batch_para *para, struct copy_batch_va_info *info)
{
    int ret;

    ret = async_cpy_batch_addr_info_init(para, info);
    if (ret != 0) {
        return ret;
    }

    ret = async_copy_task_submit_batch(ctx, info, &para->id);
    if (ret != 0) {
        svm_err("Batch copy task submit failed. (ret=%d)\n", ret);
    }

    async_cpy_batch_addr_info_uninit(info);

    return ret;
}

static int async_copy_submit_batch(struct async_copy_ctx *ctx, struct svm_async_copy_submit_batch_para *para, struct copy_batch_va_info *info)
{
    int ret;

    ret = async_copy_batch_para_check(para);
    if (ret != 0) {
        return ret;
    }

    return _async_copy_submit_batch(ctx, para, info);
}

static int _async_copy_ioctl_submit_batch(struct async_copy_ctx *ctx, unsigned long __ka_user arg)
{
    struct svm_async_copy_submit_batch_para para;
    struct copy_batch_va_info info;
    int ret;

    if (ka_base_copy_from_user(&para, (void __ka_user *)(uintptr_t)arg, sizeof(struct svm_async_copy_submit_batch_para)) != 0) {
        svm_err("Copy from user batch cpy ioctl para fail.\n");
        return -EINVAL;
    }

    ret = async_copy_submit_batch(ctx, &para, &info);
    if (ret != 0) {
        svm_err("Async copy submit batch failed. (ret=%d)\n", ret);
        return ret;
    }

    if (ka_base_copy_to_user((void *)(uintptr_t)arg, (void *)&para, sizeof(struct svm_async_copy_submit_batch_para)) != 0) {
        svm_err("Copy to user failed. (ret=%d)\n", ret);
        async_copy_task_wait(ctx, para.id);
        return -EINVAL;
    }

    return 0;
}

static int async_copy_ioctl_submit_batch(u32 udevid, u32 cmd, unsigned long arg)
{
    struct async_copy_ctx *ctx = NULL;
    int ret;

    ctx = async_copy_ctx_get(udevid, ka_task_get_current_tgid());
    if (ctx == NULL) {
        return -EINVAL;
    }

    ret = _async_copy_ioctl_submit_batch(ctx, arg);
    async_copy_ctx_put(ctx);
    return ret;
}

int async_copy_batch_ioctl_init(void)
{
    svm_register_ioctl_cmd_handle(_IOC_NR(SVM_ASYNC_COPY_SUBMIT_BATCH), async_copy_ioctl_submit_batch);
    return 0;
}
DECLAER_FEATURE_AUTO_INIT(async_copy_batch_ioctl_init, FEATURE_LOADER_STAGE_6);

