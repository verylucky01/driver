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

#include "ka_kernel_def_pub.h"
#include "pbl/pbl_uda.h"
#include "comm_kernel_interface.h"
#include "pbl/pbl_soc_res.h"

#include "trs_res_id_def.h"
#include "trs_stars_com.h"
#include "trs_stars_comm.h"
#include "trs_stars_func_com.h"
#include "trs_stars_func_adapt.h"
#include "stars_event_tbl_ns.h"
#include "stars_notify_tbl.h"
#include "stars_cnt_notify_tbl.h"
#include "trs_stars_function.h"

static int trs_stars_func_event_id_ctrl(struct trs_id_inst *inst, u32 id, u32 cmd)
{
    if (cmd == TRS_STARS_RES_OP_RESET) {
        return trs_stars_func_event_id_reset(inst, id);
    }

    if (cmd == TRS_STARS_RES_OP_CHECK_AND_RESET) {
        return trs_stars_func_event_id_check_and_reset(inst, id);
    }

    if (cmd == TRS_STARS_RES_OP_RECORD) {
        return trs_stars_func_event_id_record(inst, id);
    }

    return -EOPNOTSUPP;
}

static int trs_stars_func_notify_id_ctrl(struct trs_id_inst *inst, u32 id, u32 cmd)
{
    if (cmd == TRS_STARS_RES_OP_RECORD) {
        return trs_stars_func_notify_id_record(inst, id);
    }

    if ((cmd == TRS_STARS_RES_OP_RESET) || (cmd == TRS_STARS_RES_OP_CHECK_AND_RESET)) {
        return trs_stars_func_notify_id_reset(inst, id);
    }

    return -EOPNOTSUPP;
}

int trs_stars_func_res_id_ctrl(struct trs_id_inst *inst, u32 type, u32 id, u32 cmd)
{
    if (type == TRS_EVENT) {
        return trs_stars_func_event_id_ctrl(inst, id, cmd);
    }

    if (type == TRS_NOTIFY) {
        return trs_stars_func_notify_id_ctrl(inst, id, cmd);
    }

    if (type == TRS_CNT_NOTIFY) {
        return trs_stars_func_cnt_notify_id_ctrl(inst, id, cmd);
    }

    return -EOPNOTSUPP;
}

static struct trs_stars_ops trs_stars_func_ops = {
    .res_id_ctrl = trs_stars_ops_func_res_id_ctrl,
};

struct trs_stars_ops *trs_stars_func_op_get(void)
{
    return &trs_stars_func_ops;
}

int trs_stars_func_init(struct trs_id_inst *inst)
{
    int ret;

    ret = trs_init_event_tbl_ns_base_addr(inst);
    if (ret != 0) {
#ifndef EMU_ST
        trs_warn("Without event table addr.\n");
#endif
    }

    ret = trs_init_notify_tbl_ns_base_addr(inst);
    if (ret != 0) {
        trs_warn("Without notify table addr.\n");
    }

    ret = trs_init_cnt_notify_tbl(inst);
    if (ret != 0) {
        trs_warn("Without cnt notify table addr.\n");
    }

    return 0;
}

void trs_stars_func_uninit(struct trs_id_inst *inst)
{
    trs_uninit_cnt_notify_tbl(inst);
    trs_uninit_notify_tbl_ns_base_addr(inst);
    trs_uninit_event_tbl_ns_base_addr(inst);
}

#define TRS_STARS_NOTIFIER "trs_stars"
int trs_stars_notifier_register(void)
{
    struct uda_dev_type type;
    int ret;

    uda_davinci_near_real_entity_type_pack(&type);
    ret = uda_notifier_register(TRS_STARS_NOTIFIER, &type, UDA_PRI1, trs_stars_notifier_func);
    if (ret != 0) {
        trs_err("Register near notifier failed. (ret=%d)\n", ret);
        return ret;
    }
    return ret;
}

void trs_stars_notifier_unregister(void)
{
    struct uda_dev_type type;

    uda_davinci_near_real_entity_type_pack(&type);
    (void)uda_notifier_unregister(TRS_STARS_NOTIFIER, &type);
}
KA_MODULE_SOFTDEP("pre: ascend_soc_platform");

