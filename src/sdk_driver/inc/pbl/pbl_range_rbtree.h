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

#ifndef PBL_RANGE_RBTREE_H
#define PBL_RANGE_RBTREE_H

#include <linux/rbtree.h>

/*
   usage :
        range_rbtree_init
        range_rbtree_insert
        range_rbtree_erase
        range_rbtree_search
        range_rbtree_get_first
*/

struct range_rbtree {
    struct rb_root root;
    u32 node_num;
};

struct range_rbtree_node {
    struct rb_node node;
    u64 start;
    u64 size;
};

/* interface */
static inline void range_rbtree_init(struct range_rbtree *range_tree)
{
    range_tree->root = RB_ROOT;
    range_tree->node_num = 0;
}

static inline int _range_rbtree_insert(struct range_rbtree *range_tree, struct range_rbtree_node *range_node)
{
    struct rb_node **cur_node = &range_tree->root.rb_node, *parent = NULL;

    while (*cur_node) {
        struct range_rbtree_node *cur_range_node = rb_entry(*cur_node, struct range_rbtree_node, node);

        parent = *cur_node;
        if ((range_node->start + range_node->size) <= cur_range_node->start) {
            cur_node = &((*cur_node)->rb_left);
        } else if (range_node->start >= (cur_range_node->start + cur_range_node->size)) {
            cur_node = &((*cur_node)->rb_right);
        } else {
            return ((cur_range_node->start == range_node->start) && (cur_range_node->size == range_node->size)) ?
                -EEXIST : -EINVAL;
        }
    }

    rb_link_node(&range_node->node, parent, cur_node);
    rb_insert_color(&range_node->node, &range_tree->root);
    return 0;
}

/* interface */
static inline int range_rbtree_insert(struct range_rbtree *range_tree, struct range_rbtree_node *range_node)
{
    int ret;

    ret = _range_rbtree_insert(range_tree, range_node);
    if (ret == 0) {
        range_tree->node_num++;
    }

    return ret;
}

/* interface */
static inline void range_rbtree_erase(struct range_rbtree *range_tree, struct range_rbtree_node *range_node)
{
    range_tree->node_num--;
    rb_erase(&range_node->node, &range_tree->root);
    RB_CLEAR_NODE(&range_node->node);
}

/* interface */
static inline struct range_rbtree_node *range_rbtree_search(struct range_rbtree *range_tree, u64 start, u64 size)
{
    struct rb_node *node = range_tree->root.rb_node;

    while (node) {
        struct range_rbtree_node *range_node = rb_entry(node, struct range_rbtree_node, node);

        if ((start + size) <= range_node->start) {
            node = node->rb_left;
        } else if (start >= (range_node->start + range_node->size)) {
            node = node->rb_right;
        } else if ((start >= range_node->start) && ((start + size) <= (range_node->start + range_node->size))) {
            return range_node;
        } else {
            return NULL;
        }
    }

    return NULL;
}

/* interface */
static inline struct range_rbtree_node *range_rbtree_get_first(struct range_rbtree *range_tree)
{
    struct range_rbtree_node *range_node = NULL;
    struct rb_node *node = NULL;

    node = range_tree->root.rb_node;
    if (node != NULL) {
        range_node = rb_entry(node, struct range_rbtree_node, node);
    }

    return range_node;
}

/* interface */
static inline struct range_rbtree_node *range_rbtree_erase_one(struct range_rbtree *range_tree)
{
    struct range_rbtree_node *node = NULL;

    node = range_rbtree_get_first(range_tree);
    if (node != NULL) {
        range_rbtree_erase(range_tree, node);
    }

    return node;
}

/* interface, check [start, size) is in all or partial in range tree  */
static inline bool range_rbtree_check_exist(struct range_rbtree *range_tree, u64 start, u64 size)
{
    struct rb_node *node = range_tree->root.rb_node;

    while (node) {
        struct range_rbtree_node *range_node = rb_entry(node, struct range_rbtree_node, node);

        if ((start + size) <= range_node->start) {
            node = node->rb_left;
        } else if (start >= (range_node->start + range_node->size)) {
            node = node->rb_right;
        } else {
            return true;
        }
    }

    return false;
}

#endif

