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

#ifndef DMA_MAP_KERNEL_H
#define DMA_MAP_KERNEL_H

#include <linux/types.h>

#include "svm_pub.h"
#include "svm_addr_desc.h"
#include "dma_map_flag.h"

struct svm_dma_addr_info {
    bool is_write;

    u64 dma_addr_seg_num;
    u64 first_seg_offset;
    u64 last_seg_len;
    struct svm_dma_addr_seg *seg;
};

/*
    may be not in current context when recycle, so add tgid para
    if handle is not null when dma map, must use svm_dma_unmap_addr_by_handle to unmap
*/
int svm_dma_map_addr(u32 udevid, int tgid, struct svm_global_va *dst_va, u32 flag, void **handle);
int svm_dma_unmap_addr(u32 udevid, int tgid, struct svm_global_va *dst_va);
void svm_dma_unmap_addr_by_handle(void *handle);
int svm_dma_addr_query_by_handle(void *handle, struct svm_global_va *dst_va, struct svm_dma_addr_info *dma_info);

int svm_dma_addr_get(u32 udevid, int tgid, struct svm_global_va *dst_va, struct svm_dma_addr_info *dma_info);
void svm_dma_addr_put(u32 udevid, int tgid, struct svm_global_va *dst_va);

#endif

