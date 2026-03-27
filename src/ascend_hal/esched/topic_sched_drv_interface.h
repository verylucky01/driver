/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef TOPIC_SCHED_DRV_INTERFACE_H
#define TOPIC_SCHED_DRV_INTERFACE_H

#define TOPIC_SCHED_SQE_TYPE_HOST 4
#define TOPIC_SCHED_SQE_TYPE_DEVICE 5

#define TOPIC_SCHED_USER_DATA_LEN 10

#define TOPIC_SCHED_TASK_SQE_SIZE (64)
#define TOPIC_SCHED_TASK_CQE_SIZE (32)

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
