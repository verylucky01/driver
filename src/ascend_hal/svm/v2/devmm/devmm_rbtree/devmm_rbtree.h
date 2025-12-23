/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef DEVMM_RBTREE_H
#define DEVMM_RBTREE_H

#include <stdint.h>

#include "list.h"
#include "ascend_hal.h"
#include "multi_rbtree.h"

#define MAX_RBTREE_NUM         64
#define DEVMM_MALLOC_ALIGN_NUM 64U

enum devmm_rbtree_type {
    DEVMM_ALLOCED_TREE = 0,
    DEVMM_IDLE_SIZE_TREE,
    DEVMM_IDLE_VA_TREE,
    DEVMM_IDLE_MAPPED_TREE,
    DEVMM_TREE_TYPE_MAX
};

enum devmm_mapped_rbtree_type {
    DEVMM_MAPPED_RW_TREE = 0,
    DEVMM_MAPPED_RDONLY_TREE,
    DEVMM_MAPPED_TREE_TYPE_MAX
};

struct devmm_node_data {
    uint64_t va;
    uint64_t size;
    uint64_t total;
    uint32_t flag;
    DVmem_advise advise;
};

struct devmm_rbtree_node {
    struct multi_rb_node va_node;
    struct multi_rb_node size_node;
    struct multi_rb_node cache_node;
    struct devmm_node_data data;
};

struct devmm_cache_list {
    struct devmm_rbtree_node cache;
    struct list_node list;
    uint8_t is_new;
};

struct devmm_heap_rbtree {
    struct rbtree_root *alloced_tree;
    struct rbtree_root *idle_size_tree;
    struct rbtree_root *idle_va_tree;
    struct rbtree_root *idle_mapped_cache_tree[DEVMM_MAPPED_TREE_TYPE_MAX];
    struct devmm_cache_list *head;
    uint32_t devmm_cache_numsize;
};

struct devmm_rbtree_node *devmm_rbtree_get_alloced_node_in_range(uint64_t va,
    struct devmm_heap_rbtree *rbtree_queue);
struct devmm_rbtree_node *devmm_rbtree_get_idle_va_node_in_range(uint64_t va,
    struct devmm_heap_rbtree *rbtree_queue);
void devmm_assign_rbtree_node_data(uint64_t va, uint64_t size, uint64_t total, uint32_t flag, DVmem_advise advise,
    struct devmm_rbtree_node *rbtree_node);
void devmm_free_rbtree_node(struct devmm_rbtree_node *rb_node, struct devmm_heap_rbtree *rbtree_queue);
DVresult devmm_rbtree_alloc(struct devmm_heap_rbtree *rbtree_queue, uint32_t cache_numsize);
struct devmm_rbtree_node *devmm_alloc_rbtree_node(struct devmm_heap_rbtree *rbtree_queue);
DVresult devmm_rbtree_insert_idle_va_tree(struct devmm_rbtree_node *rbtree_node,
    struct devmm_heap_rbtree *rbtree_queue);
DVresult devmm_rbtree_insert_idle_size_tree(struct devmm_rbtree_node *rbtree_node,
    struct devmm_heap_rbtree *rbtree_queue);
DVresult devmm_rbtree_insert_alloced_tree(struct devmm_rbtree_node *rbtree_node,
    struct devmm_heap_rbtree *rbtree_queue);
DVresult devmm_rbtree_erase_idle_va_tree(struct devmm_rbtree_node *rbtree_node,
    struct devmm_heap_rbtree *rbtree_queue);
DVresult devmm_rbtree_erase_idle_size_tree(struct devmm_rbtree_node *rbtree_node,
    struct devmm_heap_rbtree *rbtree_queue);
DVresult devmm_rbtree_erase_alloced_tree(struct devmm_rbtree_node *rbtree_node,
    struct devmm_heap_rbtree *rbtree_queue);
void devmm_rbtree_destory(struct devmm_heap_rbtree *rbtree_queue);
struct devmm_rbtree_node *devmm_rbtree_get_alloced_node(uint64_t va, struct devmm_heap_rbtree *rbtree_queue);
struct devmm_rbtree_node *devmm_rbtree_get_from_idle_size_tree(uint64_t size, struct devmm_heap_rbtree *rbtree_queue);
uint64_t devmm_rbtree_get_mapped_len(struct devmm_heap_rbtree *rbtree_queue, uint32_t mapped_tree_type);
struct devmm_rbtree_node *devmm_rbtree_get_idle_mapped_node(uint64_t size, struct devmm_heap_rbtree *rbtree_queue,
    uint32_t mapped_tree_type);
struct devmm_rbtree_node *devmm_rbtree_get_full_mapped_node(struct devmm_heap_rbtree *rbtree_queue,
    uint32_t mapped_tree_type);
DVresult devmm_rbtree_insert_idle_readonly_mapped_tree(struct devmm_rbtree_node *rbtree_node,
    struct devmm_heap_rbtree *rbtree_queue);
DVresult devmm_rbtree_insert_idle_rw_mapped_tree(struct devmm_rbtree_node *rbtree_node,
    struct devmm_heap_rbtree *rbtree_queue);
DVresult devmm_rbtree_erase_idle_readonly_mapped_tree(struct devmm_rbtree_node *rbtree_node,
    struct devmm_heap_rbtree *rbtree_queue);
DVresult devmm_rbtree_erase_idle_rw_mapped_tree(struct devmm_rbtree_node *rbtree_node,
    struct devmm_heap_rbtree *rbtree_queue);
bool devmm_rbtree_is_empty(struct rbtree_root *root);
struct devmm_rbtree_node *devmm_get_rbtree_node_by_type(struct multi_rb_node *rb_node, uint32_t tree_type);

#endif
