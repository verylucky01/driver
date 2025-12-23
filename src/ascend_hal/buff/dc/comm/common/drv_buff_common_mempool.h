/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef DRV_BUFF_COMMON_MEMPOOL_H
#define DRV_BUFF_COMMON_MEMPOOL_H

#include "drv_buff_common.h"
#include "drv_buff_unibuff.h"
#include "drv_user_common.h"

#ifndef BUFF_SP_HUGEPAGE
#define BUFF_SP_HUGEPAGE (1 << 0)
#endif

#ifndef BUFF_SP_HUGEPAGE_ONLY
#define BUFF_SP_HUGEPAGE_ONLY (1 << 1)
#endif

#ifndef BUFF_SP_DVPP
#define BUFF_SP_DVPP (1 << 2)
#endif

#define BUFF_DEFAULT_POOL_TIME_OUT 1000 /* time_out: 26us */

struct mp_stat {
    uint32 alloc_fail_scence;
    uint32 fail_cnt;
    uint32 buff_time_stat_enable;
    uint32 over_time_value;
    uint32 blk_over_time_num;
    uint32 blk_max_use_num;
    uint64 blk_max_use_time;
};

#define MP_TRACE_PRINT_INTERVAL 23040000000ULL /* 10min almost = 38.4 * 1000 * 1000 * 600 */
#define MAX_MP_PRINT_NUM 50

struct mp_trace {
    uint32 print_flag;
    uint64 last_alloc_fail_timestamp;
};

struct mempool_t {
    struct common_handle_t head;
    pthread_mutex_t mutex;
    uint32 scan_in_alloc;
    struct list_head user_list;
    uint32 type;
    uint32 blk_num;
    uint32 blk_available;
    uint32 blk_size;
    uint32 align_size;  /* buff + blk_size + align_size: uni_tail */
    uint32 curr_index;
    uint32 free_index;
    uint32 bit_num;
    pid_t  owner;
    uint32_t blk_id;
    void *blk_start;
    uint64 blk_total_len;
    struct mp_stat stat;
    char pool_name[BUFF_POOL_NAME_LEN];
    struct mp_trace trace;
    bitmap_t bitmap[];
};

enum {
    MP_FREE_ONE_BUFF = 1,
    MP_FREE_ALL_BUFF,
};

void mp_scan_free_all_buff(struct mempool_t *mp);
int mp_scan_free_single_buff(struct mempool_t *mp);
drvError_t mp_free_buff(void *mempool, void *buff);
void *mp_get_valist_start_addr(struct mempool_t *mp, uint32 bit_num);
void mp_mng_list_add(struct mempool_t *mp);

#endif
