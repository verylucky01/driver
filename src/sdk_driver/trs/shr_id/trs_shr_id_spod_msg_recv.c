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
#include <linux/types.h>

#include "ascend_hal_define.h"

#include "trs_shr_id.h"
#include "trs_shr_id_spod_msg.h"

static int shr_id_create_shadow_node(u32 devid, struct trs_pod_msg_data *pod_msg)
{
    struct shr_id_pod_create_msg *create_msg = (struct shr_id_pod_create_msg *)pod_msg->payload;

    if (create_msg->attr.type >= SHR_ID_TYPE_MAX) {
        trs_err("Invalid type. (devid=%u; type=%d)\n", devid, create_msg->attr.type);
        return -EINVAL;
    }

    create_msg->attr.name[SHR_ID_NSM_NAME_SIZE - 1] = '\0';
    create_msg->attr.res_get = NULL;
    create_msg->attr.res_put = NULL;
    create_msg->attr.pid = create_msg->pid;
    create_msg->attr.flag |= TSDRV_FLAG_SHR_ID_SHADOW;
    trs_debug("Create info. (name=%s; sdid=0x%x; type=%d; pid=%d; flag=0x%x)\n",
        create_msg->attr.name, create_msg->attr.inst.devid, create_msg->attr.type,
        create_msg->pid, create_msg->attr.flag);

    return shr_id_node_create(&create_msg->attr);
}

static int shr_id_set_pid_shadow_node(u32 devid, struct trs_pod_msg_data *pod_msg)
{
    struct shr_id_pod_set_pid_msg *msg = (struct shr_id_pod_set_pid_msg *)pod_msg->payload;
    struct shr_id_node_op_attr attr;
    int ret;

    ret = shr_id_node_get_attr(msg->name, &attr);
    if (ret != 0) {
        return ret;
    }

    ret = shr_id_node_set_pids(attr.name, attr.type, attr.pid, &msg->pid, 1);
    trs_debug("Set pod pid. (devid=%u; name=%s; type=%d; id=%u; pid=%d; ret=%d)\n",
        attr.inst.devid, attr.name, attr.type, attr.id, msg->pid, ret);

    return ret;
}

static int _shr_id_query_shadow_node(char *name, int type)
{
    void *node = shr_id_node_get(name, type);
    if (node == NULL) {
        trs_debug("Query shadow node. (name=%s; type=%d)\n", name, type);
        return 0;
    }
    shr_id_node_put(node);
    return -EEXIST;
}

static int shr_id_query_shadow_node(u32 devid, struct trs_pod_msg_data *pod_msg)
{
    struct shr_id_pod_query_msg *msg = (struct shr_id_pod_query_msg *)pod_msg->payload;
    int ret, type;

    ret = shr_id_get_type_by_name(msg->name, &type);
    if (ret != 0) {
        return ret;
    }

    return _shr_id_query_shadow_node(msg->name, type);
}

static int shr_id_destory_shadow_node(u32 devid, struct trs_pod_msg_data *pod_msg)
{
    struct shr_id_pod_destroy_msg *msg = (struct shr_id_pod_destroy_msg *)pod_msg->payload;
    int ret, type;

    ret = shr_id_get_type_by_name(msg->name, &type);
    if (ret != 0) {
        return ret;
    }

    (void)shr_id_node_destroy(msg->name, type, msg->pid, DEVDRV_S2S_SYNC_MODE);

    return _shr_id_query_shadow_node(msg->name, type);
}

typedef int (*trs_pod_msg_rcv_func_t)(u32 devid, struct trs_pod_msg_data *msg);
static const trs_pod_msg_rcv_func_t rcv_ops[TRS_POD_MSG_MAX] = {
    [TRS_POD_MSG_CREATE_SHADOW] = shr_id_create_shadow_node,
    [TRS_POD_MSG_SET_PID] = shr_id_set_pid_shadow_node,
    [TRS_POD_MSG_DESTORY_SHADOW] = shr_id_destory_shadow_node,
    [TRS_POD_MSG_QUERY_SHADOW] = shr_id_query_shadow_node,
};

static int trs_s2s_msg_recv_check(u32 devid, struct data_input_info *data)
{
    struct trs_pod_msg_data *msg = NULL;

    if (devid >= TRS_DEV_MAX_NUM) {
        trs_err("Invalid devid. (devid=%u)\n", devid);
        return -EINVAL;
    }

    if ((data == NULL) || (data->data == NULL) || (data->in_len != (u32)sizeof(struct trs_pod_msg_data))) {
        trs_err("Invalid. (devid=%u; para=%d; len=%ld)\n", devid,
            (data == NULL) ? 1 : data->in_len, sizeof(struct trs_pod_msg_data));
        return -EINVAL;
    }

    msg = (struct trs_pod_msg_data *)data->data;
    if (msg->header.valid != TRS_POD_MSG_SEND_MAGIC) {
        trs_err("Invalid magic. (devid=%u; magic=0x%x)\n", devid, msg->header.valid);
        return -EINVAL;
    }

    if (msg->header.cmdtype >= TRS_POD_MSG_MAX) {
        trs_err("Invalid cmd. (devid=%u; cmd=%u)\n", devid, msg->header.cmdtype);
        return -EINVAL;
    }
    return 0;
}

int trs_s2s_msg_recv(u32 devid, u32 sdid, struct data_input_info *data)
{
    struct trs_pod_msg_data *msg = NULL;
    int ret;

    ret = trs_s2s_msg_recv_check(devid, data);
    if (ret != 0) {
        return ret;
    }

    msg = (struct trs_pod_msg_data *)data->data;
    if (rcv_ops[msg->header.cmdtype] != NULL) {
        ret = rcv_ops[msg->header.cmdtype](devid, msg);
        data->out_len = (u32)sizeof(struct trs_pod_msg_data);
        msg->header.valid = TRS_POD_MSG_RCV_MAGIC;
        msg->header.result = (s16)ret;
    }
    return 0;
}
