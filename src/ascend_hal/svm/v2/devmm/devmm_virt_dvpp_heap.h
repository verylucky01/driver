/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef DEVMM_VIRT_DVPP_HEAP_H
#define DEVMM_VIRT_DVPP_HEAP_H

#include "devmm_virt_interface.h"
#include "devmm_rbtree/devmm_rbtree.h"

DVresult devmm_init_dvpp_heap_by_devid(uint32_t device);
void devmm_fill_dvpp_heap_type(uint32_t device, struct devmm_virt_heap_type *heap_type);

#endif /* DEVMM_VIRT_DVPP_HEAP_H_ */
