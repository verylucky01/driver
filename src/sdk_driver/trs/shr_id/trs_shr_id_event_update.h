/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */
#ifndef TRS_SHR_ID_EVENT_UPDATE_H
#define TRS_SHR_ID_EVENT_UPDATE_H

#include <linux/types.h>

#include "ascend_hal_define.h"

#include "trs_pub_def.h"
#include "trs_res_id_def.h"

#define SHRID_NOTIFY_SIZE   4
#define SHRID_NOTIFY_SLICE_SIZE (64 * 1024)
#define SHRID_NOTIFY_NUM_PER_SLICE  512
static inline u32 shr_id_get_notify_offset(u32 id)
{
    return (id % SHRID_NOTIFY_NUM_PER_SLICE) * SHRID_NOTIFY_SIZE +
        (id / SHRID_NOTIFY_NUM_PER_SLICE) * SHRID_NOTIFY_SLICE_SIZE;
}

static int shr_id_type_trans[SHR_ID_TYPE_MAX] = {
    [SHR_ID_NOTIFY_TYPE] = TRS_NOTIFY,
    [SHR_ID_EVENT_TYPE] = TRS_EVENT,
};

static inline int shr_id_type_trans_res_type(u32 shr_id_type)
{
    if (shr_id_type >= SHR_ID_TYPE_MAX) {
        return TRS_MAX_ID_TYPE;
    }
    return shr_id_type_trans[shr_id_type];
}

int shr_id_event_update(unsigned int devid, struct sched_published_event_info *event_info,
                        struct sched_published_event_func *event_func);
#endif /* TRS_SHR_ID_EVENT_UPDATE_H */
