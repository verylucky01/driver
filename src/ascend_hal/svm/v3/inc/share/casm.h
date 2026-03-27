/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef CASM_H
#define CASM_H

#include "svm_pub.h"
#include "svm_addr_desc.h"

/*
 *  CASM: cross application shared memory, also support sharing within a single application
 *  create/destroy: add/del memory white list mem
 *  add task: add trusted task to memory white list mem
 *  get_key_info: get key info
 *  map/unmap: find src info by key, check memory white list mem and pin/unpin smp mem, store src info in map ctx
 */

#define SVM_CASM_FLAG_PG_NC             (1U << 0U)
#define SVM_CASM_FLAG_PG_RDONLY         (1U << 1U)

/* owner app call */
int svm_casm_create_key(struct svm_dst_va *dst_va, u64 *key);
int svm_casm_destroy_key(u64 key);

int svm_casm_add_task(u64 key, u32 server_id, int tgid[], u32 num); /* tgid[]: shared app pid */
int svm_casm_del_task(u64 key, u32 server_id, int tgid[], u32 num);
/* DRV_ERROR_NO_RESOURCES: tgid not exist */
int svm_casm_check_task(u64 key, u32 server_id, int tgid[], u32 num);

static inline int svm_casm_add_local_task(u64 key, int tgid[], u32 num)
{
    return svm_casm_add_task(key, SVM_INVALID_SERVER_ID, tgid, num);
}

static inline int svm_casm_del_local_task(u64 key, int tgid[], u32 num)
{
    return svm_casm_del_task(key, SVM_INVALID_SERVER_ID, tgid, num);
}

static inline int svm_casm_check_local_task(u64 key, int tgid[], u32 num)
{
    return svm_casm_check_task(key, SVM_INVALID_SERVER_ID, tgid, num);
}

/* shared app call */
int svm_casm_get_src_va_ex(u32 devid, u64 key, struct svm_global_va *src_va, u64 *ex_info);
static inline int svm_casm_get_src_va(u32 devid, u64 key, struct svm_global_va *src_va)
{
    u64 tmp;
    return svm_casm_get_src_va_ex(devid, key, src_va, &tmp);
}

int svm_casm_mem_map(u32 devid, u64 va, u64 size, u64 key, u32 flag);
int svm_casm_mem_unmap(u32 devid, u64 va, u64 size);

/* casm map call */
int svm_casm_mem_pin(u32 devid, u64 va, u64 size, u64 key);
int svm_casm_mem_unpin(u32 devid, u64 va, u64 size);

#endif

