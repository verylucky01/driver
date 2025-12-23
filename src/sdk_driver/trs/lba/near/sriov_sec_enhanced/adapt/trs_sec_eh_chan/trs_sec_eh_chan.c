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
#include "securec.h"
#include "soc_adapt.h"

#include "ascend_kernel_hal.h"
#include "trs_chan.h"
#include "trs_pm_adapt.h"
#include "trs_sec_eh_msg.h"
#include "trs_chan_stars_v1_ops.h"
#include "trs_chan_near_ops_rsv_mem.h"
#include "trs_chan_update.h"
#include "trs_sec_eh_vpc.h"
#include "trs_sec_eh_chan.h"
#include "trs_sec_eh_auto_init.h"

int trs_sec_eh_chan_sqe_update(struct trs_id_inst *inst, struct trs_sqe_update_info *update_info)
{
    struct trs_sec_eh_sq_update_info sqe_update;
    const size_t data_size = 256; /* 256 bytes is max size */
    int ret;

    ret = trs_chan_ops_sqe_update(inst, update_info);
    if (ret != 0) {
        trs_err("Sqe update fail. (ret=%d)\n", ret);
        return ret;
    }

    sqe_update.head.cmd_type = TRS_SEC_EH_SQE_UPDATE;
    sqe_update.head.result = 0;
    sqe_update.head.tsid = inst->tsid;
    sqe_update.head.rsv = 0;
    sqe_update.pid = update_info->pid;
    sqe_update.sqid = update_info->sqid;
    sqe_update.sqe_num = 1; /* Send 1 sqe for default */
    sqe_update.first_sqeid = update_info->sqeid;
    ret = memcpy_s((void *)sqe_update.data, data_size, update_info->sqe, TRS_HW_SQE_SIZE);
    if (ret != EOK) {
        trs_err("Sqe copy fail. (ret=%d; max_size=0x%ld; sqe_size=0x%d)\n", ret, data_size, TRS_HW_SQE_SIZE);
        return ret;
    }
    ret = trs_sec_eh_vpc_msg_send(inst->devid, (void *)&sqe_update,
        sizeof(struct trs_sec_eh_sq_update_info) - (data_size - TRS_HW_SQE_SIZE));
    if ((ret != 0) || (sqe_update.head.result != 0)) {
        trs_err("Vpc send fail. (devid=%u; ret=%d; result=%d)\n", inst->devid, ret, sqe_update.head.result);
        return -EFAULT;
    }
    return 0;
}

int trs_sec_eh_chan_stars_ops_query_sqcq(struct trs_id_inst *inst, struct trs_chan_type *types, u32 id,
    u32 cmd, u64 *value)
{
    return trs_chan_ops_query_sqcq(inst, types, id, cmd, value);
}

int trs_chan_config(struct trs_id_inst *inst)
{
    struct trs_chan_adapt_ops *chan_ops = NULL;
    int ret;

    ret = trs_soc_chan_stars_ops_init(inst);
    if (ret != 0) {
        trs_chan_near_ops_rsv_mem_uninit(inst);
        return ret;
    }

    chan_ops = trs_soc_chan_get_stars_adapt_ops(inst);
    chan_ops->sqe_update = trs_sec_eh_chan_sqe_update;
    chan_ops->sqcq_query = trs_sec_eh_chan_stars_ops_query_sqcq;
    ret = trs_chan_ts_inst_register(inst, trs_soc_get_hw_type(inst->devid), chan_ops);
    if (ret != 0) {
        trs_soc_chan_stars_ops_uninit(inst);
    }
    return ret;
}

void trs_chan_deconfig(struct trs_id_inst *inst)
{
    trs_chan_ts_inst_unregister(inst);
    trs_soc_chan_stars_ops_uninit(inst);
}

int trs_chan_init(u32 ts_inst_id)
{
    struct trs_id_inst inst;
    int ret;

    trs_ts_inst_to_id_inst(ts_inst_id, &inst);
    ret = trs_chan_config(&inst);
    if (ret != 0) {
        trs_err("Failed to config channel. (devid=%u; tsid=%u; ret=%d)\n", inst.devid, inst.tsid, ret);
    }
    return ret;
}
DECLAER_FEATURE_AUTO_INIT_DEV(trs_chan_init, FEATURE_LOADER_STAGE_4);

void trs_chan_uninit(u32 ts_inst_id)
{
    struct trs_id_inst inst;

    trs_ts_inst_to_id_inst(ts_inst_id, &inst);
    trs_chan_deconfig(&inst);
}
DECLAER_FEATURE_AUTO_UNINIT_DEV(trs_chan_uninit, FEATURE_LOADER_STAGE_4);
