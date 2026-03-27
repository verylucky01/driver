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

#ifndef CASM_SRC_H
#define CASM_SRC_H

#include "ka_task_pub.h"
#include "ka_list_pub.h"
#include "ka_common_pub.h"

#include "svm_addr_desc.h"
#include "casm_kernel.h"

/* just for quickly release */
struct casm_src_ctx {
    ka_rwlock_t lock;
    ka_list_head_t head;
};

struct casm_src_node {
    ka_list_head_t node;
    struct svm_global_va src_va;
    struct casm_src_ex src_ex;
    u64 key;
};

void casm_src_ctx_init(u32 udevid, struct casm_src_ctx *src_ctx);
void casm_src_ctx_uninit(u32 udevid, struct casm_src_ctx *src_ctx);
int casm_add_src_node(u32 udevid, int tgid, struct casm_src_node *node);
void casm_del_src_node(u32 udevid, int tgid, struct casm_src_node *node);
void casm_src_ctx_show(struct casm_src_ctx *src_ctx, ka_seq_file_t *seq);

#endif

