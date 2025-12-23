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

#ifndef PBL_ARRAY_CTX_H
#define PBL_ARRAY_CTX_H

#include <linux/kref.h>
#include <linux/list.h>
#include <linux/spinlock.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/mutex.h>
#include <linux/seq_file.h>

#include "pbl_kref_safe.h"

/*
   usage : Process Context Management with reference counting
        1. call array_ctx_domain_create create a domain when module_init
        2. call array_ctx_create create a task context in the domain when process open a file
        3. call array_ctx_get and array_ctx_put in business
        4. call array_ctx_destroy when process exit or close a file
        5. call array_ctx_for_each_safe traverse a domain
        6. array_ctx_domain_show
        7. call array_ctx_domain_destroy destroy domain when module_exit
*/

#define MAX_ARRAY_SIZE 4096

struct array_ctx_domain {
    rwlock_t ctx_lock;
    struct mutex mutex;
    const char *name;
    u32 array_size;
    u32 array_num;
    void *ctx_table[0];
};

struct array_ctx {
    struct kref_safe ref;
    u32 id;
    struct mutex mutex;
    struct array_ctx_domain *domain;
    void *priv;
    void (*release)(struct array_ctx *ctx);
};

/* array_size=0, Not restricted */
static inline void array_ctx_domain_init(struct array_ctx_domain *domain, const char *name, u32 array_size)
{
    domain->name = name;
    domain->array_size = array_size;
    mutex_init(&domain->mutex);
    rwlock_init(&domain->ctx_lock);
}

/* interface */
static inline struct array_ctx_domain *array_ctx_domain_create(const char *name, u32 array_size)
{
    struct array_ctx_domain *domain = NULL;

    if ((array_size == 0) || (array_size > MAX_ARRAY_SIZE)) {
        return NULL;
    }

    domain = vzalloc(sizeof(*domain) + (sizeof(void *) * array_size));
    if (domain == NULL) {
        return NULL;
    }

    array_ctx_domain_init(domain, name, array_size);

    return domain;
}

/* interface */
static inline void array_ctx_domain_destroy(struct array_ctx_domain *domain)
{
    mutex_destroy(&domain->mutex);
    vfree(domain);
}

static inline int array_ctx_add_to_domain(struct array_ctx_domain *domain, struct array_ctx *ctx)
{
    write_lock_bh(&domain->ctx_lock);
    if (domain->ctx_table[ctx->id] != NULL) {
        write_unlock_bh(&domain->ctx_lock);
        return -EEXIST;
    }
    domain->ctx_table[ctx->id] = ctx;
    domain->array_num++;
    write_unlock_bh(&domain->ctx_lock);

    return 0;
}

static inline void _array_ctx_release(struct array_ctx *ctx)
{
    if (ctx->release != NULL) {
        ctx->release(ctx);
    }
    vfree(ctx);
}

static inline void array_ctx_release(struct kref_safe *ref)
{
    struct array_ctx *ctx = container_of(ref, struct array_ctx, ref);
    _array_ctx_release(ctx);
}

static inline struct array_ctx *_array_ctx_get(struct array_ctx_domain *domain, u32 id)
{
    struct array_ctx *ctx = domain->ctx_table[id];
    if (ctx != NULL) {
        if (kref_safe_get_unless_zero(&ctx->ref) == 0) {
            return NULL;
        }
    }

    return ctx;
}

/* interface */
static inline struct array_ctx *array_ctx_get(struct array_ctx_domain *domain, u32 id)
{
    struct array_ctx *ctx = NULL;

    if (id >= domain->array_size) {
        return NULL;
    }

    read_lock_bh(&domain->ctx_lock);
    ctx = _array_ctx_get(domain, id);
    read_unlock_bh(&domain->ctx_lock);

    return ctx;
}

/* interface */
static inline void array_ctx_put(struct array_ctx *ctx)
{
    kref_safe_put(&ctx->ref, array_ctx_release);
}

/* interface */
static inline int array_ctx_ref_cnt(struct array_ctx *ctx)
{
    return kref_safe_read(&ctx->ref);
}

/* interface */
static inline void *array_ctx_priv(struct array_ctx *ctx)
{
    return ctx->priv;
}

static inline int array_ctx_init(struct array_ctx_domain *domain, struct array_ctx *ctx,
    u32 id, void *priv, void (*release)(struct array_ctx *ctx))
{
    ctx->id = id;
    ctx->priv = priv;
    mutex_init(&ctx->mutex);
    ctx->domain = domain;
    ctx->release = release;
    kref_safe_init(&ctx->ref);
    return array_ctx_add_to_domain(domain, ctx);
}

static inline int _array_ctx_create(struct array_ctx_domain *domain,
    u32 id, void *priv, void (*release)(struct array_ctx *ctx))
{
    int ret;
    struct array_ctx *ctx = vzalloc(sizeof(*ctx));
    if (ctx == NULL) {
        return -ENOMEM;
    }

    ret = array_ctx_init(domain, ctx, id, priv, release);
    if (ret != 0) {
        vfree(ctx);
    }

    return ret;
}

/* interface */
static inline int array_ctx_create(struct array_ctx_domain *domain,
    u32 id, void *priv, void (*release)(struct array_ctx *ctx))
{
    if (id >= domain->array_size) {
        return -EINVAL;
    }

    return _array_ctx_create(domain, id, priv, release);
}

/* interface */
static inline void array_ctx_destroy(struct array_ctx *ctx)
{
    struct array_ctx_domain *domain = ctx->domain;
    write_lock_bh(&domain->ctx_lock);
    domain->ctx_table[ctx->id] = NULL;
    domain->array_num--;
    write_unlock_bh(&domain->ctx_lock);
    array_ctx_put(ctx);
}

/* interface */
static inline void array_ctx_for_each_safe(struct array_ctx_domain *domain,
    void *priv, void (*func)(struct array_ctx *ctx, void *priv))
{
    u32 id;

    for (id = 0; id < domain->array_size; id++) {
        struct array_ctx *ctx = array_ctx_get(domain, id);
        if (ctx != NULL) {
            func(ctx, priv);
            array_ctx_put(ctx);
        }
    }
}

static inline void array_ctx_item_show(struct array_ctx *ctx, void *priv)
{
    struct seq_file *seq = (struct seq_file *)priv;
    seq_printf(seq, "    id %u ref %u\n", ctx->id, kref_safe_read(&ctx->ref));
}

/* interface */
static inline void array_ctx_domain_show(struct array_ctx_domain *domain, struct seq_file *seq)
{
    seq_printf(seq, "domain: %s array_size %u\n", domain->name, domain->array_size);

    array_ctx_for_each_safe(domain, seq, array_ctx_item_show);
    seq_printf(seq, "\n");
}

/* interface */
static inline int array_ctx_lock_call_func(struct array_ctx_domain *domain, u32 id,
    int (*func)(struct array_ctx *ctx, void *para), void *para)
{
    int ret = -ESRCH;
    struct array_ctx *ctx = array_ctx_get(domain, id);
    if (ctx != NULL) {
        mutex_lock(&ctx->mutex);
        ret = func(ctx, para);
        mutex_unlock(&ctx->mutex);
        array_ctx_put(ctx);
    }

    return ret;
}
#endif
