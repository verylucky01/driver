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

#ifndef CASM_DST_H
#define CASM_DST_H

#include "ka_common_pub.h"
#include "ka_task_pub.h"

#include "pbl_range_rbtree.h"

struct casm_dst_ctx {
    ka_rwlock_t lock;
    struct range_rbtree range_tree;
};

void casm_dst_ctx_init(u32 udevid, struct casm_dst_ctx *dst_ctx);
void casm_dst_ctx_uninit(u32 udevid, int tgid, struct casm_dst_ctx *dst_ctx);

void casm_dst_ctx_show(struct casm_dst_ctx *dst_ctx, ka_seq_file_t *seq);

int casm_mem_pin(u32 udevid, u64 va, u64 size, u64 key);
int casm_mem_unpin(u32 udevid, int tgid, u64 va, u64 size);
int casm_dst_init(void);

#endif

