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

#ifndef KA_RBTREE_H
#define KA_RBTREE_H

#include <linux/rbtree.h>

typedef unsigned long (*rb_handle_func)(struct rb_node *node);

int ka_rb_erase(struct rb_root *root, struct rb_node *node);
int ka_rb_insert(struct rb_root *root, struct rb_node *node, rb_handle_func get_handle);
struct rb_node *ka_rb_search(struct rb_root *root, unsigned long handle, rb_handle_func get_handle);
struct rb_node *ka_rb_erase_one_node(struct rb_root *root);

#endif /* KA_RBTREE_H */

