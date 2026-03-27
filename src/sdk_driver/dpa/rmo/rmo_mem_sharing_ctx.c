/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2026. All rights reserved.
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
#include "ka_base_pub.h"
#include "ka_common_pub.h"
#include "ka_task_pub.h"
#include "ka_list_pub.h"
#include "pbl_ka_memory.h"
#include "rmo_kern_log.h"
#include "rmo_sched.h"
#include "rmo_slab.h"
#include "rmo_mem_sharing.h"
#include "rmo_mem_sharing_ctx.h"

static void rmo_mem_sharing_ctx_try_recycle_node(struct rmo_mem_sharing_ctx *mem_ctx)
{
    struct rmo_mem_sharing_node *node = NULL, *tmp = NULL;
    unsigned long stamp = ka_jiffies;

    ka_list_for_each_entry_safe(node, tmp, &mem_ctx->head, node) {
        (void)rmo_mem_addr_unmap(node->ctx.mem_shr.devid, &node->ctx.convert_addr, node->ctx.mem_shr.size);
        ka_list_del(&node->node);
        rmo_vfree(node);
        rmo_try_cond_resched(&stamp);
    }
}

static void rmo_mem_sharing_ctx_release(struct task_ctx *ctx)
{
    struct rmo_mem_sharing_ctx *mem_ctx = ctx->priv;

    rmo_mem_sharing_ctx_try_recycle_node(mem_ctx);
    rmo_vfree(ctx->priv);
    ctx->priv = NULL;
    rmo_debug("Release. (tgid=%d)\n", ctx->tgid);
}

static int _rmo_mem_sharing_ctx_create(struct task_ctx_domain *domain, int tgid)
{
    struct rmo_mem_sharing_ctx *ctx = NULL;
    int ret;

    ctx = rmo_vzalloc(sizeof(struct rmo_mem_sharing_ctx));
    if (ctx == NULL) {
        rmo_err("Alloc memory failed. (size=%lu)\n", sizeof(struct rmo_mem_sharing_ctx));
        return -ENOMEM;
    }

    KA_INIT_LIST_HEAD(&ctx->head);

    ret = task_ctx_create(domain, tgid, ctx, rmo_mem_sharing_ctx_release);
    if (ret != 0) {
        rmo_err("Create failed. (ret=%d; tgid=%d)\n", ret, tgid);
        rmo_vfree(ctx);
    }

    return ret;
}

static int rmo_mem_sharing_ctx_create(struct task_ctx_domain *domain, int tgid)
{
    struct task_ctx *ctx = NULL;
    int ret;

    ka_task_mutex_lock(&domain->mutex);
    ctx = task_ctx_get(domain, tgid);
    if (ctx == NULL) {
        ret = _rmo_mem_sharing_ctx_create(domain, tgid);
        if (ret != 0) {
            ka_task_mutex_unlock(&domain->mutex);
            return ret;
        }
    } else {
        task_ctx_put(ctx);
    }
    ka_task_mutex_unlock(&domain->mutex);
    return 0;
}

void rmo_mem_sharing_ctx_destroy(struct task_ctx_domain *domain, int tgid)
{
    struct task_ctx *ctx = NULL;

    rmo_debug("Destroy. (tgid=%d)\n", tgid);
    ka_task_mutex_lock(&domain->mutex);
    ctx = task_ctx_get(domain, tgid);
    if (ctx == NULL) {
        ka_task_mutex_unlock(&domain->mutex);
        return;
    }
    task_ctx_destroy(ctx);
    task_ctx_put(ctx);
    ka_task_mutex_unlock(&domain->mutex);
}

static struct rmo_mem_sharing_node *rmo_mem_sharing_ctx_find_node(struct rmo_mem_sharing_ctx *mem_ctx,
    struct rmo_mem_sharing_info *para)
{
    struct rmo_mem_sharing_node *node = NULL;
    unsigned long stamp = ka_jiffies;

    ka_list_for_each_entry(node, &mem_ctx->head, node) {
        if ((node->ctx.mem_shr.devid == para->mem_shr.devid) && (node->ctx.mem_shr.side == para->mem_shr.side) &&
            (node->ctx.mem_shr.ptr == para->mem_shr.ptr) && (node->ctx.mem_shr.size == para->mem_shr.size) &&
            (node->ctx.mem_shr.accessor == para->mem_shr.accessor) &&
            (node->ctx.mem_shr.pg_prot == para->mem_shr.pg_prot)) {
            return node;
        }
        rmo_try_cond_resched(&stamp);
    }
    return NULL;
}

static int rmo_mem_sharing_ctx_add_node(struct task_ctx *ctx, void *priv)
{
    struct rmo_mem_sharing_info *para = (struct rmo_mem_sharing_info *)priv;
    struct rmo_mem_sharing_ctx *mem_ctx = ctx->priv;
    struct rmo_mem_sharing_node *node = rmo_mem_sharing_ctx_find_node(mem_ctx, para);
    if (node != NULL) {
        rmo_err("Already added. (devid=%u; accessor=%d; tgid=%d)\n",
            para->mem_shr.devid, para->mem_shr.accessor, ctx->tgid);
        return -EAGAIN;
    }

    node = rmo_vzalloc(sizeof(struct rmo_mem_sharing_node));
    if (node == NULL) {
        rmo_err("Failed to alloc node. (devid=%u; accessor=%d; tgid=%d)\n",
            para->mem_shr.devid, para->mem_shr.accessor, ctx->tgid);
        return -ENOMEM;
    }

    node->ctx = *para;
    ka_list_add_tail(&node->node, &mem_ctx->head);
    rmo_debug("Add node success. (devid=%u; accessor=%d; tgid=%d)\n",
        para->mem_shr.devid, para->mem_shr.accessor, ctx->tgid);
    return 0;
}

int rmo_mem_sharing_add_node(struct task_ctx_domain *domain, int tgid, struct rmo_mem_sharing_info *para)
{
    struct task_start_time start_time;
    int ret = rmo_mem_sharing_ctx_create(domain, tgid);
    if (ret != 0) {
        return ret;
    }

    ret = task_get_start_time_by_tgid(tgid, &start_time);
    if (ret != 0) {
        rmo_err("Failed to get start time. (tgid=%d)\n", tgid);
        return ret;
    }
    return task_ctx_time_check_lock_call_func(domain, tgid, &start_time, rmo_mem_sharing_ctx_add_node, (void *)para);
}

static int rmo_mem_sharing_ctx_del_node(struct task_ctx *ctx, void *priv)
{
    struct rmo_mem_sharing_info *para = (struct rmo_mem_sharing_info *)priv;
    struct rmo_mem_sharing_ctx *mem_ctx = ctx->priv;
    struct rmo_mem_sharing_node *node = rmo_mem_sharing_ctx_find_node(mem_ctx, para);
    if (node == NULL) {
        rmo_err("Not add. (devid=%u; accessor=%d; tgid=%d)\n",
            para->mem_shr.devid, para->mem_shr.accessor, ctx->tgid);
        return -EFAULT;
    }

    rmo_debug("Del node success. (devid=%u; accessor=%d; tgid=%d)\n",
        para->mem_shr.devid, para->mem_shr.accessor, ctx->tgid);
    ka_list_del(&node->node);
    rmo_vfree(node);
    return 0;
}

int rmo_mem_sharing_del_node(struct task_ctx_domain *domain, int tgid, struct rmo_mem_sharing_info *para)
{
    return task_ctx_lock_call_func(domain, tgid, rmo_mem_sharing_ctx_del_node, (void *)para);
}

static int rmo_mem_sharing_ctx_query_node(struct task_ctx *ctx, void *priv)
{
    struct rmo_mem_sharing_info *para = (struct rmo_mem_sharing_info *)priv;
    struct rmo_mem_sharing_ctx *mem_ctx = ctx->priv;
    struct rmo_mem_sharing_node *node = rmo_mem_sharing_ctx_find_node(mem_ctx, para);
    if (node == NULL) {
        rmo_err("Not add. (devid=%u; accessor=%d; tgid=%d)\n",
            para->mem_shr.devid, para->mem_shr.accessor, ctx->tgid);
        return -EFAULT;
    }

    para->sharing_pa = node->ctx.sharing_pa;
    para->convert_addr = node->ctx.convert_addr;
    return 0;
}

int rmo_mem_sharing_query_node(struct task_ctx_domain *domain, int tgid, struct rmo_mem_sharing_info *para)
{
    return task_ctx_lock_call_func(domain, tgid, rmo_mem_sharing_ctx_query_node, (void *)para);
}

