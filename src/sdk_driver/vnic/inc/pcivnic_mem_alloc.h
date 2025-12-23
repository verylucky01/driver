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

#ifndef PCIVNIC_MEM_ALLOC_H
#define PCIVNIC_MEM_ALLOC_H

#include <linux/types.h>
#include <linux/printk.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/mman.h>
#include <linux/dma-mapping.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/version.h>
#include <linux/nodemask.h>

#include "pbl_ka_memory.h"
#include "ascend_hal_define.h"

#define pcivnic_kvzalloc(size, flags) ka_kvzalloc(size, flags, ka_get_module_id(HAL_MODULE_TYPE_VNIC, KA_SUB_MODULE_TYPE_0))
#define pcivnic_kvfree(addr) ka_kvfree(addr, ka_get_module_id(HAL_MODULE_TYPE_VNIC, KA_SUB_MODULE_TYPE_0))

#define pcivnic_kmalloc(size, flags) \
    ka_kmalloc(size, flags, ka_get_module_id(HAL_MODULE_TYPE_VNIC, KA_SUB_MODULE_TYPE_0))
#define pcivnic_kzalloc(size, flags) \
    ka_kzalloc(size, flags, ka_get_module_id(HAL_MODULE_TYPE_VNIC, KA_SUB_MODULE_TYPE_0))
#define pcivnic_kcalloc(size, flags) \
    ka_kcalloc(size, flags, ka_get_module_id(HAL_MODULE_TYPE_VNIC, KA_SUB_MODULE_TYPE_0))
#define pcivnic_kfree(addr) ka_kfree(addr, ka_get_module_id(HAL_MODULE_TYPE_VNIC, KA_SUB_MODULE_TYPE_0))

#define pcivnic_vzalloc(size)  ka_vzalloc(size, ka_get_module_id(HAL_MODULE_TYPE_VNIC, KA_SUB_MODULE_TYPE_0))
#define pcivnic_vfree(addr) ka_vfree(addr, ka_get_module_id(HAL_MODULE_TYPE_VNIC, KA_SUB_MODULE_TYPE_0))

#define pcivnic_alloc_pages(gfp_mask, order) \
    ka_alloc_pages(gfp_mask, order, ka_get_module_id(HAL_MODULE_TYPE_VNIC, KA_SUB_MODULE_TYPE_0))
#define pcivnic_free_pages(addr, order) \
    ka_free_pages(addr, order, ka_get_module_id(HAL_MODULE_TYPE_VNIC, KA_SUB_MODULE_TYPE_0))

#define pcivnic_alloc_pages_node(nid, gfp_mask, order) \
    ka_alloc_pages_node(nid, gfp_mask, order, ka_get_module_id(HAL_MODULE_TYPE_VNIC, KA_SUB_MODULE_TYPE_0))
#define __pcivnic_free_pages(page, order) \
    __ka_free_pages(page, order, ka_get_module_id(HAL_MODULE_TYPE_VNIC, KA_SUB_MODULE_TYPE_0))
#endif

