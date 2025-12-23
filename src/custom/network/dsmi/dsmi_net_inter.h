/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef __DSMI_NET_INTER_H__
#define __DSMI_NET_INTER_H__

#include <stdbool.h>
#include "dsmi_network_interface.h"

/* the num of ds_port_stat_info pkt stat items */
#define ETH_PKT_STAT_NUM    28
#define NIC_PKT_STAT_NUM    4
#define ROCE_PKT_STAT_NUM   14

// supernodes ping reply
#define PING_PACKET_NUM_MAX 1000
#define TASK_WAIT_MAX_TIME 60
#define ARGC_NUM_10 10
#define STR_TO_INT_TEN 10

struct prbs_adapt_mode_info {
    unsigned int mode;
    unsigned int master_flag; /* 主从DIE标记 */
};

typedef struct {
    hccs_ping_reply_info info;
    bool completed;
} hccs_ping_reply_info_ext;

enum ds_net_opcode_type_ext {
    DS_HCCS_PING = 0x00A0,
    DS_SET_CDR_MODE_CMD = 0x010B,
    DS_GET_TRACEROUTE_INFO = 0x010D,
    DS_START_TRACEROUTE = 0x010E,
    DS_GET_TRACEROUTE_STATUS = 0x010F,
    DS_RESET_TRACEROUTE = 0x0110,
    DS_GET_EXTRA_STAT_INFO = 0x0117,
    DS_SET_NPU_PRBS_FLAG = 0x0119,
    DS_GET_PFC_D_INFO = 0x011F,
    DS_CLEAR_PFC_DURATION = 0x0120,
    DS_GET_TC_STAT = 0x0121,
    DS_CLEAR_TC_PACKET_STATISTICS = 0x0122,
    DS_GET_QPN_LIST = 0x0125,
    DS_GET_QP_CONTEXT = 0x0126,
    DS_GET_PING_MESH_INFO = 0x012E,
    DS_GET_PING_MESH_STATE = 0x012F,
    DS_STOP_HCCS_PING_MESH = 0x0130,
    DS_START_PING_MESH_TASK = 0x0131,
    DS_SET_PRBS_ADAPT_IN_ORDER = 0x0136,
};

int dsmi_del_default_gateway_address(int logic_id, int port, unsigned int gateway, bool is_gateway_on_eth);
int dsmi_get_netdev_link(int logic_id, int port, int *link);
int dsmi_clear_netdev_stat(int logic_id, int port);

#endif
