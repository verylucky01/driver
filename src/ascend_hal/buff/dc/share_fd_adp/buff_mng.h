/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef BUFF_MNG_H
#define BUFF_MNG_H

#include "ascend_hal_define.h"
#include "ascend_hal_error.h"

#define BUFF_MEM_MAX_SIZE (256ULL * 1024 * 1024 * 1024)
#define BUFF_MEM_MIN_SIZE (256ULL * 1024 * 1024)
#define BUFF_MEM_BASE 0x200000000000ULL

void *buff_blk_alloc(int pool_id, unsigned long size, unsigned long flag, uint32_t *blk_id);
void buff_blk_free(int pool_id, void *addr);
drvError_t buff_blk_get(void *addr, int *pool_id, void **alloc_addr, unsigned long *alloc_size, uint32_t *blk_id);
void buff_blk_put(int pool_id, void *addr);

drvError_t buff_set_prop(const char *prop_name, unsigned long value);
drvError_t buff_get_prop(const char *prop_name, unsigned long *value);

drvError_t buff_pool_init(int pool_id, int mem_fd, unsigned long long max_mem_size, GroupShareAttr attr);
bool buff_pool_is_ready(void);
unsigned long long buff_get_base_addr(void);
bool is_buff_addr(unsigned long va);

#endif
