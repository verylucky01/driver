/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef SMM_H
#define SMM_H

#include "svm_pub.h"
#include "svm_addr_desc.h"
#include "smm_flag.h"

/*
    SMM: share mem map
*/

/* pcie th map va is output, else va is input */
int svm_smm_mmap(u32 devid, u64 *va, u64 size, u32 flag, struct svm_global_va *src_info);
int svm_smm_munmap(u32 devid, u64 va, u64 size, u32 flag, struct svm_global_va *src_info);

#endif

