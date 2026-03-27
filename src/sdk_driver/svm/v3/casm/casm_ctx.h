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

#ifndef CASM_CTX_H
#define CASM_CTX_H

#include "casm_src.h"
#include "casm_dst.h"

struct casm_ctx {
    u32 udevid;
    int tgid;
    void *task_ctx;
    struct casm_src_ctx src_ctx;
    struct casm_dst_ctx dst_ctx;
};

struct casm_ctx *casm_ctx_get(u32 udevid, int tgid);
void casm_ctx_put(struct casm_ctx *ctx);

int casm_init_task(u32 udevid, int tgid, void *start_time);
void casm_uninit_task(u32 udevid, int tgid, void *start_time);
void casm_show_task(u32 udevid, int tgid, int feature_id, ka_seq_file_t *seq);
int svm_casm_init(void);
void svm_casm_uninit(void);

#endif

