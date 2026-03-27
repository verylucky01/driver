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

#ifndef PBL_RANGE_RBTREE_H
#define PBL_RANGE_RBTREE_H

#include "ka_base_pub.h"

/*
   usage :
        range_rbtree_init
        range_rbtree_insert
        range_rbtree_erase
        range_rbtree_search
        range_rbtree_get_first
*/

struct range_rbtree {
    ka_rb_root_t root;
    u32 node_num;
};

struct range_rbtree_node {
    ka_rb_node_t node;
    u64 start;
    u64 size;
};

/* interface */
static inline void range_rbtree_init(struct range_rbtree *range_tree)
{
    range_tree->root = KA_RB_ROOT;
    range_tree->node_num = 0;
}

static inline int _range_rbtree_insert(struct range_rbtree *range_tree, struct range_rbtree_node *range_node)
{
    ka_rb_node_t **cur_node = &range_tree->root.rb_node, *parent = NULL;

    while (*cur_node) {
        struct range_rbtree_node *cur_range_node = ka_base_rb_entry(*cur_node, struct range_rbtree_node, node);

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

    ka_base_rb_link_node(&range_node->node, parent, cur_node);
    ka_base_rb_insert_color(&range_node->node, &range_tree->root);
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
    ka_base_rb_erase(&range_node->node, &range_tree->root);
    KA_BASE_RB_CLEAR_NODE(&range_node->node);
}

/* interface */
static inline struct range_rbtree_node *range_rbtree_search(struct range_rbtree *range_tree, u64 start, u64 size)
{
    ka_rb_node_t *node = range_tree->root.rb_node;

    while (node) {
        struct range_rbtree_node *range_node = ka_base_rb_entry(node, struct range_rbtree_node, node);

        if ((start + size) <= range_node->start) {
            node = ka_base_get_rb_node_left(node);
        } else if (start >= (range_node->start + range_node->size)) {
            node = ka_base_get_rb_node_right(node);
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
    ka_rb_node_t *node = NULL;

    node = range_tree->root.rb_node;
    if (node != NULL) {
        range_node = ka_base_rb_entry(node, struct range_rbtree_node, node);
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
    ka_rb_node_t *node = range_tree->root.rb_node;

    while (node) {
        struct range_rbtree_node *range_node = ka_base_rb_entry(node, struct range_rbtree_node, node);

        if ((start + size) <= range_node->start) {
            node = ka_base_get_rb_node_left(node);
        } else if (start >= (range_node->start + range_node->size)) {
            node = ka_base_get_rb_node_right(node);
        } else {
            return true;
        }
    }

    return false;
}

#endif

