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

#include "stars_cnt_notify_tbl.h"
#include "stars_notify_tbl.h"
#include "trs_stars_func_adapt.h"
#include "trs_res_id_def.h"
#include "trs_stars_com.h"
#include "trs_stars_comm.h"

int trs_init_cnt_notify_tbl(struct trs_id_inst *inst)
{
    return trs_init_cnt_notify_tbl_ns_base_addr(inst);
}

void trs_uninit_cnt_notify_tbl(struct trs_id_inst *inst)
{
    trs_uninit_cnt_notify_tbl_ns_base_addr(inst);
}

int trs_stars_func_cnt_notify_id_ctrl(struct trs_id_inst *inst, u32 id, u32 cmd)
{
    if ((cmd == TRS_STARS_RES_OP_RESET) || (cmd == TRS_STARS_RES_OP_CHECK_AND_RESET)) {
        trs_stars_set_cnt_notify_tbl_flag(inst, id, 0);
        return 0;
    }

    return -EOPNOTSUPP;
}

int trs_stars_func_notify_id_reset(struct trs_id_inst *inst, u32 id)
{
    trs_stars_set_notify_tbl_flag(inst, id, 0);
    trs_debug("Reset notify id. (devid=%u; notify_id=%u)\n", inst->devid, id);
    return 0;
}

int trs_stars_ops_func_res_id_ctrl(struct trs_id_inst *inst, u32 type, u32 id, u32 cmd)
{
    return trs_stars_func_res_id_ctrl(inst, type, id, cmd);
}
