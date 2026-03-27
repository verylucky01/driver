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

#include "securec.h"

#include "rbtree.h"

#include "svm_log.h"
#include "cache_pub.h"
#include "cache_recycle_seg.h"

struct cache_recycle_seg {
    struct rbtree_node node;

    u64 start;
    u64 size;
    u64 align;
    u32 flag;
    u32 devid;
};

static pthread_rwlock_t g_rwlock = PTHREAD_RWLOCK_INITIALIZER;
static struct rbtree_root g_rb_root = RB_ROOT;

static void rb_range_handle_of_cache_recycle_seg(struct rbtree_node *node, struct rb_range_handle *range_handle)
{
    struct cache_recycle_seg *seg = rb_entry(node, struct cache_recycle_seg, node);

    range_handle->start = seg->start;
    range_handle->end = seg->start + seg->size - 1;
}

void cache_recycle_add_seg(u64 start, u64 size, u64 align, u32 devid, u32 flag)
{
    struct cache_recycle_seg *seg = NULL;
    int ret;

    seg = calloc(1, sizeof(struct cache_recycle_seg));
    if (seg == NULL) {
        svm_warn("Calloc cache_recycle_seg check. (size=%llu)\n", sizeof(struct cache_recycle_seg));
        return;
    }

    RB_CLEAR_NODE(&seg->node);
    seg->start = start;
    seg->size = size;
    seg->align = align;
    seg->flag = flag;
    seg->devid = devid;

    (void)pthread_rwlock_wrlock(&g_rwlock);
    ret = rbtree_insert_by_range(&g_rb_root, &seg->node, rb_range_handle_of_cache_recycle_seg);
    (void)pthread_rwlock_unlock(&g_rwlock);
    if (ret != DRV_ERROR_NONE) {
        svm_warn("Insert recycle seg node check. (start=0x%llx; size=%llu)\n", start, size);
        free(seg);
    }
}

static int cache_recycle_del_seg(u64 start, u64 *size, u64 *align, u32 *devid, u32 *flag)
{
    struct cache_recycle_seg *seg = NULL;
    struct rbtree_node *rb_node = NULL;
    struct rb_range_handle range = {.start = start, .end = start};

    (void)pthread_rwlock_wrlock(&g_rwlock);
    rb_node = rbtree_search_by_range(&g_rb_root, &range, rb_range_handle_of_cache_recycle_seg);
    if (rb_node == NULL) {
        (void)pthread_rwlock_unlock(&g_rwlock);
        return DRV_ERROR_NOT_EXIST;
    }

    seg = rb_entry(rb_node, struct cache_recycle_seg, node);
    if (start != seg->start) {
        (void)pthread_rwlock_unlock(&g_rwlock);
        return DRV_ERROR_NOT_EXIST;
    }
    _rbtree_erase(&g_rb_root, &seg->node);

    *size = seg->size;
    *align = seg->align;
    *devid = seg->devid;
    *flag = seg->flag;
    free(seg);
    (void)pthread_rwlock_unlock(&g_rwlock);
    return DRV_ERROR_NONE;
}

int svm_cache_recycle_seg_release(u64 va)
{
    u32 devid, flag, normal_flag;
    u64 size, align;
    int ret;

    ret = cache_recycle_del_seg(va, &size, &align, &devid, &flag);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    normal_flag = cache_flag_to_normal_flag(flag);
    return svm_normal_free(devid, normal_flag, align, va, size);
}

static u32 _cache_recyle_seg_show(struct cache_recycle_seg *seg, int id, char *buf, u32 buf_len)
{
    if (buf == NULL) {
        svm_info("recycle seg id %d: flag=0x%x start=0x%llx size=0x%llx\n",
            id, seg->flag, seg->start, seg->size);
        return 0;
    } else {
        int len;

        len = snprintf_s(buf, buf_len, buf_len - 1,
            "recycle seg id %d: flag=0x%x start=0x%llx size=0x%llx\n",
            id, seg->flag, seg->start, seg->size);
        return (len < 0) ? 0 : (u32)len;
    }
}

u32 cache_recycle_seg_show(u32 devid, char *buf, u32 buf_len)
{
    struct cache_recycle_seg *seg = NULL;
    struct rbtree_node *cur = NULL;
    int id = 0;
    u32 len = 0;

    (void)pthread_rwlock_rdlock(&g_rwlock);
    rbtree_node_for_each(cur, &g_rb_root) {
        seg = rb_entry(cur, struct cache_recycle_seg, node);
        if ((devid == seg->devid) || (devid == SVM_INVALID_DEVID)) {
            len += _cache_recyle_seg_show(seg, id++, buf + len, buf_len - len);
        }
    }
    (void)pthread_rwlock_unlock(&g_rwlock);

    return len;
}
