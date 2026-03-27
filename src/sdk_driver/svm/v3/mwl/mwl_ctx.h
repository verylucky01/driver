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

#ifndef MWL_CTX_H
#define MWL_CTX_H

#include "ka_common_pub.h"
#include "ka_list_pub.h"

#include "pbl_range_rbtree.h"
#include "pbl_kref_safe.h"

struct mwl_ctx {
    u32 udevid;
    int tgid;
    void *task_ctx;
    ka_rw_semaphore_t rwsem;
    struct range_rbtree range_tree;
};

struct task_node {
    ka_list_head_t list;
    u32 server_id;
    int tgid;
    unsigned long set_time;
};

struct mwl_mem_node {
    struct range_rbtree_node range_node;
    ka_list_head_t task_list;
    u64 start;
    u64 size;
    u32 node_num;
};

struct mwl_ctx *mwl_ctx_get(u32 udevid, int tgid);
void mwl_ctx_put(struct mwl_ctx *mwl_ctx);

int mwl_init_task(u32 udevid, int tgid, void *start_time);
void mwl_uninit_task(u32 udevid, int tgid, void *start_time);
void mwl_show_task(u32 udevid, int tgid, int feature_id, ka_seq_file_t *seq);
int svm_mwl_init(void);
void svm_mwl_uninit(void);

#endif

