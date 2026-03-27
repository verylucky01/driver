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

#ifndef UBMM_CTX_H
#define UBMM_CTX_H

#include "ka_common_pub.h"
#include "ka_task_pub.h"
#include "ka_hashtable_pub.h"

#define UBMM_NODE_HASH_BIT    10

struct ubmm_ctx {
    u32 udevid;
    int tgid;
    void *task_ctx;
    ka_mutex_t mutex;
    KA_DECLARE_HASHTABLE(htable, UBMM_NODE_HASH_BIT);
};

struct ubmm_node {
    ka_hlist_node_t link;
    int ref;
    u64 va;
    u64 size;
    u64 uba;
};

struct ubmm_ctx *ubmm_ctx_get(u32 udevid, int tgid);
void ubmm_ctx_put(struct ubmm_ctx *ctx);

#endif

