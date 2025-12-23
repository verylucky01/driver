/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "devmm_cache_coherence.h"
#include "devmm_virt_interface.h"

int devmm_init_convert_task_mgmt(void)
{
    return DRV_ERROR_NONE;
}

int devmm_destroy_convert_task_node(void *priv)
{
    (void)priv;
    return DRV_ERROR_NONE;
}

int devmm_convert_pre(struct devmm_mem_convrt_addr_para *convert_para, void **priv)
{
    (void)convert_para;
    (void)priv;
    return DRV_ERROR_NONE;
}

void devmm_convert_post(struct devmm_mem_convrt_addr_para *convert_para, void **priv, void *dma_addr)
{
    (void)convert_para;
    (void)priv;
    (void)dma_addr;
    return;
}

int devmm_destroy_convert(void *dma_ptr)
{
    (void)dma_ptr;
    return DRV_ERROR_NONE;
}

int devmm_memcpy2d_cache_incoherence(struct devmm_mem_copy2d_para *copy2d_para, int *flag)
{
    (void)copy2d_para;
    (void)flag;
    return DRV_ERROR_NONE;
}
