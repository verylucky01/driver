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

#include "ascend_kernel_hal.h"
#include "trs_pub_def.h"
#include "trs_ub_info.h"

struct trs_ub_info g_trs_ub_info[TRS_TS_INST_MAX_NUM];

int trs_ub_info_query(struct trs_id_inst *inst, u32 *die_id, u32 *func_id)
{
    u32 ts_inst_id = trs_id_inst_to_ts_inst(inst);

    *die_id = g_trs_ub_info[ts_inst_id].die_id;
    *func_id = g_trs_ub_info[ts_inst_id].func_id;

    return 0;
}

#ifndef EMU_ST
void trs_ub_info_set(u32 devid, struct trs_ub_info *ub_info)
{
    struct trs_id_inst inst;
    u32 ts_inst_id;

    inst.devid = devid;
    inst.tsid = 0;
    ts_inst_id = trs_id_inst_to_ts_inst(&inst);

    g_trs_ub_info[ts_inst_id].die_id = ub_info->die_id;
    g_trs_ub_info[ts_inst_id].func_id = ub_info->func_id;

    trs_info("devid=%u, dieid=%u, funcid=%u\n", devid, g_trs_ub_info[ts_inst_id].die_id,
        g_trs_ub_info[ts_inst_id].func_id);
}
#endif
