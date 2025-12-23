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
#include <pthread.h>
 
#include "ascend_hal_error.h"
#include "drv_user_common.h"
 
#include "drv_buff_adp.h"
#include "buff_recycle.h"
#include "buff_recycle_ctx.h"
 
#define RECYCLE_THREAD_MIN_PERIOD_US 10000 /* 10ms */
#define RECYCLE_THREAD_PERIOD_NS ((RECYCLE_THREAD_PERIOD_US - RECYCLE_THREAD_MIN_PERIOD_US) * 1000)
 
static THREAD struct buff_recycle_ctx_mng g_ctx_mng;
STATIC void _buff_recycle_ctx_mng_init(void)
{
    g_ctx_mng.ctx_num = 0;
    INIT_LIST_HEAD(&g_ctx_mng.head);
    (void)pthread_rwlock_init(&g_ctx_mng.rwlock, NULL);
}
 
static void __attribute__ ((constructor))buff_recycle_ctx_mng_init(void)
{
    _buff_recycle_ctx_mng_init();
}
 
struct buff_recycle_ctx_mng *buff_get_recycle_ctx_mng(void)
{
    return &g_ctx_mng;
}
 
static struct buff_recycle_ctx *_buff_get_recycle_ctx(struct list_head *head, int pool_id)
{
    struct buff_recycle_ctx *ctx = NULL;
    struct list_head *pos = NULL, *n = NULL;
 
    list_for_each_safe(pos, n, head) {
        ctx = list_entry(pos, struct buff_recycle_ctx, node);
        if (ctx->p_mng.pool_id == pool_id) {
            return ctx;
        }
    }
 
    return NULL;
}
 
struct buff_recycle_ctx *buff_get_recycle_ctx(int pool_id)
{
    struct buff_recycle_ctx *ctx = NULL;
 
    (void)pthread_rwlock_rdlock(&g_ctx_mng.rwlock);
    ctx = _buff_get_recycle_ctx(&g_ctx_mng.head, pool_id);
    (void)pthread_rwlock_unlock(&g_ctx_mng.rwlock);
 
    return ctx;
}
 
static void pool_ctx_init(struct proc_mng_ctx *p_mng, int pool_id)
{
    int i;
 
    for (i = 0; i < MEM_LIST_NUM; i++) {
        INIT_LIST_HEAD(&p_mng->alloc_list[i]);
    }
 
    INIT_LIST_HEAD(&p_mng->alloc_list_tmp);
 
    p_mng->pool_id = pool_id;
    p_mng->task_id = buff_get_process_uni_id();
    p_mng->pid = buff_get_current_pid();
    p_mng->block_num = 0;
    p_mng->free_num = 0;
    p_mng->exit_task_id = 0;
 
    (void)pthread_mutex_init(&p_mng->alloc_list_mutex, NULL);
    (void)pthread_mutex_init(&p_mng->alloc_tmp_list_mutex, NULL);
 
    buff_event("pool_id %d add task node uid %llu pid %d\n", p_mng->pool_id, p_mng->task_id, p_mng->pid);
}
 
static struct buff_recycle_ctx *_buff_recycle_ctx_create(int pool_id)
{
    struct buff_recycle_ctx *ctx = NULL;
 
    ctx = (struct buff_recycle_ctx *)malloc(sizeof(struct buff_recycle_ctx));
    if (ctx == NULL) {
        buff_err("Malloc struct buff_recycle_ctx failed.\n");
        return NULL;
    }
 
    INIT_LIST_HEAD(&ctx->node);
    ctx->thread = 0;
    ctx->status = RECYCLE_THREAD_STATUS_IDLE;
    ctx->wait_flag = 0;
    ctx->wake_up_cnt = 0;
    ctx->period = RECYCLE_THREAD_PERIOD_US;
    ctx->min_period = RECYCLE_THREAD_MIN_PERIOD_US;
    ctx->period_ns = RECYCLE_THREAD_PERIOD_NS;
    (void)sem_init(&ctx->sem, 0, 0);
    pool_ctx_init(&ctx->p_mng, pool_id);
 
    return ctx;
}
 
drvError_t buff_recycle_ctx_create(int pool_id, struct buff_recycle_ctx **out_ctx)
{
    struct buff_recycle_ctx *ctx = NULL;
 
    (void)pthread_rwlock_wrlock(&g_ctx_mng.rwlock);
    ctx = _buff_get_recycle_ctx(&g_ctx_mng.head, pool_id);
    if (ctx != NULL) {
        (void)pthread_rwlock_unlock(&g_ctx_mng.rwlock);
        return DRV_ERROR_REPEATED_INIT;
    }
 
    ctx = _buff_recycle_ctx_create(pool_id);
    if (ctx  == NULL) {
        (void)pthread_rwlock_unlock(&g_ctx_mng.rwlock);
        return DRV_ERROR_OUT_OF_MEMORY;
    }
 
    buff_api_atomic_inc(&g_ctx_mng.ctx_num);
    drv_user_list_add_head(&ctx->node, &g_ctx_mng.head);
    (void)pthread_rwlock_unlock(&g_ctx_mng.rwlock);
 
    *out_ctx = ctx;
    return DRV_ERROR_NONE;
}