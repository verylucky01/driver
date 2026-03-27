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
#include "ka_task_pub.h"
#include "ka_memory_pub.h"
#include "ka_common_pub.h"
#include "ka_hashtable_pub.h"
#include "ka_fs_pub.h"

#include "svm_slab.h"
#include "svm_kern_log.h"
#include "svm_pub.h"
#include "svm_gfp.h"
#include "pmq.h"
#include "ubmm_ctx.h"
#include "ubmm_uba.h"
#include "ubmm_map.h"
#include "ubmm_core.h"

static inline u32 ubmm_get_bkt(u64 va)
{
    return ka_hash_min((u32)(va >> 10), UBMM_NODE_HASH_BIT); /*  ignore lower 10 bit of va */
}

static inline void ubmm_node_add(struct ubmm_ctx *ctx, struct ubmm_node *node)
{
    u32 bkt = ubmm_get_bkt(node->va);
    ka_hash_add(ctx->htable, &node->link, bkt);
}

static inline void ubmm_node_del(struct ubmm_node *node)
{
    ka_hash_del(&node->link);
}

static inline struct ubmm_node *ubmm_node_find(struct ubmm_ctx *ctx, u64 va, u64 size)
{
    u32 bkt = ubmm_get_bkt(va);
    struct ubmm_node *node = NULL;

    ka_hash_for_each_possible(ctx->htable, node, link, bkt) {
        if ((node->va == va) && (node->size == size)) {
            return node;
        }
    }
    return NULL;
}

void ubmm_node_recycle(struct ubmm_ctx *ctx)
{
    struct ubmm_node *node = NULL;
    ka_hlist_node_t *hnode = NULL;
    u32 bkt, num = 0;

    ka_task_mutex_lock(&ctx->mutex);
    ka_hash_for_each_safe(ctx->htable, bkt, hnode, node, link) {
        ubmm_node_del(node);
        (void)ubmm_unmap(ctx->udevid, node->uba, node->size);
        (void)ubmm_free_uba(node->uba, node->size);
        svm_kvfree(node);
        num++;
    }
    ka_task_mutex_unlock(&ctx->mutex);

    if (num > 0) {
        svm_warn("Recycle uba. (udevid=%u; tgid=%d; num=%u)\n", ctx->udevid, ctx->tgid, num);
    }
}

void ubmm_node_show(struct ubmm_ctx *ctx, ka_seq_file_t *seq)
{
    struct ubmm_node *node = NULL;
    ka_hlist_node_t *hnode = NULL;
    u32 bkt;
    int i = 0;

    ka_task_mutex_lock(&ctx->mutex);
    ka_hash_for_each_safe(ctx->htable, bkt, hnode, node, link) {
        if (i == 0) {
            ka_fs_seq_printf(seq, "ubmm:   index     ref     va    size   uba\n");
        }
        ka_fs_seq_printf(seq, "   %d   %d     %llx     %llx     %llx \n", i++, node->ref, node->va, node->size, node->uba);
        if (i >= 128) { /* show max 128 item */
            break;
        }
    }
    ka_task_mutex_unlock(&ctx->mutex);
}

static int ubmm_create_node(struct ubmm_ctx *ctx, u64 va, u64 size, u64 uba)
{
    struct ubmm_node *node = NULL;

    node = (struct ubmm_node *)svm_kvmalloc(sizeof(*node), KA_GFP_KERNEL | __KA_GFP_ACCOUNT);
    if (node == NULL) {
        svm_err("No mem. (udevid=%u; tgid=%d; va=0x%llx)\n", ctx->udevid, ctx->tgid, va);
        return -ENOMEM;
    }

    node->ref = 1;
    node->va = va;
    node->size = size;
    node->uba = uba;
    ubmm_node_add(ctx, node);

    return 0;
}

static void ubmm_destroy_node(struct ubmm_node *node)
{
    ubmm_node_del(node);
    svm_kvfree(node);
}

static int ubmm_map_uba(struct ubmm_ctx *ctx, u64 va, u64 size, u64 *uba)
{
    struct ubmm_node *node = NULL;
    struct svm_pa_seg *pa_seg = NULL;
    u64 seg_num = 0;
    int ret;

    node = ubmm_node_find(ctx, va, size);
    if (node != NULL) {
        *uba = node->uba;
        node->ref += 1;
        return 0;
    }

    seg_num = svm_get_align_up_num(va, size, KA_MM_PAGE_SIZE);
    pa_seg = svm_kvmalloc(sizeof(struct svm_pa_seg) * seg_num, KA_GFP_KERNEL | __KA_GFP_ACCOUNT);
    if (pa_seg == NULL) {
        svm_err("Map kvmalloc pa_seg failed. (alloc_seg_num=%llu)\n", seg_num);
        return -ENOMEM;
    }

    ret = svm_pmq_pa_query(ctx->tgid, va, size, pa_seg, &seg_num);
    if ((ret != 0) || (size != svm_get_pa_size(pa_seg, seg_num))) {
        svm_kvfree(pa_seg);
        svm_err("Pmq pa query failed. (tgid=%d; va=0x%llx; size=0x%llx)\n", ctx->tgid, va, size);
        return -EINVAL;
    }

    ret = ubmm_alloc_uba(size, uba);
    if (ret != 0) {
        svm_kvfree(pa_seg);
        svm_err("Alloc uba failed. (ret=%d; tgid=%d; va=0x%llx; size=0x%llx)\n",
            ret, ctx->tgid, va, size);
        return ret;
    }

    ret = ubmm_map(ctx->udevid, *uba, size, pa_seg, seg_num);
    svm_kvfree(pa_seg);
    if (ret != 0) {
        ubmm_free_uba(*uba, size);
        svm_err("Map uba failed. (tgid=%d; va=0x%llx; size=0x%llx)\n", ctx->tgid, va, size);
        return ret;
    }

    ret = ubmm_create_node(ctx, va, size, *uba);
    if (ret != 0) {
        (void)ubmm_unmap(ctx->udevid, *uba, size);
        ubmm_free_uba(*uba, size);
        return ret;
    }

    return 0;
}

int ubmm_do_map(u32 udevid, int tgid, u64 va, u64 size, u64 *uba)
{
    int ret;
    struct ubmm_ctx *ctx = ubmm_ctx_get(udevid, tgid);
    if (ctx == NULL) {
        svm_err("Invalid para. (udevid=%u; tgid=%d; va=0x%llx; size=0x%llx)\n", udevid, tgid, va, size);
        return -EINVAL;
    }

    ka_task_mutex_lock(&ctx->mutex);
    ret = ubmm_map_uba(ctx, va, size, uba);
    ka_task_mutex_unlock(&ctx->mutex);
    ubmm_ctx_put(ctx);

    if (ret == 0) {
        *uba += va - svm_align_down(va, KA_MM_PAGE_SIZE);
    }

    return ret;
}

static int ubmm_unmap_uba(struct ubmm_ctx *ctx, u64 va, u64 size)
{
    struct ubmm_node *node = NULL;
    int ret;

    node = ubmm_node_find(ctx, va, size);
    if (node == NULL) {
        svm_err("Invalid para. (udevid=%u; tgid=%d; va=0x%llx; size=0x%llx)\n", ctx->udevid, ctx->tgid, va, size);
        return -EINVAL;
    }

    node->ref -= 1;
    if (node->ref > 0) {
        return 0;
    }

    ret = ubmm_unmap(ctx->udevid, node->uba, node->size);
    if (ret != 0) {
        svm_err("Unmap failed. (udevid=%u; tgid=%d; va=0x%llx; uba=0x%llx)\n", ctx->udevid, ctx->tgid, va, node->uba);
        return ret;
    }

    (void)ubmm_free_uba(node->uba, node->size);
    ubmm_destroy_node(node);

    return 0;
}

int ubmm_do_unmap(u32 udevid, int tgid, u64 va, u64 size)
{
    int ret;
    struct ubmm_ctx *ctx = ubmm_ctx_get(udevid, tgid);
    if (ctx == NULL) {
        svm_err("Invalid para. (udevid=%u; tgid=%d; va=0x%llx)\n", udevid, tgid, va);
        return -EINVAL;
    }

    ka_task_mutex_lock(&ctx->mutex);
    ret = ubmm_unmap_uba(ctx, va, size);
    ka_task_mutex_unlock(&ctx->mutex);
    ubmm_ctx_put(ctx);

    return ret;
}

int ubmm_query_uba_base(u32 udevid, u64 *uba_base)
{
    u64 total_size, avail_size;
    return ubmm_get_uba_pool(uba_base, &total_size, &avail_size);
}
