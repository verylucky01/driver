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

#ifndef SMP_CTX_H
#define SMP_CTX_H

#include "ka_base_pub.h"
#include "ka_common_pub.h"
#include "ka_task_pub.h"

#include "pbl_range_rbtree.h"
#include "pbl_kref_safe.h"

struct smp_ctx {
    u32 udevid;
    int tgid;
    void *task_ctx;
    ka_rwlock_t lock;
    struct range_rbtree range_tree;
};

struct smp_mem_node {
    struct range_rbtree_node range_node;
    ka_atomic_t refcnt;
    u32 flag;
    int status;
};

struct smp_ctx *smp_ctx_get(u32 udevid, int tgid);
void smp_ctx_put(struct smp_ctx *smp_ctx);

int smp_init_task(u32 udevid, int tgid, void *start_time);
void smp_uninit_task(u32 udevid, int tgid, void *start_time);
void smp_show_task(u32 udevid, int tgid, int feature_id, ka_seq_file_t *seq);
int svm_smp_init(void);
void svm_smp_uninit(void);

#endif

