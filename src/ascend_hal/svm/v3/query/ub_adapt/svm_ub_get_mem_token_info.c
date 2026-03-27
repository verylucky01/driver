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
#include "svm_get_mem_token_info_ops_register.h"
#include "svm_urma_seg_mng.h"

int svm_ub_get_mem_token_info(u32 devid, u64 va, u64 size, u32 *token_id, u32 *token_val)
{
    struct svm_dst_va dst_va;
    int ret;

    svm_dst_va_pack(devid, PROCESS_CP1, va, size, &dst_va);

    ret = svm_urma_get_token_info(devid, &dst_va, token_id, token_val);
    if (ret != 0) {
        svm_err("Svm urma get token info failed. (devid=%u; va=0x%llx; size=%llu)\n", devid, va, size);
        return ret;
    }

    return 0;
}

void svm_ub_get_mem_token_info_ops_register(u32 devid)
{
    svm_get_mem_token_info_ops_register(devid, svm_ub_get_mem_token_info);
}
