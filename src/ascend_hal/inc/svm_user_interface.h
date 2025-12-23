/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef SVM_USER_INTERFACE_H
#define SVM_USER_INTERFACE_H

#include <stdbool.h>
#include <stdint.h>
#include <drv_type.h>

#include "ascend_hal.h"

#define SVM_MEM_READ  (0x1 << 0)
#define SVM_MEM_WRITE (0x1 << 1)

#define REMOTE_REQUEST 0x5a
#define LOCAL_REQUEST  0xa5
#define SVM_MEM_READ  (0x1 << 0)
#define SVM_MEM_WRITE (0x1 << 1)

bool svm_support_get_user_malloc_attr(uint32_t dev_id);
bool svm_support_vmm_normal_granularity(uint32_t dev_id);

drvError_t drvMemDeviceOpenInner(uint32_t devid, halDevOpenIn *in, halDevOpenOut *out);
drvError_t drvMemDeviceCloseInner(uint32_t devid, halDevCloseIn *in);
drvError_t drvMemMmapInner(uint32_t device, void **pp, size_t size, int side);
drvError_t drvMemUnmapInner(void *pp, int side, size_t *size);

drvError_t drvMemDeviceCloseUserRes(uint32_t devid, halDevCloseIn *in);
drvError_t drvMemProcResBackup(halProcResBackupInfo *info);
drvError_t drvMemProcResRestore(halProcResRestoreInfo *info);

bool svm_va_is_in_svm_range(uint64_t va);

/* only support device addr(svm addr, apm maped reg addr) register to host to access */

/* halSvmRegister return access_va which may be same or diff from va */
uint64_t halSvmRegister(uint32_t devid, uint64_t va, uint32_t size);
int halSvmUnRegister(uint32_t devid, uint64_t va, uint32_t size);
int halSvmAccess(uint32_t devid, uint64_t access_va, uint64_t local_va, uint32_t size, uint32_t flag);

int halMemGet(unsigned long long addr, unsigned long long size);
int halMemPut(unsigned long long addr, unsigned long long size);

#endif
