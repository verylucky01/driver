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
#include "trs_chan.h"
#include "trs_core.h"
#include "trs_core_stars_v2_ops.h"
#include "trs_chan_stars_v2_ops.h"
#include "soc_adapt.h"
#include "soc_adapt_res_cloud_v4.h"

int trs_soc_cloud_v4_chan_stars_init(struct trs_id_inst *inst)
{
    int ret;

    ret = trs_chan_stars_v2_ops_init(inst);
    if (ret != 0) {
        return ret;
    }

    ret = trs_chan_ts_inst_register(inst, trs_soc_get_hw_type(inst->devid), trs_chan_get_stars_v2_adapt_ops());
    if (ret != 0) {
        trs_chan_stars_v2_ops_uninit(inst);
    }
    return ret;
}

void trs_soc_cloud_v4_chan_stars_uninit(struct trs_id_inst *inst)
{
    trs_chan_ts_inst_unregister(inst);
    trs_chan_stars_v2_ops_uninit(inst);
}

#ifndef EMU_ST
int trs_soc_cloud_v4_chan_stars_ops_init(struct trs_id_inst *inst)
{
    return trs_chan_stars_v2_ops_init(inst);
}

void trs_soc_cloud_v4_chan_stars_ops_uninit(struct trs_id_inst *inst)
{
    trs_chan_stars_v2_ops_uninit(inst);
}

struct trs_chan_adapt_ops *trs_chan_cloud_v4_get_stars_adapt_ops(void)
{
    return trs_chan_get_stars_v2_adapt_ops();
}
#endif

struct trs_core_adapt_ops *trs_core_cloud_v4_get_stars_adapt_ops(void)
{
    return trs_core_get_stars_v2_adapt_ops();
}

int trs_soc_cloud_v4_sq_send_trigger_db_init(struct trs_id_inst *inst)
{
    return 0;
}

void trs_soc_cloud_v4_sq_send_trigger_db_uninit(struct trs_id_inst *inst)
{
    return;
}
