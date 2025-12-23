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
#include "trs_msg.h"
#include "trs_mailbox_def.h"
#include "trs_host_msg.h"
#include "trs_mbox.h"
#include "trs_host_soft_mbox.h"
#include "trs_sia_adapt_auto_init.h"

static int trs_sqcq_ext_info_copy(struct trs_id_inst *inst, void *data, size_t size, struct trs_msg_data *msg_data)
{
    struct trs_normal_cqsq_mailbox *mbox_data = (struct trs_normal_cqsq_mailbox *)data;
    u32 info_len = sizeof(u32) * SQCQ_INFO_LENGTH;
    int ret;

    ret = memcpy_s(msg_data->payload + size, TRS_MSG_DATA_LEN - size, mbox_data->ts_info.info, info_len);
    if (ret != 0) {
        trs_err("Failed to memcpy info. (devid=%u)\n", inst->devid);
        return ret;
    }
    msg_data->data_len += info_len;
    trs_debug("Copy info success. (devid=%u; info[0]=%u; data_len=%llu)\n",
        inst->devid, mbox_data->ts_info.info[0], msg_data->data_len);

    if ((mbox_data->ts_info.ext_msg != NULL) && (mbox_data->ts_info.ext_msg_len > 0)) {
        ret = memcpy_s(msg_data->payload + size + info_len, TRS_MSG_DATA_LEN - size - info_len,
            mbox_data->ts_info.ext_msg, mbox_data->ts_info.ext_msg_len);
        if (ret != 0) {
            trs_err("Failed to memcpy ext_msg. (devid=%u; ext_msg_len=%u)\n", inst->devid,
                mbox_data->ts_info.ext_msg_len);
            return ret;
        }
        msg_data->data_len += mbox_data->ts_info.ext_msg_len;
        trs_debug("Copy ext_msg success. (devid=%u; ext_msg_len=%u; data_len=%llu)\n",
            inst->devid, mbox_data->ts_info.ext_msg_len, msg_data->data_len);
    }

    return 0;
}

int trs_soft_mbox_send(struct trs_id_inst *inst, u32 chan_id, void *data, size_t size, int timeout)
{
    struct trs_mb_header *header = (struct trs_mb_header *)data;
    struct trs_msg_data msg_data = {0};
    size_t msg_len;
    int ret;

    msg_data.header.devid = inst->devid;
    msg_data.header.tsid = inst->tsid;
    msg_data.header.valid = TRS_MSG_SEND_MAGIC;
    msg_data.header.cmdtype = TRS_MSG_NOTICE_TS;
    msg_data.header.result = 0;
    msg_data.data_len = size;

    ret = memcpy_s(msg_data.payload, TRS_MSG_DATA_LEN, data, size);
    if (ret != 0) {
        trs_err("Failed to memcpy. (devid=%u)\n", inst->devid);
        return ret;
    }

    if ((header->cmd_type == TRS_MBOX_CREATE_CQSQ_CALC) || (header->cmd_type == TRS_MBOX_RELEASE_CQSQ_CALC) ||
        (header->cmd_type == TRS_MBOX_CREATE_TOPIC_SQCQ) || (header->cmd_type == TRS_MBOX_RELEASE_TOPIC_SQCQ)) {
        ret = trs_sqcq_ext_info_copy(inst, data, size, &msg_data);
        if (ret != 0) {
            trs_err("Failed to copy ext info. (devid=%u)\n", inst->devid);
            return ret;
        }
    }

    msg_len = sizeof(struct trs_msg_head) + sizeof(u64) + msg_data.data_len;
    ret = trs_host_msg_send(inst->devid, &msg_data, msg_len);
    if ((ret != 0) || (msg_data.header.result != 0)) {
        if (ret == -ENODEV) {
            return -ENXIO;
        }
#ifndef EMU_ST
        trs_warn("Msg send warn. (devid=%u; tsid=%u; ret=%d; result=%d; data_len=%llu; msg_len=%u)\n",
            inst->devid, inst->tsid, ret, msg_data.header.result, msg_data.data_len, (u32)msg_len);
#endif
        return -ENODEV;
    }

    if (header->cmd_type == TRS_MBOX_RPC_CALL) {
        struct trs_rpc_call_msg *rpc_call_msg;
        rpc_call_msg = (struct trs_rpc_call_msg *)msg_data.payload;
        ret = memcpy_s(data, size, rpc_call_msg, msg_data.data_len);
        if (ret != 0) {
            trs_err("Failed to memcpy. (devid=%u; dest_size=0x%x; src_size=0x%x)\n",
                inst->devid, (u32)size, (u32)msg_data.data_len);
            return ret;
        }
    }

    return 0;
}

static struct trs_mbox_send_ops g_soft_mbox_ops[TRS_DEV_MAX_NUM];

int trs_soft_mbox_init(u32 ts_inst_id)
{
    struct trs_id_inst inst;

    trs_ts_inst_to_id_inst(ts_inst_id, &inst);
    if (inst.tsid == 0) {
        g_soft_mbox_ops[inst.devid].mbox_send = trs_soft_mbox_send;
        g_soft_mbox_ops[inst.devid].mbox_send_rpc_call_msg = trs_soft_mbox_send;
        trs_register_soft_mbox_send_ops(inst.devid, &g_soft_mbox_ops[inst.devid]);
    }
    trs_info("Register soft mbox success. (devid=%u)\n", inst.devid);
    return 0;
}
DECLAER_FEATURE_AUTO_INIT_DEV(trs_soft_mbox_init, FEATURE_LOADER_STAGE_2);

void trs_soft_mbox_uninit(u32 ts_inst_id)
{
    struct trs_id_inst inst;
    trs_ts_inst_to_id_inst(ts_inst_id, &inst);
    if (inst.tsid == 0) {
        trs_unregister_soft_mbox_send_ops(inst.devid);
    }
}
DECLAER_FEATURE_AUTO_UNINIT_DEV(trs_soft_mbox_uninit, FEATURE_LOADER_STAGE_2);
