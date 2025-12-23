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
#include "trs_host_db.h"
#include "trs_host_hard_mbox.h"
#include "trs_sec_eh_agent_chan.h"

#include "trs_sec_eh_agent_dev_init.h"

typedef int (* trs_config_list)(struct trs_id_inst *);
typedef void (* trs_decofnig_list)(struct trs_id_inst *);

static const trs_config_list g_trs_hw_init[] = {
#ifndef EMU_ST
    trs_ts_db_config,
    trs_mbox_config,
#endif
};

static const trs_decofnig_list g_trs_hw_uninit[] = {
#ifndef EMU_ST
    trs_ts_db_deconfig,
    trs_mbox_deconfig,
#endif
};

static const trs_config_list g_trs_func_init[] = {
    trs_agent_chan_config,
};

static const trs_decofnig_list g_trs_func_uninit[] = {
    trs_agent_chan_deconfig,
};

static int trs_ts_hw_init(struct trs_id_inst *inst)
{
    int type, ret, i;

    for (type = 0; type < ARRAY_SIZE(g_trs_hw_init); type++) {
        ret = g_trs_hw_init[type](inst);
        if (ret != 0) {
            for (i = type - 1; i >= 0; i--) {
                g_trs_hw_uninit[i](inst);
            }
            trs_err("Trs ts hw init fail. (devid=%u; tsid=%u; type=%d)\n", inst->devid, inst->tsid, type);
            return ret;
        }
    }
    return 0;
}

static void trs_ts_hw_uninit(struct trs_id_inst *inst)
{
    int type;

    for (type = ARRAY_SIZE(g_trs_hw_uninit) - 1; type >= 0; type--) {
        g_trs_hw_uninit[type](inst);
    }
}

static int trs_ts_func_init(struct trs_id_inst *inst)
{
    int type, ret, i;

    for (type = 0; type < ARRAY_SIZE(g_trs_func_init); type++) {
        ret = g_trs_func_init[type](inst);
        if (ret != 0) {
#ifndef EMU_ST
            for (i = type - 1; i >= 0; i--) {
                g_trs_func_uninit[i](inst);
            }
            return ret;
#endif
        }
    }

    return 0;
}

static void trs_ts_func_uninit(struct trs_id_inst *inst)
{
    int type;

    for (type = ARRAY_SIZE(g_trs_func_uninit) - 1; type >= 0; type--) {
        g_trs_func_uninit[type](inst);
    }
}

int trs_sec_eh_ts_init(struct trs_id_inst *inst)
{
    int ret;

    ret = trs_ts_hw_init(inst);
    if (ret != 0) {
        return ret;
    }

    ret = trs_ts_func_init(inst);
    if (ret != 0) {
        trs_ts_hw_uninit(inst);
        return ret;
    }

    return 0;
}

void trs_sec_eh_ts_uninit(struct trs_id_inst *inst)
{
    trs_ts_func_uninit(inst);
    trs_ts_hw_uninit(inst);
}
