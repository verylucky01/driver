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

#ifndef MMS_CTX_H
#define MMS_CTX_H

#include "ka_memory_pub.h"
#include "ka_list_pub.h"

#include "ascend_kernel_hal.h"
#include "pbl_task_ctx.h"

#include "mms_def.h"

/* MSS: module memory statistics. */

struct mms_ctx {
    u32 udevid;
    int tgid;
    void *task_ctx;

    ka_rw_semaphore_t rw_sem;
    u64 uva;
    u64 npage_num;
    ka_page_t **pages;
    struct mms_stats *stats;
    bool is_pfn_map;
};

u32 mms_get_feature_id(void);
struct mms_ctx *mms_ctx_get(u32 udevid, int tgid);
void mms_ctx_put(struct mms_ctx *ctx);

int mms_init_task(u32 udevid, int tgid, void *start_time);
void mms_uninit_task(u32 udevid, int tgid, void *start_time);
void mms_show_task(u32 udevid, int tgid, int feature_id, ka_seq_file_t *seq);
int mms_kern_init(void);
void mms_kern_uninit(void);

#endif