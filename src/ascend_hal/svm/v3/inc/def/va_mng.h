/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef VA_MNG_H
#define VA_MNG_H

#include "svm_pub.h"

/*
   total 256T virsual address space:
   1. 4-8T 4T pcieth
   2. 16-224T 208T svm
   3. 224-240 16T share pool
   4. 249-249.5 0.5T ub
*/

#define PCIE_TH_VA_START        (4ULL * SVM_BYTES_PER_TB)
#define PCIE_TH_VA_SIZE         (4ULL * SVM_BYTES_PER_TB)
#define SVM_VA_RESERVE_START    (16 * SVM_BYTES_PER_TB)
#define SVM_VA_RESERVE_SIZE     (208 * SVM_BYTES_PER_TB)
#define SP_VA_START             (224ULL * SVM_BYTES_PER_TB)
#define SP_VA_SIZE              (16ULL * SVM_BYTES_PER_TB)

#define SVM_VA_RESERVE_ALIGN    (1 * SVM_BYTES_PER_GB)

#define VA_TYPE_SVM 1
#define VA_TYPE_NON_SVM 2

/* Interface */
static inline void svm_get_pcie_th_va_range(u64 *va, u64 *size)
{
    *va = PCIE_TH_VA_START;
    *size = PCIE_TH_VA_SIZE;
}

/* Interface */
static inline void svm_get_va_range(u64 *va, u64 *size)
{
    *va = SVM_VA_RESERVE_START;
    *size = SVM_VA_RESERVE_SIZE;
}

/* Interface */
static inline bool va_is_in_pcie_th_range(u64 va, u64 size)
{
    u64 start, total_size;
    svm_get_pcie_th_va_range(&start, &total_size);
    return (svm_check_va_range(va, size, start, start + total_size) == 0);
}

/* Interface */
static inline bool va_is_in_sp_range(u64 va, u64 size)
{
    return (svm_check_va_range(va, size, SP_VA_START, SP_VA_START + SP_VA_SIZE) == 0);
}

#endif
