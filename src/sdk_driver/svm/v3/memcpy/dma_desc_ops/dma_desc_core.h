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
#ifndef DMA_DESC_CORE_H
#define DMA_DESC_CORE_H

#include "ascend_hal_define.h"

#include "copy_pub.h"
#include "dma_desc_ctx.h"

int dma_desc_convert(struct dma_desc_ctx *ctx, struct copy_va_info *info, struct DMA_ADDR *dma_desc);
int dma_desc_destroy(struct dma_desc_ctx *ctx, struct DMA_ADDR *dma_desc);
int dma_desc_submit(struct dma_desc_ctx *ctx, struct DMA_ADDR *dma_desc, int sync_flag);
int dma_desc_wait(struct dma_desc_ctx *ctx, struct DMA_ADDR *dma_desc);

int dma_desc_convert_2d(struct dma_desc_ctx *ctx, struct copy_2d_va_info *info, struct DMA_ADDR *dma_desc);

int dma_desc_ioctl_init(void);
int dma_desc_2d_ioctl_init(void);

#endif
