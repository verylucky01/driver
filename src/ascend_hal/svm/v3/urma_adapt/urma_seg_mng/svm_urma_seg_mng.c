/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include <stdint.h>
#include <stdatomic.h>
#include <pthread.h>

#include "rbtree.h"
#include "ascend_hal.h"

#include "svm_urma_def.h"
#include "svm_pub.h"
#include "svm_addr_desc.h"
#include "svm_log.h"
#include "svm_user_adapt.h"
#include "svm_dbi.h"
#include "svm_urma_seg_register_client.h"
#include "svm_urma_seg_mng.h"

struct seg_node_mng {
    pthread_rwlock_t rwlock;
    struct rbtree_root rb_root;
};

struct svm_urma_seg_node {
    struct rbtree_node node;
    u64 va;
    u64 size;

    u32 seg_flag;
    struct svm_urma_client_seg client_seg;
};

struct seg_node_mng g_local_seg_mng[SVM_MAX_AGENT_NUM];
struct seg_node_mng g_remote_seg_mng[SVM_MAX_AGENT_NUM];

static void rb_range_handle_of_urma_seg_node(struct rbtree_node *node, struct rb_range_handle *range_handle)
{
    struct svm_urma_seg_node *seg_node = rb_entry(node, struct svm_urma_seg_node, node);

    range_handle->start = seg_node->va;
    range_handle->end = seg_node->va + seg_node->size - 1;
}

static struct svm_urma_seg_node *svm_get_urma_seg_node(struct seg_node_mng *mng, u64 va, u64 size)
{
    struct rbtree_node *rb_node = NULL;
    struct rb_range_handle range = {.start = va, .end = va + size - 1};

    rb_node = rbtree_search_by_range(&mng->rb_root, &range, rb_range_handle_of_urma_seg_node);
    if (rb_node == NULL) {
        return NULL;
    }

    return rb_entry(rb_node, struct svm_urma_seg_node, node);
}

static int svm_urma_seg_node_insert(struct seg_node_mng *mng, struct svm_urma_seg_node *seg_node)
{
    int ret;

    (void)pthread_rwlock_wrlock(&mng->rwlock);
    ret = rbtree_insert_by_range(&mng->rb_root, &seg_node->node, rb_range_handle_of_urma_seg_node);
    (void)pthread_rwlock_unlock(&mng->rwlock);
    return (ret != 0) ? DRV_ERROR_INVALID_VALUE : DRV_ERROR_NONE;
}

static int _svm_urma_seg_node_erase(struct seg_node_mng *mng, u64 va, u64 size, struct svm_urma_seg_node **seg_node_out)
{
    struct svm_urma_seg_node *seg_node = NULL;

    seg_node = svm_get_urma_seg_node(mng, va, 1);
    if (seg_node == NULL) {
        return DRV_ERROR_INVALID_VALUE;
    }

    if (va != seg_node->va) {
        svm_err("Va is not first addr. (va=0x%llx; first_va=0x%llx)\n", va, seg_node->va);
        return DRV_ERROR_INVALID_VALUE;
    }

    if (size != seg_node->size) {
        svm_err("Size is not match. (size=0x%llx; node.size=0x%llx)\n", size, seg_node->size);
        return DRV_ERROR_INVALID_VALUE;
    }

    _rbtree_erase(&mng->rb_root, &seg_node->node);
    *seg_node_out = seg_node;
    return DRV_ERROR_NONE;
}

static int svm_urma_seg_node_erase(struct seg_node_mng *mng, u64 va, u64 size, struct svm_urma_seg_node **seg_node)
{
    int ret;

    (void)pthread_rwlock_wrlock(&mng->rwlock);
    ret = _svm_urma_seg_node_erase(mng, va, size, seg_node);
    (void)pthread_rwlock_unlock(&mng->rwlock);
    return ret;
}

static int _svm_urma_insert_client_seg(struct seg_node_mng *mng, u64 va, u64 size,
    struct svm_urma_client_seg *client_seg, u32 seg_flag)
{
    struct svm_urma_seg_node *node = NULL;
    int ret;

    node = svm_ua_calloc(1, sizeof(struct svm_urma_seg_node));
    if (node == NULL) {
        svm_err("Calloc failed. (size=%llu)\n", sizeof(struct svm_urma_seg_node));
        return DRV_ERROR_OUT_OF_MEMORY;
    }

    RB_CLEAR_NODE(&node->node);
    node->va = va;
    node->size = size;
    node->seg_flag = seg_flag;
    node->client_seg = *client_seg;

    ret = svm_urma_seg_node_insert(mng, node);
    if (ret != DRV_ERROR_NONE) {
        svm_err("Insert seg node failed. (ret=%d)\n", ret);
        free(node);
    }

    return ret;
}

static int _svm_urma_erase_client_seg(struct seg_node_mng *mng, u64 va, u64 size,
    struct svm_urma_client_seg *client_seg)
{
    struct svm_urma_seg_node *node = NULL;
    int ret;

    ret = svm_urma_seg_node_erase(mng, va, size, &node);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    *client_seg = node->client_seg;
    free(node);
    return DRV_ERROR_NONE;
}

static int _svm_urma_get_seg_with_token_info(struct seg_node_mng *mng, u64 va, u64 size,
    urma_seg_t *seg, u32 *token_id, u32 *token_val)
{
    struct svm_urma_seg_node *node = NULL;

    (void)pthread_rwlock_rdlock(&mng->rwlock);
    node = svm_get_urma_seg_node(mng, va, size);
    if (node != NULL) {
        *seg = node->client_seg.seg;
        *token_id = node->client_seg.token_id;
        *token_val = node->client_seg.token_val;
        (void)pthread_rwlock_unlock(&mng->rwlock);
        return DRV_ERROR_NONE;
    }
    (void)pthread_rwlock_unlock(&mng->rwlock);

    return DRV_ERROR_INVALID_VALUE;
}

static int _svm_urma_get_tseg(struct seg_node_mng *mng, u64 va, u64 size, urma_target_seg_t **tseg)
{
    struct svm_urma_seg_node *node = NULL;

    (void)pthread_rwlock_rdlock(&mng->rwlock);
    node = svm_get_urma_seg_node(mng, va, size);
    if (node != NULL) {
        *tseg = node->client_seg.tseg;
        (void)pthread_rwlock_unlock(&mng->rwlock);
        return DRV_ERROR_NONE;
    }
    (void)pthread_rwlock_unlock(&mng->rwlock);

    return DRV_ERROR_INVALID_VALUE;
}

static int _svm_urma_get_token_id(struct seg_node_mng *mng, u64 va, u64 size, u32 *token_id, u32 *token_val)
{
    struct svm_urma_seg_node *node = NULL;

    (void)pthread_rwlock_rdlock(&mng->rwlock);
    node = svm_get_urma_seg_node(mng, va, size);
    if (node != NULL) {
        *token_id = node->client_seg.token_id;
        *token_val = node->client_seg.token_val;
        (void)pthread_rwlock_unlock(&mng->rwlock);
        return DRV_ERROR_NONE;
    }
    (void)pthread_rwlock_unlock(&mng->rwlock);

    return DRV_ERROR_INVALID_VALUE;
}

static int svm_urma_insert_client_local_seg(u32 devid, u64 va, u64 size,
    struct svm_urma_client_seg *client_seg, u32 seg_flag)
{
    return _svm_urma_insert_client_seg(&g_local_seg_mng[devid], va, size, client_seg, seg_flag);
}

static int svm_urma_erase_client_local_seg(u32 devid, u64 va, u64 size,
    struct svm_urma_client_seg *client_seg)
{
    return _svm_urma_erase_client_seg(&g_local_seg_mng[devid], va, size, client_seg);
}

static int svm_urma_get_local_seg_with_token_info(u32 devid, u64 va, u64 size,
    urma_seg_t *seg, u32 *token_id, u32 *token_val)
{
    return _svm_urma_get_seg_with_token_info(&g_local_seg_mng[devid], va, size, seg, token_id, token_val);
}

static int svm_urma_get_local_tseg(u32 devid, u64 va, u64 size, urma_target_seg_t **tseg)
{
    return _svm_urma_get_tseg(&g_local_seg_mng[devid], va, size, tseg);
}

static int svm_urma_get_local_seg_token_info(u32 devid, u64 va, u64 size, u32 *token_id, u32 *token_val)
{
    return _svm_urma_get_token_id(&g_local_seg_mng[devid], va, size, token_id, token_val);
}

static int svm_urma_insert_client_remote_seg(u32 devid, u64 va, u64 size,
    struct svm_urma_client_seg *client_seg, u32 seg_flag)
{
    return _svm_urma_insert_client_seg(&g_remote_seg_mng[devid], va, size, client_seg, seg_flag);
}

static int svm_urma_erase_client_remote_seg(u32 devid, u64 va, u64 size,
    struct svm_urma_client_seg *client_seg)
{
    return _svm_urma_erase_client_seg(&g_remote_seg_mng[devid], va, size, client_seg);
}

static int svm_urma_get_remote_seg_with_token_info(u32 devid, u64 va, u64 size,
    urma_seg_t *seg, u32 *token_id, u32 *token_val)
{
    return _svm_urma_get_seg_with_token_info(&g_remote_seg_mng[devid], va, size, seg, token_id, token_val);
}

static int svm_urma_get_remote_tseg(u32 devid, u64 va, u64 size, urma_target_seg_t **tseg)
{
    return _svm_urma_get_tseg(&g_remote_seg_mng[devid], va, size, tseg);
}

static int svm_urma_get_remote_seg_token_info(u32 devid, u64 va, u64 size, u32 *token_id, u32 *token_val)
{
    return _svm_urma_get_token_id(&g_remote_seg_mng[devid], va, size, token_id, token_val);
}

int svm_urma_register_seg(u32 user_devid, struct svm_dst_va *dst_va, u32 seg_flag)
{
    struct svm_urma_client_seg client_seg;
    int ret;

    ret = svm_urma_register_seg_client(user_devid, dst_va, &client_seg, seg_flag);
    if (ret != DRV_ERROR_NONE) {
        svm_debug("Client register urma seg check. (ret=%d; devid=%u; va=0x%llx; size=%llu)\n",
            ret, dst_va->devid, dst_va->va, dst_va->size);
        return ret;
    }

    if (!svm_urma_seg_flag_is_self_user(seg_flag)) {
        if (dst_va->devid == svm_get_host_devid()) {
            ret = svm_urma_insert_client_local_seg(user_devid, dst_va->va, dst_va->size, &client_seg, seg_flag);
        } else {
            ret = svm_urma_insert_client_remote_seg(dst_va->devid, dst_va->va, dst_va->size, &client_seg, seg_flag);
        }
        if (ret != DRV_ERROR_NONE) {
            (void)svm_urma_unregister_seg_client(user_devid, dst_va, &client_seg, seg_flag);
        }
    }

    return ret;
}

int svm_urma_unregister_seg(u32 user_devid, struct svm_dst_va *dst_va, u32 seg_flag)
{
    struct svm_urma_client_seg client_seg = {0};
    int ret;

    if (!svm_urma_seg_flag_is_self_user(seg_flag)) {
        if (dst_va->devid == svm_get_host_devid()) {
            ret = svm_urma_erase_client_local_seg(user_devid, dst_va->va, dst_va->size, &client_seg);
        } else {
            ret = svm_urma_erase_client_remote_seg(dst_va->devid, dst_va->va, dst_va->size, &client_seg);
        }
        if (ret != DRV_ERROR_NONE) {
            return ret;
        }
    }

    ret = svm_urma_unregister_seg_client(user_devid, dst_va, &client_seg, seg_flag);
    if (ret != DRV_ERROR_NONE) {
        svm_err("Client unregister urma seg failed. (ret=%d; devid=%u; va=0x%llx; size=%llu)\n",
            ret, dst_va->devid, dst_va->va, dst_va->size);
    }

    return ret;
}

int svm_urma_get_seg_with_token_info(u32 user_devid, struct svm_dst_va *dst_va,
    urma_seg_t *seg, u32 *token_id, u32 *token_val)
{
    if (dst_va->devid == svm_get_host_devid()) {
        return svm_urma_get_local_seg_with_token_info(user_devid, dst_va->va, dst_va->size,
            seg, token_id, token_val);
    } else {
        return svm_urma_get_remote_seg_with_token_info(dst_va->devid, dst_va->va, dst_va->size,
            seg, token_id, token_val);
    }
}

int svm_urma_get_tseg(u32 user_devid, struct svm_dst_va *dst_va, urma_target_seg_t **tseg)
{
    if (dst_va->devid == svm_get_host_devid()) {
        return svm_urma_get_local_tseg(user_devid, dst_va->va, dst_va->size, tseg);
    } else {
        return svm_urma_get_remote_tseg(dst_va->devid, dst_va->va, dst_va->size, tseg) ;
    }
}

int svm_urma_get_token_info(u32 user_devid, struct svm_dst_va *dst_va, u32 *token_id, u32 *token_val)
{
    if (dst_va->devid == svm_get_host_devid()) {
        return svm_urma_get_local_seg_token_info(user_devid, dst_va->va, dst_va->size, token_id, token_val);
    } else {
        return svm_urma_get_remote_seg_token_info(dst_va->devid, dst_va->va, dst_va->size, token_id, token_val) ;
    }
}

void __attribute__((constructor)) urma_seg_mng_init(void)
{
    u32 devid;

    for (devid = 0; devid < SVM_MAX_AGENT_NUM; devid++) {
        (void)pthread_rwlock_init(&g_local_seg_mng[devid].rwlock, NULL);
        rbtree_init(&g_local_seg_mng[devid].rb_root);
    }
    for (devid = 0; devid < SVM_MAX_AGENT_NUM; devid++) {
        (void)pthread_rwlock_init(&g_remote_seg_mng[devid].rwlock, NULL);
        rbtree_init(&g_remote_seg_mng[devid].rb_root);
    }
}
