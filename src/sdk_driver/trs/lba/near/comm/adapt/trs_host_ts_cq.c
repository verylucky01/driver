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
#include <securec.h>
#include "ascend_kernel_hal.h"
#include "trs_chan.h"
#include "trs_res_id_def.h"
#include "trs_mailbox_def.h"
#include "trs_host_ts_cq.h"

static struct trs_ts_cq_info g_trs_ts_cq_info[TRS_DEV_MAX_NUM];

struct trs_ts_cq_info *trs_get_ts_cq_info(u32 udevid)
{
    if (udevid < TRS_DEV_MAX_NUM) {
        return &(g_trs_ts_cq_info[udevid]);
    }
    return NULL;
}

int trs_register_ts_cq_process_info(u32 udevid, ts_cq_process_func func, void *para)
{
    if (udevid >= TRS_DEV_MAX_NUM) {
        return -EINVAL;
    }
    g_trs_ts_cq_info[udevid].host_ts_cq_process_func = func;
    g_trs_ts_cq_info[udevid].para = para;
    trs_info("Register ts cq process func success. (udevid=%u)\n", udevid);
    return 0;
}

void trs_unregister_ts_cq_process_info(u32 udevid)
{
    if (udevid >= TRS_DEV_MAX_NUM) {
        return;
    }
    g_trs_ts_cq_info[udevid].host_ts_cq_process_func = NULL;
    g_trs_ts_cq_info[udevid].para = NULL;
    trs_info("Unregister ts cq process func success. (udevid=%u)\n", udevid);
}

int trs_ts_cq_cpy(struct trs_id_inst *inst, u32 cqid, u32 cq_type, u8 *cqe)
{
    struct trs_chan_cq_info cq_info;
    u32 chan_id, cq_head;
    int ret;

    if (cq_type != CHAN_SUB_TYPE_MAINT_DBG) {
#ifndef EMU_ST
        trs_err("Invalid cq type. (cq_type=%d)\n", cq_type);
        return -EINVAL;
#endif
    }

    ret = trs_chan_get_chan_id(inst, TRS_MAINT_CQ, cqid, &chan_id);
    if (ret != 0) {
#ifndef EMU_ST
        trs_err("Failed to get chan id. (ret=%d; devid=%u; cqid=%u)\n", ret, inst->devid, cqid);
        return ret;
#endif
    }

    ret = trs_chan_get_cq_info(inst, chan_id, &cq_info);
    if (ret != 0) {
#ifndef EMU_ST
        trs_err("Failed to get cq info. (ret=%d; devid=%u; cqid=%u)\n", ret, inst->devid, cqid);
        return ret;
#endif
    }

    ret = trs_chan_query(inst, chan_id, CHAN_QUERY_CMD_CQ_HEAD, &cq_head);
    if (ret != 0) {
#ifndef EMU_ST
        trs_err("Failed to query cq head. (ret=%d; devid=%u; cqid=%u; chan_id=%u)\n",
            ret, inst->devid, cqid, chan_id);
        return ret;
#endif
    }

#ifndef EMU_ST
    ret = memcpy_s(cq_info.cq_vaddr + cq_info.cq_para.cqe_size * cq_head, cq_info.cq_para.cqe_size,
        cqe, TRS_MAINT_MAX_CQE_SIZE);
    if (ret != 0) {
        trs_err("Memcpy failed. (ret=%d; cqe_size=%u)\n", ret, cq_info.cq_para.cqe_size);
        return -EFAULT;
    }
#endif

    trs_debug("Cq copy success. (devid=%u; tsid=%u; cqid=%u; chan_id=%u; cq_head=%u)\n",
        inst->devid, inst->tsid, cqid, chan_id, cq_head);
    return 0;
}
