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

#ifndef _VNIC_CMD_MSG_H_
#define _VNIC_CMD_MSG_H_

/* cq/sq must aligned with 128, not within same cacheline */
struct pcivnic_sq_desc {
    u64 data_buf_addr;
    u32 data_len;
    u32 valid;
}__attribute__((aligned(128)));

struct pcivnic_cq_desc {
    u32 sq_head;
    u32 status;
    u32 cq_id;
    u32 valid;
}__attribute__((aligned(128)));

struct pcivnic_s2s_data_info {
    u32 reserve[62]; /* reserve 248 Byte */
    u32 msg_len;     /* must be here, otherwise, there will be version compatibility issues */
    unsigned char data[];
};

#endif
