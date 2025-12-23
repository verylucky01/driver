/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include <stdio.h>

#include "drv_user_common.h"

int drv_user_list_empty(const struct list_head *head)
{
    return (head->next) == head;
}

static inline void drv_user_list_add(struct list_head *new_node, struct list_head *prev, struct list_head *next)
{
    next->prev = new_node;
    new_node->next = next;
    new_node->prev = prev;
    prev->next = new_node;
}

void drv_user_list_add_tail(struct list_head *new_node, struct list_head *head)
{
    drv_user_list_add(new_node, head->prev, head);
}

void drv_user_list_add_head(struct list_head *new_node, struct list_head *head)
{
    drv_user_list_add(new_node, head, head->next);
}

void drv_user_list_del(struct list_head *entry)
{
    entry->next->prev = entry->prev;
    entry->prev->next = entry->next;

    entry->next = NULL;
    entry->prev = NULL;
}

void drv_user_list_for_each(struct list_head *head, drv_list_handle_func func)
{
    struct list_head *pos = NULL, *n = NULL;

    list_for_each_safe(pos, n, head) {
        func(pos);
    }
}


