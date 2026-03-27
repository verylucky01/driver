/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef GEN_ALLOCATOR_H
#define GEN_ALLOCATOR_H
#include <stdbool.h>

#include "svm_pub.h"

/*
    gen_allocator: an general allocator that supports the following features:
        (1) Manage multiple non-consecutive address ranges.
        (2) Dynamically add or delete address range.
        (3) Allocate according to the specified address size.
        (4) Allocate according to the specified address range.
        attention: The allocated address must be in a range.
    create/destroy: create/destroy an general allocator
    add_range/del_range: add/del address range
    recycle_idle_range: recycle one idle range
    alloc/free: alloc/free address
    get_stats: get address statistics from allocator
*/

struct svm_ga_attr {
    u64 gran_size;
};

static inline void svm_ga_attr_pack(u64 gran_size, struct svm_ga_attr *attr)
{
    attr->gran_size = gran_size;
}

/*
    FIXED_ADDR: alloc by fixed addr, if addr is already alloced, return fail
    IN_FIXED_ADDR_RANGE: alloc in the addr range which seg[addr, addr+1) belongs, if fail go alloc by size
    priority: FIXED_ADDR > IN_FIXED_ADDR_RANGE
*/
#define SVM_GA_FLAG_FIXED_ADDR                      (1U << 0U)
#define SVM_GA_FLAG_IN_FIXED_ADDR_RANGE             (1U << 1U)

void *svm_ga_inst_create(struct svm_ga_attr *attr);
void svm_ga_inst_destroy(void *ga_inst);
int svm_ga_add_range(void *ga_inst, u64 start, u64 size);
int svm_ga_del_range(void *ga_inst, u64 start);
bool svm_ga_owner_range_is_idle(void *ga_inst, u64 addr);
int svm_ga_recycle_one_idle_range(void *ga_inst, u64 *start, u64 *size);
int svm_ga_for_each_range(void *ga_inst, int (*func)(u64 start, u64 size, void *priv), void *priv);

/*
 * If alloc by fixed addr, and no such virtual addr, will return DRV_ERROR_PARA_ERROR.
 * If alloc by size, and no enough virtual addr, will return DRV_ERROR_OUT_OF_MEMORY.
 */
int svm_ga_alloc(void *ga_inst, u32 flag, u64 *addr, u64 size);
int svm_ga_free(void *ga_inst, u64 addr, u64 size);
u32 svm_ga_show(void *ga_inst, char *buf, u32 buf_len);

#endif

