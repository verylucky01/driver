/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include "devmm_rbtree.h"
#include "devmm_svm.h"
#include "devmm_virt_com_heap.h"

struct devmm_rbtree_node *devmm_get_rbtree_node_by_type(struct multi_rb_node *rb_node, uint32_t tree_type)
{
    struct devmm_rbtree_node *rbtree_node = NULL;

    if (tree_type == DEVMM_ALLOCED_TREE) {
        rbtree_node = rb_entry(rb_node, struct devmm_rbtree_node, va_node);
    } else if (tree_type == DEVMM_IDLE_SIZE_TREE) {
        rbtree_node = rb_entry(rb_node, struct devmm_rbtree_node, size_node);
    } else if (tree_type == DEVMM_IDLE_VA_TREE) {
        rbtree_node = rb_entry(rb_node, struct devmm_rbtree_node, va_node);
    } else if (tree_type == DEVMM_IDLE_MAPPED_TREE) {
        rbtree_node = rb_entry(rb_node, struct devmm_rbtree_node, cache_node);
    }

    return rbtree_node;
}

uint64_t devmm_rbtree_get_mapped_len(struct devmm_heap_rbtree *rbtree_queue, uint32_t mapped_tree_type)
{
    return rbtree_queue->idle_mapped_cache_tree[mapped_tree_type]->rbtree_len;
}

bool devmm_rbtree_is_empty(struct rbtree_root *root)
{
    return ((root == NULL) || (root->rbtree_len == 0));
}

struct devmm_rbtree_node *devmm_rbtree_get_alloced_node(uint64_t va, struct devmm_heap_rbtree *rbtree_queue)
{
    struct multi_rb_node *rb_node = multi_rbtree_get(va, rbtree_queue->alloced_tree);
    if (rb_node == NULL) {
        DEVMM_DRV_ERR("Get node failed with key. (key=0x%llx)\n", va);
        return NULL;
    }
    return rb_entry(rb_node, struct devmm_rbtree_node, va_node);
}

static struct multi_rb_node *devmm_multi_rb_node_get_in_range(uint64_t key, struct rbtree_root *root)
{
    struct multi_rb_node *rb_node = NULL;
    struct rbtree_node *node = NULL;

    rb_node = multi_rbtree_get_upper_bound(key, root);
    if (rb_node == NULL) {
        /* can not find upper bound node */
        node = rbtree_last(root);
    } else if (&rb_node->multi_rbtree_node.rbtree_node == rbtree_first(root)) {
        /* key is not in range */
        return NULL;
    } else {
        node = rbtree_prev(&rb_node->multi_rbtree_node.rbtree_node);
    }

    return multi_rbtree_get_node_from_rb_node(node);
}

static struct devmm_rbtree_node *devmm_rbtree_node_get_in_range(uint64_t va, struct rbtree_root *root)
{
    struct devmm_rbtree_node *va_node = NULL;
    struct multi_rb_node *rb_node = NULL;

    rb_node = devmm_multi_rb_node_get_in_range(va, root);
    if (rb_node == NULL) {
        return NULL;
    }
    va_node = rb_entry(rb_node, struct devmm_rbtree_node, va_node);
    if ((va >= va_node->data.va) && (va < (va_node->data.va + va_node->data.size))) {
        return va_node;
    }
    /* key is not in range */
    return NULL;
}

struct devmm_rbtree_node *devmm_rbtree_get_alloced_node_in_range(uint64_t va,
    struct devmm_heap_rbtree *rbtree_queue)
{
    return devmm_rbtree_node_get_in_range(va, rbtree_queue->alloced_tree);
}

struct devmm_rbtree_node *devmm_rbtree_get_idle_va_node_in_range(uint64_t va,
    struct devmm_heap_rbtree *rbtree_queue)
{
    return devmm_rbtree_node_get_in_range(va, rbtree_queue->idle_va_tree);
}

struct devmm_rbtree_node *devmm_rbtree_get_from_idle_size_tree(uint64_t size, struct devmm_heap_rbtree *rbtree_queue)
{
    struct multi_rb_node *rb_node = NULL;

    rb_node = multi_rbtree_get(size, rbtree_queue->idle_size_tree);
    if (rb_node != NULL) {
        return rb_entry(rb_node, struct devmm_rbtree_node, size_node);
    }

    rb_node = multi_rbtree_get_upper_bound(size, rbtree_queue->idle_size_tree);
    if (rb_node == NULL) {
        /* can not find upper bound node with the key */
        return NULL;
    }
    return rb_entry(rb_node, struct devmm_rbtree_node, size_node);
}

struct devmm_rbtree_node *devmm_rbtree_get_idle_mapped_node(uint64_t size, struct devmm_heap_rbtree *rbtree_queue,
    uint32_t mapped_tree_type)
{
    struct multi_rb_node *rb_node = NULL;

    rb_node = multi_rbtree_get(size, rbtree_queue->idle_mapped_cache_tree[mapped_tree_type]);
    if (rb_node != NULL) {
        return rb_entry(rb_node, struct devmm_rbtree_node, cache_node);
    }

    rb_node = multi_rbtree_get_upper_bound(size, rbtree_queue->idle_mapped_cache_tree[mapped_tree_type]);
    if (rb_node == NULL) {
        /* can not find upper bound node with the key */
        return NULL;
    }

    return rb_entry(rb_node, struct devmm_rbtree_node, cache_node);
}

STATIC struct devmm_rbtree_node *devmm_rblist_get_full_mapped_node(struct multi_rb_node *rb_node)
{
    struct devmm_rbtree_node *rbtree_node = NULL;
    struct multi_rb_node *rb_node_list = NULL;
    struct list_node *head = NULL;

    if ((rb_node != NULL) && (list_empty(&rb_node->list) == 0)) {
        list_for_each_node(head, &rb_node->list) {
            rb_node_list = rb_entry(head, struct multi_rb_node, list);
            rbtree_node = rb_entry(rb_node_list, struct devmm_rbtree_node, cache_node);
            if (rbtree_node->data.total == rbtree_node->data.size) {
                return rbtree_node;
            }
        }
    }

    return NULL;
}

struct devmm_rbtree_node *devmm_rbtree_get_full_mapped_node(struct devmm_heap_rbtree *rbtree_queue,
    uint32_t mapped_tree_type)
{
    struct devmm_rbtree_node *rbtree_node = NULL;
    struct multi_rb_node *rb_node = NULL;
    struct rbtree_node *cur = NULL;

    rbtree_node_for_each(cur, rbtree_queue->idle_mapped_cache_tree[mapped_tree_type]) {
        rb_node = multi_rbtree_get_node_from_rb_node(cur);
        rbtree_node = rb_entry(rb_node, struct devmm_rbtree_node, cache_node);
        if (rbtree_node->data.total == rbtree_node->data.size) {
            return rbtree_node;
        }
        rbtree_node = devmm_rblist_get_full_mapped_node(rb_node);
        if (rbtree_node != NULL) {
            return rbtree_node;
        }
    }
    return NULL;
}

static inline DVresult devmm_rbtree_insert(uint64_t key, struct multi_rb_node *rb_node, struct rbtree_root *root)
{
    multi_rbtree_insert(key, root, rb_node);
    return DRV_ERROR_NONE;
}

DVresult devmm_rbtree_insert_idle_va_tree(struct devmm_rbtree_node *rbtree_node, struct devmm_heap_rbtree *rbtree_queue)
{
    return devmm_rbtree_insert(rbtree_node->data.va, &rbtree_node->va_node, rbtree_queue->idle_va_tree);
}

DVresult devmm_rbtree_insert_idle_size_tree(struct devmm_rbtree_node *rbtree_node,
    struct devmm_heap_rbtree *rbtree_queue)
{
    return devmm_rbtree_insert(rbtree_node->data.size, &rbtree_node->size_node, rbtree_queue->idle_size_tree);
}

DVresult devmm_rbtree_insert_alloced_tree(struct devmm_rbtree_node *rbtree_node, struct devmm_heap_rbtree *rbtree_queue)
{
    return devmm_rbtree_insert(rbtree_node->data.va, &rbtree_node->va_node, rbtree_queue->alloced_tree);
}

DVresult devmm_rbtree_insert_idle_readonly_mapped_tree(struct devmm_rbtree_node *rbtree_node,
    struct devmm_heap_rbtree *rbtree_queue)
{
    return devmm_rbtree_insert(rbtree_node->data.size, &rbtree_node->cache_node,
        rbtree_queue->idle_mapped_cache_tree[DEVMM_MAPPED_RDONLY_TREE]);
}

DVresult devmm_rbtree_insert_idle_rw_mapped_tree(struct devmm_rbtree_node *rbtree_node,
    struct devmm_heap_rbtree *rbtree_queue)
{
    return devmm_rbtree_insert(rbtree_node->data.size, &rbtree_node->cache_node,
        rbtree_queue->idle_mapped_cache_tree[DEVMM_MAPPED_RW_TREE]);
}

static inline DVresult devmm_rbtree_erase(struct multi_rb_node *rb_node, struct rbtree_root *tree_root)
{
    if (devmm_rbtree_is_empty(tree_root)) {
        DEVMM_DRV_ERR("Tree_root is empty.\n");
        return DRV_ERROR_INVALID_VALUE;
    }
    multi_rbtree_erase(tree_root, rb_node);

    return DRV_ERROR_NONE;
}

DVresult devmm_rbtree_erase_idle_va_tree(struct devmm_rbtree_node *rbtree_node, struct devmm_heap_rbtree *rbtree_queue)
{
    return devmm_rbtree_erase(&rbtree_node->va_node, rbtree_queue->idle_va_tree);
}

DVresult devmm_rbtree_erase_idle_size_tree(struct devmm_rbtree_node *rbtree_node,
    struct devmm_heap_rbtree *rbtree_queue)
{
    return devmm_rbtree_erase(&rbtree_node->size_node, rbtree_queue->idle_size_tree);
}

DVresult devmm_rbtree_erase_alloced_tree(struct devmm_rbtree_node *rbtree_node, struct devmm_heap_rbtree *rbtree_queue)
{
    return devmm_rbtree_erase(&rbtree_node->va_node, rbtree_queue->alloced_tree);
}

DVresult devmm_rbtree_erase_idle_readonly_mapped_tree(struct devmm_rbtree_node *rbtree_node,
    struct devmm_heap_rbtree *rbtree_queue)
{
    return devmm_rbtree_erase(&rbtree_node->cache_node,
        rbtree_queue->idle_mapped_cache_tree[DEVMM_MAPPED_RDONLY_TREE]);
}

DVresult devmm_rbtree_erase_idle_rw_mapped_tree(struct devmm_rbtree_node *rbtree_node,
    struct devmm_heap_rbtree *rbtree_queue)
{
    return devmm_rbtree_erase(&rbtree_node->cache_node,
        rbtree_queue->idle_mapped_cache_tree[DEVMM_MAPPED_RW_TREE]);
}

void devmm_assign_rbtree_node_data(uint64_t va, uint64_t size, uint64_t total, uint32_t flag, DVmem_advise advise,
    struct devmm_rbtree_node *rbtree_node)
{
    rbtree_node->data.va = va;
    rbtree_node->data.size = size;
    rbtree_node->data.total = total;
    rbtree_node->data.flag = flag;
    rbtree_node->data.advise = advise;
}

STATIC struct devmm_rbtree_node *devmm_get_rbtree_node_from_list(struct devmm_cache_list *head)
{
    struct devmm_cache_list *list_new = NULL;
    struct devmm_cache_list *last = NULL;

    if (list_empty(&head->list) != 0) {
        list_new = (struct devmm_cache_list *)malloc(sizeof(struct devmm_cache_list));
        if (list_new == NULL) {
            DEVMM_DRV_ERR("Malloc new list memory failed.\n");
            return NULL;
        }
        list_new->is_new = 1;
        return &list_new->cache;  //lint !e429
    }
    last = rb_entry(head->list.prev, struct devmm_cache_list, list);
    list_del_node(&last->list);
    return &last->cache;
}

struct devmm_rbtree_node *devmm_alloc_rbtree_node(struct devmm_heap_rbtree *rbtree_queue)
{
    struct devmm_rbtree_node *rbtree_node = NULL;

    rbtree_node = devmm_get_rbtree_node_from_list(rbtree_queue->head);
    if (rbtree_node == NULL) {
        DEVMM_DRV_ERR("Devmm_alloc_rbtree_node failed.\n");
        return NULL;
    }

    devmm_assign_rbtree_node_data(0, 0, 0, 0, 0, rbtree_node);
    INIT_LIST_HEAD(&rbtree_node->size_node.list);
    INIT_LIST_HEAD(&rbtree_node->va_node.list);
    INIT_LIST_HEAD(&rbtree_node->cache_node.list);

    return rbtree_node;
}

STATIC void devmm_put_rbtree_node_to_list(struct devmm_rbtree_node *rb_node, struct list_node *cache_list)
{
    struct devmm_cache_list *list_node = NULL;

    list_node = rb_entry(rb_node, struct devmm_cache_list, cache);
    list_add_node_tail(&list_node->list, cache_list);
    return;
}

void devmm_free_rbtree_node(struct devmm_rbtree_node *rb_node, struct devmm_heap_rbtree *rbtree_queue)
{
    devmm_put_rbtree_node_to_list(rb_node, &rbtree_queue->head->list);
    return;
}

STATIC void devmm_rbtree_free_nodes(struct rbtree_root *tree_root, struct list_node *cache_list, uint32_t tree_type)
{
    struct devmm_rbtree_node *rbtree_node = NULL;
    struct multi_rb_node *rb_node_list = NULL;
    struct multi_rb_node *rb_node = NULL;
    struct list_node *tmp_head = NULL;
    struct list_node *head = NULL;
    struct rbtree_node *cur = NULL;
    struct rbtree_node *tmp = NULL;

    if (devmm_rbtree_is_empty(tree_root)) {
        return;
    }

    rbtree_node_for_each_prev_safe(cur, tmp, tree_root) {
        rb_node = multi_rbtree_get_node_from_rb_node(cur);
        __rbtree_erase(cur, tree_root);
        if (list_empty(&rb_node->list) == 0) {
            list_for_each_node_safe(head, tmp_head, &rb_node->list) {
                list_del_node(head);
                rb_node_list = rb_entry(head, struct multi_rb_node, list);
                rbtree_node = devmm_get_rbtree_node_by_type(rb_node_list, tree_type);
                if (tree_type == DEVMM_ALLOCED_TREE) {
                    devmm_module_mem_stats_dec(rbtree_node);
                }
                if ((tree_type == DEVMM_ALLOCED_TREE) || (tree_type == DEVMM_IDLE_MAPPED_TREE)) {
                    devmm_rbtree_free_node_resources(rbtree_node);
                }
                devmm_put_rbtree_node_to_list(rbtree_node, cache_list);
            }
        }
        rbtree_node = devmm_get_rbtree_node_by_type(rb_node, tree_type);
        if (tree_type == DEVMM_ALLOCED_TREE) {
            devmm_module_mem_stats_dec(rbtree_node);
        }
        if ((tree_type == DEVMM_ALLOCED_TREE) || (tree_type == DEVMM_IDLE_MAPPED_TREE)) {
            devmm_rbtree_free_node_resources(rbtree_node);
        }
        devmm_put_rbtree_node_to_list(rbtree_node, cache_list);
    }
    tree_root->rbtree_len = 0;
    return;
}

STATIC void devmm_rbtree_clear(struct rbtree_root *root)
{
    struct multi_rb_node *rb_node = NULL;
    struct list_node *tmp_head = NULL;
    struct list_node *head = NULL;
    struct rbtree_node *cur = NULL;
    struct rbtree_node *tmp = NULL;

    if (devmm_rbtree_is_empty(root)) {
        return;
    }

    rbtree_node_for_each_prev_safe(cur, tmp, root) {
        rb_node = multi_rbtree_get_node_from_rb_node(cur);
        __rbtree_erase(cur, root);
        if (list_empty(&rb_node->list) == 0) {
            list_for_each_node_safe(head, tmp_head, &rb_node->list) {
                list_del_node(head);
            }
        }
    }
    root->rbtree_len = 0;
}

STATIC struct rbtree_root *devmm_rbtree_alloc_root(void)
{
    struct rbtree_root *root = NULL;

    root = (struct rbtree_root *)malloc(sizeof(struct rbtree_root));
    if (root == NULL) {
        return NULL;
    }
    root->rbtree_node = NULL;
    root->rbtree_len = 0;

    return root;
}

STATIC struct devmm_cache_list *devmm_rbtree_alloc_cache(uint32_t node_num)
{
    uint64_t list_node_size = sizeof(struct devmm_cache_list);
    struct devmm_cache_list *list_node = NULL;
    struct devmm_cache_list *head = NULL;
    void *tmp = NULL;
    uint32_t i;

    tmp = malloc(list_node_size * node_num);
    if (tmp == NULL) {
        DEVMM_DRV_ERR("Malloc cache failed.\n");
        return NULL;
    }
    head = (struct devmm_cache_list *)tmp;
    head->is_new = 0;
    INIT_LIST_HEAD(&head->list);

    tmp = (void *)((uintptr_t)tmp + list_node_size);
    for (i = 1; i < node_num; i++) {
        list_node = (struct devmm_cache_list *)tmp;
        list_node->is_new = 0;
        list_add_node_tail(&list_node->list, &head->list);
        tmp = (void *)((uintptr_t)tmp + list_node_size);
    }
    return head;
}

STATIC void devmm_rbtree_free_cache(struct devmm_cache_list **head)
{
    struct devmm_cache_list *list_node = NULL;
    struct list_node *tmp_head = NULL;
    struct list_node *cur = NULL;

    if (*head == NULL) {
        return;
    }
    list_for_each_node_safe(cur, tmp_head, &(*head)->list) {
        list_del_node(cur);
        list_node = rb_entry(cur, struct devmm_cache_list, list);
        if (list_node->is_new != 0) {
            free(list_node); //lint !e424
            list_node = NULL;
        }
    }
    free(*head);
    *head = NULL;
}

DVresult devmm_rbtree_alloc(struct devmm_heap_rbtree *rbtree_queue, uint32_t cache_numsize)
{
    uint32_t i, j;

    rbtree_queue->devmm_cache_numsize = cache_numsize;

    rbtree_queue->alloced_tree = devmm_rbtree_alloc_root();
    if (rbtree_queue->alloced_tree == NULL) {
        DEVMM_DRV_ERR("Malloc alloced_tree memory failed.\n");
        goto malloc_alloced_tree_err;
    }

    rbtree_queue->idle_size_tree = devmm_rbtree_alloc_root();
    if (rbtree_queue->idle_size_tree == NULL) {
        DEVMM_DRV_ERR("Malloc idle_size_tree memory failed.\n");
        goto malloc_idle_size_tree_err;
    }

    rbtree_queue->idle_va_tree = devmm_rbtree_alloc_root();
    if (rbtree_queue->idle_va_tree == NULL) {
        DEVMM_DRV_ERR("Malloc idle_va_tree memory failed.\n");
        goto malloc_idle_va_tree_err;
    }

    for (i = 0; i < DEVMM_MAPPED_TREE_TYPE_MAX; i++) {
        rbtree_queue->idle_mapped_cache_tree[i] = devmm_rbtree_alloc_root();
        if (rbtree_queue->idle_mapped_cache_tree[i] == NULL) {
            DEVMM_DRV_ERR("Malloc idle_mapped_cache_tree memory failed.\n");
            goto malloc_cache_tree_or_cache_head_err;
        }
    }

    rbtree_queue->head = devmm_rbtree_alloc_cache(rbtree_queue->devmm_cache_numsize);
    if (rbtree_queue->head == NULL) {
        DEVMM_DRV_ERR("Rbtree alloc cache memory failed.\n");
        goto malloc_cache_tree_or_cache_head_err;
    }

    return DRV_ERROR_NONE;

malloc_cache_tree_or_cache_head_err:
    for (j = 0; j < i; j++) {
        free(rbtree_queue->idle_mapped_cache_tree[j]);
        rbtree_queue->idle_mapped_cache_tree[j] = NULL;
    }
    free(rbtree_queue->idle_va_tree);
    rbtree_queue->idle_va_tree = NULL;
malloc_idle_va_tree_err:
    free(rbtree_queue->idle_size_tree);
    rbtree_queue->idle_size_tree = NULL;
malloc_idle_size_tree_err:
    free(rbtree_queue->alloced_tree);
    rbtree_queue->alloced_tree = NULL;
malloc_alloced_tree_err:
    return DRV_ERROR_OUT_OF_MEMORY;
}

STATIC void devmm_rbtree_free(struct devmm_heap_rbtree *rbtree_queue)
{
    uint32_t i;

    if (rbtree_queue->alloced_tree != NULL) {
        free(rbtree_queue->alloced_tree);
        rbtree_queue->alloced_tree = NULL;
    }
    if (rbtree_queue->idle_size_tree != NULL) {
        free(rbtree_queue->idle_size_tree);
        rbtree_queue->idle_size_tree = NULL;
    }
    if (rbtree_queue->idle_va_tree != NULL) {
        free(rbtree_queue->idle_va_tree);
        rbtree_queue->idle_va_tree = NULL;
    }
    for (i = 0; i < DEVMM_MAPPED_TREE_TYPE_MAX; i++) {
        if (rbtree_queue->idle_mapped_cache_tree[i] != NULL) {
            free(rbtree_queue->idle_mapped_cache_tree[i]);
            rbtree_queue->idle_mapped_cache_tree[i] = NULL;
        }
    }
    devmm_rbtree_free_cache(&rbtree_queue->head);
}

void devmm_rbtree_destory(struct devmm_heap_rbtree *rbtree_queue)
{
    uint32_t i;

    devmm_rbtree_clear(rbtree_queue->idle_va_tree);
    for (i = 0; i < DEVMM_MAPPED_TREE_TYPE_MAX; i++) {
        devmm_rbtree_free_nodes(rbtree_queue->idle_mapped_cache_tree[i], &rbtree_queue->head->list,
            DEVMM_IDLE_MAPPED_TREE);
    }
    devmm_rbtree_free_nodes(rbtree_queue->idle_size_tree, &rbtree_queue->head->list, DEVMM_IDLE_SIZE_TREE);
    devmm_rbtree_free_nodes(rbtree_queue->alloced_tree, &rbtree_queue->head->list, DEVMM_ALLOCED_TREE);

    devmm_rbtree_free(rbtree_queue);
}
