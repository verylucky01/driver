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
#include "stars_notify_tbl_c_union_define.h"
#include "trs_pub_def.h"
#include "trs_stars_comm.h"
#include "stars_notify_tbl.h"

static volatile StarsNotifyTblRegsType *stars_notify_table_ns_all_reg[TRS_DEV_MAX_NUM][TRS_TS_MAX_NUM] = {NULL};
static KA_TASK_DEFINE_RWLOCK(notify_table_all_reg_lock);
int trs_init_notify_tbl_ns_base_addr(struct trs_id_inst *inst)
{
    struct soc_reg_base_info io_base;
    struct res_inst_info res_inst;
    void *base_va;
    int ret;

    soc_resmng_inst_pack(&res_inst, inst->devid, TS_SUBSYS, inst->tsid);
    ret = soc_resmng_get_reg_base(&res_inst, "TS_STARS_NOTIFY_TBL_REG", &io_base);
    if (ret != 0) {
        trs_warn("No event status reg base. (devid=%u; tsid=%u)\n", inst->devid, inst->tsid);
        return ret;
    }

    base_va = ka_mm_ioremap(io_base.io_base, io_base.io_base_size);
    if (base_va == NULL) {
        trs_err("Failed to ioremap. (devid=%u; tsid=%u)\n", inst->devid, inst->tsid);
        return -ENOMEM;
    }

    ka_task_write_lock(&notify_table_all_reg_lock);
    stars_notify_table_ns_all_reg[inst->devid][inst->tsid] = base_va;
    ka_task_write_unlock(&notify_table_all_reg_lock);
    return 0;
}

void trs_uninit_notify_tbl_ns_base_addr(struct trs_id_inst *inst)
{
    void *reg_va = NULL;

    ka_task_write_lock(&notify_table_all_reg_lock);
    reg_va = (void *)stars_notify_table_ns_all_reg[inst->devid][inst->tsid];
    stars_notify_table_ns_all_reg[inst->devid][inst->tsid] = NULL;
    ka_task_write_unlock(&notify_table_all_reg_lock);

    if (reg_va != NULL) {
        ka_mm_iounmap(reg_va);
    }
}

static StarsNotifyTableSlice *trs_get_notify_table(struct trs_id_inst *inst, u32 id)
{
    StarsNotifyTblRegsType *tbl_info = (StarsNotifyTblRegsType *)stars_notify_table_ns_all_reg[inst->devid][inst->tsid];

    if (tbl_info != NULL) {
        return trs_get_stars_notify_tab_slice(tbl_info, id);
    }

    return NULL;
}

int trs_stars_get_notify_tbl_flag(struct trs_id_inst *inst, u32 id)
{
    StarsNotifyTableSlice *slice = NULL;
    int tbl_flag = INT_MAX;

    ka_task_read_lock(&notify_table_all_reg_lock);
    slice = trs_get_notify_table(inst, id);
    if (slice != NULL) {
        tbl_flag = (int)(slice->bits.notifyTableFlagSlice);
    }
    ka_task_read_unlock(&notify_table_all_reg_lock);

    return tbl_flag;
}

void trs_stars_set_notify_tbl_flag(struct trs_id_inst *inst, u32 id, u32 status)
{
    StarsNotifyTableSlice *slice = NULL;
    u32 read_status = 0;

    ka_task_read_lock(&notify_table_all_reg_lock);
    slice = trs_get_notify_table(inst, id);
    if (slice != NULL) {
        slice->bits.notifyTableFlagSlice = status;
        read_status = slice->bits.notifyTableFlagSlice;
    }
    ka_task_read_unlock(&notify_table_all_reg_lock);
    trs_debug("Read after clear. (id=%u; read_status=%u; status=%u)\n", id, read_status, status);
}

void trs_stars_set_notify_tbl_pending_clr(struct trs_id_inst *inst, u32 id, u32 status)
{
    StarsNotifyTableSlice *slice = NULL;

    ka_task_read_lock(&notify_table_all_reg_lock);
    slice = trs_get_notify_table(inst, id);
    if (slice != NULL) {
        slice->bits.notifyTablePendingClrSlice = status;
    }
    ka_task_read_unlock(&notify_table_all_reg_lock);
}
