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

#ifndef SMM_CTX_H
#define SMM_CTX_H

#include "svm_addr_desc.h"
#include "svm_pub.h"
#include "smm_kernel.h"

struct smm_ctx {
    u32 udevid;
    void *dev_ctx;
    struct smm_ops *cs_ops;
    struct smm_ops **ops;
};

struct smm_ctx *smm_ctx_get(u32 udevid);
void smm_ctx_put(struct smm_ctx *ctx);

int smm_pa_get(struct smm_ctx *ctx, struct svm_global_va *src_info, struct svm_pa_seg pa_seg[], u64 *seg_num);
int smm_pa_put(struct smm_ctx *ctx, struct svm_global_va *src_info, struct svm_pa_seg pa_seg[], u64 seg_num);

u32 smm_get_src_max_udev_num(void);

static inline int smm_alloc_va(struct smm_ctx *ctx, int tgid, u32 src_udevid, u64 size, u64 *va)
{
    struct smm_ops *ops = ctx->ops[src_udevid];
    if (ops->alloc_va == NULL) {
        return 0;
    }

    return ops->alloc_va(ctx->udevid, tgid, size, va);
}

static inline int smm_free_va(struct smm_ctx *ctx, int tgid, u32 src_udevid, u64 va, u64 size)
{
    struct smm_ops *ops = ctx->ops[src_udevid];
    if (ops->alloc_va == NULL) {
        return 0;
    }

    return ops->free_va(ctx->udevid, tgid, va, size);
}

static inline bool smm_is_ops_support_remap(struct smm_ctx *ctx, u32 src_udevid)
{
    struct smm_ops *ops = ctx->ops[src_udevid];
    return (ops->remap != NULL);
}

static inline enum SMM_PA_LOCATION_TYPE smm_get_pa_location(struct smm_ctx *ctx, u32 src_udevid)
{
    struct smm_ops *ops = ctx->ops[src_udevid];
    return ops->pa_location;
}

int smm_ops_init_dev(u32 udevid);
void smm_ops_uninit_dev(u32 udevid);
int smm_ops_feature_init(void);
void smm_ops_feature_uninit(void);

#endif

