/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef SVM_SYS_CMD_H
#define SVM_SYS_CMD_H

#include <stddef.h>
#include <sys/mman.h>

#include "svm_pub.h"

int svm_ioctl_dev_init(u32 devid);
int svm_ioctl_dev_uninit(u32 devid);

int svm_cmd_ioctl(u32 devid, u32 cmd, void *para);

void *svm_cmd_mmap(void *addr, size_t length, int prot, int flags, off_t offset);
int svm_cmd_munmap(void *addr, size_t length);

int svm_register_ioctl_dev_init_post_handle(int (*fn)(u32 devid));
int svm_register_ioctl_dev_uninit_pre_handle(int (*fn)(u32 devid));

#endif

