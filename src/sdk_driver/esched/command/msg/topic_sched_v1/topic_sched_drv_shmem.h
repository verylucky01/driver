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
    u16 usr_pid_flag : 1; /* If flag is 1, use the pid in user_data. */
    /********12 bytes**********/

    u32 pid;
    /********16 bytes**********/

    /* user_data format:
      "user msg" + 2Byte devid(if devid_flag == 1) + 2Byte tid(if tid_flag == 1) + 4Byte pid(if usr_pid_flag == 1) */
    u32 user_data[TOPIC_SCHED_USER_DATA_LEN];
    /********56 bytes**********/

    u32 subtopic_id : 12;
    u32 topic_id : 6;
    u32 gid : 6;
    /* user_data_len: chip define 8 bits, not care value just transparently transmitted from sqe to mb,
       40 byte only need 6 bits, so we use 2 bits indicate that the user_data has devid and tid */
    u8 user_data_len : 6;
    u8 devid_flag : 1;
    u8 tid_flag : 1;
    /********60 bytes**********/

    u16 hac_sn : 5;
    u16 rsv1 : 11;
    u16 tq_id; /* report to host cpu in msg que mode, ai cpu not use */
    /********64 bytes**********/
};

struct topic_sched_sqe {
    u16 type : 6; /* must set 1, topic sched sqe */
    u16 res0 : 2;
    u16 ie : 2;
    u16 pre_p : 2;
    u16 post_p : 2;
    u16 wr_cqe : 1;
    u16 rd_cond : 1;
    u16 blk_dim;
    /********4 bytes**********/

    u16 rt_streamid;
    u16 task_id;
    /********8 bytes**********/

    u16 block_id;
    u16 kernel_type : 7;
    u16 batch_mode : 1;
    u16 topic_type : 4;
    u16 qos : 3;
    u16 usr_pid_flag : 1; /* If flag is 1, use the pid in user_data. */
    /********12 bytes**********/

    u16 sqe_index;
    u8  kernel_credit;
    u8  res1;
    /********16 bytes**********/

    /* user_data format:
      "user msg" + 2Byte devid(if devid_flag == 1) + 2Byte tid(if tid_flag == 1) + 4Byte pid(if usr_pid_flag == 1) */
    u32 user_data[TOPIC_SCHED_USER_DATA_LEN];
    /********56 bytes**********/

    u32 subtopic_id : 12;
    u32 topic_id : 6;
    u32 gid : 6;
    /* user_data_len: chip define 8 bits, not care value just transparently transmitted from sqe to mb,
       40 byte only need 6 bits, so we use 2 bits indicate that the user_data has devid and tid */
    u8 user_data_len : 6;
    u8 devid_flag : 1;
    u8 tid_flag : 1;
    /********60 bytes**********/

    u32 pid;
    /********64 bytes**********/
};
#endif

