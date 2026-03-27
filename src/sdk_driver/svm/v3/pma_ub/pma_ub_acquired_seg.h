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
#ifndef PMA_UB_ACQUIRED_SEG_H
#define PMA_UB_ACQUIRED_SEG_H

#include "ka_fs_pub.h"
#include "ka_list_pub.h"
#include "ka_common_pub.h"

#include "pbl_range_rbtree.h"

struct pma_ub_acquired_segs_mng {
    ka_rw_semaphore_t rw_sem;
    ka_list_head_t head;
    u64 num;

    u32 udevid;
    int tgid;
};

void pma_ub_acquired_segs_mng_init(struct pma_ub_acquired_segs_mng *mng, u32 udevid, int tgid);
void pma_ub_acquired_segs_mng_uninit(struct pma_ub_acquired_segs_mng *mng);

void pma_ub_acquired_segs_mng_show(struct pma_ub_acquired_segs_mng *mng, ka_seq_file_t *seq);

int pma_ub_add_acquired_seg(struct pma_ub_acquired_segs_mng *mng, u64 va, u64 size,
    int (*invalidate)(u64 invalidate_tag), u64 invalidate_tag);
int pma_ub_del_acquired_seg(struct pma_ub_acquired_segs_mng *mng, u64 va, u64 size);

#endif

