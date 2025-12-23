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
#include "ascend_hal_define.h"

#include "soc_adapt.h"
#include "trs_core.h"
#include "trs_chan.h"
#include "trs_cdqm.h"
#include "trs_chan_update.h"
#include "trs_chip_def_comm.h"
#include "trs_core_near_ops.h"
#include "trs_near_adapt_init.h"
#ifdef CFG_FEATURE_TRS_SIA_ADAPT
#include "trs_sia_adapt_auto_init.h"
#elif defined(CFG_FEATURE_TRS_SEC_EH_ADAPT)
#include "trs_sec_eh_auto_init.h"
#endif

static int trs_chan_get_sq_info_by_sqid(struct trs_id_inst *inst, int res_type, u32 res_id, void *info)
{
    struct trs_chan_sq_info *sq_info = (struct trs_chan_sq_info *)info;
    int ret, chan_id;

    ret = trs_chan_get_chan_id(inst, res_type, res_id, &chan_id);
    if (ret != 0) {
        return -EINVAL;
    }

    ret = trs_chan_get_sq_info(inst, chan_id, sq_info);
    if (ret == 0) {
        trs_debug("Sq. (devid=%u; tsid=%u; sqid=%u)\n", inst->devid, inst->tsid, res_id);
    }

    return ret;
}

int trs_res_ops_init(u32 ts_inst_id)
{
    struct trs_res_ops ops = {{0}};
    int chip_type, hw_type, type;
    struct trs_id_inst inst;

    trs_ts_inst_to_id_inst(ts_inst_id, &inst);
    if (inst.tsid > 0) {
        return 0;
    }
    chip_type = trs_soc_get_chip_type(inst.devid);
    hw_type = trs_get_hw_type_by_chip_type(chip_type);
    for (type = 0; type < TRS_CORE_MAX_ID_TYPE; type++) {
        ops.res_belong_proc[type] = trs_res_is_belong_to_proc;
    }

    if (hw_type == TRS_HW_TYPE_STARS) {
        ops.res_get_info[TRS_HW_SQ] = trs_chan_get_sq_info_by_sqid;
    } else {
        ops.res_belong_proc[TRS_CDQ] = NULL;
    }
    trs_res_ops_register(inst.devid, &ops);
    trs_info("Res ops init success.\n");

    return 0;
}
DECLAER_FEATURE_AUTO_INIT_DEV(trs_res_ops_init, FEATURE_LOADER_STAGE_6);

void trs_res_ops_uninit(u32 ts_inst_id)
{
    struct trs_id_inst inst;

    trs_ts_inst_to_id_inst(ts_inst_id, &inst);
    if (inst.tsid == 0) {
        trs_res_ops_unregister(inst.devid);
    }
}
DECLAER_FEATURE_AUTO_UNINIT_DEV(trs_res_ops_uninit, FEATURE_LOADER_STAGE_6);
