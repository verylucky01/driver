/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "svm_pub.h"
#include "svm_log.h"
#include "dma_map_flag.h"
#include "dma_map.h"
#include "svm_dbi.h"
#include "svm_register_to_master.h"

int svm_pcie_register_to_master(u32 user_devid, struct svm_dst_va *register_va, u32 flag)
{
    int ret = 0;

    /* device support sva, not need dma map */
    if (!svm_dbi_is_support_sva(register_va->devid)) {
        u32 dma_map_flag = register_to_master_flag_is_access_write(flag) ? SVM_DMA_MAP_ACCESS_WRITE : 0;
        dma_map_flag |= register_to_master_flag_is_va_io_map(flag) ? SVM_DMA_MAP_VA_IO_MAP : 0;
        ret = svm_dma_map(user_devid, register_va, dma_map_flag);
    }

    return ret;
}

int svm_pcie_unregister_to_master(u32 user_devid, struct svm_dst_va *register_va, u32 flag)
{
    int ret = 0;

    SVM_UNUSED(flag);

    if (!svm_dbi_is_support_sva(register_va->devid)) {
        ret = svm_dma_unmap(user_devid, register_va);
    }

    return ret;
}

static struct svm_register_to_master_ops pcie_register_to_master_ops = {
    .register_to_master = svm_pcie_register_to_master,
    .unregister_to_master = svm_pcie_unregister_to_master
};

void svm_pcie_register_to_master_ops_register(u32 user_devid, u32 devid)
{
    svm_register_to_master_set_ops(user_devid, devid, &pcie_register_to_master_ops);
}

