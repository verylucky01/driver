/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include <stdlib.h>
#include <stdint.h>
#include <securec.h>

#include "urma_api.h"
#include "urma_types.h"

#include "ascend_hal.h"
#include "ascend_hal_error.h"
#include "dms_user_interface.h"
#include "drv_user_common.h"
#include "uref.h"

#include "ascend_urma_pub.h"
#include "ascend_urma_log.h"
#include "comm_user_interface.h"
#include "ascend_urma_dev.h"

struct ascend_urma_ctx {
    struct uref ref;

    uint32_t eid_index;
    urma_eid_t eid;
    urma_device_t *urma_dev;
    urma_context_t *urma_ctx;
};

static pthread_rwlock_t g_rwlock = PTHREAD_RWLOCK_INITIALIZER;
static struct ascend_urma_ctx *g_ascend_urma_ctxs[ASCEND_URMA_MAX_DEV_NUM] = {NULL};

static struct ascend_urma_ctx *ascend_urma_ctx_create(u32 devid)
{
    struct ascend_urma_ctx *ctx = NULL;

    ctx = calloc(1, sizeof(struct ascend_urma_ctx));
    if (ctx == NULL) {
        ascend_urma_err("Calloc failed. (size=%llu)\n", (u64)sizeof(struct ascend_urma_ctx));
        return NULL;
    }
    uref_init(&ctx->ref);

    ctx->urma_dev = ascend_urma_get_device(devid, &ctx->eid_index);
    if (ctx->urma_dev == NULL) {
        ascend_urma_err("Get urma device failed. (devid=%u)\n", devid);
        free(ctx);
        return NULL;
    }

    ctx->urma_ctx = urma_create_context(ctx->urma_dev, ctx->eid_index);
    if (ctx->urma_ctx == NULL) {
        ascend_urma_err("Create urma ctx failed. (eid=%u)\n", ctx->eid_index);
        free(ctx);
        return NULL;
    }

    return ctx;
}

static void _ascend_urma_ctx_release(struct ascend_urma_ctx *ctx)
{
    (void)urma_delete_context(ctx->urma_ctx);
    free(ctx);
} 

static void ascend_urma_ctx_release(struct uref *uref)
{
    struct ascend_urma_ctx *ctx = container_of(uref, struct ascend_urma_ctx, ref);

    _ascend_urma_ctx_release(ctx);
}

void *ascend_urma_ctx_get(uint32_t devid)
{
    struct ascend_urma_ctx *ctx = NULL;

    if (devid >= ASCEND_URMA_MAX_DEV_NUM) {
        ascend_urma_err("Invalid devid. (devid=%u)\n", devid);
        return NULL;
    }

    (void)pthread_rwlock_wrlock(&g_rwlock);
    ctx = g_ascend_urma_ctxs[devid];
    if (ctx != NULL) {
        uref_get(&ctx->ref);
        (void)pthread_rwlock_unlock(&g_rwlock);
        return (void *)ctx;
    }

    ctx = ascend_urma_ctx_create(devid);
    if (ctx == NULL) {
        ascend_urma_err("Create urma ctx failed. (devid=%u)\n", devid);
        (void)pthread_rwlock_unlock(&g_rwlock);
        return NULL;
    }

    g_ascend_urma_ctxs[devid] = ctx;
    uref_get(&ctx->ref);
    (void)pthread_rwlock_unlock(&g_rwlock);
    return (void *)ctx;
}

void ascend_urma_ctx_put(void *ctx)
{
    struct ascend_urma_ctx *ascend_urma_ctx = (struct ascend_urma_ctx *)ctx;

    (void)uref_put(&ascend_urma_ctx->ref, ascend_urma_ctx_release);
}

urma_context_t *ascend_to_urma_ctx(void *ctx)
{
    struct ascend_urma_ctx *ascend_urma_ctx = (struct ascend_urma_ctx *)ctx;
    return ascend_urma_ctx->urma_ctx;
}

void ascend_urma_ctxs_release(void)
{
    u32 i;

    (void)pthread_rwlock_wrlock(&g_rwlock);
    for (i = 0; i < ASCEND_URMA_MAX_DEV_NUM; i++) {
        if (g_ascend_urma_ctxs[i] != NULL) {
            _ascend_urma_ctx_release(g_ascend_urma_ctxs[i]);
            g_ascend_urma_ctxs[i] = NULL;
        }
    }
    (void)pthread_rwlock_unlock(&g_rwlock);
}

