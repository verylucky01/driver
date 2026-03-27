/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef CACHE_MALLOC_H
#define CACHE_MALLOC_H

#include <stdint.h>
#include <stdbool.h>

#include "svm_pub.h"

#define SVM_CACHE_MALLOC_FLAG_PA_HPAGE          (1U << 0U)
#define SVM_CACHE_MALLOC_FLAG_PA_P2P            (1U << 1U)

#define SVM_CACHE_MALLOC_FLAG_MASTER_UVA        (1U << 8U)

/*
 * Devid & flag should match with align, current support details are as follows:
 * host   devid & npage: PAGE_SIZE
 * device devid & npage: PAGE_SIZE
 * device devid & hpage: HPAGE_SIZE
 */
bool svm_cache_is_support(u32 devid, u32 flag, u64 align, u32 size);
int svm_cache_malloc(u32 devid, u32 flag, u64 align, u64 *va, u64 size);
int svm_cache_free(u32 devid, u32 flag, u64 align, u64 va, u64 size);
void svm_cache_shrink(u32 devid, u32 flag, u64 *size);

/* If no such va will return DRV_ERROR_NOT_EXIST. */
int svm_cache_recycle_seg_release(u64 va);

void svm_show_cache(u32 devid, char *buf, u32 buf_len);
u64 svm_cache_get_stats_idle_size(u32 devid, u32 flag);

/* cache expand shrink ops */
struct svm_cache_ops {
    int (*post_expand)(u32 devid, u64 start, u64 size);
    void (*pre_shrink)(u32 devid, u64 start, u64 size);
};

void svm_cache_set_ops(struct svm_cache_ops *ops);
int svm_cache_for_each_range(u32 devid, int (*handle)(u32 devid, u64 start, u64 size, void *priv), void *priv);

#endif
