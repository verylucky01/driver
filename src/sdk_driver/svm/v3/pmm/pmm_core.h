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

#ifndef PMM_CORE_H
#define PMM_CORE_H

#include "ka_common_pub.h"
#include "ka_memory_pub.h"

#include "pmm_ctx.h"

void pmm_mem_show(struct pmm_ctx *pmm_ctx, ka_seq_file_t *seq);
void pmm_mem_recycle(struct pmm_ctx *pmm_ctx);
u64 pmm_mem_recycle_by_vma(ka_vm_area_struct_t *vma, u32 udevid, int tgid);
int pmm_mem_query(u32 udevid, int tgid, u64 *out_size);

#endif
