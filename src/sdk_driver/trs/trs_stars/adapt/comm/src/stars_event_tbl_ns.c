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
#include "ka_task_pub.h"
#include "ka_memory_pub.h"
#include "pbl/pbl_soc_res.h"
#include "stars_event_table_ns_c_union_define.h"
#include "trs_pub_def.h"
#include "trs_stars_comm.h"
#include "stars_event_tbl_ns.h"

static volatile StarsEventTableNsRegsType *stars_event_table_ns_all_reg[TRS_DEV_MAX_NUM][TRS_TS_MAX_NUM];
ka_spinlock_t event_table_all_reg_lock[TRS_DEV_MAX_NUM][TRS_TS_MAX_NUM];

int trs_init_event_tbl_ns_base_addr(struct trs_id_inst *inst)
{
    struct soc_reg_base_info io_base;
    struct res_inst_info res_inst;
    void *base_va;
    int ret;

    soc_resmng_inst_pack(&res_inst, inst->devid, TS_SUBSYS, inst->tsid);
    ret = soc_resmng_get_reg_base(&res_inst, "TS_STARS_EVENT_TBL_NS_REG", &io_base);
    if (ret != 0) {
#ifndef EMU_ST
        trs_warn("Without event status reg base. (devid=%u; tsid=%u)\n", inst->devid, inst->tsid);
        return 0;
#endif
    }

    base_va = ka_mm_ioremap(io_base.io_base, io_base.io_base_size);
    if (base_va == NULL) {
        trs_err("Failed to ioremap. (devid=%u; tsid=%u)\n", inst->devid, inst->tsid);
        return -ENOMEM;
    }

    ka_task_spin_lock_init(&event_table_all_reg_lock[inst->devid][inst->tsid]);
    stars_event_table_ns_all_reg[inst->devid][inst->tsid] = base_va;

    return 0;
}

void trs_uninit_event_tbl_ns_base_addr(struct trs_id_inst *inst)
{
    void *base_va = (void *)stars_event_table_ns_all_reg[inst->devid][inst->tsid];

    ka_task_spin_lock(&event_table_all_reg_lock[inst->devid][inst->tsid]);
    if (stars_event_table_ns_all_reg[inst->devid][inst->tsid] != NULL) {
        stars_event_table_ns_all_reg[inst->devid][inst->tsid] = NULL;
    }
    ka_task_spin_unlock(&event_table_all_reg_lock[inst->devid][inst->tsid]);

    if (base_va != NULL) {
        ka_mm_iounmap(base_va);
    }
}

static StarsEventTable *trs_get_event_table(struct trs_id_inst *inst, u32 id)
{
    u32 group_id = id / STARS_TABLE_EVENT_NUM;
    u32 offset = id % STARS_TABLE_EVENT_NUM;
    StarsEventGroupTableInfo *group_info = NULL;
    StarsEventTableNsRegsType *tbl_info = (StarsEventTableNsRegsType *)stars_event_table_ns_all_reg[inst->devid][inst->tsid];
    if (tbl_info == NULL) {
        return NULL;
    }

    group_info = (StarsEventGroupTableInfo *)&(tbl_info->StarsEventGroupTable[group_id]);
    return &(group_info->starsEventTable[offset]);
}

int trs_stars_get_event_tbl_flag(struct trs_id_inst *inst, u32 id)
{
    int tbl_flag = 0;

    ka_task_spin_lock(&event_table_all_reg_lock[inst->devid][inst->tsid]);
    if (trs_get_event_table(inst, id) != NULL) {
        tbl_flag = trs_get_event_table(inst, id)->bits.eventTableFlag;
    }
    ka_task_spin_unlock(&event_table_all_reg_lock[inst->devid][inst->tsid]);

    return tbl_flag;
}

void trs_stars_set_event_tbl_flag(struct trs_id_inst *inst, u32 id, u32 val)
{
    ka_task_spin_lock(&event_table_all_reg_lock[inst->devid][inst->tsid]);
    if (trs_get_event_table(inst, id) != NULL) {
        trs_get_event_table(inst, id)->bits.eventTableFlag = val;
    }
    ka_task_spin_unlock(&event_table_all_reg_lock[inst->devid][inst->tsid]);
}

void trs_stars_reset_event(struct trs_id_inst *inst, u32 id)
{
    ka_task_spin_lock(&event_table_all_reg_lock[inst->devid][inst->tsid]);
    if (trs_get_event_table(inst, id) != NULL) {
        ka_mm_writel_relaxed(0, &trs_get_event_table(inst, id)->bits);
    }
    ka_task_spin_unlock(&event_table_all_reg_lock[inst->devid][inst->tsid]);
}
