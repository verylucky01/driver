/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef SVM_BASE_HEAP_H
#define SVM_BASE_HEAP_H
#include <sys/types.h>
#include <unistd.h>
#include "devmm_virt_interface.h"

DVresult devmm_virt_init_base_heap(struct devmm_virt_heap_mgmt *mgmt);
DVresult devmm_free_to_base_heap(struct devmm_virt_heap_mgmt *mgmt, struct devmm_virt_com_heap *heap, virt_addr_t ptr);
virt_addr_t devmm_alloc_from_base_heap(struct devmm_virt_heap_mgmt *mgmt,
    size_t alloc_size, struct devmm_virt_heap_type *heap_type, DVmem_advise advise, virt_addr_t va);
virt_addr_t devmm_virt_alloc_mem_from_base(struct devmm_virt_heap_mgmt *mgmt, size_t alloc_size, DVmem_advise advise,
    virt_addr_t alloc_ptr);
DVresult devmm_virt_free_mem_to_base(struct devmm_virt_heap_mgmt *mgmt, virt_addr_t ptr);

void devmm_primary_heap_module_mem_stats_dec(struct devmm_virt_com_heap *heap);
#endif /* _SVM_BASE_HEAP_H_ */
