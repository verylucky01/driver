/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef BBOX_LIST_H
#define BBOX_LIST_H

#include <stdbool.h>
#include "list.h"
#include "bbox_int.h"
#include "bbox_system_api.h"

struct bbox_list_node {
    struct list_head list;
    const void *data;
};

struct bbox_list {
    struct list_head list;
    u32 cnt;
    bbox_lock_t lock;
    bool valid;
};

typedef s32 (*bbox_list_elem_func)(const buff *, const arg_void *);

bbox_status bbox_list_init(struct bbox_list *bbox_list_);
bbox_status bbox_list_destory(struct bbox_list *bbox_list_);
bbox_status bbox_list_insert(struct bbox_list *bbox_list_, const buff *data);
bbox_status bbox_list_remove(struct bbox_list *bbox_list_, const buff *data);
void *bbox_list_take_out(struct bbox_list *bbox_list_);
void *bbox_list_for_each(struct bbox_list *bbox_list_, const bbox_list_elem_func func, const arg_void *arg);
u32 bbox_list_get_node_num(struct bbox_list *bbox_list_);
const void *bbox_list_for_each_reverse(struct bbox_list *bbox_list_, const bbox_list_elem_func func, const arg_void *arg);

#endif /* BBOX_LIST_H */
