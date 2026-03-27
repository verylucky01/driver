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
#include "ka_system_pub.h"
#include "ka_kernel_def_pub.h"
#include "ka_base_pub.h"
#include "ka_task_pub.h"
#include "ka_common_pub.h"
#include "ka_fs_pub.h"
#include "ka_sched_pub.h"

#include "svm_kern_log.h"
#include "svm_slab.h"
#include "smp_event.h"
#include "svm_smp.h"
#include "smp_ctx.h"
#include "smp_core.h"
#include "svm_smp.h"

static struct smp_mem_node *smp_mem_node_create(struct smp_ctx *smp_ctx, u64 start, u64 size, u32 flag)
{
    struct smp_mem_node *mem_node = svm_vzalloc(sizeof(*mem_node));
    if (mem_node == NULL) {
        svm_err("Alloc mem node failed. (udevid=%u; tgid=%d; start=%llx; size=%llx)\n",
            smp_ctx->udevid, smp_ctx->tgid, start, size);
        return NULL;
    }

    mem_node->range_node.start = start;
    mem_node->range_node.size = size;
    mem_node->flag = flag;
    mem_node->status = 1;
    ka_base_atomic_set(&mem_node->refcnt, 0);

    return mem_node;
}

static inline void smp_mem_node_destroy(struct smp_mem_node *mem_node)
{
    svm_vfree(mem_node);
}

static struct smp_mem_node *smp_mem_node_search(struct smp_ctx *smp_ctx, u64 va, u64 size)
{
    struct smp_mem_node *mem_node = NULL;
    struct range_rbtree_node *range_node = NULL;

    range_node = range_rbtree_search(&smp_ctx->range_tree, va, size);
    if (range_node != NULL) {
        mem_node = ka_container_of(range_node, struct smp_mem_node, range_node);
    }

    return mem_node;
}

static int smp_add_mem(struct smp_ctx *smp_ctx, u64 start, u64 size, u32 flag)
{
    struct smp_mem_node *mem_node = NULL;
    int ret;

    mem_node = smp_mem_node_create(smp_ctx, start, size, flag);
    if (mem_node == NULL) {
        return -ENOMEM;
    }

    ka_task_write_lock_bh(&smp_ctx->lock);
    ret = range_rbtree_insert(&smp_ctx->range_tree, &mem_node->range_node);
    ka_task_write_unlock_bh(&smp_ctx->lock);
    if (ret != 0) {
        smp_mem_node_destroy(mem_node);
        if (ret == -EEXIST) {
            return 0;
        }

        svm_err("Insert failed. (udevid=%u; tgid=%d; start=%llx; size=%llx)\n",
            smp_ctx->udevid, smp_ctx->tgid, start, size);
    }

    return ret;
}

int svm_smp_add_mem(u32 udevid, int tgid, u64 start, u64 size, u32 flag)
{
    struct smp_ctx *smp_ctx = NULL;
    int ret;

    smp_ctx = smp_ctx_get(udevid, tgid);
    if (smp_ctx == NULL) {
        svm_err("Invalid dev task. (udevid=%u; tgid=%d)\n", udevid, tgid);
        return -EINVAL;
    }

    ret = smp_add_mem(smp_ctx, start, size, flag);
    smp_ctx_put(smp_ctx);

    return ret;
}

static int smp_del_mem(struct smp_ctx *smp_ctx, u64 start)
{
    struct smp_mem_node *mem_node = NULL;

    ka_task_write_lock_bh(&smp_ctx->lock);

    mem_node = smp_mem_node_search(smp_ctx, start, 1);
    if (mem_node == NULL) {
        ka_task_write_unlock_bh(&smp_ctx->lock);
        svm_err("Search failed. (udevid=%u; tgid=%d; start=0x%llx)\n", smp_ctx->udevid, smp_ctx->tgid, start);
        return -EINVAL;
    }

    if (mem_node->range_node.start != start) {
        ka_task_write_unlock_bh(&smp_ctx->lock);
        svm_err("Not add addr. (udevid=%u; tgid=%d; start=0x%llx)\n", smp_ctx->udevid, smp_ctx->tgid, start);
        return -EINVAL;
    }

    mem_node->status = 0;
    if (ka_base_atomic_read(&mem_node->refcnt) > 0) {
        ka_task_write_unlock_bh(&smp_ctx->lock);
        return -EBUSY;
    }

    range_rbtree_erase(&smp_ctx->range_tree, &mem_node->range_node);

    ka_task_write_unlock_bh(&smp_ctx->lock);

    smp_mem_node_destroy(mem_node);

    return 0;
}

int svm_smp_del_mem(u32 udevid, int tgid, u64 start)
{
    struct smp_ctx *smp_ctx = NULL;
    int ret;

    smp_ctx = smp_ctx_get(udevid, tgid);
    if (smp_ctx == NULL) {
        svm_err("Invalid dev task. (udevid=%u; tgid=%d)\n", udevid, tgid);
        return -EINVAL;
    }

    ret = smp_del_mem(smp_ctx, start);
    smp_ctx_put(smp_ctx);

    return ret;
}

static int smp_pin_mem(struct smp_ctx *smp_ctx, u64 va, u64 size, bool is_dev_cp_only)
{
    struct smp_mem_node *mem_node = NULL;
    int ret = 0;

    ka_task_read_lock_bh(&smp_ctx->lock);

    mem_node = smp_mem_node_search(smp_ctx, va, size);
    if (mem_node == NULL) {
        ka_task_read_unlock_bh(&smp_ctx->lock);
        return -EINVAL;
    }
    if (((mem_node->flag & SVM_SMP_FLAG_DEV_CP_ONLY) != 0) != is_dev_cp_only) {
        ret = -EINVAL;
    } else if (mem_node->status == 0) {
        ret = -EOWNERDEAD;
    } else {
        ka_base_atomic_inc(&mem_node->refcnt);
    }

    ka_task_read_unlock_bh(&smp_ctx->lock);

    return ret;
}

static int _svm_smp_pin_mem(u32 udevid, int tgid, u64 va, u64 size, bool is_dev_cp_only)
{
    struct smp_ctx *smp_ctx = NULL;
    int ret;

    smp_ctx = smp_ctx_get(udevid, tgid);
    if (smp_ctx == NULL) {
        return -ESRCH;
    }

    ret = smp_pin_mem(smp_ctx, va, size, is_dev_cp_only);
    smp_ctx_put(smp_ctx);

    return ret;
}

int svm_smp_pin_mem(u32 udevid, int tgid, u64 va, u64 size)
{
    return _svm_smp_pin_mem(udevid, tgid, va, size, false);
}
KA_EXPORT_SYMBOL_GPL(svm_smp_pin_mem);

int svm_smp_pin_dev_cp_only_mem(u32 udevid, int tgid, u64 va, u64 size)
{
    return _svm_smp_pin_mem(udevid, tgid, va, size, true);
}
KA_EXPORT_SYMBOL_GPL(svm_smp_pin_dev_cp_only_mem);

static int smp_unpin_mem(struct smp_ctx *smp_ctx, u64 va, u64 size, bool is_dev_cp_only)
{
    struct smp_mem_node *mem_node = NULL;
    int refcnt, trigger_event = 0;
    u64 range_start, range_size;

    ka_task_read_lock_bh(&smp_ctx->lock);

    mem_node = smp_mem_node_search(smp_ctx, va, size);
    if (mem_node == NULL) {
        ka_task_read_unlock_bh(&smp_ctx->lock);
        svm_err("Search failed. (udevid=%u; tgid=%d; va=%llx; size=%llx)\n",
            smp_ctx->udevid, smp_ctx->tgid, va, size);
        return -EINVAL;
    }

    if (((mem_node->flag & SVM_SMP_FLAG_DEV_CP_ONLY) != 0) != is_dev_cp_only) {
        ka_task_read_unlock_bh(&smp_ctx->lock);
        return -EINVAL;
    }

    refcnt = ka_base_atomic_dec_return(&mem_node->refcnt);
    if (refcnt < 0) {
        ka_base_atomic_inc(&mem_node->refcnt); /* restore refcnt, hold read lock, del can not access refcnt same time */
        ka_task_read_unlock_bh(&smp_ctx->lock);
        svm_err("No pin, can not unpin. (udevid=%u; tgid=%d; va=%llx; size=%llx; refcnt=%d)\n",
            smp_ctx->udevid, smp_ctx->tgid, va, size, refcnt);
        return -EINVAL;
    } else if ((refcnt == 0) && (mem_node->status == 0)) {
        trigger_event = 1;
    }

    range_start = mem_node->range_node.start;
    range_size = mem_node->range_node.size;
    ka_task_read_unlock_bh(&smp_ctx->lock);

    if (trigger_event == 1) {
        smp_trigger_event(smp_ctx->udevid, smp_ctx->tgid, range_start, range_size);
    }

    return 0;
}

static int _svm_smp_unpin_mem(u32 udevid, int tgid, u64 va, u64 size, bool is_dev_cp_only)
{
    struct smp_ctx *smp_ctx = NULL;
    int ret;

    smp_ctx = smp_ctx_get(udevid, tgid);
    if (smp_ctx == NULL) {
        svm_err("Invalid dev task. (udevid=%u; tgid=%d)\n", udevid, tgid);
        return -EINVAL;
    }

    ret = smp_unpin_mem(smp_ctx, va, size, is_dev_cp_only);
    smp_ctx_put(smp_ctx);

    return ret;
}

int svm_smp_unpin_mem(u32 udevid, int tgid, u64 va, u64 size)
{
    return _svm_smp_unpin_mem(udevid, tgid, va, size, false);
}
KA_EXPORT_SYMBOL_GPL(svm_smp_unpin_mem);

int svm_smp_unpin_dev_cp_only_mem(u32 udevid, int tgid, u64 va, u64 size)
{
    return _svm_smp_unpin_mem(udevid, tgid, va, size, true);
}
KA_EXPORT_SYMBOL_GPL(svm_smp_unpin_dev_cp_only_mem);

static int smp_check_mem(struct smp_ctx *smp_ctx, u64 va, u64 size, bool is_dev_cp_only)
{
    struct smp_mem_node *mem_node = NULL;
    int ret = 0;

    ka_task_read_lock_bh(&smp_ctx->lock);

    mem_node = smp_mem_node_search(smp_ctx, va, size);
    if (mem_node == NULL) {
        ka_task_read_unlock_bh(&smp_ctx->lock);
        return -EINVAL;
    }

    if (((mem_node->flag & SVM_SMP_FLAG_DEV_CP_ONLY) != 0) != is_dev_cp_only) {
        svm_err("Flag not match. (flag=%u; is_dev_cp_only=%d)\n", mem_node->flag, is_dev_cp_only);
        ret = -EINVAL;
    } else if (mem_node->status == 0) {
        ret = -EOWNERDEAD;
    } else {
        ret = (ka_base_atomic_read(&mem_node->refcnt) > 0) ? -EBUSY : 0;
    }

    ka_task_read_unlock_bh(&smp_ctx->lock);

    return ret;
}

static int _svm_smp_check_mem(u32 udevid, int tgid, u64 va, u64 size, bool is_dev_cp_only)
{
    struct smp_ctx *smp_ctx = NULL;
    int ret;

    smp_ctx = smp_ctx_get(udevid, tgid);
    if (smp_ctx == NULL) {
        return -EINVAL;
    }

    ret = smp_check_mem(smp_ctx, va, size, is_dev_cp_only);
    smp_ctx_put(smp_ctx);

    return ret;
}

int svm_smp_check_mem(u32 udevid, int tgid, u64 va, u64 size)
{
    return _svm_smp_check_mem(udevid, tgid, va, size, false);
}

int svm_smp_check_dev_cp_only_mem(u32 udevid, int tgid, u64 va, u64 size)
{
    return _svm_smp_check_mem(udevid, tgid, va, size, true);
}

void smp_mem_show(struct smp_ctx *smp_ctx, ka_seq_file_t *seq)
{
    struct range_rbtree_node *range_node, *next;
    int i = 0;

    ka_task_read_lock_bh(&smp_ctx->lock);

    ka_fs_seq_printf(seq, "smp: udevid %u tgid %d mem num %u\n", smp_ctx->udevid, smp_ctx->tgid, smp_ctx->range_tree.node_num);

    ka_base_rbtree_postorder_for_each_entry_safe(range_node, next, &smp_ctx->range_tree.root, node) {
        struct smp_mem_node *mem_node = ka_container_of(range_node, struct smp_mem_node, range_node);
        if (i == 0) {
            ka_fs_seq_printf(seq, "   index   va      size     status   refcnt  flag\n");
        }
        ka_fs_seq_printf(seq, "   %d       %llx     %llx     %d  %d   %u\n",
            i++, range_node->start, range_node->size, mem_node->status, ka_base_atomic_read(&mem_node->refcnt), mem_node->flag);
    }

    ka_task_read_unlock_bh(&smp_ctx->lock);
}

static int _smp_mem_recycle(struct smp_ctx *smp_ctx)
{
    unsigned long stamp = (unsigned long)ka_jiffies;
    int recycle_num = 0;

    do {
        struct smp_mem_node *mem_node = NULL;
        struct range_rbtree_node *range_node = NULL;

        ka_task_write_lock_bh(&smp_ctx->lock);
        range_node = range_rbtree_get_first(&smp_ctx->range_tree);
        if (range_node == NULL) {
            ka_task_write_unlock_bh(&smp_ctx->lock);
            break;
        }
        recycle_num++;
        mem_node = ka_container_of(range_node, struct smp_mem_node, range_node);
        range_rbtree_erase(&smp_ctx->range_tree, range_node);
        ka_task_write_unlock_bh(&smp_ctx->lock);
        smp_mem_node_destroy(mem_node);
        ka_try_cond_resched(&stamp);
    } while (1);

    return recycle_num;
}

void smp_mem_recycle(struct smp_ctx *smp_ctx)
{
    int recycle_num = 0;

    recycle_num = _smp_mem_recycle(smp_ctx);
    if (recycle_num > 0) {
        svm_warn("Recycle mem. (udevid=%u; tgid=%d; recycle_num=%d)\n", smp_ctx->udevid, smp_ctx->tgid, recycle_num);
    }
}
