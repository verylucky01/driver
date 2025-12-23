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
#include "trs_chan.h"
#include "trs_id.h"
#include "trs_res_id_def.h"
#include "ascend_kernel_hal.h"
#include "trs_mailbox_def.h"
#include "trs_chan_stars_v2_maint_sqcq.h"

#define TRS_MAINT_MBOX_TIMEOUT_MS 3000

static int trs_stars_v2_maint_sq_cpy(struct trs_id_inst *inst, u32 sqid, u32 tail, u8 *sqe, u32 *pos)
{
    struct trs_chan_sq_info sq_info;
    int ret, chan_id;
    u32 sq_pos;

    ret = trs_chan_get_chan_id(inst, TRS_MAINT_SQ, sqid, &chan_id);
    if (ret != 0) {
        trs_err("Failed to get chan id. (ret=%d; devid=%u; sqid=%u)\n", ret, inst->devid, sqid);
        return ret;
    }

    ret = trs_chan_get_sq_info(inst, chan_id, &sq_info);
    if (ret != 0) {
        trs_err("Failed to get sq info. (ret=%d; devid=%u; sqid=%u)\n", ret, inst->devid, sqid);
        return ret;
    }
    ret = trs_chan_query(inst, chan_id, CHAN_QUERY_CMD_SQ_POS, &sq_pos);
    if (ret != 0) {
        trs_err("Failed to query sq position. (ret=%d; devid=%u; sqid=%u)\n", ret, inst->devid, sqid);
        return ret;
    }

#ifndef EMU_ST
    if ((sq_pos + 1) % sq_info.sq_para.sq_depth != tail) {
        trs_err("Invalid sqe_num. (pos=%u; sq_tail=%u)\n", sq_pos, tail);
        return -EINVAL;
    }

    ret = memcpy_s(sqe, TRS_MAINT_MAX_SQE_SIZE,
        sq_info.sq_vaddr + sq_info.sq_para.sqe_size * sq_pos, sq_info.sq_para.sqe_size);
    if (ret != 0) {
        trs_err("Memcpy failed. (ret=%d; sqe_size=%u)\n", ret, sq_info.sq_para.sqe_size);
        return -EFAULT;
    }
#endif

    *pos = sq_pos;
    trs_debug("Copy ts sq success. (devid=%u; sqid=%u; sq_pos=%u)\n", inst->devid, sqid, sq_pos);
    return 0;
}

static int trs_stars_v2_maint_set_sq_tail(struct trs_id_inst *inst, u32 sqid, u32 type, u32 tail)
{
    struct trs_sq_task_send_mailbox mbox_data = {0};
    int ret;

    trs_mbox_init_header(&mbox_data.header, TRS_MBOX_SQ_TASK_SEND);
    mbox_data.sq_type = type;
    mbox_data.sqid = sqid;
    mbox_data.sq_tail = tail;
    ret = trs_stars_v2_maint_sq_cpy(inst, sqid, tail, mbox_data.sqe, &mbox_data.pos);
    if (ret != 0) {
        trs_err("Failed to copy sq task. (devid=%u; sqid=%u; sq_type=%u; ret=%d)\n", inst->devid, sqid, type, ret);
        return ret;
    }

    ret = trs_mbox_send(inst, 0, &mbox_data, sizeof(struct trs_sq_task_send_mailbox), TRS_MAINT_MBOX_TIMEOUT_MS);
    if ((ret != 0) || (mbox_data.header.result != 0)) {
        trs_err("Mbox send fail. (devid=%u; tsid=%u; result=%u; ret=%d)\n",
            inst->devid, inst->tsid, mbox_data.header.result, ret);
        ret = -EFAULT;
    }
    trs_debug("Maint set sq tail. (devid=%u; sqid=%u; sq_type=%u; sq_tail=%u)\n",
        inst->devid, sqid, type, tail);
    return ret;
}

static int trs_stars_v2_maint_cq_update_head(struct trs_id_inst *inst, u32 cqid, u32 type, u32 head)
{
    struct trs_cq_report_recv_mailbox mbox_data = {0};
    int ret;

    trs_mbox_init_header(&mbox_data.header, TRS_MBOX_CQ_REPORT_RECV);
    mbox_data.cq_type = type;
    mbox_data.cqid = cqid;
    mbox_data.cq_head = head;

    ret = trs_mbox_send(inst, 0, &mbox_data, sizeof(struct trs_cq_report_recv_mailbox), TRS_MAINT_MBOX_TIMEOUT_MS);
    if ((ret != 0) || (mbox_data.header.result != 0)) {
        trs_err("Mbox send fail. (devid=%u; tsid=%u; result=%u; ret=%d)\n",
            inst->devid, inst->tsid, mbox_data.header.result, ret);
        ret = -EFAULT;
    }
    trs_debug("Maint cq update head. (devid=%u; cqid=%u; cq_type=%u; cq_head=%u)\n",
        inst->devid, cqid, type, head);
    return ret;
}

int trs_stars_v2_chan_ops_ctrl_maint_sqcq(struct trs_id_inst *inst, u32 id, u32 type, u32 cmd, u32 para)
{
    int ret = -EINVAL;

    switch (cmd) {
        case CTRL_CMD_SQ_TAIL_UPDATE:
            ret = trs_stars_v2_maint_set_sq_tail(inst, id, type, para);
            break;
        case CTRL_CMD_CQ_HEAD_UPDATE:
            ret = trs_stars_v2_maint_cq_update_head(inst, id, type, para);
            break;
        default:
            break;
    }

    return ret;
}
