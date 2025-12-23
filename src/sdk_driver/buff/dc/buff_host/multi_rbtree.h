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

#ifndef MULTI_RBTREE_H
#define MULTI_RBTREE_H

#include <linux/rbtree.h>
#include <linux/list.h>

struct multi_rb_node {
    struct rb_node multi_rbtree_node;
    u64 key;

    struct list_head list;
    bool is_list_first;
};

void multi_rbtree_insert(struct rb_root *root, struct multi_rb_node *node, u64 key);
void multi_rbtree_erase(struct rb_root *root, struct multi_rb_node *node);
struct multi_rb_node *multi_rbtree_get(struct rb_root *root, u64 key);
struct multi_rb_node *multi_rbtree_get_upper_bound(struct rb_root *root, u64 key);
struct multi_rb_node *multi_rbtree_get_node_from_rb_node(struct rb_node *node);

#endif

