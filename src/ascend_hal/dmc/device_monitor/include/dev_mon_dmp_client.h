/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef DEV_MON_DMP_CLIENT_H
#define DEV_MON_DMP_CLIENT_H

#include "device_monitor_type.h"
#include "dm_common.h"
#include "dev_mon_protocol_macro.h"

#define DATA_LEN_OFFSET 8

int dev_mon_send_request(DM_INTF_S *intf, DM_ADDR_ST *addr, unsigned int addr_len, const DM_MSG_ST *msg,
                         DM_MSG_CMD_HNDL_T rsp_hndl, const void *user_data, int data_len);

int dmp_msg_recv_resp(DM_INTF_S *intf, DM_RECV_ST *recv, SEND_CTL_CB *ctl);

int slice_msg_list_init(void);
void slice_msg_list_uninit(void);

int client_rsp_hashtable_init(void);
void client_rsp_hashtable_uninit(void);

#endif
