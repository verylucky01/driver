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
#ifndef QUEUE_UT
#include <linux/spinlock.h>
#include <linux/hashtable.h>
#include <linux/slab.h>

#include "queue_module.h"
#include "queue_fops.h"
#include "queue_status_record.h"
#include "queue_context.h"

#define CONTEXT_HASH_TABLE_BIT 10
#define CONTEXT_HASH_TABLE_MASK ((0x1 << CONTEXT_HASH_TABLE_BIT) - 1)

static DEFINE_SPINLOCK(context_spinlock);
static DEFINE_HASHTABLE(context_table, CONTEXT_HASH_TABLE_BIT);

static void queue_context_add(struct queue_context *context)
{
    u32 key = (u32)context->pid & CONTEXT_HASH_TABLE_MASK;

    hash_add(context_table, &context->link, key);
}

static void queue_context_del(struct queue_context *context)
{
    hash_del(&context->link);
}

STATIC struct queue_context *queue_context_find(pid_t pid)
{
    struct queue_context *context = NULL;
    u32 key = (u32)pid & CONTEXT_HASH_TABLE_MASK;

    hash_for_each_possible(context_table, context, link, key) {
        if (context->pid == pid) {
            return context;
        }
    }

    return NULL;
}

struct queue_context *queue_context_get(pid_t pid)
{
    struct queue_context *ctx = NULL;

    spin_lock_bh(&context_spinlock);
    ctx = queue_context_find(pid);
    if (ctx != NULL) {
        atomic_inc(&ctx->refcnt);
    }
    spin_unlock_bh(&context_spinlock);

    return ctx;
}

void queue_context_put(struct queue_context *ctx)
{
    spin_lock_bh(&context_spinlock);
    if (!atomic_dec_and_test(&ctx->refcnt)) {
        spin_unlock_bh(&context_spinlock);
        return;
    }
    queue_context_del(ctx);
    spin_unlock_bh(&context_spinlock);

    queue_context_private_data_destroy(ctx->private_data);
    queue_free_ctx_all_qid_status(ctx);
    ctx->private_data = NULL;
    queue_drv_kfree(ctx);

    return;
}

struct queue_context *queue_context_init(pid_t pid)
{
    struct queue_context *ctx = NULL;
    void *private_data = NULL;

    private_data = queue_context_private_data_create();
    if (private_data == NULL) {
        queue_err("Private data create failed. (tgid=%d)\n", pid);
        return NULL;
    }

    spin_lock_bh(&context_spinlock);
    ctx = queue_context_find(pid);
    if (ctx != NULL) {
        spin_unlock_bh(&context_spinlock);
        queue_context_private_data_destroy(private_data);
        queue_err("Queue context had existed. (tgid=%d)\n", pid);
        return NULL;
    }
    ctx = (struct queue_context *)queue_drv_kzalloc(sizeof(struct queue_context), GFP_ATOMIC | __GFP_ACCOUNT);
    if (ctx == NULL) {
        spin_unlock_bh(&context_spinlock);
        queue_context_private_data_destroy(private_data);
        queue_err("Context kmalloc failed. (tgid=%d)\n", pid);
        return NULL;
    }

    ctx->private_data = private_data;
    spin_lock_init(&ctx->qid_status_lock);

    atomic_set(&ctx->refcnt, 1);
    ctx->pid = current->tgid;
    queue_context_add(ctx);

    spin_unlock_bh(&context_spinlock);

    return ctx;
}

void queue_context_uninit(struct queue_context *ctx)
{
    queue_context_put(ctx);
}
#else
void queue_context_common(void)
{
    return;
}
#endif
