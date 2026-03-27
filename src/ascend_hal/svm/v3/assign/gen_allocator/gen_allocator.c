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

#include "multi_rbtree.h"
#include "rbtree.h"

#include "svm_user_adapt.h"
#include "gen_allocator.h"
#include "svm_log.h"

struct ga_range {
    struct rbtree_node node;

    /* In order to quickly get the inst which the current range belongs to. */
    struct ga_inst *inst;

    u64 start;
    u64 size;

    u64 idle_area_size;

    struct rbtree_root addr_area_tree;      // rb tree, val is area, key is addr
};

struct ga_area {
    struct multi_rb_node size_tree_node;
    struct rbtree_node addr_tree_node;

    /* In order to quickly get the inst/range which the current area belongs to. */
    struct ga_inst *inst;
    struct ga_range *range;

    u64 start;
    u64 size;
};

struct ga_stats {
    u32 range_num;
};

struct ga_inst {
    struct svm_ga_attr attr;

    pthread_rwlock_t rwlock;
    struct ga_stats stats;
    struct rbtree_root addr_range_tree;     // rb tree, val is range, key is addr
    struct rbtree_root size_area_tree;      // multi rb tree, val is area, key is size
};

static bool ga_flag_is_fixed_addr(u32 flag)
{
    return ((flag & SVM_GA_FLAG_FIXED_ADDR) != 0);
}

static bool ga_flag_is_in_fixed_addr_range(u32 flag)
{
    return ((flag & SVM_GA_FLAG_IN_FIXED_ADDR_RANGE) != 0);
}

static bool ga_addr_is_aligned(struct ga_inst *inst, u64 addr)
{
    return SVM_IS_ALIGNED(addr, inst->attr.gran_size);
}

static void rb_range_handle_of_ga_area(struct rbtree_node *node, struct rb_range_handle *range_handle)
{
    struct ga_area *area = rb_entry(node, struct ga_area, addr_tree_node);

    range_handle->start = area->start;
    range_handle->end = area->start + area->size - 1;
}

static void rb_range_handle_of_ga_range(struct rbtree_node *node, struct rb_range_handle *range_handle)
{
    struct ga_range *range = rb_entry(node, struct ga_range, node);

    range_handle->start = range->start;
    range_handle->end = range->start + range->size - 1;
}

static struct ga_area *ga_alloc_area(u64 start, u64 size)
{
    struct ga_area *area = NULL;

    area = svm_ua_calloc(1, sizeof(struct ga_area));
    if (area == NULL) {
        svm_err("svm_ua_calloc ga_area failed. (size=%llu)\n", sizeof(struct ga_area));
        return NULL;
    }

    multi_rb_node_init(&area->size_tree_node);
    RB_CLEAR_NODE(&area->addr_tree_node);
    area->start = start;
    area->size = size;
    return area;
}

static void ga_free_area(struct ga_area *area)
{
    svm_ua_free(area);
}

static int ga_insert_area(struct ga_inst *inst, struct ga_range *range, struct ga_area *area)
{
    int ret;

    area->inst = inst;
    area->range = range;

    ret = rbtree_insert_by_range(&range->addr_area_tree, &area->addr_tree_node, rb_range_handle_of_ga_area);
    if (ret != 0) {
        return DRV_ERROR_INVALID_VALUE;
    }
    range->idle_area_size += area->size;

    multi_rbtree_insert(area->size, &inst->size_area_tree, &area->size_tree_node);
    return DRV_ERROR_NONE;
}

static void ga_erase_area(struct ga_area *area)
{
    area->range->idle_area_size -= area->size;
    multi_rbtree_erase(&area->inst->size_area_tree, &area->size_tree_node);
    _rbtree_erase(&area->range->addr_area_tree, &area->addr_tree_node);

    area->inst = NULL;
    area->range = NULL;
}

static struct ga_area *ga_create_area(struct ga_inst *inst, struct ga_range *range, u64 start, u64 size)
{
    struct ga_area *area = NULL;
    int ret;

    area = ga_alloc_area(start, size);
    if (area == NULL) {
        return NULL;
    }

    ret = ga_insert_area(inst, range, area);
    if (ret != DRV_ERROR_NONE) {
        ga_free_area(area);
        return NULL;
    }

    return area;
}

static void ga_destroy_area(struct ga_area *area)
{
    ga_erase_area(area);
    ga_free_area(area);
}

static struct ga_area *ga_get_area_by_size(struct ga_inst *inst, u64 size)
{
    struct ga_area *area = NULL;
    struct multi_rb_node *m_rb_node = NULL;

    m_rb_node = multi_rbtree_get(size, &inst->size_area_tree);
    if (m_rb_node == NULL) {
        m_rb_node = multi_rbtree_get_upper_bound(size, &inst->size_area_tree);
    }

    if (m_rb_node != NULL) {
        area = rb_entry(m_rb_node, struct ga_area, size_tree_node);
    }

    return area;
}

static struct ga_area *ga_range_get_area(struct ga_range *range, u64 addr, u64 size)
{
    struct rbtree_node *node = NULL;
    struct rb_range_handle rb_range = {.start = addr, .end = addr + size - 1};

    node = rbtree_search_by_range(&range->addr_area_tree, &rb_range, rb_range_handle_of_ga_area);
    if (node == NULL) {
        return NULL;
    }

    return rb_entry(node, struct ga_area, addr_tree_node);
}

static struct ga_range *ga_get_range(struct ga_inst *inst, u64 addr, u64 size)
{
    struct ga_range *range = NULL;
    struct rbtree_node *node = NULL;
    struct rb_range_handle rb_range = {.start = addr, .end = addr + size - 1};

    node = rbtree_search_by_range(&inst->addr_range_tree, &rb_range, rb_range_handle_of_ga_range);
    if (node != NULL) {
        range = rb_entry(node, struct ga_range, node);
    }

    return range;
}

static struct ga_area *ga_get_area_by_addr(struct ga_inst *inst, u64 addr, u64 size)
{
    struct ga_range *range = NULL;

    range = ga_get_range(inst, addr, size);
    if (range == NULL) {
        return NULL;
    }

    return ga_range_get_area(range, addr, size);
}

static void ga_try_slice_area(struct ga_area *area, u64 addr, u64 size)
{
    struct ga_inst *inst = area->inst;
    struct ga_range *range = area->range;
    struct ga_area *area_l = NULL;
    struct ga_area *area_r = NULL;
    u64 start_l, size_l, start_r, size_r;
    u64 area_start = area->start;
    u64 area_size = area->size;

    ga_erase_area(area);
    if (area_size == size) {
        goto free_area;
    }

    if (area_start < addr) {
        start_l = area_start;
        size_l = addr - area_start;
        area_l = ga_create_area(inst, range, start_l, size_l);
        if (area_l == NULL) {
            svm_info("Unlikely, unless out of memory. (start=0x%llx; size=%llu)\n", start_l, size_l);
        }
    }

    if ((area_start + area_size) > (addr + size)) {
        start_r = addr + size;
        size_r = (area_start + area_size) - (addr + size);
        area_r = ga_create_area(inst, range, start_r, size_r);
        if (area_r == NULL) {
            svm_info("Unlikely, unless out of memory. (start=0x%llx; size=%llu)\n", start_r, size_r);
        }
    }
free_area:
    ga_free_area(area);
}

static int ga_try_merge_area(struct ga_area *area)
{
    struct ga_inst *inst = area->inst;
    struct ga_range *range = area->range;
    struct ga_area *area_l = NULL;
    struct ga_area *area_r = NULL;
    u64 new_start = area->start;
    u64 new_size = area->size;
    int ret;

    if (area->start > 0) {
        area_l = ga_range_get_area(range, area->start - 1, 1);
        if (area_l != NULL) {
            new_start = area_l->start;
            new_size += area_l->size;
            ga_erase_area(area_l);
            ga_free_area(area_l);
        }
    }

    area_r = ga_range_get_area(range, area->start + area->size, 1);
    if (area_r != NULL) {
        new_size += area_r->size;
        ga_erase_area(area_r);
        ga_free_area(area_r);
    }

    if (new_size != area->size) {
        ga_erase_area(area);
        area->start = new_start;
        area->size = new_size;
        ret = ga_insert_area(inst, range, area);
        if (ret != DRV_ERROR_NONE) {
            svm_err("Unlikely fail. (ret=%d; start=0x%llx; size=%llu)\n",
                ret, area->start, area->size);
            ga_free_area(area);
            return ret;
        }
    }

    return DRV_ERROR_NONE;
}

static struct ga_range *ga_alloc_range(u64 start, u64 size)
{
    struct ga_range *range = NULL;

    range = svm_ua_calloc(1, sizeof(struct ga_range));
    if (range == NULL) {
        svm_err("svm_ua_calloc ga_range failed. (size=%llu)\n", sizeof(struct ga_range));
        return NULL;
    }

    rbtree_init(&range->addr_area_tree);
    RB_CLEAR_NODE(&range->node);
    range->start = start;
    range->size = size;
    return range;
}

static void ga_free_range(struct ga_range *range)
{
    svm_ua_free(range);
}

static int ga_insert_range(struct ga_inst *inst, struct ga_range *range)
{
    int ret;

    ret = rbtree_insert_by_range(&inst->addr_range_tree, &range->node, rb_range_handle_of_ga_range);
    if (ret != 0) {
        return DRV_ERROR_INVALID_VALUE;
    }

    range->inst = inst;

    return DRV_ERROR_NONE;
}

static void ga_erase_range(struct ga_range *range)
{
    _rbtree_erase(&range->inst->addr_range_tree, &range->node);
    range->inst = NULL;
}

static int ga_create_range(struct ga_inst *inst, u64 start, u64 size)
{
    struct ga_range *range = NULL;
    struct ga_area *area = NULL;
    int ret;

    range = ga_alloc_range(start, size);
    if (range == NULL) {
        return DRV_ERROR_OUT_OF_MEMORY;
    }

    ret = ga_insert_range(inst, range);
    if (ret != DRV_ERROR_NONE) {
        ga_free_range(range);
        return ret;
    }

    area = ga_create_area(inst, range, start, size);
    if (area == NULL) {
        ga_erase_range(range);
        ga_free_range(range);
        return DRV_ERROR_OUT_OF_MEMORY;
    }

    return DRV_ERROR_NONE;
}

static void ga_destroy_range_all_area(struct ga_range *range)
{
    struct ga_area *area = NULL;
    struct rbtree_node *cur = NULL;
    struct rbtree_node *n = NULL;

    rbtree_node_for_each_prev_safe(cur, n, &range->addr_area_tree) {
        area = rb_entry(cur, struct ga_area, addr_tree_node);
        ga_destroy_area(area);
    }

    range->idle_area_size = 0;
}

static void ga_destroy_range(struct ga_range *range)
{
    ga_destroy_range_all_area(range);
    ga_erase_range(range);
    ga_free_range(range);
}

static bool ga_is_idle_range(struct ga_range *range)
{
    return (range->idle_area_size == range->size);
}

bool svm_ga_owner_range_is_idle(void *ga_inst, u64 addr)
{
    struct ga_inst *inst = (struct ga_inst *)ga_inst;
    struct ga_range *range = NULL;
    bool is_idle = false;

    (void)pthread_rwlock_rdlock(&inst->rwlock);

    range = ga_get_range(inst, addr, 1);
    is_idle = (range != NULL) ? ga_is_idle_range(range) : false;

    (void)pthread_rwlock_unlock(&inst->rwlock);
    return is_idle;
}

static struct ga_range *ga_get_one_idle_range(struct ga_inst *inst)
{
    struct ga_range *range = NULL;
    struct rbtree_node *cur = NULL;
    struct rbtree_node *n = NULL;

    rbtree_node_for_each_prev_safe(cur, n, &inst->addr_range_tree) {
        range = rb_entry(cur, struct ga_range, node);
        if (ga_is_idle_range(range)) {
            return range;
        }
    }

    return NULL;
}

static void ga_destroy_inst_all_range(struct ga_inst *inst)
{
    struct ga_range *range = NULL;
    struct rbtree_node *cur = NULL;
    struct rbtree_node *n = NULL;

    rbtree_node_for_each_prev_safe(cur, n, &inst->addr_range_tree) {
        range = rb_entry(cur, struct ga_range, node);
        ga_destroy_range(range);
    }
}

void *svm_ga_inst_create(struct svm_ga_attr *attr)
{
    struct ga_inst *inst = NULL;

    inst = svm_ua_calloc(1, sizeof(struct ga_inst));
    if (inst == NULL) {
        svm_err("svm_ua_calloc ga_inst failed. (size=%llu)\n", sizeof(struct ga_inst));
        return NULL;
    }

    inst->attr = *attr;
    (void)pthread_rwlock_init(&inst->rwlock, NULL);
    rbtree_init(&inst->addr_range_tree);
    rbtree_init(&inst->size_area_tree);
    inst->stats.range_num = 0;

    return (void *)inst;
}

void svm_ga_inst_destroy(void *ga_inst)
{
    struct ga_inst *inst = (struct ga_inst *)ga_inst;

    ga_destroy_inst_all_range(inst);

    svm_ua_free(inst);
}

int svm_ga_add_range(void *ga_inst, u64 start, u64 size)
{
    struct ga_inst *inst = (struct ga_inst *)ga_inst;
    int ret;

    if ((ga_addr_is_aligned(inst, start) == false) || (ga_addr_is_aligned(inst, size) == false)) {
        svm_err("Addr isn't aligned by order. (gran_size=%llu; start=0x%llx; size=%llu)\n",
            inst->attr.gran_size, start, size);
        return DRV_ERROR_INVALID_VALUE;
    }

    pthread_rwlock_wrlock(&inst->rwlock);
    ret = ga_create_range(inst, start, size);
    pthread_rwlock_unlock(&inst->rwlock);

    return ret;
}

int svm_ga_del_range(void *ga_inst, u64 start)
{
    struct ga_inst *inst = (struct ga_inst *)ga_inst;
    struct ga_range *range = NULL;

    (void)pthread_rwlock_wrlock(&inst->rwlock);
    range = ga_get_range(inst, start, 1);
    if (range == NULL) {
        (void)pthread_rwlock_unlock(&inst->rwlock);
        svm_err("No matched range. (start=0x%llx)\n", start);
        return DRV_ERROR_INVALID_VALUE;
    }

    if (range->start != start) {
        svm_err("Start isn't matched with range_start. (start=0x%llx; range_start=0x%llx)\n", start, range->start);
        (void)pthread_rwlock_unlock(&inst->rwlock);
        return DRV_ERROR_INVALID_VALUE;
    }

    if (ga_is_idle_range(range) == false) {
        svm_err("Range isn't idle. (start=0x%llx; size=%llu)\n", range->start, range->size);
        (void)pthread_rwlock_unlock(&inst->rwlock);
        return DRV_ERROR_INVALID_VALUE;
    }

    ga_destroy_range(range);
    (void)pthread_rwlock_unlock(&inst->rwlock);
    return DRV_ERROR_NONE;
}

int svm_ga_recycle_one_idle_range(void *ga_inst, u64 *start, u64 *size)
{
    struct ga_inst *inst = (struct ga_inst *)ga_inst;
    struct ga_range *range = NULL;

    (void)pthread_rwlock_wrlock(&inst->rwlock);
    range = ga_get_one_idle_range(inst);
    if (range == NULL) {
        (void)pthread_rwlock_unlock(&inst->rwlock);
        return DRV_ERROR_NO_RESOURCES;
    }

    *start = range->start;
    *size = range->size;
    ga_destroy_range(range);
    (void)pthread_rwlock_unlock(&inst->rwlock);
    return DRV_ERROR_NONE;
}

int svm_ga_for_each_range(void *ga_inst, int (*func)(u64 start, u64 size, void *priv), void *priv)
{
    struct ga_inst *inst = (struct ga_inst *)ga_inst;
    struct ga_range *range = NULL;
    struct rbtree_node *cur = NULL;
    struct rbtree_node *n = NULL;
    int ret = 0;

    (void)pthread_rwlock_rdlock(&inst->rwlock);
    rbtree_node_for_each_prev_safe(cur, n, &inst->addr_range_tree) {
        range = rb_entry(cur, struct ga_range, node);
        ret = func(range->start, range->size, priv);
        if (ret != DRV_ERROR_NONE) {
            break;
        }
    }
    (void)pthread_rwlock_unlock(&inst->rwlock);
    return ret;
}

static int ga_alloc_by_fixed_addr(struct ga_inst *inst, u64 addr, u64 size)
{
    struct ga_area *area = NULL;

    area = ga_get_area_by_addr(inst, addr, size);
    if (area == NULL) {
        return DRV_ERROR_PARA_ERROR;
    }

    ga_try_slice_area(area, addr, size);
    return DRV_ERROR_NONE;
}

static int ga_alloc_by_size(struct ga_inst *inst, u64 size, u64 *addr)
{
    struct ga_area *area = NULL;

    area = ga_get_area_by_size(inst, size);
    if (area == NULL) {
        return DRV_ERROR_OUT_OF_MEMORY;
    }

    *addr = area->start;
    ga_try_slice_area(area, *addr, size);
    return DRV_ERROR_NONE;
}

static int ga_alloc_para_check(struct ga_inst *inst, u32 flag, u64 *addr, u64 size)
{
    if (ga_flag_is_fixed_addr(flag) && (ga_addr_is_aligned(inst, *addr) == false)) {
        svm_err("Add should aligned by gran_size. (addr=%llu; gran_size=%llu)\n", *addr, inst->attr.gran_size);
        return DRV_ERROR_PARA_ERROR;
    }

    if (ga_addr_is_aligned(inst, size) == false) {
        svm_err("Size should aligned by gran_size. (size=%llu; gran_size=%llu)\n", size, inst->attr.gran_size);
        return DRV_ERROR_PARA_ERROR;
    }

    if (size == 0) {
        return DRV_ERROR_PARA_ERROR;
    }

    return DRV_ERROR_NONE;
}

int svm_ga_alloc(void *ga_inst, u32 flag, u64 *addr, u64 size)
{
    struct ga_inst *inst = (struct ga_inst *)ga_inst;
    int ret;

    ret = ga_alloc_para_check(inst, flag, addr, size);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    (void)pthread_rwlock_wrlock(&inst->rwlock);
    if (ga_flag_is_fixed_addr(flag)) {
        ret = ga_alloc_by_fixed_addr(inst, *addr, size);
    } else if (ga_flag_is_in_fixed_addr_range(flag)) {
        ret = DRV_ERROR_NOT_SUPPORT;
    } else {
        ret = ga_alloc_by_size(inst, size, addr);
    }
    (void)pthread_rwlock_unlock(&inst->rwlock);

    return ret;
}

static int ga_free_para_check(struct ga_inst *inst, u64 addr, u64 size)
{
    if (ga_addr_is_aligned(inst, addr) == false) {
        svm_err("Add should aligned by gran_size. (addr=%llu; gran_size=%llu)\n", addr, inst->attr.gran_size);
        return DRV_ERROR_PARA_ERROR;
    }

    if (ga_addr_is_aligned(inst, size) == false) {
        svm_err("Size should aligned by gran_size. (size=%llu; gran_size=%llu)\n", size, inst->attr.gran_size);
        return DRV_ERROR_PARA_ERROR;
    }

    return DRV_ERROR_NONE;
}

int svm_ga_free(void *ga_inst, u64 addr, u64 size)
{
    struct ga_inst *inst = (struct ga_inst *)ga_inst;
    struct ga_range *range = NULL;
    struct ga_area *area = NULL;
    int ret;

    ret = ga_free_para_check(inst, addr, size);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    (void)pthread_rwlock_wrlock(&inst->rwlock);
    range = ga_get_range(inst, addr, size);
    if (range == NULL) {
        (void)pthread_rwlock_unlock(&inst->rwlock);
        svm_err("No matched range. (addr=0x%llx; size=%llu)\n", addr, size);
        return DRV_ERROR_INVALID_VALUE;
    }

    area = ga_create_area(inst, range, addr, size);
    if (area == NULL) {
        (void)pthread_rwlock_unlock(&inst->rwlock);
        svm_err("Create area failed. (addr=0x%llx; size=%llu)\n", addr, size);
        return DRV_ERROR_INVALID_VALUE;
    }

    ret = ga_try_merge_area(area);   // unlikely fail, if merge return fail, it will free area */
    (void)pthread_rwlock_unlock(&inst->rwlock);

    return ret;
}

u32 svm_ga_show(void *ga_inst, char *buf, u32 buf_len)
{
    SVM_UNUSED(ga_inst);
    SVM_UNUSED(buf);
    SVM_UNUSED(buf_len);

    return 0;
}
