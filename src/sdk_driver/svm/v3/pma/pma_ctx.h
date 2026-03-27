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

#ifndef PMA_CTX_H
#define PMA_CTX_H

#include <linux/types.h>
#include "ka_base_pub.h"
#include "ka_list_pub.h"
#include "ka_common_pub.h"

#include "ascend_kernel_hal.h"

struct pma_ctx {
    u32 udevid;
    int tgid;
    void *task_ctx;

    /*
     * Mutual exclusion api
     * r: p2p_get_pages
     * w: recycle
     */
    ka_rw_semaphore_t pipeline_rw_sem;

    ka_rw_semaphore_t rw_sem;
    ka_list_head_t head;
    u64 mem_node_num;
    u64 get_cnt;
    u64 put_cnt;
};

struct pma_mem_node {
    ka_list_head_t node;
    ka_kref_t ref;

    u32 udevid;
    int tgid;

    u64 va;
    u64 size;
    void (*free_callback)(void *data);
    void *data;

    struct p2p_page_table page_table;
};

struct pma_ctx *pma_ctx_get(u32 udevid, int tgid);
void pma_ctx_put(struct pma_ctx *pma_ctx);

void pma_use_pipeline(struct pma_ctx *pma_ctx);
void pma_unuse_pipeline(struct pma_ctx *pma_ctx);
void pma_occupy_pipeline(struct pma_ctx *pma_ctx);
void pma_release_pipeline(struct pma_ctx *pma_ctx);

int pma_init_task(u32 udevid, int tgid, void *start_time);
void pma_uninit_task(u32 udevid, int tgid, void *start_time);
void pma_show_task(u32 udevid, int tgid, int feature_id, ka_seq_file_t *seq);
int pma_init(void);
void pma_uninit(void);

#endif

