/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef MGA_H
#define MGA_H

#include "svm_pub.h"

#define SVM_MGA_MAX_GRAN SVM_BYTES_PER_GB

/* mga: multi-alignment gen allocator. */
struct mga_attr {
    u64 max_align_size;
    u64 expand_gran;

    u64 expand_thres; /* total_size >= expand_thres, stop expand */
    u64 shrink_thres; /* total_size <= shrink_thres, stop shrink */

    int (*expand)(void *mga_inst, u64 *size, u64 *va); /* expand size may be small than want */
    int (*shrink)(void *mga_inst, u64 va, u64 size);
};

void *mga_inst_create(struct mga_attr *attr);
void mga_inst_destroy(void *mga_inst);

int mga_va_alloc(void *mga_inst, u64 align, u64 size, u64 *va);
int mga_va_free(void *mga_inst, u64 va, u64 size, u64 align);

u64 mga_get_max_align_size(void *mga_inst);
u64 mga_get_total_size(void *mga_inst);

#endif
