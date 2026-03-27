/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <pthread.h>
#include <unistd.h>
#include "ascend_hal_error.h"
#include "queue_kernel_api.h"
#include "que_urma.h"
#include "que_comm_agent.h"
#include "que_compiler.h"
#include "que_mem_merge.h"

static struct que_mem_node *rbtree_can_insert_range_ex(struct rbtree_root *root, struct rb_range_handle *range,
    rb_range_handle_func get_range)
{
    struct rbtree_node *rb_node = NULL;
    struct rbtree_node **tmp = &(root->rbtree_node);
#ifndef COMPILE_UT  /* device_monitor's UT macro */
    while (*tmp != NULL) {
        struct rb_range_handle tmp_range;

        get_range(*tmp, &tmp_range);
        if (range->end < tmp_range.start) {
            tmp = &((*tmp)->rbtree_left);
        } else if (range->start > tmp_range.end) {
            tmp = &((*tmp)->rbtree_right);
        } else {
            rb_node = *tmp;
            return rb_entry(rb_node, struct que_mem_node, node);
        }
    }
#endif
    return NULL;
}
static void que_rb_range_handle_of_mem(struct rbtree_node *node, struct rb_range_handle *range_handle)
{
    struct que_mem_node *mem_node = rb_entry(node, struct que_mem_node, node);

    range_handle->start = mem_node->va;
    range_handle->end = mem_node->va + mem_node->size - 1;
}

static int que_mem_new_node_insert(struct que_mem_merge_ctx *mem_ctx, struct que_mem_node *new_node)
{
    int ret;

    ret = rbtree_insert_by_range(&mem_ctx->rb_root, &new_node->node, que_rb_range_handle_of_mem);
    return (ret != 0) ? DRV_ERROR_BUSY : DRV_ERROR_NONE;
}

static struct que_mem_node *que_mem_find_node(struct que_mem_merge_ctx *mem_ctx, unsigned long long va, unsigned long long size)
{
    struct rbtree_node *rb_node = NULL;
    struct rb_range_handle range = {.start = va, .end = va + size - 1};

    rb_node = rbtree_search_by_range(&mem_ctx->rb_root, &range, que_rb_range_handle_of_mem);
    if (que_unlikely(rb_node == NULL)) {
        return NULL;
    }

    return rb_entry(rb_node, struct que_mem_node, node);
}

static struct que_mem_node *que_mem_node_erase(struct que_mem_merge_ctx *mem_ctx, unsigned long long va)
{
    struct que_mem_node *mem_node = NULL;

    mem_node = que_mem_find_node(mem_ctx, va, 1);
    if (que_unlikely(mem_node == NULL)) {
        return NULL;
    }

    _rbtree_erase(&mem_ctx->rb_root, &mem_node->node);
    return mem_node;
}

static struct que_mem_node *que_exist_mem_conflict(struct que_mem_merge_ctx *mem_ctx, unsigned long long va, unsigned long long size)
{
    struct rb_range_handle range_handle = {.start = va, .end = va + size - 1};

    return rbtree_can_insert_range_ex(&mem_ctx->rb_root, &range_handle, que_rb_range_handle_of_mem);
}

static void que_mem_range_merge(struct que_mem_node *mem_node, unsigned long long *va_new, unsigned long long *size_new)
{
    unsigned long long new_end_addr = *va_new + *size_new;
    unsigned long long curr_end_addr = mem_node->va + mem_node->size;

    *va_new = ((mem_node->va > *va_new) ? *va_new : mem_node->va);
    *size_new = ((curr_end_addr > new_end_addr) ? (curr_end_addr - *va_new) : (new_end_addr - *va_new));
}

static struct que_mem_node *que_mem_node_alloc(unsigned long long va, unsigned long long size)
{
    struct que_mem_node *mem_node = NULL;

    mem_node = (struct que_mem_node *)malloc(sizeof(struct que_mem_node));
    if (que_unlikely(mem_node == NULL)) {
        QUEUE_LOG_ERR("malloc new node failed. (size=%llu)\n", sizeof(struct que_mem_node));
        return NULL;
    }

    RB_CLEAR_NODE(&mem_node->node);
    mem_node->va = va;
    mem_node->size = size;
    mem_node->tseg = NULL;

    return mem_node;
}
static void que_mem_node_free(struct que_mem_node *mem_node)
{
    free(mem_node);
}

void que_mem_erase_all_node(struct que_mem_merge_ctx *mem_ctx)
{
    struct rbtree_node *rb_node = NULL;
    struct que_mem_node *mem_node = NULL;

    (void)pthread_rwlock_wrlock(&mem_ctx->rwlock);
    if (RB_EMPTY_ROOT(&mem_ctx->rb_root) == true) {
        goto erase_finish;
    }
    rb_node = rbtree_erase_one_node(&mem_ctx->rb_root);
    while (rb_node != NULL) {
        mem_node = rb_entry(rb_node, struct que_mem_node, node);
        if (que_unlikely(mem_node->tseg != NULL)) {
            if (que_is_share_mem(mem_node->va) == false) {
                que_seg_destroy(mem_node->tseg);
            }  
        }
        que_mem_node_free(mem_node);
        rb_node = rbtree_erase_one_node(&mem_ctx->rb_root);
    }

erase_finish:
    (void)pthread_rwlock_unlock(&mem_ctx->rwlock);
    return;
}

int que_mem_merge(struct que_mem_merge_ctx *mem_ctx, unsigned int devid, unsigned long long va, unsigned long long size)
{
    struct que_mem_node *mem_node = NULL;
    struct que_mem_node *confict_node = NULL;
    struct que_mem_node *new_mem_node = NULL;
    unsigned long long aligned_va = que_align_down(va, (size_t)getpagesize());
    unsigned long long aligned_size = que_align_up(va + size, (size_t)getpagesize()) - aligned_va;
    unsigned long long judge_va = aligned_va - (size_t)getpagesize();
    unsigned long long judge_size = aligned_size + (size_t)getpagesize() + 1;
    int ret;

    (void)pthread_rwlock_wrlock(&mem_ctx->rwlock);
    /* if mem region overlap or continuous, then merge node */
    confict_node = que_exist_mem_conflict(mem_ctx, judge_va, judge_size);
    while (confict_node != NULL) {
        mem_node = que_mem_node_erase(mem_ctx, confict_node->va);
        if (que_unlikely(mem_node == NULL)) {
            (void)pthread_rwlock_unlock(&mem_ctx->rwlock);
            QUEUE_LOG_ERR("merge mem node fail. (devid=%u; va=0x%llx; size=%llu)\n", devid, va, size);
            return DRV_ERROR_INNER_ERR;
        }
        que_mem_range_merge(mem_node, &aligned_va, &aligned_size);
        que_mem_node_free(mem_node);

        judge_va = aligned_va - (size_t)getpagesize();
        judge_size = aligned_size + (size_t)getpagesize() + 1;
        confict_node = que_exist_mem_conflict(mem_ctx, judge_va, judge_size);
    }

    /* insert merged node */
    new_mem_node = que_mem_node_alloc(aligned_va, aligned_size);
    if (que_unlikely(new_mem_node == NULL)) {
        (void)pthread_rwlock_unlock(&mem_ctx->rwlock);
        QUEUE_LOG_ERR("malloc new node failed. (size=%llu)\n", sizeof(struct que_mem_node));
        return DRV_ERROR_OUT_OF_MEMORY;
    }
    ret = que_mem_new_node_insert(mem_ctx, new_mem_node);
    if (que_unlikely(ret != DRV_ERROR_NONE)) {
        (void)pthread_rwlock_unlock(&mem_ctx->rwlock);
        que_mem_node_free(new_mem_node);
        QUEUE_LOG_ERR("insert mem node fail. (devid=%u; va=0x%llx; size=%llu)\n", devid, va, size);
        return ret;
    }

    (void)pthread_rwlock_unlock(&mem_ctx->rwlock);
    return DRV_ERROR_NONE;
}

urma_target_seg_t *que_mem_seg_register(struct que_mem_merge_ctx *mem_ctx, unsigned int d2d_flag, struct que_urma_token *token,
    unsigned int pin_flg, unsigned int devid, unsigned long long va, unsigned int access)
{
    urma_target_seg_t *tseg = NULL;
    struct que_mem_node *mem_node = NULL;

    (void)pthread_rwlock_wrlock(&mem_ctx->rwlock);
    mem_node = que_mem_find_node(mem_ctx, va, 1);
    if (que_unlikely(mem_node == NULL)) {
        (void)pthread_rwlock_unlock(&mem_ctx->rwlock);
        return NULL;
    }

    tseg = mem_node->tseg;
    if (que_unlikely(tseg != NULL)) {
        (void)pthread_rwlock_unlock(&mem_ctx->rwlock);
        return tseg;
    }

    if (que_is_share_mem(mem_node->va) == true) {
        tseg = que_get_urma_ctx_tseg(devid, d2d_flag);
    } else {
        if (pin_flg == 0) {
            tseg = que_nonpin_seg_create(devid, mem_node->va, mem_node->size, access, token, d2d_flag);
        } else {
            tseg = que_pin_seg_create(devid, mem_node->va, mem_node->size, access, token, d2d_flag);
        }
    }
    
    if (que_unlikely(tseg == NULL)) {
        (void)pthread_rwlock_unlock(&mem_ctx->rwlock);
        QUEUE_LOG_ERR("que mem seg fail. (devid=%u; va=0x%llx)\n", devid, va);
        return NULL;
    }

    mem_node->tseg = tseg;
    (void)pthread_rwlock_unlock(&mem_ctx->rwlock);

    return tseg;
}

void que_mem_seg_unregister(struct que_mem_merge_ctx *mem_ctx, unsigned int devid, unsigned long long va)
{
    struct que_mem_node *mem_node = NULL;

    (void)pthread_rwlock_wrlock(&mem_ctx->rwlock);
    mem_node = que_mem_node_erase(mem_ctx, va);
    if (que_unlikely(mem_node == NULL)) {
        (void)pthread_rwlock_unlock(&mem_ctx->rwlock);
        return;
    }

    if (que_unlikely(mem_node->tseg != NULL)) {
        if (que_is_share_mem(mem_node->va) == false) {
            que_seg_destroy(mem_node->tseg);
        }
    }

    que_mem_node_free(mem_node);
    (void)pthread_rwlock_unlock(&mem_ctx->rwlock);
    return;
}

void que_mem_ctx_init(struct que_mem_merge_ctx *mem_ctx)
{
    (void)pthread_rwlock_init(&mem_ctx->rwlock, NULL);
    rbtree_init(&mem_ctx->rb_root);
}

