/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef __DCMI_NETWORK_INTF_H__
#define __DCMI_NETWORK_INTF_H__

#include "dcmi_interface_api.h"
 
#define MAX_PAYLOAD_TYPE 3
#define NETWORK_RDMA_BYTE_TO_MBYTE 100
#define NETWORK_PORT_COUNT_DEFAULT 1
#define Atlas_9000_A3_SuperPoD_MAIN_BOARD_ID1     0x1C
#define Atlas_9000_A3_SuperPoD_MAIN_BOARD_ID2     0x1D
#define MAX_PKT_SIZE    3000
#define MIN_PKT_SIZE    1792
#define MAX_SEND_NUM    1000
#define MAX_PKT_INTERVAL 1000
#define MAX_TASK_INTERVAL 60
#define MAX_PAYLOAD_LEN   3
#define MAX_TASK_ID       1
#define STR_NUM_BASE    10
#define NS_CONVERT_TO_US  1000
#define NS_CONVERT_TO_S   1000000000
#define SDID_LEN_MAX    10
 
// ip地址转换为sdid
// ip地址格式：[192].[server_id].[2+die_id].[199-device_id]
// sdid格式：[10位server_id][4位chip_id][2位die_id][16位device_id]
#define A3_SUPERPOD_MAX_NUMS 48
#define CHIP_DIE_CNT 2
#define VNIC_IP_HEADER 192
 
#define IP_GET_HEADER(A) ((A) & 0xFF)
#define IP_GET_SERVERID(A) (((A) >> 8) & 0xFF)
#define IP_GET_DIEID(A)    ((((A) >> 16) & 0xFF) - 2)
#define IP_GET_DEVICEID(A) (199 - (((A) >> 24) & 0xFF))
#define IP_CONVERT_SDID(A) ((IP_GET_SERVERID(A) << 22) | ((IP_GET_DEVICEID(A) / 2) << 18) | \
        (IP_GET_DIEID(A) << 16) | (IP_GET_DEVICEID(A)))
#define SDID_GET_SERVERID(A) (((A) >> 22) & 0xFF)
#define SDID_GET_CHIPID(A) (((A) >> 18) & 0xF)
#define SDID_GET_DIEID(A) (((A) >> 16) & 0x3)
#define SDID_GET_DEVICEID(A) ((A) & 0xFFFF)
 
#endif /* __DCMI_NETWORK_INTF_H__ */