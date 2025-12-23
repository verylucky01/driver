/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef DEVMM_CACHE_COHERENCE_H
#define DEVMM_CACHE_COHERENCE_H
#include "ascend_hal.h"
#include "devmm_virt_list.h"
#include "svm_ioctl.h"

int devmm_init_convert_task_mgmt(void);
int devmm_convert_pre(struct devmm_mem_convrt_addr_para *convert_para, void **priv);
void devmm_convert_post(struct devmm_mem_convrt_addr_para *convert_para, void **priv, void *dma_addr);
int devmm_destroy_convert(void *dma_ptr);
int devmm_destroy_convert_task_node(void *priv);
int devmm_memcpy2d_cache_incoherence(struct devmm_mem_copy2d_para *copy2d_para, int *flag);

#endif
