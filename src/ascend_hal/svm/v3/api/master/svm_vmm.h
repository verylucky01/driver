/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef SVM_VMM_H
#define SVM_VMM_H

#include "svm_pub.h"
#include "svm_addr_desc.h"
#include "svm_flag.h"
#include "malloc_mng.h"

void *vmm_get_svmm(void *va_handle);
void vmm_recycle(u32 devid);
void vmm_restore_real_src_va(struct svm_global_va *src_info);
int vmm_query_src_info(u64 va, u64 size, struct svm_global_va *src_info);

/* vmm map unmap ops */
struct svm_vmm_ops {
    int (*post_map)(void *svmm_inst, u32 devid, u64 start, u64 svm_flag, struct svm_global_va *src_info);
    int (*pre_unmap)(u32 task_bitmap, u32 devid, u64 start, u64 svm_flag, struct svm_global_va *src_info);
};

void svm_vmm_set_ops(struct svm_vmm_ops *ops);

static inline int vmm_query_ipc_src_info(u64 va, u64 size, struct svm_global_va *src_info)
{
    struct svm_prop prop;
    if ((svm_get_prop(va, &prop) == 0) && svm_flag_cap_is_support_vmm_ipc_unmap(prop.flag)) {
        return vmm_query_src_info(va, size, src_info);
    }

    return DRV_ERROR_NOT_EXIST;
}

#endif

