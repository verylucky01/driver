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

#ifndef PMA_CORE_H
#define PMA_CORE_H

#include "ka_common_pub.h"

#include "pma_ctx.h"

int pma_mem_node_create(struct pma_ctx *ctx, u64 va, u64 size,
    void (*free_callback)(void *data), void *data, struct pma_mem_node **mem_node);
void pma_mem_node_get(struct pma_mem_node *mem_node);
void pma_mem_node_put(struct pma_mem_node *mem_node);
int pma_mem_node_erase(struct pma_ctx *ctx, struct pma_mem_node *mem_node);

void pma_mem_show(struct pma_ctx *ctx, ka_seq_file_t *seq);
void pma_mem_recycle(struct pma_ctx *ctx);

#endif
