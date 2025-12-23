/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef BBOX_REMOTE_MSG_H
#define BBOX_REMOTE_MSG_H

#include "bbox_int.h"

typedef struct bbox_msg_header {
    u32 type;   // header type
    u32 len;    // whole message length
} bbox_msg_header_t;

typedef struct bbox_hello_msg {
    bbox_msg_header_t header;
    u32 capacity;
    u32 stream_tag; // mark up-stream or down-stream
    u32 sequence; // global exception sequence
    u32 reservered;
} bbox_hello_msg_t;

typedef struct bbox_except_msg {
    bbox_msg_header_t header;
    bbox_time_t tm; // time stamp
    u32 except_id; // exception id
    u32 d_type; // data set type
    u32 d_len; // length of the rest data exclude this header
    u8 core_id; // module id
    u8 e_type; // exception reason
    u16 pad; // reserved padding
    u8 data[]; // data
} bbox_except_msg_t;

typedef struct bbox_reboot_msg {
    bbox_msg_header_t header;
    u32 reboot_type;
    u32 dev_mem_len;
    u64 dev_mem_addr;
} bbox_reboot_msg_t;

typedef struct bbox_reply_msg {
    bbox_msg_header_t header;
    u64 reserved; // reserved
} bbox_reply_msg_t;

typedef struct bbox_ctrl_msg {
    bbox_msg_header_t header;
    u32 type;
    u32 flag;
} bbox_ctrl_msg_t;

// enum type of segment flag
enum bbox_remote_seg_flag {
    BBOX_PKT_SEG_NULL = 0, // no multi-msg received
    BBOX_PKT_SEG_STRT = 1, // the first segment pkt
    BBOX_PKT_SEG_CONT = 2, // middle segment pkts
    BBOX_PKT_SEG_FINL = 3, // the last segment pkt
};

#endif // BBOX_REMOTE_MSG_H
