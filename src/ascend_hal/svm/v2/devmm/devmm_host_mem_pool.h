/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef DEVMM_HOST_MEM_POOL_H
#define DEVMM_HOST_MEM_POOL_H

void devmm_host_mem_pool_init(uint32_t devid);
DVresult devmm_host_mem_pool_uninit(uint32_t devid);
void *devmm_host_mem_pool_get(uint32_t devid, size_t size, void **cache_va);
void devmm_host_mem_pool_put(uint32_t devid, void *fd);
void devmm_restore_host_mem_pool(void);

#endif
