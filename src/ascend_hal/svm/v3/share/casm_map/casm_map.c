/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include "ascend_hal_define.h"
#include "svm_pub.h"
#include "svm_log.h"
#include "smm_client.h"
#include "casm.h"

int svm_casm_mem_map(u32 devid, u64 va, u64 size, u64 key, u32 flag)
{
    struct svm_dst_va dst_info;
    struct svm_global_va src_info = {0};
    u32 smm_flag = SVM_SMM_FLAG_SRC_INVALID;
    int ret;

    ret = svm_casm_mem_pin(devid, va, size, key);
    if (ret != 0) {
        return ret;
    }

    if ((flag & SVM_CASM_FLAG_PG_NC) != 0) {
        smm_flag |= SVM_SMM_FLAG_PG_NC;
    }

    if ((flag & SVM_CASM_FLAG_PG_RDONLY) != 0) {
        smm_flag |= SVM_SMM_FLAG_PG_RDONLY;
    }

    svm_dst_va_pack(devid, PROCESS_CP1, va, size, &dst_info);

    /* update src info in kernel */
    ret = svm_smm_client_map(&dst_info, &src_info, smm_flag);
    if (ret != 0) {
        (void)svm_casm_mem_unpin(devid, va, size);
        svm_err("Smm map failed. (devid=%u; va=0x%llx; size=0x%llx; key=0x%llx; ret=%d)\n", devid, va, size, key, ret);
        return ret;
    }

    return 0;
}

int svm_casm_mem_unmap(u32 devid, u64 va, u64 size)
{
    struct svm_dst_va dst_info;
    struct svm_global_va src_info = {0};
    u32 smm_flag = SVM_SMM_FLAG_SRC_INVALID;
    int ret;

    svm_dst_va_pack(devid, PROCESS_CP1, va, size, &dst_info);

    /* update src info in kernel */
    ret = svm_smm_client_unmap(&dst_info, &src_info, smm_flag);
    if (ret != 0) {
        svm_err("Smm unmap failed. (devid=%u; va=0x%llx; ret=%d)\n", devid, va, ret);
        return ret;
    }

    ret = svm_casm_mem_unpin(devid, va, size);
    if (ret != 0) {
        return ret;
    }

    return 0;
}

