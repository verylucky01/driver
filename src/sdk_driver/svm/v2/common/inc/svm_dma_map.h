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
#ifndef SVM_DMA_MAP_H
#define SVM_DMA_MAP_H

#include <linux/dma-mapping.h>
#include <linux/mm.h>

#include "svm_kernel_msg.h"

/* Will try map continuous pages, the actual mapped blk_num will be returned. */
int devmm_dma_map_normal_pages(u32 devid, ka_page_t **pages, u64 pg_num,
    struct devmm_dma_blk *dma_blks, u64 *blk_num);
int devmm_dma_map_huge_pages(u32 devid, ka_page_t **pages, u64 pg_num,
    struct devmm_dma_blk *dma_blks, u64 *blk_num);
void devmm_dma_unmap_pages(u32 devid, struct devmm_dma_blk *dma_blks, u64 blk_num);

#endif
