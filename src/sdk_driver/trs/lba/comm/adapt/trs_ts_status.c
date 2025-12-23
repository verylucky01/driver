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
#include "ka_base_pub.h"
#include "ka_kernel_def_pub.h"
#include "tsdrv_interface.h"
#include "pbl/pbl_soc_res.h"
#include "pbl/pbl_uda.h"
#include "dms_template.h"
#include "dms/dms_cmd_def.h"
#include "devdrv_user_common.h"
#include "urd_acc_ctrl.h"
#include "urd_feature.h"
#include "trs_pub_def.h"
#include "trs_ts_status.h"
#ifdef CFG_FEATURE_TRS_SIA_ADAPT
#include "trs_sia_adapt_auto_init.h"
#elif defined(CFG_FEATURE_TRS_SEC_EH_ADAPT)
#include "trs_sec_eh_auto_init.h"
#endif

int trs_set_ts_status(struct trs_id_inst *inst, u32 status)
{
    struct res_inst_info res_inst;
    int ret;

    soc_resmng_inst_pack(&res_inst, inst->devid, TS_SUBSYS, inst->tsid);
    ret = soc_resmng_set_ts_status(&res_inst, status);
    if (ret != 0) {
        trs_err("Set ts status failed. (devid=%u; tsid=%u; status=%u; ret=%d)\n",
            inst->devid, inst->tsid, status, ret);
        return ret;
    }

    trs_info("Set ts status success. (devid=%u; tsid=%u; status=%u)\n", inst->devid, inst->tsid, status);
    return 0;
}
KA_EXPORT_SYMBOL_GPL(trs_set_ts_status);

int trs_get_ts_status(struct trs_id_inst *inst, u32 *status)
{
    struct res_inst_info res_inst;
    int ret;

    soc_resmng_inst_pack(&res_inst, inst->devid, TS_SUBSYS, inst->tsid);
    ret = soc_resmng_get_ts_status(&res_inst, status);
    if (ret != 0) {
        trs_err("Get ts status failed. (devid=%u; tsid=%u; ret=%d)\n", inst->devid, inst->tsid, ret);
        return ret;
    }

    return 0;
}
KA_EXPORT_SYMBOL_GPL(trs_get_ts_status);

int trs_get_ts_status_ioctl(void *feature, char *in, u32 in_len, char *out, u32 out_len)
{
    enum devdrv_device_status {    /* Keep consistent with the ascend_hal.h */
        DRV_STATUS_INITING = 0x0,
        DRV_STATUS_WORK,
        DRV_STATUS_EXCEPTION,
        DRV_STATUS_SLEEP,
        DRV_STATUS_COMMUNICATION_LOST,
        DRV_STATUS_RESERVED
    };
    u32 trs_ts_status_tansfrom[TRS_INST_STATUS_MAX] = {
        [TRS_INST_STATUS_UNINIT] = DRV_STATUS_INITING,
        [TRS_INST_STATUS_NORMAL] = DRV_STATUS_WORK,
        [TRS_INST_STATUS_ABNORMAL] = DRV_STATUS_EXCEPTION
    };
    struct tsdrv_id_inst tsdrv_inst;
    struct trs_id_inst inst;
    u32 phys_id = DEVDRV_MAX_DAVINCI_NUM;
    u32 vf_id = 0;
    u32 status;
    int ret;

    if ((in == NULL) || (out == NULL) || (in_len != sizeof(struct tsdrv_id_inst)) || (out_len != sizeof(u32))) {
        trs_err("Input invalid. (in=%pK; out=%pK; in_len=%u; out_len=%u)\n", in, out, in_len, out_len);
        return -EINVAL;
    }

    ret = memcpy_s(&tsdrv_inst, sizeof(struct tsdrv_id_inst), in, sizeof(struct tsdrv_id_inst));
    if (ret != 0) {
        trs_err("Get user info failed. (ret=%d)\n", ret);
        return ret;
    }

    ret = uda_devid_to_phy_devid(tsdrv_inst.devid, &phys_id, &vf_id);
    if (ret != 0) {
        trs_err("Failed to transfer logical ID to physical ID. (logic_id=%u; ret=%d)\n", tsdrv_inst.devid, ret);
        return ret;
    }

    trs_id_inst_pack(&inst, phys_id, tsdrv_inst.tsid);
    ret = trs_get_ts_status(&inst, &status);
    if (ret == 0) {
#ifndef EMU_ST
        if (status >= TRS_INST_STATUS_MAX) {
            trs_err("Invalid ts status. (devid=%u; tsid=%u; status=%u)\n", inst.devid, inst.tsid, status);
            return -EINVAL;
        }
#endif
        *(u32 *)out = trs_ts_status_tansfrom[status];
    }
    trs_debug("Get ts status. (devid=%u; tsid=%u; status=%u; ret=%d)\n", inst.devid, inst.tsid, status, ret);
    return ret;
}

#define DMS_MODULE_TRS_ADAPT    "trs_adapt"
BEGIN_DMS_MODULE_DECLARATION(DMS_MODULE_TRS_ADAPT)
BEGIN_FEATURE_COMMAND()
ADD_FEATURE_COMMAND(DMS_MODULE_TRS_ADAPT,
    DMS_MAIN_CMD_TRS,
    DMS_SUBCMD_GET_TS_HB_STATUS,
    NULL,
    NULL,
    DMS_SUPPORT_ALL_USER,
    trs_get_ts_status_ioctl)
END_FEATURE_COMMAND()
END_MODULE_DECLARATION()

static ka_atomic_t mng_ref = KA_BASE_ATOMIC_INIT(0);
void trs_ts_status_mng_init(struct trs_id_inst *inst)
{
    if (ka_base_atomic_inc_return(&mng_ref) == 1) {
        trs_info("trs_ts_status_feature register feature to dms\n");
        CALL_INIT_MODULE(DMS_MODULE_TRS_ADAPT);
    }
    (void)trs_set_ts_status(inst, TRS_INST_STATUS_NORMAL);
}
KA_EXPORT_SYMBOL_GPL(trs_ts_status_mng_init);

void trs_ts_status_mng_exit(struct trs_id_inst *inst)
{
    if (ka_base_atomic_dec_return(&mng_ref) == 0) {
        CALL_EXIT_MODULE(DMS_MODULE_TRS_ADAPT);
    }
    (void)trs_set_ts_status(inst, TRS_INST_STATUS_UNINIT);
}
KA_EXPORT_SYMBOL_GPL(trs_ts_status_mng_exit);

int trs_ts_status_init(u32 ts_inst_id)
{
    struct trs_id_inst inst;

    trs_ts_inst_to_id_inst(ts_inst_id, &inst);
    trs_ts_status_mng_init(&inst);
    trs_info("Ts status init success.\n");
    return 0;
}
DECLAER_FEATURE_AUTO_INIT_DEV(trs_ts_status_init, FEATURE_LOADER_STAGE_7);

void trs_ts_status_uninit(u32 ts_inst_id)
{
    struct trs_id_inst inst;

    trs_ts_inst_to_id_inst(ts_inst_id, &inst);
    trs_ts_status_mng_exit(&inst);
}
DECLAER_FEATURE_AUTO_UNINIT_DEV(trs_ts_status_uninit, FEATURE_LOADER_STAGE_7);
