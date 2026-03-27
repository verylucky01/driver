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
#ifndef DMA_COPY_H
#define DMA_COPY_H

#include "comm_kernel_interface.h"

typedef void (*dma_finish_notify)(void *, u32, u32);

int svm_dma_sync_cpy(u32 udevid, struct devdrv_dma_node *dma_nodes, u32 cnt, u32 instance);
int svm_dma_async_cpy(u32 udevid, struct devdrv_dma_node *dma_nodes, u32 cnt,
    dma_finish_notify call_back, void *priv, u32 instance);

#endif
