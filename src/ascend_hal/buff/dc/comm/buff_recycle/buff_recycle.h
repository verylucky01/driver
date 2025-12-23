/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef BUFF_RECYCLE_H
#define BUFF_RECYCLE_H

#include <pthread.h>
#include "ascend_hal_define.h"
#include "drv_user_common.h"
#include "buff_recycle_ctx.h"

#define RECYCLE_THREAD_PERIOD_US 200000U /* 200ms */

enum buff_recycle_type {
    PASSIVE_RECYCLE = 0, /* recycle by recycle thread */
    ACTIVE_RECYCLE, /* recycle by user who will call halBuffProcCacheFree */
    RECYCLE_MAX_TYPE,
};

enum {
    RECYCLE_THREAD_STATUS_IDLE = 0,
    RECYCLE_THREAD_STATUS_RUN,
    RECYCLE_THREAD_STATUS_STOP
};

void buf_recycle_init(int pool_id);
void wake_up_recyle_thread(int pool_id);

void proc_block_add(struct block_mem_ctx *block);
void proc_block_del(void *blk);
void proc_set_buff_invalid(void *blk);
drvError_t buff_proc_cache_free(uint32_t devid);
drvError_t proc_block_ctx_init(int pool_id, int type, void *mng, struct block_mem_ctx *block);

#endif

