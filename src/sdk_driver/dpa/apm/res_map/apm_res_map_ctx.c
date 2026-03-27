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
#include "ka_base_pub.h"
#include "ka_common_pub.h"
#include "ka_fs_pub.h"
#include "ka_list_pub.h"
#include "ka_task_pub.h"

#include "apm_task_group_def.h"
#include "apm_res_map_ctx.h"
#include "apm_slab.h"
#include "apm_res_map_ops.h"
#include "apm_sched.h"

void apm_res_map_free_node_pa_array(struct apm_res_map_info *res_map_info)
{
    if (res_map_info->pa_array != NULL) {
        apm_put_res_addr(res_map_info->udevid, &res_map_info->res_info, res_map_info->pa_array, res_map_info->len);
        apm_vfree(res_map_info->pa_array);
        res_map_info->pa_array = NULL;
    }
}

static void apm_res_map_ctx_try_recycle_node(struct apm_res_map_ctx *map_ctx)
{
    struct apm_res_map_node *node = NULL, *tmp_node = NULL;
    unsigned long stamp = ka_jiffies;

    ka_list_for_each_entry_safe(node, tmp_node, &map_ctx->head, node) {
        int proc_type = node->ctx.res_info.target_proc_type;
        apm_debug("Recycle map node. (udevid=%u; proc_type=%d(%s); res_type=%d; res_id=%u; va=0x%lx; len=%u\n",
            node->ctx.udevid, proc_type, apm_proc_type_to_name(proc_type), node->ctx.res_info.res_type,
            node->ctx.res_info.res_id, node->ctx.va, node->ctx.len);
        apm_res_map_free_node_pa_array(&node->ctx);
        ka_list_del(&node->node);
        apm_vfree(node);

        apm_try_cond_resched(&stamp);
    }
}

static void apm_res_map_ctx_release(struct task_ctx *ctx)
{
    struct apm_res_map_ctx *map_ctx = (struct apm_res_map_ctx *)ctx->priv;

    apm_res_map_ctx_try_recycle_node(map_ctx);
    apm_proc_fs_del_task(map_ctx->entry);
    apm_vfree(ctx->priv);
    ctx->priv = NULL;
}

static int _apm_res_map_ctx_create(struct task_ctx_domain *domain, int tgid)
{
    struct apm_res_map_ctx *map_ctx = NULL;
    long size = sizeof(*map_ctx);
    int ret;

    map_ctx = apm_vzalloc(size);
    if (map_ctx == NULL) {
        apm_err("Vmalloc res map ctx fail. (size=%ld)\n", size);
        return -ENOMEM;
    }

    KA_INIT_LIST_HEAD(&map_ctx->head);

    ret = task_ctx_create(domain, tgid, map_ctx, apm_res_map_ctx_release);
    if (ret != 0) {
        apm_err("Create fail. (ret=%d; tgid=%d)\n", ret, tgid);
        apm_vfree(map_ctx);
        return ret;
    }

    map_ctx->entry = apm_proc_fs_add_task(domain->name, tgid);

    return 0;
}

int apm_res_map_ctx_create(struct task_ctx_domain *domain, int tgid)
{
    struct task_ctx *ctx = NULL;
    int ret;

    ka_task_mutex_lock(&domain->mutex);
    ctx = task_ctx_get(domain, tgid);
    if (ctx == NULL) {
        ret = _apm_res_map_ctx_create(domain, tgid);
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

void apm_res_map_ctx_destroy(struct task_ctx_domain *domain, int tgid)
{
    struct task_ctx *ctx = NULL;

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

static struct apm_res_map_node *apm_res_map_ctx_find_node(struct apm_res_map_ctx *map_ctx,
    struct apm_res_map_info *para)
{
    struct apm_res_map_node *node = NULL;

    ka_list_for_each_entry(node, &map_ctx->head, node) {
        if ((node->ctx.udevid == para->udevid) &&
            (node->ctx.res_info.id == para->res_info.id) &&
            (node->ctx.res_info.target_proc_type == para->res_info.target_proc_type) &&
            (node->ctx.res_info.res_type == para->res_info.res_type) &&
            (node->ctx.res_info.res_id == para->res_info.res_id)) {
            return node;
        }
    }

    return NULL;
}

static int apm_res_map_ctx_add_node(struct task_ctx *ctx, void *priv)
{
    struct apm_res_map_ctx *map_ctx = (struct apm_res_map_ctx *)ctx->priv;
    struct apm_res_map_info *para = (struct apm_res_map_info *)priv;
    struct apm_res_map_node *node = apm_res_map_ctx_find_node(map_ctx, para);
    if (node != NULL) {
        ka_base_atomic_inc(&node->ctx.ref);
#ifndef EMU_ST
        apm_debug("Already mapped. (res_type=%d; res_id=%u; va=0x%lx; ref=%d)\n", para->res_info.res_type,
            para->res_info.res_id, node->ctx.va,  ka_base_atomic_read(&node->ctx.ref));
#endif
        return 0;
    }

    node = apm_vzalloc(sizeof(struct apm_res_map_node));
    if (node == NULL) {
        apm_err("Alloc node failed. (res_type=%d; res_id=%u)\n", para->res_info.res_type, para->res_info.res_id);
        return -ENOMEM;
    }

    ka_base_atomic_set(&para->ref, 1);
    node->ctx = *para;
    ka_list_add_tail(&node->node, &map_ctx->head);
    apm_debug("Add map node success. (res_type=%d; res_id=%u; va=0x%lx; ref=%d)\n", para->res_info.res_type,
        para->res_info.res_id, node->ctx.va,  ka_base_atomic_read(&node->ctx.ref));

    return 0;
}

int apm_res_map_add_node(struct task_ctx_domain *domain, int tgid, struct apm_res_map_info *para)
{
    int ret = apm_res_map_ctx_create(domain, tgid);
    if (ret != 0) {
        return ret;
    }

    return task_ctx_lock_call_func(domain, tgid, apm_res_map_ctx_add_node, (void *)para);
}

static int apm_res_map_ctx_del_node(struct task_ctx *ctx, void *priv)
{
    struct apm_res_map_ctx *map_ctx = (struct apm_res_map_ctx *)ctx->priv;
    struct apm_res_map_info *para = (struct apm_res_map_info *)priv;
    struct apm_res_map_node *node = apm_res_map_ctx_find_node(map_ctx, para);
    int ref;

    if (node == NULL) {
#ifndef EMU_ST
        apm_err("Not mapped. (res_type=%d; res_id=%u)\n", para->res_info.res_type, para->res_info.res_id);
#endif
        return -EINVAL;
    }

    if (ka_base_atomic_dec_and_test(&node->ctx.ref)) {
        apm_debug("Del success. (res_type=%d; res_id=%u; va=0x%lx; ref=%d)\n", para->res_info.res_type,
            para->res_info.res_id, node->ctx.va, ka_base_atomic_read(&node->ctx.ref));
        ref = (int)ka_base_atomic_read(&node->ctx.ref);
        apm_res_map_free_node_pa_array(&node->ctx);
        ka_list_del(&node->node);
        apm_vfree(node);
    } else {
        ref = (int)ka_base_atomic_read(&node->ctx.ref);
        apm_debug("Not del for ref. (res_type=%d; res_id=%u; va=0x%lx; ref=%d)\n", para->res_info.res_type,
            para->res_info.res_id, node->ctx.va, ka_base_atomic_read(&node->ctx.ref));
    }
    return (ref == 0) ? 0 : -EBUSY;
}

int apm_res_map_del_node(struct task_ctx_domain *domain, int tgid, struct apm_res_map_info *para)
{
    return task_ctx_lock_call_func(domain, tgid, apm_res_map_ctx_del_node, (void *)para);
}

static int apm_res_map_ctx_query_node(struct task_ctx *ctx, void *priv)
{
    struct apm_res_map_ctx *map_ctx = (struct apm_res_map_ctx *)ctx->priv;
    struct apm_res_map_info *para = (struct apm_res_map_info *)priv;
    struct apm_res_map_node *node = apm_res_map_ctx_find_node(map_ctx, para);
    if (node == NULL) {
        apm_debug("Not mapped. (res_type=%d; res_id=%u)\n", para->res_info.res_type, para->res_info.res_id);
        return -ENXIO;
    }

    para->slave_tgid = node->ctx.slave_tgid;
    para->va = node->ctx.va;
    para->pa = node->ctx.pa;
    para->pa_array = NULL;
    para->len = node->ctx.len;
    para->ref = node->ctx.ref;
    return 0;
}

int apm_res_map_query_node(struct task_ctx_domain *domain, int tgid, struct apm_res_map_info *para)
{
    return task_ctx_lock_call_func(domain, tgid, apm_res_map_ctx_query_node, (void *)para);
}

static int apm_res_map_ctx_show_details(struct task_ctx *ctx, void *priv)
{
    struct apm_res_map_ctx *map_ctx = (struct apm_res_map_ctx *)ctx->priv;
    ka_seq_file_t *seq = (ka_seq_file_t *)priv;
    struct apm_res_map_node *node = NULL;

    ka_fs_seq_printf(seq, "domain: %s tgid %d(%s)\n", ctx->domain->name, ctx->tgid, ctx->name);

    ka_list_for_each_entry(node, &map_ctx->head, node) {
        int proc_type = node->ctx.res_info.target_proc_type;
        ka_fs_seq_printf(seq, "    udevid %u proc_type %d(%s) res_type %d res_id %u va 0x%lx pa 0x%llx len %u\n",
            node->ctx.udevid, proc_type, apm_proc_type_to_name(proc_type), node->ctx.res_info.res_type,
            node->ctx.res_info.res_id, node->ctx.va, node->ctx.pa, node->ctx.len);
    }

    ka_fs_seq_printf(seq, "\n");
    return 0;
}

void apm_res_map_ctx_show(struct task_ctx_domain *domain, int tgid, ka_seq_file_t *seq)
{
    (void)task_ctx_lock_call_func(domain, tgid, apm_res_map_ctx_show_details, (void *)seq);
}

