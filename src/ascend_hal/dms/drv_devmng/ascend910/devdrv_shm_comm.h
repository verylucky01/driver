/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef DEVDRV_SHM_COMM_H
#define DEVDRV_SHM_COMM_H

#include <sys/types.h>

#include "tsdrv_user_common.h"
#include "devmng_common.h"
#include "tsdrv_conflict_check.h"

#define OUT_OF_RANGE(value, min, max) ((((min) <= (value)) && ((value) <= (max))) ? (0) : (1))

#define DEVDRV_SHARED_MEMORY_INITED (1)
#define DEVDRV_SHARED_MEMORY_NOT_INITED (0)

#define DEVDRV_MAX_SQ_SLOT_NUM (DEVDRV_MAX_SQ_DEPTH)
#define DEVDRV_MAX_CQ_SLOT_NUM (DEVDRV_MAX_CQ_DEPTH)
#define DEVDRV_SQ_PER_SLOT_SIZE (DEVDRV_SQ_SLOT_SIZE)
#define DEVDRV_CQ_PER_SLOT_SIZE (DEVDRV_CQ_SLOT_SIZE)
#define DEVDRV_SQ_SIZE (DEVDRV_SQ_SLOT_SIZE * DEVDRV_MAX_SQ_SLOT_NUM)
#define DEVDRV_CQ_SIZE (DEVDRV_CQ_SLOT_SIZE * DEVDRV_MAX_CQ_SLOT_NUM)

#define DEVDRV_SQ_INFO_NUM DEVDRV_MAX_SQ_NUM
#define DEVDRV_CQ_INFO_NUM DEVDRV_MAX_CQ_NUM
#define DEVDRV_SQ_INFO_SIZE (sizeof(struct devdrv_ts_sq_info))
#define DEVDRV_CQ_INFO_SIZE (sizeof(struct devdrv_ts_cq_info))
#define DEVDRV_STREAM_ID_NUM (DEVDRV_MAX_STREAM_ID)
#define DEVDRV_EVENT_ID_NUM (DEVDRV_MAX_SW_EVENT_ID)
#define DEVDRV_NOTIFY_ID_NUM (DEVDRV_MAX_NOTIFY_ID)

#define DEVDRV_MODEL_ID_NUM (DEVDRV_MAX_MODEL_ID)
#define DEVDRV_CMO_ID_NUM (DEVDRV_MAX_CMO_ID)

#define DEVDRV_SQ_INFO_OFFSET (0)
#define DEVDRV_CQ_INFO_OFFSET (DEVDRV_SQ_INFO_OFFSET + DEVDRV_SQ_INFO_SIZE * DEVDRV_SQ_INFO_NUM)

#define DEVDRV_PHASE_TOGGLE_STATE_0 (0)
#define DEVDRV_PHASE_TOGGLE_STATE_1 (1)

#define DEVDRV_REPORT_IRQ_WAIT (1)
#define DEVDRV_REPORT_POLL_WAIT (0)

#define DEVDRV_SIG_PROCESS_MODULE_NUM (32)

struct tsdrv_user_info {
    int devdrv_process_cq;
    pthread_mutex_t devdrv_sqcq_mutex;
    struct devdrv_ts_sq_info sq_info[DEVDRV_MAX_SQ_NUM];
    struct devdrv_ts_cq_info cq_info[DEVDRV_MAX_CQ_NUM];
};

u32 devdrv_get_bind_cq_id(u32 devid, u32 tsid, u32 sq_id);
void devdrv_set_bind_cq_id(u32 devid, u32 tsid, u32 sq_id, u32 cq_id);

struct devdrv_ts_sq_info *devdrv_get_sq_info(u32 devid, u32 tsid, u32 sq_index);
struct devdrv_ts_cq_info *devdrv_get_cq_info(u32 devid, u32 tsid, u32 cq_index);

u8 *devdrv_get_sq_slot(u32 devid, u32 tsid, u32 sq_index, u32 slot_index);
u8 *devdrv_get_cq_slot(u32 slot_index, struct devdrv_ts_cq_info *cq_info);

void devdrv_init_struct(u32 tsid);
drvError_t tsdrv_init_user_info(u32 devid, u32 tsid);
void tsdrv_deinit_user_info(u32 devid, u32 tsid);

int devdrv_is_shared_memory_inited(u32 devid, u32 tsid);

int devdrv_init_cq_uio(u32 devid, u32 tsid, struct devdrv_ts_cq_info *cq_info);
int devdrv_exit_cq_uio(u32 devid, u32 tsid, struct devdrv_ts_cq_info *cq_info);
int devdrv_init_sq_uio(u32 devid, u32 tsid, struct devdrv_ts_sq_info *sq_info);
void devdrv_exit_sq_uio(u32 devid, u32 tsid, struct devdrv_ts_sq_info *sq_info);

/* used inside device driver */
extern void devdrv_flush_cache(uint64_t base, uint32_t len);
extern void mb(void);

#endif
