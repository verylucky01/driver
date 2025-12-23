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
#include "pbl/pbl_soc_res.h"
#include "soc_adapt.h"
#include "trs_ts_db.h"
#include "trs_host_db.h"
#ifdef CFG_FEATURE_TRS_SIA_ADAPT
#include "trs_sia_adapt_auto_init.h"
#elif defined(CFG_FEATURE_TRS_SEC_EH_ADAPT)
#include "trs_sec_eh_auto_init.h"
#endif

int trs_ts_db_config(struct trs_id_inst *inst)
{
    struct soc_reg_base_info io_base;
    struct res_inst_info res_inst;
    size_t stride;
    int ret;

    soc_resmng_inst_pack(&res_inst, inst->devid, TS_SUBSYS, inst->tsid);
    ret = soc_resmng_get_reg_base(&res_inst, "TS_DOORBELL_REG", &io_base);
    if (ret != 0) {
        trs_err("Get ts doorbell reg fail. (devid=%u; tsid=%u; ret=%d)\n", inst->devid, inst->tsid, ret);
        return ret;
    }

    ret = trs_soc_get_db_stride(inst, &stride);
    if (ret != 0) {
        trs_err("Get soc db stride fail. (ret=%d)\n", ret);
        return ret;
    }

    ret = trs_ts_db_cfg(inst, io_base.io_base, io_base.io_base_size, stride);
    if (ret != 0) {
        trs_err("Config ts doorbell reg addr fail. (devid=%u; tsid=%u; ret=%d)\n", inst->devid, inst->tsid, ret);
    }

    return ret;
}
KA_EXPORT_SYMBOL_GPL(trs_ts_db_config);

void trs_ts_db_deconfig(struct trs_id_inst *inst)
{
    trs_ts_db_decfg(inst);
}
KA_EXPORT_SYMBOL_GPL(trs_ts_db_deconfig);

int trs_ts_doorbell_init(u32 ts_inst_id)
{
    struct trs_id_inst inst;
    int ret;

    trs_ts_inst_to_id_inst(ts_inst_id, &inst);
    ret = trs_ts_db_config(&inst);
    if (ret != 0) {
        trs_err("Failed to config ts doorbell. (devid=%u; tsid=%u; ret=%d)\n", inst.devid, inst.tsid, ret);
    }
    return ret;
}
DECLAER_FEATURE_AUTO_INIT_DEV(trs_ts_doorbell_init, FEATURE_LOADER_STAGE_1);

void trs_ts_doorbell_uninit(u32 ts_inst_id)
{
    struct trs_id_inst inst;

    trs_ts_inst_to_id_inst(ts_inst_id, &inst);
    trs_ts_db_deconfig(&inst);
}
DECLAER_FEATURE_AUTO_UNINIT_DEV(trs_ts_doorbell_uninit, FEATURE_LOADER_STAGE_1);
