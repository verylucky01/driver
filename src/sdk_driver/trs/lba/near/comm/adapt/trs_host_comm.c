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
#include "ka_system_pub.h"
#include "ascend_hal_define.h"
#include "pbl/pbl_soc_res.h"
#ifdef CFG_FEATURE_VM_ADAPT
#include "trs_sec_eh_auto_init.h"
#else
#include "trs_sia_adapt_auto_init.h"
#endif
#include "dpa/dpa_rmo_kernel.h"
#include "trs_pub_def.h"
#include "trs_msg.h"
#include "trs_host_msg.h"
#include "trs_chan.h"
#include "trs_core.h"
#include "trs_abnormal_info.h"
#include "trs_ts_status.h"
#include "trs_id.h"
#include "comm_kernel_interface.h"
#include "trs_ioctl.h"
#include "trs_mailbox_def.h"
#include "trs_host_ts_cq.h"
#include "trs_host_comm.h"

#ifdef CFG_FEATURE_SUPPORT_RMO
static int trs_mem_dispatch_to_ts(u32 devid, u64 addr, u64 len)
{
    struct trs_mem_dispatch_msg mbox_data = {0};
    struct trs_id_inst inst;
    int ret;

    trs_mbox_init_header(&mbox_data.header, TRS_MBOX_MEM_DISPATCH);
    mbox_data.paddr = addr;
    mbox_data.size = len;
    mbox_data.hostpid = ka_task_get_current_tgid();

    trs_id_inst_pack(&inst, devid, 0);
    ret = trs_mbox_send(&inst, 0, &mbox_data, sizeof(struct trs_mem_dispatch_msg), 3000); // timeout: 3000 ms
    if ((ret != 0) || (mbox_data.header.result) != 0) {
        trs_err("Send mbox fail. (devid=%u; result=%u; ret=%d)\n",
            devid, mbox_data.header.result, ret);
        return -EFAULT;
    }

    trs_debug("Dispatch success. (devid=%u; hostpid=%d; len=%llu)\n", devid, mbox_data.hostpid, len);
    return 0;
}
#endif

int trs_host_mem_sharing_register(void)
{
#ifdef CFG_FEATURE_SUPPORT_RMO
    rmo_mem_sharing_register(trs_mem_dispatch_to_ts, TS_ACCESSOR);
    trs_info("Register success.\n");
#endif
    return 0;
}
DECLAER_FEATURE_AUTO_INIT(trs_host_mem_sharing_register, FEATURE_LOADER_STAGE_6);

void trs_host_mem_sharing_unregister(void)
{
#ifdef CFG_FEATURE_SUPPORT_RMO
    rmo_mem_sharing_unregister(TS_ACCESSOR);
#endif
}
DECLAER_FEATURE_AUTO_UNINIT(trs_host_mem_sharing_unregister, FEATURE_LOADER_STAGE_6);

int trs_host_get_ssid(struct trs_id_inst *inst, int *user_visible_flag, int *ssid)
{
    struct trs_msg_sync_ssid *ssid_msg = NULL;
    struct trs_msg_data msg;
    int ret;

    msg.header.tsid = inst->tsid;
    msg.header.valid = TRS_MSG_SEND_MAGIC;
    msg.header.cmdtype = TRS_MSG_GET_SSID;
    msg.header.result = 0;

    ssid_msg = (struct trs_msg_sync_ssid *)msg.payload;
    ssid_msg->vfid = 0;
    ssid_msg->hpid = ka_task_get_current_tgid();

    ret = trs_host_msg_send(inst->devid, &msg, sizeof(struct trs_msg_data));
    if ((ret != 0) || (msg.header.result != 0)) {
        trs_err("Msg send fail. (devid=%u; tsid=%u; ret=%d; result=%d)\n",
            inst->devid, inst->tsid, ret, msg.header.result);
        return -ENODEV;
    }
    *user_visible_flag = 0;
    *ssid = ssid_msg->ssid;
    return 0;
}

int trs_host_ts_adapt_abnormal_proc(u32 devid, struct trs_msg_data *msg)
{
    struct stars_abnormal_info *abnormal_info = (struct stars_abnormal_info *)msg->payload;
    struct trs_id_inst inst;

    if (abnormal_info == NULL) {
        trs_err("Data is NULL. (devid=%u; tsid=%u)\n", devid, msg->header.tsid);
        return -EFAULT;
    }

    trs_id_inst_pack(&inst, devid, msg->header.tsid);
    return trs_chan_abnormal_proc(&inst, abnormal_info);
}

int trs_host_set_ts_status(u32 devid, struct trs_msg_data *data)
{
    u32 status = data->payload[0];
    struct trs_id_inst inst;

    if ((status != TRS_INST_STATUS_NORMAL) && (status != TRS_INST_STATUS_ABNORMAL)) {
        trs_err("Invalid para. (devid=%u; tsid=%u; status=%u)\n", devid, data->header.tsid, status);
        return -EINVAL;
    }

    trs_id_inst_pack(&inst, devid, data->header.tsid);
    return trs_set_ts_status(&inst, status);
}

int trs_host_flush_id(u32 devid, struct trs_msg_data *data)
{
    struct trs_msg_id_sync_head *msg_info = (struct trs_msg_id_sync_head *)data->payload;
    struct trs_id_inst inst;

    trs_id_inst_pack(&inst, devid, data->header.tsid);
    return trs_id_free_batch_by_type(&inst, msg_info->type);
}

int trs_host_res_is_check_msg_proc(u32 devid, struct trs_msg_data *data)
{
    struct trs_msg_res_id_check *id_msg = (struct trs_msg_res_id_check *)data->payload;
    struct trs_id_inst inst;

    if ((id_msg->id_type < 0) || (id_msg->id_type >= TRS_MAX_ID_TYPE)) {
#ifndef EMU_ST
        trs_err("Invalid para. (devid=%u; res_type=%d)\n", devid, id_msg->id_type);
        return -EINVAL;
#endif
    }

    trs_id_inst_pack(&inst, devid, data->header.tsid);
    if (trs_res_is_belong_to_proc(&inst, id_msg->hpid, id_msg->id_type, id_msg->res_id)) {
        trs_debug("Res id belong to master process. (master_tgid=%u; type=%s; id=%d)\n",
            id_msg->hpid, trs_id_type_to_name(id_msg->id_type), id_msg->res_id);
        return 0;
    }
#ifndef ENU_ST
    trs_err("Res id not belong to master process. (master_tgid=%u; type=%s; id=%d)\n",
        id_msg->hpid, trs_id_type_to_name(id_msg->id_type), id_msg->res_id);
    return -1;
#endif
}

int trs_host_res_id_check(struct trs_id_inst *inst, int id_type, u32 res_id)
{
#ifndef EMU_ST
    struct trs_msg_res_id_check *id_msg = NULL;
    struct trs_msg_data msg;
    int ret;

    msg.header.tsid = inst->tsid;
    msg.header.valid = TRS_MSG_SEND_MAGIC;
    msg.header.cmdtype = TRS_MSG_RES_ID_CHECK;
    msg.header.result = 0;

    id_msg = (struct trs_msg_res_id_check *)msg.payload;
    id_msg->hpid = (u32)ka_task_get_current_tgid();
    id_msg->id_type = id_type;
    id_msg->res_id = res_id;

    ret = trs_host_msg_send(inst->devid, &msg, sizeof(struct trs_msg_data));
    if ((ret != 0) || (msg.header.result != 0)) {
        trs_err("Msg send fail. (devid=%u; tsid=%u; ret=%d; result=%d)\n",
            inst->devid, inst->tsid, ret, msg.header.result);
        return -ENODEV;
    }
#endif
    return 0;
}

int trs_host_ras_report(struct trs_id_inst *inst)
{
    struct trs_msg_data msg;
    int ret;
 
    msg.header.tsid = inst->tsid;
    msg.header.valid = TRS_MSG_SEND_MAGIC;
    msg.header.cmdtype = TRS_MSG_RAS_REPORT;
    msg.header.result = 0;

    ret = trs_host_msg_send(inst->devid, &msg, sizeof(struct trs_msg_data));
    if ((ret != 0) || (msg.header.result != 0)) {
        trs_err("Msg send fail. (devid=%u; tsid=%u; ret=%d; result=%d)\n",
            inst->devid, inst->tsid, ret, msg.header.result);
        return ret;
    }
    return 0;
}

int trs_host_ts_cq_process(u32 devid, struct trs_msg_data *data)
{
    struct trs_msg_ts_cq_process *msg = (struct trs_msg_ts_cq_process *)data->payload;
    struct trs_ts_cq_info *cq_info = NULL;
    struct trs_id_inst inst;
    int ret;

    trs_id_inst_pack(&inst, devid, data->header.tsid);

    cq_info = trs_get_ts_cq_info(devid);
    if ((cq_info == NULL) || (cq_info->host_ts_cq_process_func == NULL) || (cq_info->para == NULL)) {
        trs_err("Invalid ts cq info. (devid=%u)\n", devid);
        return -EINVAL;
    }

    ret = trs_ts_cq_cpy(&inst, msg->cqid, msg->cq_type, msg->cqe);
    if (ret != 0) {
        trs_err("Failed to copy ts cq. (ret=%d; devid=%u; cq_type=%u; cqid=%u)\n",
            ret, devid, msg->cq_type, msg->cqid);
        return ret;
    }
    ret = cq_info->host_ts_cq_process_func(0, 0, cq_info->para, &msg->cqid, 1);
    return (ret == -EBUSY) ? 0 : ret; /* -EBUSY means just schedule in work */
}

int trs_host_get_connect_protocol(struct trs_id_inst *inst)
{
    int type = devdrv_get_connect_protocol(inst->devid);

    switch (type) {
        case CONNECT_PROTOCOL_PCIE:
            return TRS_CONNECT_PROTOCOL_PCIE;
        case CONNECT_PROTOCOL_HCCS:
            return TRS_CONNECT_PROTOCOL_HCCS;
#ifndef EMU_ST
        case CONNECT_PROTOCOL_UB:
            return TRS_CONNECT_PROTOCOL_UB;
        default:
            return TRS_CONNECT_PROTOCOL_UNKNOWN;
#endif
    }
}

int trs_host_request_irq(struct trs_id_inst *inst, struct trs_adapt_irq_attr *attr,
    void *para, const char *name, ka_irqreturn_t (*handler)(int irq, void *para))
{
    int ret;
    struct res_inst_info res_inst;
    u32 hwirq;
    soc_resmng_inst_pack(&res_inst, inst->devid, TS_SUBSYS, inst->tsid);
    ret = soc_resmng_get_hwirq(&res_inst, attr->irq_type, attr->irq, &hwirq);
    if (ret != 0) {
        trs_err("Get hw irq failed. (devid=%u; irqtype=%u; irq=%u; ret=%d)\n",
            inst->devid, attr->irq_type, attr->irq, ret);
        return ret;
    }

#ifndef EMU_ST
    ret = devdrv_register_irq_by_vector_index(inst->devid, hwirq, handler, para, name);
#else
    ret = ka_system_request_irq(attr->irq, handler, 0, name, para);
#endif
    if (ret != 0) {
        trs_err("Request irq failed. (devid=%u; irq_type=%u; irq=%u; hwirq=%u; name=%s; ret=%d)\n",
            inst->devid, attr->irq_type, attr->irq, hwirq, name, ret);
    }
    return ret;
}

void trs_host_free_irq(struct trs_id_inst *inst,struct trs_adapt_irq_attr *attr, void *para)
{
    struct res_inst_info res_inst;
    u32 hwirq;
    int ret;
    soc_resmng_inst_pack(&res_inst, inst->devid, TS_SUBSYS, inst->tsid);
    ret = soc_resmng_get_hwirq(&res_inst, attr->irq_type, attr->irq, &hwirq);
    if (ret != 0) {
        trs_warn("Get hw irq warn. (devid=%u; irqtype=%u; irq=%u; ret=%d)\n",
            inst->devid, attr->irq_type, attr->irq, ret);
        return;
    }
#ifndef EMU_ST
    ret = devdrv_unregister_irq_by_vector_index(inst->devid, hwirq, para);
#else
    ret = ka_system_free_irq(attr->irq, para);
#endif
    if (ret != 0) {
        trs_warn("Free irq warn. (devid=%u; irqtype=%u; irq=%u; hwirq=%u; ret=%d)\n",
            inst->devid, attr->irq_type, attr->irq, hwirq, ret);
        return;
    }
}

int trs_adapt_ops_request_irq(struct trs_id_inst *inst, u32 irq_type,  u32 irq,
                                void *para, ka_irqreturn_t (*handler)(int irq, void *para))
{
    struct trs_adapt_irq_attr attr;
    const char *name = NULL;

    attr.irq_type = irq_type;
    attr.irq = irq;
    name = soc_resmng_get_ts_irq_name_by_type(irq_type);
    if (name == NULL) {
        trs_err("Get irq name failed.(devid=%u; irq_type=%d)\n", inst->devid, irq_type);
        return -EINVAL;
    }

    return trs_host_request_irq(inst, &attr, para, name, handler);
}

void trs_adapt_ops_free_irq(struct trs_id_inst *inst, u32 irq_type, u32 irq, void *para)
{
    struct trs_adapt_irq_attr attr;
    attr.irq_type = irq_type;
    attr.irq = irq;
    (void)trs_host_free_irq(inst, &attr, para);
}