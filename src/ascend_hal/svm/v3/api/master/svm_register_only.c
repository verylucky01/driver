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

#include "pbl_uda_user.h"

#include "svm_log.h"
#include "svm_register.h"
#include "svm_register_pcie_th.h"
#include "svm_pipeline.h"
#include "svm_dbi.h"
#include "va_allocator.h"
#include "malloc_mng.h"

static inline u32 svm_get_register_map_type(uint32_t flag)
{
    u32 mask = ((1U << MEM_PROC_TYPE_BIT) - 1);
    return (flag & mask);
}

static inline u32 svm_get_register_proc_type(uint32_t flag)
{
    return (flag >> MEM_PROC_TYPE_BIT);
}

static int svm_register_dev_flag_check(u32 devid, u32 flag)
{
    u32 map_type = svm_get_register_map_type(flag);
    int task_type = (int)svm_get_register_proc_type(flag);

    if (map_type >= HOST_REGISTER_MAX_TPYE) {
        return DRV_ERROR_NOT_SUPPORT;
    }

    if (task_type != DEVDRV_PROCESS_CP1) {
        return DRV_ERROR_NOT_SUPPORT;
    }

    if ((map_type != DEV_SVM_MAP_HOST) && (devid >= SVM_MAX_DEV_NUM)) {
        svm_err("Invalid devid. (map_type=%u; devid=%u)\n", map_type, devid);
        return DRV_ERROR_INVALID_DEVICE;
    }

    return 0;
}

drvError_t halHostRegister(void *src_ptr, UINT64 size, UINT32 flag, UINT32 devid, void **dst_ptr)
{
    u64 dst_va;
    u64 va = (u64)(uintptr_t)(src_ptr);
    u32 map_type = svm_get_register_map_type(flag);
    int ret;

    if (dst_ptr == NULL) {
        svm_err("dst_ptr is null.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    if (src_ptr == NULL) {
        svm_err("srcPtr is null.\n");
        return DRV_ERROR_PARA_ERROR;
    }

    if (svm_is_valid_range(va, size) == false) {
        svm_err("Invalid para. (va=0x%llx; size=0x%llx)\n", va, size);
        return DRV_ERROR_PARA_ERROR;
    }

    ret = svm_register_dev_flag_check(devid, flag);
    if (ret != 0) {
        return (drvError_t)ret;
    }

    switch (map_type) {
        case HOST_MEM_MAP_DEV:
            /* todo add dev method check */
            ret = DRV_ERROR_NOT_SUPPORT;
            break;
        case HOST_SVM_MAP_DEV:
            /* todo add dev method check */
            ret = DRV_ERROR_NOT_SUPPORT;
            break;
        case DEV_SVM_MAP_HOST:
            ret = svm_register_to_peer(va, size, svm_get_host_devid());
            dst_va = va;
            break;
        case HOST_MEM_MAP_DEV_PCIE_TH:
            ret = svm_register_pcie_th(va, size, 0, devid, &dst_va);
            break;
        case HOST_IO_MAP_DEV:
            ret = svm_register_pcie_th(va, size, SVM_REGISTER_PCIE_TH_FLAG_VA_IO_MAP, devid, &dst_va);
            break;
        default:
            return DRV_ERROR_NOT_SUPPORT;
    }

    *dst_ptr = (ret == 0) ? (void *)(uintptr_t)dst_va : *dst_ptr;

    return (drvError_t)ret;
}

drvError_t halHostUnregisterEx(void *src_ptr, UINT32 devid, UINT32 flag)
{
    u64 va = (u64)(uintptr_t)(src_ptr);
    u32 map_type = svm_get_register_map_type(flag);
    int ret;

    if (src_ptr == NULL) {
        svm_err("Ptr is null.\n");
        return DRV_ERROR_PARA_ERROR;
    }

    ret = svm_register_dev_flag_check(devid, flag);
    if (ret != 0) {
        return (drvError_t)ret;
    }

    switch (map_type) {
        case HOST_MEM_MAP_DEV:
            /* todo add dev method check */
            ret = DRV_ERROR_NOT_SUPPORT;
            break;
        case HOST_SVM_MAP_DEV:
            /* todo add dev method check */
            ret = DRV_ERROR_NOT_SUPPORT;
            break;
        case DEV_SVM_MAP_HOST:
            ret = svm_unregister_to_peer(va, svm_get_host_devid());
            break;
        case HOST_MEM_MAP_DEV_PCIE_TH:
            ret = svm_unregister_pcie_th(va, devid);
            break;
        case HOST_IO_MAP_DEV:
            ret = svm_unregister_pcie_th(va, devid);
            break;
        default:
            return DRV_ERROR_NOT_SUPPORT;
    }

    return (drvError_t)ret;
}

static int svm_get_unregister_map_type(u64 src_va, u32 devid, u32 *flag)
{
    struct svm_prop prop;
    int ret;

    if (svm_va_is_pcie_th_register(src_va, devid)) {
        *flag = HOST_MEM_MAP_DEV_PCIE_TH;
    } else {
        if (svm_va_is_in_range(src_va, 1ULL)) {
            ret = svm_get_prop(src_va, &prop);
            if (ret != DRV_ERROR_NONE) {
                svm_err("Invalid addr. (ret=%d; src_va=0x%llx)\n", ret, src_va);
                return DRV_ERROR_PARA_ERROR;
            }

            *flag = (prop.devid == svm_get_host_devid()) ? HOST_SVM_MAP_DEV : DEV_SVM_MAP_HOST;
        } else {
            *flag = HOST_MEM_MAP_DEV;
        }
    }

    return DRV_ERROR_NONE;
}

drvError_t halHostUnregister(void *src_ptr, UINT32 devid)
{
    u32 flag;
    int ret;

    ret = svm_get_unregister_map_type((u64)src_ptr, devid, &flag);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    return halHostUnregisterEx(src_ptr, devid, flag);
}

drvError_t halHostRegisterCapabilities(uint32_t devid, uint32_t acc_module_type, uint32_t *mem_map_cap)
{
    u64 d2h_acc_mask = 0;
    int ret;

    if (devid >= SVM_MAX_DEV_NUM) {
        svm_err("Devid is invalid. (devid=%u)\n", devid);
        return DRV_ERROR_INVALID_DEVICE;
    }

    if (mem_map_cap == NULL) {
        svm_err("Mem_map_cap is null.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    if (acc_module_type >= DRV_ACC_MODULE_TYPE_MAX) {
        return DRV_ERROR_NOT_SUPPORT;
    }

    ret = svm_dbi_query_d2h_acc_mask(devid, &d2h_acc_mask);
    if (ret != 0) {
        return (drvError_t)ret;
    }

    *mem_map_cap = ((d2h_acc_mask & (1ULL << acc_module_type)) != 0) ?
        HOST_MEM_MAP_SUPPORTED : HOST_MEM_MAP_NOT_SUPPORTED;

    svm_debug("Host register cap. (Devid=%u; acc_module_type=%u; d2h_acc_mask=0x%llx; mem_map_cap=%u)\n",
        devid, acc_module_type, d2h_acc_mask, *mem_map_cap);
    return DRV_ERROR_NONE;
}
