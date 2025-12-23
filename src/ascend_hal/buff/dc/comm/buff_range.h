/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef BUFF_RANGE_H
#define BUFF_RANGE_H

#include "ascend_hal_define.h"

#define RANGE_OWNER_SELF 0
#define RANGE_OWNER_OTHERS 1

drvError_t add_self_buff_to_range(uint32_t blk_id, int pool_id, void *start, unsigned long size,
    unsigned long long mem_mng);
void del_self_buff_from_range(uint32_t blk_id, void *start);

drvError_t buff_range_owner_get(uint32_t blk_id, void *start, unsigned long size, int *owner);
drvError_t buff_range_mem_mng_get(uint32_t blk_id, void *start, unsigned long size, unsigned long long *mem_mng);
drvError_t buff_range_get_ref(void *start, unsigned long size, uint32_t *refcnt, uint32_t blk_id);

drvError_t buff_range_get(uint32_t blk_id, void *start, unsigned long size);
void buff_range_put(uint32_t blk_id, void *start);

drvError_t idle_buff_range_free(uint32_t devid, uint32_t using_buff_max_show_cnt);
void del_others_range(void);
void buff_range_show(void);
void idle_buff_range_free_ahead(uint32_t blk_id);

#endif

