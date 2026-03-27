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
#include "ka_task_pub.h"
#include "ka_memory_pub.h"
#include "ka_system_pub.h"
#include "ka_fs_pub.h"
#include "ka_sched_pub.h"

#include "svm_kern_log.h"
#include "svm_slab.h"
#include "ksvmm.h"
#include "ksvmm_ctx.h"
#include "ksvmm_core.h"

static struct ksvmm_seg *ksvmm_seg_create(struct ksvmm_ctx *ctx, u64 start, struct svm_global_va *src_info)
{
    struct ksvmm_seg *seg = (struct ksvmm_seg *)svm_kvzalloc(sizeof(struct ksvmm_seg), KA_GFP_KERNEL | __KA_GFP_ACCOUNT);
    if (seg == NULL) {
        svm_err("Alloc seg failed. (udevid=%u; tgid=%d; start=%llx; size=%llx)\n",
            ctx->udevid, ctx->tgid, start, src_info->size);
        return NULL;
    }

    seg->range_node.start = start;
    seg->range_node.size = src_info->size;
    seg->src_info = *src_info;
    ka_base_atomic64_set(&seg->refcnt, 0);

    return seg;
}

static inline void ksvmm_seg_destroy(struct ksvmm_seg *seg)
{
    svm_kvfree(seg);
}

static struct ksvmm_seg *ksvmm_seg_search(struct ksvmm_ctx *ctx, u64 va, u64 size)
{
    struct ksvmm_seg *seg = NULL;
    struct range_rbtree_node *range_node = NULL;

    range_node = range_rbtree_search(&ctx->range_tree, va, size);
    if (range_node != NULL) {
        seg = ka_container_of(range_node, struct ksvmm_seg, range_node);
    }

    return seg;
}

static int _ksvmm_add_seg(struct ksvmm_ctx *ctx, u64 start, struct svm_global_va *src_info)
{
    struct ksvmm_seg *seg = NULL;
    int ret;

    seg = ksvmm_seg_create(ctx, start, src_info);
    if (seg == NULL) {
        return -ENOMEM;
    }

    ka_task_down_write(&ctx->rw_sem);
    ret = range_rbtree_insert(&ctx->range_tree, &seg->range_node);
    ka_task_up_write(&ctx->rw_sem);
    if (ret != 0) {
        svm_err("Insert failed. (udevid=%u; tgid=%d; start=%llx; size=%llx)\n",
            ctx->udevid, ctx->tgid, start, src_info->size);
        ksvmm_seg_destroy(seg);
    }

    return ret;
}

static int _ksvmm_del_seg(struct ksvmm_ctx *ctx, u64 start)
{
    struct ksvmm_seg *seg = NULL;

    ka_task_down_write(&ctx->rw_sem);
    seg = ksvmm_seg_search(ctx, start, 1);
    if (seg == NULL) {
        ka_task_up_write(&ctx->rw_sem);
        svm_err("Search failed. (udevid=%u; tgid=%d; start=%llx)\n", ctx->udevid, ctx->tgid, start);
        return -EINVAL;
    }

    if (seg->range_node.start != start) {
        ka_task_up_write(&ctx->rw_sem);
        svm_err("Not add addr. (udevid=%u; tgid=%d; start=%llx)\n", ctx->udevid, ctx->tgid, start);
        return -EINVAL;
    }

    if (ka_base_atomic64_read(&seg->refcnt) > 0) {
        ka_task_up_write(&ctx->rw_sem);
        return -EBUSY;
    }

    range_rbtree_erase(&ctx->range_tree, &seg->range_node);
    ka_task_up_write(&ctx->rw_sem);

    ksvmm_seg_destroy(seg);

    return 0;
}

static int _ksvmm_get_seg(struct ksvmm_ctx *ctx, u64 va, u64 *start, struct svm_global_va *src_info)
{
    struct ksvmm_seg *seg = NULL;

    ka_task_down_read(&ctx->rw_sem);
    seg = ksvmm_seg_search(ctx, va, 1);
    if (seg == NULL) {
        ka_task_up_read(&ctx->rw_sem);
        return -EINVAL;
    }

    *start = seg->range_node.start;
    *src_info = seg->src_info;
    ka_task_up_read(&ctx->rw_sem);
    return 0;
}

static int _ksvmm_pin_seg(struct ksvmm_ctx *ctx, u64 va, u64 size)
{
    struct ksvmm_seg *seg = NULL;
    int ret = 0;

    ka_task_down_read(&ctx->rw_sem);
    seg = ksvmm_seg_search(ctx, va, size);
    if (seg == NULL) {
        ka_task_up_read(&ctx->rw_sem);
        return -EINVAL;
    }

    ka_base_atomic64_inc(&seg->refcnt);
    ka_task_up_read(&ctx->rw_sem);

    return ret;
}

static int _ksvmm_unpin_seg(struct ksvmm_ctx *ctx, u64 va, u64 size)
{
    struct ksvmm_seg *seg = NULL;
    s64 refcnt;

    ka_task_down_read(&ctx->rw_sem);
    seg = ksvmm_seg_search(ctx, va, size);
    if (seg == NULL) {
        ka_task_up_read(&ctx->rw_sem);
        svm_err("Search failed. (udevid=%u; tgid=%d; va=%llx; size=%llx)\n",
            ctx->udevid, ctx->tgid, va, size);
        return -EINVAL;
    }

    refcnt = ka_base_atomic64_dec_return(&seg->refcnt);
    if (refcnt < 0) {
        ka_base_atomic64_inc(&seg->refcnt); /* restore refcnt, hold read lock, del can not access refcnt same time */
        ka_task_up_read(&ctx->rw_sem);
        svm_err("No pin, can not unpin. (udevid=%u; tgid=%d; va=%llx; size=%llx; refcnt=%lld)\n",
            ctx->udevid, ctx->tgid, va, size, refcnt);
        return -EINVAL;
    }

    ka_task_up_read(&ctx->rw_sem);

    return 0;
}

int ksvmm_add_seg(u32 udevid, int tgid, u64 start, struct svm_global_va *src_info)
{
    struct ksvmm_ctx *ctx = NULL;
    int ret;

    ctx = ksvmm_ctx_get(udevid, tgid);
    if (ctx == NULL) {
        svm_err("Invalid dev task. (udevid=%u; tgid=%d)\n", udevid, tgid);
        return -EINVAL;
    }

    ret = _ksvmm_add_seg(ctx, start, src_info);
    ksvmm_ctx_put(ctx);

    return ret;
}

int ksvmm_del_seg(u32 udevid, int tgid, u64 start)
{
    struct ksvmm_ctx *ctx = NULL;
    int ret;

    ctx = ksvmm_ctx_get(udevid, tgid);
    if (ctx == NULL) {
        svm_err("Invalid dev task. (udevid=%u; tgid=%d)\n", udevid, tgid);
        return -EINVAL;
    }

    ret = _ksvmm_del_seg(ctx, start);
    ksvmm_ctx_put(ctx);

    return ret;
}

int ksvmm_get_seg(u32 udevid, int tgid, u64 va, u64 *start, struct svm_global_va *src_info)
{
    struct ksvmm_ctx *ctx = NULL;
    int ret;

    ctx = ksvmm_ctx_get(udevid, tgid);
    if (ctx == NULL) {
        return -EINVAL;
    }

    ret = _ksvmm_get_seg(ctx, va, start, src_info);
    ksvmm_ctx_put(ctx);

    return ret;
}

int ksvmm_check_range(u32 udevid, int tgid, u64 va, u64 size)
{
    unsigned long stamp = (unsigned long)ka_jiffies;
    u64 check_va = va;
    u64 end = va + size;

    while (check_va < end) {
        struct svm_global_va src_info;
        u64 start;
        int ret = ksvmm_get_seg(udevid, tgid, check_va, &start, &src_info);
        if (ret != 0) {
            return ret;
        }

        check_va = start + src_info.size;
        ka_try_cond_resched(&stamp);
    }

    return 0;
}

int ksvmm_pin_seg(u32 udevid, int tgid, u64 va, u64 size)
{
    struct ksvmm_ctx *ksvmm_ctx = NULL;
    int ret;

    ksvmm_ctx = ksvmm_ctx_get(udevid, tgid);
    if (ksvmm_ctx == NULL) {
        svm_err("Invalid dev task. (udevid=%u; tgid=%d)\n", udevid, tgid);
        return -EINVAL;
    }

    ret = _ksvmm_pin_seg(ksvmm_ctx, va, size);
    ksvmm_ctx_put(ksvmm_ctx);

    return ret;
}

int ksvmm_unpin_seg(u32 udevid, int tgid, u64 va, u64 size)
{
    struct ksvmm_ctx *ctx = NULL;
    int ret;

    ctx = ksvmm_ctx_get(udevid, tgid);
    if (ctx == NULL) {
        svm_err("Invalid dev task. (udevid=%u; tgid=%d)\n", udevid, tgid);
        return -EINVAL;
    }

    ret = _ksvmm_unpin_seg(ctx, va, size);
    ksvmm_ctx_put(ctx);

    return ret;
}

void ksvmm_seg_show(struct ksvmm_ctx *ctx, ka_seq_file_t *seq)
{
    struct range_rbtree_node *range_node = NULL, *next = NULL;
    int i = 0;

    ka_task_down_read(&ctx->rw_sem);

    ka_fs_seq_printf(seq, "ksvmm: udevid %u tgid %d seg num %u\n", ctx->udevid, ctx->tgid, ctx->range_tree.node_num);

    ka_base_rbtree_postorder_for_each_entry_safe(range_node, next, &ctx->range_tree.root, node) {
        struct ksvmm_seg *seg = ka_container_of(range_node, struct ksvmm_seg, range_node);
        if (i == 0) {
            ka_fs_seq_printf(seq, "   %-5s %-17s %-15s %-5s\n", "id", "va", "size(Bytes)", "refcnt");
        }
        ka_fs_seq_printf(seq, "   %-5d 0x%-15llx %-15llu %-5llu\n",
            i++, range_node->start, range_node->size, (u64)ka_base_atomic64_read(&seg->refcnt));
    }

    ka_task_up_read(&ctx->rw_sem);
}

static int _ksvmm_seg_recycle(struct ksvmm_ctx *ctx)
{
    unsigned long stamp = (unsigned long)ka_jiffies;
    int recycle_num = 0;

    do {
        struct ksvmm_seg *seg = NULL;
        struct range_rbtree_node *range_node = NULL;

        ka_task_down_write(&ctx->rw_sem);
        range_node = range_rbtree_get_first(&ctx->range_tree);
        if (range_node == NULL) {
            ka_task_up_write(&ctx->rw_sem);
            break;
        }
        recycle_num++;
        seg = ka_container_of(range_node, struct ksvmm_seg, range_node);
        range_rbtree_erase(&ctx->range_tree, range_node);
        ka_task_up_write(&ctx->rw_sem);
        ksvmm_seg_destroy(seg);
        ka_try_cond_resched(&stamp);
    } while (1);

    return recycle_num;
}

void ksvmm_seg_recycle(struct ksvmm_ctx *ctx)
{
    int recycle_num = 0;

    recycle_num = _ksvmm_seg_recycle(ctx);
    if (recycle_num > 0) {
        svm_warn("Recycle mem. (udevid=%u; tgid=%d; recycle_num=%d)\n", ctx->udevid, ctx->tgid, recycle_num);
    }
}
