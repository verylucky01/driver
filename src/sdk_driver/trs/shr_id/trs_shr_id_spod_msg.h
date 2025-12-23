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
#ifndef TRS_SHR_ID_SPOD_MSG_H
#define TRS_SHR_ID_SPOD_MSG_H

#include "comm_kernel_interface.h"
#include "trs_shr_id_node.h"

struct trs_pod_msg_head {
    u32 devid;
    u32 cmdtype;
    u16 valid;  /* validity judgement, 0x5A5A is valide */
    s16 result; /* process result from rp, zero for succ, non zero for fail */
    u32 rsv;
};

#define TRS_POD_MSG_TOTAL_LEN       192
#define TRS_POD_MSG_DATA_LEN        (TRS_POD_MSG_TOTAL_LEN - sizeof(struct trs_pod_msg_head))

#define TRS_POD_MSG_SEND_MAGIC      0x5A5A
#define TRS_POD_MSG_RCV_MAGIC       0xA5A5
#define TRS_POD_MSG_INVALID_RESULT  0x1A

struct trs_pod_msg_data {
    struct trs_pod_msg_head header;
    char payload[TRS_POD_MSG_DATA_LEN];
};

enum shr_id_pod_msg_cmd {
    TRS_POD_MSG_CREATE_SHADOW,
    TRS_POD_MSG_SET_PID,
    TRS_POD_MSG_DESTORY_SHADOW,
    TRS_POD_MSG_QUERY_SHADOW,
    TRS_POD_MSG_MAX
};

struct shr_id_pod_create_msg {
    struct shr_id_node_op_attr attr;
    pid_t pid;
};

struct shr_id_pod_set_pid_msg {
    char name[SHR_ID_NSM_NAME_SIZE];
    pid_t pid;
};

struct shr_id_pod_destroy_msg {
    char name[SHR_ID_NSM_NAME_SIZE];
    pid_t pid;
};

struct shr_id_pod_query_msg {
    char name[SHR_ID_NSM_NAME_SIZE];
};

int trs_s2s_msg_recv(u32 devid, u32 sdid, struct data_input_info *data);

#endif
