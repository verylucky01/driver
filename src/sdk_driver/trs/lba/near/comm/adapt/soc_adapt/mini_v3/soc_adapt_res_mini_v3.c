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
#include "trs_pub_def.h"
#include "trs_chan_mem.h"
#include "trs_chip_def_comm.h"
#include "trs_chan.h"
#include "trs_chan_stars_v1_ops.h"
#include "trs_core_stars_v1_ops.h"
#include "soc_adapt_res_mini_v3.h"

/* Notify */
#define TRS_MINI_V3_NOTIFY_SIZE             8

/* Doorbell */
#define TRS_SOC_MINI_V3_DB_STRIDE   (4 * 1024)
#define TRS_DB_MINI_V3_ONLINE_MBOX_START    (1006u - 512u)
#define TRS_DB_MINI_V3_ONLINE_MBOX_END      (1007u - 512u)

#define TRS_DB_MINI_V3_TRIGGER_SQ_START    (1005u - 512u)
#define TRS_DB_MINI_V3_TRIGGER_SQ_END      (1006u - 512u)

#define TRS_MINI_V3_STARS_SCHED_STRIDE      (4 * 1024)

/* Event */
#define TRS_MINI_V3_EVENT_SIZE          4
#define TRS_MINI_V3_EVENT_SLICE_SIZE    (64 * 1024)
#define TRS_MINI_V3_EVENT_NUM_PER_SLICE 4096   /* total silce num is 16 */

u32 trs_soc_get_mini_v3_notify_offset(u32 notify_id)
{
    return notify_id * TRS_MINI_V3_NOTIFY_SIZE;
}

size_t trs_soc_get_mini_v3_notify_size(void)
{
    return (size_t)TRS_MINI_V3_NOTIFY_SIZE;
}

u32 trs_soc_get_mini_v3_event_offset(u32 event_id)
{
#ifndef EMU_ST
    return (event_id % TRS_MINI_V3_EVENT_NUM_PER_SLICE) * TRS_MINI_V3_EVENT_SIZE +
        (event_id / TRS_MINI_V3_EVENT_NUM_PER_SLICE) * TRS_MINI_V3_EVENT_SLICE_SIZE;
#endif
}

size_t trs_soc_get_mini_v3_db_stride(void)
{
    return (size_t)TRS_SOC_MINI_V3_DB_STRIDE;
}

size_t trs_soc_get_mini_v3_stars_sched_stride(void)
{
    return (u32)TRS_MINI_V3_STARS_SCHED_STRIDE;
}

int trs_soc_get_mini_v3_db_cfg(int db_type, u32 *start, u32 *end)
{
    switch (db_type) {
        case TRS_DB_ONLINE_MBOX:
            *start = TRS_DB_MINI_V3_ONLINE_MBOX_START;
            *end = TRS_DB_MINI_V3_ONLINE_MBOX_END;
            break;
        case TRS_DB_TRIGGER_SQ:
            *start = TRS_DB_MINI_V3_TRIGGER_SQ_START;
            *end = TRS_DB_MINI_V3_TRIGGER_SQ_END;
            break;
        case TRS_DB_MAINT_SQ:
        case TRS_DB_MAINT_CQ:
            return -EOPNOTSUPP;
        default:
            trs_err("Unkonwn db_type. (db_type=%d)\n", db_type);
            return -ENODEV;
    }

    return 0;
}

int trs_soc_mini_v3_chan_stars_init(struct trs_id_inst *inst)
{
    int ret;

    ret = trs_chan_stars_v1_ops_init(inst);
    if (ret != 0) {
        return ret;
    }

    ret = trs_chan_ts_inst_register(inst, TRS_HW_TYPE_STARS, trs_chan_get_stars_v1_adapt_ops());
    if (ret != 0) {
        trs_chan_stars_v1_ops_uninit(inst);
    }
    return ret;
}

void trs_soc_mini_v3_chan_stars_uninit(struct trs_id_inst *inst)
{
    trs_chan_ts_inst_unregister(inst);
    trs_chan_stars_v1_ops_uninit(inst);
}

#ifndef EMU_ST
int trs_soc_mini_v3_chan_stars_ops_init(struct trs_id_inst *inst)
{
    return trs_chan_stars_v1_ops_init(inst);
}

void trs_soc_mini_v3_chan_stars_ops_uninit(struct trs_id_inst *inst)
{
    trs_chan_stars_v1_ops_uninit(inst);
}

struct trs_chan_adapt_ops *trs_chan_mini_v3_get_stars_adapt_ops(void)
{
    return trs_chan_get_stars_v1_adapt_ops();
}
#endif

struct trs_core_adapt_ops *trs_core_mini_v3_get_stars_adapt_ops(void)
{
    return trs_core_get_stars_v1_adapt_ops();
}

int trs_soc_mini_v3_sq_send_trigger_db_init(struct trs_id_inst *inst)
{
    return trs_sq_send_trigger_db_init(inst);
}

void trs_soc_mini_v3_sq_send_trigger_db_uninit(struct trs_id_inst *inst)
{
    trs_sq_send_trigger_db_uninit(inst);
}
