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
#ifndef SOC_ADAPT_RES_MINI_V3_H__
#define SOC_ADAPT_RES_MINI_V3_H__

#include <linux/types.h>

#include "trs_pub_def.h"
#include "trs_rsv_mem.h"
#include "trs_pm_adapt.h"
#include "trs_core.h"
#include "trs_stars_reg_def.h"

size_t trs_soc_get_mini_v3_notify_size(void);
u32 trs_soc_get_mini_v3_notify_offset(u32 notify_id);
u32 trs_soc_get_mini_v3_event_offset(u32 event_id);
size_t trs_soc_get_mini_v3_db_stride(void);
int trs_soc_get_mini_v3_db_cfg(int db_type, u32 *start, u32 *end);
size_t trs_soc_get_mini_v3_stars_sched_stride(void);
int trs_soc_mini_v3_chan_stars_init(struct trs_id_inst *inst);
void trs_soc_mini_v3_chan_stars_uninit(struct trs_id_inst *inst);
int trs_soc_mini_v3_chan_stars_ops_init(struct trs_id_inst *inst);
void trs_soc_mini_v3_chan_stars_ops_uninit(struct trs_id_inst *inst);
struct trs_chan_adapt_ops *trs_chan_mini_v3_get_stars_adapt_ops(void);
struct trs_core_adapt_ops *trs_core_mini_v3_get_stars_adapt_ops(void);
int trs_soc_mini_v3_sq_send_trigger_db_init(struct trs_id_inst *inst);
void trs_soc_mini_v3_sq_send_trigger_db_uninit(struct trs_id_inst *inst);

static inline u32 trs_soc_get_mini_v3_sq_mem_side(u32 devid, struct trs_chan_type *types)
{
    return TRS_CHAN_DEV_RSV_MEM;
}

static inline u32 trs_soc_get_mini_v3_cq_mem_side(u32 devid)
{
    return TRS_CHAN_DEV_RSV_MEM;
}

static inline int trs_soc_get_mini_v3_hwcq_rsv_mem_type(void)
{
    return RSV_MEM_HW_SQCQ;
}

static inline int trs_soc_get_mini_v3_sq_head_reg_offset(void)
{
    return TRS_STARS_SCHED_SQ_HEAD_OFFSET;
}

static inline int trs_soc_get_mini_v3_sq_tail_reg_offset(void)
{
    return TRS_STARS_SCHED_SQ_TAIL_OFFSET;
}

static inline int trs_soc_get_mini_v3_sq_status_reg_offset(void)
{
    return TRS_STARS_SCHED_SQ_STATUS_OFFSET;
}

static inline int trs_soc_get_mini_v3_cq_head_reg_offset(void)
{
    return TRS_STARS_SCHED_CQ_HEAD_OFFSET;
}

static inline int trs_soc_get_mini_v3_cq_tail_reg_offset(void)
{
    return TRS_STARS_SCHED_CQ_TAIL_OFFSET;
}

static inline bool trs_mini_v3_is_support_soft_mbox(void)
{
    return false;
}
#endif /* SOC_ADAPT_RES_MINI_V3_H__ */
