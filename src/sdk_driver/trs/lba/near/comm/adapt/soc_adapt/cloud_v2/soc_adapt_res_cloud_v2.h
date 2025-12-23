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
#ifndef SOC_ADAPT_RES_CLOUD_V2_H__
#define SOC_ADAPT_RES_CLOUD_V2_H__

#include <linux/types.h>

#include "trs_pm_adapt.h"
#include "trs_pub_def.h"
#include "trs_rsv_mem.h"
#include "trs_stars_reg_def.h"

size_t trs_soc_get_cloud_v2_notify_size(void);
u32 trs_soc_get_cloud_v2_notify_offset(u32 id);
u32 trs_soc_get_cloud_v2_event_offset(u32 event_id);
size_t trs_soc_get_cloud_v2_db_stride(void);
int trs_soc_get_cloud_v2_db_cfg(int db_type, u32 *start, u32 *end);
size_t trs_soc_get_cloud_v2_stars_sched_stride(void);
u32 trs_soc_get_cloud_v2_sq_mem_side(u32 devid, struct trs_chan_type *types);
u32 trs_soc_get_cloud_v2_cq_mem_side(u32 devid);
int trs_soc_cloud_v2_chan_stars_init(struct trs_id_inst *inst);
void trs_soc_cloud_v2_chan_stars_uninit(struct trs_id_inst *inst);
int trs_soc_cloud_v2_chan_stars_ops_init(struct trs_id_inst *inst);
void trs_soc_cloud_v2_chan_stars_ops_uninit(struct trs_id_inst *inst);
struct trs_core_adapt_ops *trs_core_cloud_v2_get_stars_adapt_ops(void);
struct trs_chan_adapt_ops *trs_chan_cloud_v2_get_stars_adapt_ops(void);
int trs_soc_cloud_v2_sq_send_trigger_db_init(struct trs_id_inst *inst);
void trs_soc_cloud_v2_sq_send_trigger_db_uninit(struct trs_id_inst *inst);

static inline  int trs_soc_get_cloud_v2_hwcq_rsv_mem_type(void)
{
    return RSV_MEM_HW_SQCQ;
}

static inline int trs_soc_get_cloud_v2_sq_head_reg_offset(void)
{
    return TRS_STARS_SCHED_SQ_HEAD_OFFSET;
}

static inline int trs_soc_get_cloud_v2_sq_tail_reg_offset(void)
{
    return TRS_STARS_SCHED_SQ_TAIL_OFFSET;
}

static inline int trs_soc_get_cloud_v2_sq_status_reg_offset(void)
{
    return TRS_STARS_SCHED_SQ_STATUS_OFFSET;
}

static inline int trs_soc_get_cloud_v2_cq_head_reg_offset(void)
{
    return TRS_STARS_SCHED_CQ_HEAD_OFFSET;
}

static inline int trs_soc_get_cloud_v2_cq_tail_reg_offset(void)
{
    return TRS_STARS_SCHED_CQ_TAIL_OFFSET;
}

static inline bool trs_cloud_v2_is_support_soft_mbox(void)
{
    return false;
}
#endif /* SOC_ADAPT_RES_CLOUD_V2_H__ */
