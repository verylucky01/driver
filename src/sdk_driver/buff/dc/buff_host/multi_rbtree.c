/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
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

#include "multi_rbtree.h"

static struct rb_node *rbtree_insert_get_node(struct rb_root *root, struct rb_node *node, u64 key)
{
    struct rb_node **tmp = &root->rb_node;
    struct rb_node *parent = NULL;

    while (*tmp != NULL) {
        struct multi_rb_node *cur_node = rb_entry(*tmp, struct multi_rb_node, multi_rbtree_node);

        parent = *tmp;
        if (key < cur_node->key) {
            tmp = &((*tmp)->rb_left);
        } else if (key > cur_node->key) {
            tmp = &((*tmp)->rb_right);
        } else {
            /* return the same key */
            return *tmp;
        }
    }

    /* Add new node and rebalance tree. */
    rb_link_node(node, parent, tmp);
    rb_insert_color(node, root);

    return node;
}

void multi_rbtree_insert(struct rb_root *root, struct multi_rb_node *node, u64 key)
{
    struct rb_node *rb_node = NULL;

    node->key = key;
    rb_node = rbtree_insert_get_node(root, &node->multi_rbtree_node, key);
    if (rb_node != &node->multi_rbtree_node) {
        struct multi_rb_node *tmp = NULL;

        node->is_list_first = false;
        tmp = rb_entry(rb_node, struct multi_rb_node, multi_rbtree_node);
        list_add_tail(&node->list, &tmp->list);
    } else {
        INIT_LIST_HEAD(&node->list);
        node->is_list_first = true;
    }
}

void multi_rbtree_erase(struct rb_root *root, struct multi_rb_node *node)
{
    if (list_empty(&node->list) == 1) {
        rb_erase(&node->multi_rbtree_node, root);
    } else {
        if (node->is_list_first) {
            struct multi_rb_node *tmp = NULL;

            tmp = rb_entry(node->list.next, struct multi_rb_node, list);
            rb_replace_node(&node->multi_rbtree_node, &tmp->multi_rbtree_node, root);
            tmp->is_list_first = true;
        }
        list_del(&node->list);
    }
}

static inline struct multi_rb_node *multi_rbtree_get_with_list(struct multi_rb_node *node)
{
    return (list_empty(&node->list) == 1) ? node :
        rb_entry(node->list.prev, struct multi_rb_node, list);
}

static struct rb_node *rbtree_get(struct rb_root *root, u64 key)
{
    struct rb_node *tmp = root->rb_node;

    while (tmp != NULL) {
        struct multi_rb_node *cur_node = NULL;

        cur_node = rb_entry(tmp, struct multi_rb_node, multi_rbtree_node);
        if (key < cur_node->key) {
            tmp = tmp->rb_left;
        } else if (key > cur_node->key) {
            tmp = tmp->rb_right;
        } else {
            return tmp;
        }
    }
    return NULL;
}

struct multi_rb_node *multi_rbtree_get(struct rb_root *root, u64 key)
{
    struct rb_node *rb_node = NULL;

    rb_node = rbtree_get(root, key);
    if (rb_node != NULL) {
        struct multi_rb_node *tmp = NULL;

        tmp = rb_entry(rb_node, struct multi_rb_node, multi_rbtree_node);
        return multi_rbtree_get_with_list(tmp);
    }

    return NULL;
}

static struct rb_node *rbtree_get_upper_bound(struct rb_root *root, u64 key)
{
    struct multi_rb_node *tmp_upper = NULL;
    struct rb_node *tmp = root->rb_node;

    while (tmp != NULL) {
        struct multi_rb_node *cur_node = NULL;

        cur_node = rb_entry(tmp, struct multi_rb_node, multi_rbtree_node);
        if (key < cur_node->key) {
            tmp_upper = cur_node;
            tmp = tmp->rb_left;
        } else if (key > cur_node->key) {
            tmp = tmp->rb_right;
        } else {
            return rb_next(&cur_node->multi_rbtree_node);
        }
    }
    return (tmp_upper != NULL) ? &tmp_upper->multi_rbtree_node : NULL;
}

struct multi_rb_node *multi_rbtree_get_upper_bound(struct rb_root *root, u64 key)
{
    struct rb_node *rb_node = NULL;

    rb_node = rbtree_get_upper_bound(root, key);
    if (rb_node != NULL) {
        struct multi_rb_node *tmp = NULL;

        tmp = rb_entry(rb_node, struct multi_rb_node, multi_rbtree_node);
        return multi_rbtree_get_with_list(tmp);
    }

    return NULL;
}

struct multi_rb_node *multi_rbtree_get_node_from_rb_node(struct rb_node *node)
{
    return rb_entry(node, struct multi_rb_node, multi_rbtree_node);
}
