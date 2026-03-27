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
#include "ka_list_pub.h"
#include "ka_compiler_pub.h"

#include "comm_kernel_interface.h"

#include "svm_slab.h"
#include "copy_task.h"
#include "dma_desc_node.h"

#ifndef EMU_ST /* Simulation ST is required and cannot be deleted. */
#define SVM_DMA_DESC_ALLOC_SQ_CQ_ADDR_RETRY_CNT 3U
#else
#define SVM_DMA_DESC_ALLOC_SQ_CQ_ADDR_RETRY_CNT 1U
#endif

static char *dma_desc_maigc = "SVM";

static int dma_desc_alloc_sq_cq_addr(struct svm_copy_task *copy_task,
    struct devdrv_dma_prepare *dma_prepare, u64 *fixed_dma_node_num)
{
    u32 node_cnt = svm_copy_task_get_dma_node_num(copy_task);
    u32 i, num;
    int ret;

    for (i = 0; i < SVM_DMA_DESC_ALLOC_SQ_CQ_ADDR_RETRY_CNT; i++) {
        num = node_cnt >> i;

        if (num == 0) {
            return -ENOMEM;
        }

        ret = devdrv_dma_prepare_alloc_sq_addr(copy_task->udevid, num, dma_prepare);
        if (ret == 0) {
            *fixed_dma_node_num = num;
            return 0;
        }
        svm_warn("Retry alloc sq&cq address. (i=%u; cnt=%u)\n", i, num);
    }

    return -ENOMEM;
}

static void dma_desc_free_sq_cq_addr(struct svm_copy_task *copy_task,
    struct devdrv_dma_prepare *dma_prepare)
{
    devdrv_dma_prepare_free_sq_addr(copy_task->udevid, dma_prepare);
}

static u64 get_dma_nodes_addr_size(struct devdrv_dma_node *dma_nodes, u64 dma_node_num)
{
    u64 i, size;

    for (i = 0, size = 0; i < dma_node_num; i++) {
        size += dma_nodes[i].size;
    }

    return size;
}

static int fill_dma_desc_of_sq(struct svm_copy_task *copy_task,
    struct devdrv_dma_prepare *dma_prepare, u64 fixed_dma_node_num, u64 *fixed_size)
{
    struct svm_copy_subtask *subtask = NULL;
    struct svm_copy_subtask *n = NULL;
    u32 fill_status, sq_desc_size, cq_desc_size;
    u64 cur_num, filled_num = 0, size = 0;
    void *sq_base = NULL;
    int ret;

    ret = devdrv_dma_get_sq_cq_desc_size(copy_task->udevid, &sq_desc_size, &cq_desc_size);
    if (ret != 0) {
        svm_err("devdrv_dma_get_sq_cq_desc_size failed. (ret=%d; udevid=%u)\n", ret, copy_task->udevid);
        return ret;
    }

    ka_list_for_each_entry_safe(subtask, n, &copy_task->subtasks_list.head, node) {
        sq_base = dma_prepare->sq_base + filled_num * sq_desc_size;
        cur_num = ka_base_min(fixed_dma_node_num - filled_num, subtask->dma_node_num);
        fill_status = ((filled_num + cur_num) == fixed_dma_node_num) ?
            DEVDRV_DMA_DESC_FILL_FINISH : DEVDRV_DMA_DESC_FILL_CONTINUE;
        ret = devdrv_dma_fill_desc_of_sq_ext(copy_task->udevid, sq_base, subtask->dma_nodes, cur_num, fill_status);
        if (ret != 0) {
            svm_err("devdrv_dma_fill_desc_of_sq_ext failed. (ret=%d; udevid=%u)\n", ret, copy_task->udevid);
            return ret;
        }
        filled_num += cur_num;
        size += get_dma_nodes_addr_size(subtask->dma_nodes, cur_num);

        if (fill_status == DEVDRV_DMA_DESC_FILL_FINISH) {
            break;
        }
    }

    *fixed_size = size;
    return 0;
}

static int dma_prepare_init(struct svm_copy_task *copy_task, struct devdrv_dma_prepare *dma_prepare,
    u64 *fixed_dma_node_num, u64 *fixed_size)
{
    int ret;

    dma_prepare->devid = copy_task->udevid;

    ret = dma_desc_alloc_sq_cq_addr(copy_task, dma_prepare, fixed_dma_node_num);
    if (ret != 0) {
        svm_err("Alloc sq cq addr failed. (ret=%d)\n", ret);
        return ret;
    }

    ret = fill_dma_desc_of_sq(copy_task, dma_prepare, *fixed_dma_node_num, fixed_size);
    if (ret != 0) {
        svm_err("Fill dma desc of sq failed. (ret=%d)\n", ret);
        dma_desc_free_sq_cq_addr(copy_task, dma_prepare);
    }

    return ret;
}

static void dma_prepare_uninit(struct svm_copy_task *copy_task, struct devdrv_dma_prepare *dma_prepare)
{
    dma_desc_free_sq_cq_addr(copy_task, dma_prepare);
}

static void fill_convert_dma_addr(struct dma_desc_node *node, struct DMA_ADDR *dma_desc)
{
    /* dma_desc->virt_id in user */
    dma_desc->fixed_size = node->fixed_size;
    dma_desc->phyAddr.src = (void *)(uintptr_t)node->dma_prepare.sq_dma_addr;
    dma_desc->phyAddr.dst = (void *)(uintptr_t)node->dma_prepare.cq_dma_addr;
    dma_desc->phyAddr.len = node->fixed_dma_node_num;
    dma_desc->phyAddr.flag = 1;
    dma_desc->phyAddr.priv = (void *)(uintptr_t)node->handle;
}

int dma_desc_node_create(struct dma_desc_ctx *ctx,
    struct svm_copy_task *copy_task, struct DMA_ADDR *dma_desc)
{
    struct dma_desc_node *node = NULL;
    int ret;

    node = svm_kvmalloc(sizeof(struct dma_desc_node), KA_GFP_KERNEL | __KA_GFP_ACCOUNT);
    if (node == NULL) {
        svm_err("svm_kvmalloc failed. (size=%lu)\n", sizeof(struct dma_desc_node));
        return -ENOMEM;
    }

    KA_BASE_RB_CLEAR_NODE(&node->node.node);
    ka_base_kref_init(&node->ref);
    ka_task_rwlock_init(&node->rwlock);
    node->state = DMA_DESC_NODE_IDLE;
    node->copy_task = copy_task;

    ret = dma_prepare_init(copy_task, &node->dma_prepare, &node->fixed_dma_node_num, &node->fixed_size);
    if (ret != 0) {
        svm_kvfree(node);
        return ret;
    }

    node->handle = (u64)(uintptr_t)node + (u64)(uintptr_t)dma_desc_maigc;
    fill_convert_dma_addr(node, dma_desc);
    node->node.start = node->handle;
    node->node.size = 1;

    ka_task_down_write(&ctx->rw_sem);
    ret = range_rbtree_insert(&ctx->root, &node->node);
    ka_task_up_write(&ctx->rw_sem);
    if (ka_unlikely(ret != 0)) {
        svm_err("Insert dma desc node failed. (ret=%d)\n", ret);
        dma_prepare_uninit(copy_task, &node->dma_prepare);
        svm_kvfree(node);
    }

    return ret;
}

static void dma_desc_node_release(ka_kref_t *kref)
{
    struct dma_desc_node *node = ka_container_of(kref, struct dma_desc_node, ref);

    svm_kvfree(node);
}

static void _dma_desc_node_destroy(struct dma_desc_node *node)
{
    dma_prepare_uninit(node->copy_task, &node->dma_prepare);
    ka_base_kref_put(&node->ref, dma_desc_node_release);
}

void dma_desc_node_destroy(struct dma_desc_ctx *ctx, struct dma_desc_node *node)
{
    ka_task_down_write(&ctx->rw_sem);
    if (KA_BASE_RB_EMPTY_NODE(&node->node.node) == false) {
        range_rbtree_erase(&ctx->root, &node->node);
    }
    ka_task_up_write(&ctx->rw_sem);

    _dma_desc_node_destroy(node);
}

static void _dma_desc_node_get(struct dma_desc_node *node)
{
    ka_base_kref_get(&node->ref);
}

struct dma_desc_node *dma_desc_node_get(struct dma_desc_ctx *ctx, u64 handle)
{
    struct dma_desc_node *dma_desc_node = NULL;
    struct range_rbtree_node *range_node = NULL;

    ka_task_down_read(&ctx->rw_sem);
    range_node = range_rbtree_search(&ctx->root, handle, 1);
    if (range_node != NULL) {
        dma_desc_node = ka_container_of(range_node, struct dma_desc_node, node);
        _dma_desc_node_get(dma_desc_node);
    }
    ka_task_up_read(&ctx->rw_sem);
    return dma_desc_node;
}

void dma_desc_node_put(struct dma_desc_node *node)
{
    ka_base_kref_put(&node->ref, dma_desc_node_release);
}

int dma_desc_node_state_trans(struct dma_desc_node *node, int src_state, int dst_state)
{
    int ret = -EBUSY;

    ka_task_write_lock(&node->rwlock);
    if (node->state == src_state) {
        node->state = dst_state;
        ret = 0;
    }
    ka_task_write_unlock(&node->rwlock);
    return ret;
}
