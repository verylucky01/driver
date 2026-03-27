/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef SVM_REGISTER_PCIE_TH_H
#define SVM_REGISTER_PCIE_TH_H

#include "svm_pub.h"

#define SVM_REGISTER_PCIE_TH_FLAG_VA_IO_MAP (1U << 0U)

int svm_register_pcie_th(u64 va, u64 size, u32 flag, u32 devid, u64 *dst_va);
int svm_unregister_pcie_th(u64 va, u32 devid);
bool svm_va_is_pcie_th_register(u64 va, u32 devid);
void svm_register_pcie_th_recycle(u32 devid);
u32 svm_show_register_pcie_th(u32 devid, char *buf, u32 buf_len); /* return show buf len */

#endif
