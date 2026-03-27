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

#ifndef PMM_CTX_H
#define PMM_CTX_H

#include "ka_base_pub.h"
#include "ka_common_pub.h"
#include "ka_memory_pub.h"

#include "va_mng.h"

struct pmm_ctx {
    u32 udevid;
    int tgid;
    void *task_ctx;

    ka_mm_struct_t *mm;

    ka_atomic64_t pa_size;

    u64 base_va;
    u64 total_size;
    u64 size_per_bit;
    u64 nbits;
    ka_rw_semaphore_t rwsem;
    u32 *bit_size_stats;
    ka_vm_area_struct_t *recycling_vma;
};

#define PMM_MEM_SIZE_PER_BIT SVM_VA_RESERVE_ALIGN

struct pmm_ctx *pmm_ctx_get(u32 udevid, int tgid);
void pmm_ctx_put(struct pmm_ctx *pmm_ctx);

int pmm_init_task(u32 udevid, int tgid, void *start_time);
void pmm_uninit_task(u32 udevid, int tgid, void *start_time);
void pmm_show_task(u32 udevid, int tgid, int feature_id, ka_seq_file_t *seq);
int pmm_init(void);
void pmm_uninit(void);

#endif

