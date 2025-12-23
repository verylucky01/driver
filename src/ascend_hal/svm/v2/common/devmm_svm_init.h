/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef DEVMM_SVM_INIT_H
#define DEVMM_SVM_INIT_H
#include "ascend_hal.h"
#include "svm_ioctl.h"

#define DEVMM_MAX_INIT_CNT 10

DVresult devmm_svm_open(const char *davinci_sub_name);
DVresult devmm_svm_map(int side);
void devmm_svm_close(const char *davinci_sub_name);
void devmm_svm_unmap(void);
bool devmm_is_in_mmap_segs(uint64_t va, uint64_t size);
DVresult devmm_svm_ioctl(int fd, unsigned long request, struct devmm_ioctl_arg *arg);
DVresult devmm_svm_master_init(void);
DVresult devmm_svm_agent_init(void);
void devmm_svm_master_uninit(void);
void devmm_svm_agent_uninit(void);

#endif /* __DEVMM_SVM_INIT_H__ */
