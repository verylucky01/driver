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

#ifndef PMA_UB_CTX_H
#define PMA_UB_CTX_H

#include "ka_base_pub.h"
#include "ka_common_pub.h"

#include "pbl_range_rbtree.h"

#include "pma_ub_seg.h"

struct pma_ub_ctx {
    u32 udevid;
    int tgid;
    void *task_ctx;

    struct pma_ub_seg_mng seg_mng;
};

struct pma_ub_ctx *pma_ub_ctx_get(u32 udevid, int tgid);
void pma_ub_ctx_put(struct pma_ub_ctx *ctx);

int pma_ub_init_task(u32 udevid, int tgid, void *start_time);
void pma_ub_uninit_task(u32 udevid, int tgid, void *start_time);
void pma_ub_show_task(u32 udevid, int tgid, int feature_id, ka_seq_file_t *seq);

int pma_ub_ctx_init(void);
void pma_ub_ctx_uninit(void);

#endif

