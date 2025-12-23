/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef DM_LOOP_H
#define DM_LOOP_H

#include "dm_msg_intf.h"

#define RETRY_TIMEOUT_MS 100

#define SELFMSG_HEAD_SIZE (sizeof(SELF_MSG_ST) - DM_MSG_DATA_MAX)

typedef struct dm_loop_addr {
    int addr_type;
    short channel;
    int module_id;
} DM_LOOP_ADDR_S;

#pragma pack(1)
typedef struct SELF_MSG_S {
    DM_ADDR_ST addr;
    int addr_len;
    signed long msgid;
    DM_MSG_TYPE msg_type;
    unsigned char netfn;
    unsigned char cmd;
    unsigned short data_len;
    unsigned char data[DM_MSG_DATA_MAX];
} SELF_MSG_ST;
#pragma pack()

int selfloop_init(DM_INTF_S **my_intf, DM_CB_S *cb, const DM_ADDR_ST *my_addr, const char *my_name, int name_len);

#ifdef STATIC_SKIP
int __selfloop_open(DM_INTF_S *intf);
#endif

#ifdef STATIC_SKIP
void __selfloop_close(DM_INTF_S *intf);
#endif

#ifdef STATIC_SKIP
int __selfloop_send(DM_INTF_S *intf, DM_MSG_TYPE msg_type, DM_ADDR_ST *addr, unsigned int addr_len,
                    const DM_MSG_ST *msg, signed long msgid);
#endif

#ifdef STATIC_SKIP
int __selfloop_settime_send(DM_INTF_S *intf, DM_MSG_TYPE msg_type, DM_ADDR_ST *addr, unsigned int addr_len,
                            const DM_MSG_ST *msg, int retries, unsigned int retry_time_ms, signed long msgid);
#endif

#ifdef STATIC_SKIP
int __selfloop_recv(DM_INTF_S *intf, int fd, short revents, DM_RECV_ST *irecv);
#endif

#ifdef STATIC_SKIP
int __selfloop_set_retries(DM_INTF_S *intf, int retries, unsigned int retry_time_ms);
#endif

#ifdef STATIC_SKIP
int __selfloop_get_retries(DM_INTF_S *intf, int *retries, unsigned int *retry_time_ms);
#endif

#endif
