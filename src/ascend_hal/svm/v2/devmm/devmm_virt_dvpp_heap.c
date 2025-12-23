/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "ascend_hal.h"
#include "devmm_virt_com_heap.h"
#include "devmm_virt_comm.h"
#include "devmm_virt_interface.h"
#include "devmm_svm_init.h"
#include "devmm_virt_dvpp_heap.h"

STATIC struct devmm_com_heap_ops g_dvpp_heap_op = {
    devmm_virt_heap_alloc_device,
    devmm_virt_heap_free_pages
};

static void devmm_fill_dvpp_heap_info(struct devmm_virt_heap_mgmt *mgmt,
    struct devmm_virt_heap_type *heap_type, uint32_t device, struct devmm_virt_heap_para *heap_info)
{
    heap_info->start = mgmt->dvpp_start + DEVMM_DVPP_HEAP_RESERVATION_SIZE * device;
    heap_info->heap_size = mgmt->dvpp_mem_size[device];
    heap_info->page_size = DEVMM_4K_PAGE_SIZE;
    heap_info->kernel_page_size = devmm_virt_get_kernel_page_size_by_heap_type(mgmt,
        heap_type->heap_type, heap_type->heap_sub_type);
    heap_info->map_size = DEVMM_HUGE_ALLOC_MAP_SIZE;
    heap_info->need_cache_thres[DEVMM_MEM_NORMAL] = DEVMM_HUGE_CACHE_NODE_SIZE_THRES;
    heap_info->need_cache_thres[DEVMM_MEM_RDONLY] = DEVMM_HUGE_READONLY_CACHE_NODE_SIZE_THRES;
    heap_info->is_limited = true;
    heap_info->is_base_heap = false;
}

void devmm_fill_dvpp_heap_type(uint32_t device, struct devmm_virt_heap_type *heap_type)
{
    /* dvpp heap just has one heap, arrangement attribute hbm mem_type/huge heap type */
    heap_type->heap_type = DEVMM_HEAP_HUGE_PAGE;
    heap_type->heap_list_type = devmm_heap_list_type_by_device(device);
    heap_type->heap_sub_type = SUB_DVPP_TYPE;
    heap_type->heap_mem_type = DEVMM_HBM_MEM;
}

DVresult devmm_init_dvpp_heap_by_devid(uint32_t device)
{
    struct devmm_virt_heap_type heap_type = {0};
    struct devmm_virt_heap_para heap_info = {0};
    struct devmm_virt_heap_mgmt *mgmt = NULL;
    DVresult ret;

    mgmt = (struct devmm_virt_heap_mgmt *)devmm_virt_get_heap_mgmt();
    if (mgmt == NULL) {
        DEVMM_DRV_ERR("Get heap management error.\n");
        return DRV_ERROR_INVALID_HANDLE;
    }

    devmm_fill_dvpp_heap_type(device, &heap_type);
    devmm_fill_dvpp_heap_info(mgmt, &heap_type, device, &heap_info);

    ret = devmm_virt_init_heap_customize(mgmt, &heap_type, &heap_info, &g_dvpp_heap_op);
    if (ret != DRV_ERROR_NONE) {
        DEVMM_DRV_WARN("Init dvpp heap fail. (ret=%d; devid=%u)\n", ret, device);
    }
    return ret;
}

