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
#ifndef SVM_SLAB_H
#define SVM_SLAB_H

#include <linux/types.h>

#include "pbl_ka_memory.h"
#include "ascend_hal_define.h"
/*
 * KA_SUB_MODULE_TYPE_0 : ctrl memory
 * KA_SUB_MODULE_TYPE_1 : ai memory normal page
 * KA_SUB_MODULE_TYPE_2 : ai memory huge page
 */

/* KA_SUB_MODULE_TYPE_0 : ctrl memory */

#define svm_kmalloc(size, flags) ka_kmalloc(size, flags, ka_get_module_id(HAL_MODULE_TYPE_DEVMM, KA_SUB_MODULE_TYPE_0))
#define svm_kzalloc(size, flags) ka_kzalloc(size, flags, ka_get_module_id(HAL_MODULE_TYPE_DEVMM, KA_SUB_MODULE_TYPE_0))
#define svm_kcalloc(size, flags) ka_kcalloc(size, flags, ka_get_module_id(HAL_MODULE_TYPE_DEVMM, KA_SUB_MODULE_TYPE_0))
#define svm_kfree(addr) ka_kfree(addr, ka_get_module_id(HAL_MODULE_TYPE_DEVMM, KA_SUB_MODULE_TYPE_0))

#define svm_vzalloc(size) ka_vzalloc(size, ka_get_module_id(HAL_MODULE_TYPE_DEVMM, KA_SUB_MODULE_TYPE_0))

#define svm_vmalloc(size, gfp_mask, prot) \
    __ka_vmalloc(size, gfp_mask, prot, ka_get_module_id(HAL_MODULE_TYPE_DEVMM, KA_SUB_MODULE_TYPE_0))

#define svm_vfree(addr) ka_vfree(addr, ka_get_module_id(HAL_MODULE_TYPE_DEVMM, KA_SUB_MODULE_TYPE_0))

#define svm_get_free_pages(gfp_mask, order) \
    ka_get_free_pages(gfp_mask, order, ka_get_module_id(HAL_MODULE_TYPE_DEVMM, KA_SUB_MODULE_TYPE_0))

#define svm__exact(size, gfp_mask) \
    ka__exact(size, gfp_mask, ka_get_module_id(HAL_MODULE_TYPE_DEVMM, KA_SUB_MODULE_TYPE_0))
#define svm_free_pages_exact(virt, size) \
    ka_free_pages_exact(virt, size, ka_get_module_id(HAL_MODULE_TYPE_DEVMM, KA_SUB_MODULE_TYPE_0))

#define svm_kvzalloc(size, flags) \
    ka_kvzalloc(size, flags, ka_get_module_id(HAL_MODULE_TYPE_DEVMM, KA_SUB_MODULE_TYPE_0))
#define svm_kvmalloc(size, flags) \
    ka_kvmalloc(size, flags, ka_get_module_id(HAL_MODULE_TYPE_DEVMM, KA_SUB_MODULE_TYPE_0))
#define svm_kvfree(addr) ka_kvfree(addr, ka_get_module_id(HAL_MODULE_TYPE_DEVMM, KA_SUB_MODULE_TYPE_0))

/* KA_SUB_MODULE_TYPE_1 : ai memory normal page */
#define svm_kvmalloc_node(size, flags, node) ka_kvmalloc_node(size, flags, node, ka_get_module_id(HAL_MODULE_TYPE_DEVMM, KA_SUB_MODULE_TYPE_1))
#define svm_kvfree_node(addr)  ka_kvfree(addr, ka_get_module_id(HAL_MODULE_TYPE_DEVMM, KA_SUB_MODULE_TYPE_1))

#define svm_kvmalloc_node(size, flags, node) \
    ka_kvmalloc_node(size, flags, node, ka_get_module_id(HAL_MODULE_TYPE_DEVMM, KA_SUB_MODULE_TYPE_1))
#define svm_kvfree_node(addr) ka_kvfree(addr, ka_get_module_id(HAL_MODULE_TYPE_DEVMM, KA_SUB_MODULE_TYPE_1))
#endif
