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
#ifndef TOPIC_SCHED_DRV_SHMEM_H
#define TOPIC_SCHED_DRV_SHMEM_H

#define TOPIC_SCHED_USER_DATA_LEN 10

struct topic_sched_mailbox {
    u8  mailbox_id; /* only used for: host is 1630,
                                      and STARS works with EMS to schedule tasks Host CPU */
    u32 vfid : 6;
    u16 rsp_mode : 1;
    u16 sat_mode : 1;
    u16 blk_dim;
    /********4 bytes**********/

    u16 stream_id;
    u16 task_id;
    /********8 bytes**********/

    u16 blk_id;
    u16 kernel_type : 7;
    u16 batch_mode : 1;
    u32 topic_type : 4;
    u32 qos : 3;
    u16 res0 : 1;
    /********12 bytes**********/

    u32 pid;
    /********16 bytes**********/

    u32 user_data[TOPIC_SCHED_USER_DATA_LEN];
    /********56 bytes**********/

    u32 subtopic_id : 12;
    u32 topic_id : 6;
    u32 gid : 6;
    u8 user_data_len : 8;
    /********60 bytes**********/

    u16 hac_sn : 5;
    u16 res1 : 11;
    u16 tq_id; /* report to host cpu in msg que mode, ai cpu not use */
    /********64 bytes**********/
};

struct topic_sched_sqe {
    u16 type : 6; /* host aicpu : 4, device aicpu : 5 */
    u16 res0 : 2;
    u16 ie : 1;
    u16 pre_p : 1;
    u16 post_p : 1;
    u16 wr_cqe : 1;
    u16 ptr_mode : 1;
    u16 ptt_mode : 1;
    u16 head_update : 1;
    u16 res1 : 1;
    u16 blk_dim;
    /********4 bytes**********/

    u16 rt_streamid;
    u16 task_id;
    /********8 bytes**********/

    u16 block_id; // res2;
    u16 kernel_type : 7;
    u16 batch_mode : 1;
    u16 topic_type : 4;
    u16 qos : 3;
    u16 res2 : 1;
    /********12 bytes**********/

    u16 res3;
    u8  kernel_credit;
    u8  res4 : 5;
    u8  sqe_len : 3; /* 0:64B; 1:128B; 2:192B. */
    /********16 bytes**********/

    u32 user_data[TOPIC_SCHED_USER_DATA_LEN];
    /********56 bytes**********/

    u32 subtopic_id : 12;
    u32 topic_id : 6;
    u32 gid : 6;
    u8  user_data_len;
    /********60 bytes**********/

    u32 pid;
    /********64 bytes**********/
};

#endif
