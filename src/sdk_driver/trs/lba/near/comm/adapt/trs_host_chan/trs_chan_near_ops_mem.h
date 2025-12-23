/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
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
#ifndef TRS_CHAN_NEAR_OPS_MEM_H
#define TRS_CHAN_NEAR_OPS_MEM_H
#include "trs_pub_def.h"
#include "ascend_kernel_hal.h"
#include "trs_chan.h"

void *trs_chan_sq_mem_alloc(struct trs_id_inst *inst, u32 sqe_size, u32 sq_depth, struct trs_chan_mem_attr *mem_attr);
void trs_chan_sq_mem_free(struct trs_id_inst *inst, void *sq_addr, struct trs_chan_mem_attr *mem_attr);

void trs_chan_flush_sqe_cache(struct trs_id_inst *inst, u32 mem_type, u64 pa, u32 len);

int trs_chan_near_sqcq_mem_h2d(struct trs_id_inst *inst, u64 host_addr, u64 *dev_addr, u32 mem_side);
int trs_chan_sq_rsvmem_map(struct trs_id_inst *inst, struct trs_sq_mem_map_para *para, void **sq_dev_vaddr);
void trs_chan_sq_rsvmem_unmap(struct trs_id_inst *inst, struct trs_sq_mem_map_para *para, void *sq_dev_vaddr);

static inline u32 trs_chan_get_sqcq_mem_side(u32 mem_type)
{
    return mem_type & TRS_CHAN_MEM_TYPE_SIDE_MASK;
}
#endif
