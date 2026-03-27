/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef SMM_FLAG_H
#define SMM_FLAG_H

#define SVM_SMM_FLAG_PG_NC             (1U << 0U)
#define SVM_SMM_FLAG_PG_RDONLY         (1U << 1U)

#define SVM_SMM_FLAG_SRC_INVALID       (1U << 8U)
#define SVM_SMM_FLAG_SRC_NON_SVM_VA    (1U << 9U)

static inline int svm_smm_get_src_valid_flag(u32 flag)
{
    return ((flag & SVM_SMM_FLAG_SRC_INVALID) != 0) ? 0 : 1;
}

static inline int svm_smm_get_src_svm_va_flag(u32 flag)
{
    return ((flag & SVM_SMM_FLAG_SRC_NON_SVM_VA) != 0) ? 0 : 1;
}

#endif

