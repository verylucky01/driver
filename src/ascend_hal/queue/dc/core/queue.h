/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#include "ascend_hal.h"
#include "ascend_inpackage_hal.h"
#include "queue_user_manage.h"

#define QUEUE_MODULE "QUEUE"
#define QUE_INTER_DEV_INVALID_VALUE   (-1)
#define QUEUE_INTER_DEV_STATE_IMPORTED  2

#define CAS(ptr, old, new) __sync_bool_compare_and_swap(ptr, old, new)

#ifdef EMU_ST
#include <sys/syscall.h>
#ifndef THREAD
#define THREAD __thread
#endif
#define GETPID() syscall(__NR_gettid)
#else
#ifndef THREAD
#define THREAD
#endif
#define GETPID() getpid()
#endif

#if (defined DRV_HOST) || (defined QUEUE_UT)
#define SHAREPOOL_MEM_ADD_START  0x0000000000000000
#define SHAREPOOL_MEM_ADD_END    0xFFFFFFFFFFFFFFFF
#else
#define SHAREPOOL_MEM_ADD_START  0x0000E80000000000
#define SHAREPOOL_MEM_ADD_END    0x0000F80000000000
#endif

#ifdef DRV_HOST
/*
 * In 910B sriov/mdev:
 * devid = pf_id * MAX_VF_NUM(16) + (vfid - 1) + 100
 * pf:     devid 0   ~ devid 63
 * vf:     devid 100 ~ devid 1123
 * rsv:    devid 64  ~ devid 99
 */
#define MAX_DEVICE 1124
#else
/*
 * In 910B sriov/mdev:
 * pf:     devid 0   ~ devid 31
 * vf:     devid 32  ~ devid 63
 */
#define MAX_DEVICE 64
#endif

#define QUEUE_UNINITED 0
#define QUEUE_INITED   1

#define QUEUE_CREATED              0x7A1234BB
#define QUEUE_UNCREATED            0
#define QUEUE_DESTROYING           0x7B4321AA
#define QUEUE_IS_DESTROY_MAGIC  0x5A5A5A5A
#define QUEUE_IS_CLEAR_MAGIC    0x6A6A6A6A

#define MS_PER_SECOND  1000U
#define US_PER_MSECOND 1000U
#ifndef NSEC_PER_USEC
#define NSEC_PER_USEC 1000
#endif
#if (defined CFG_PLATFORM_FPGA)
#define QUEUE_SYNC_TIMEOUT (-1)   /* always wait */
#else
#define QUEUE_SYNC_TIMEOUT 5000U   /* 5s */
#endif
#define QUEUE_32BIT_MASK 0xFFFFFFFF

#include "queue_kernel_api.h"

void queue_detach_invalid_queue(void);
drvError_t check_subscribe_para(unsigned int dev_id, unsigned int qid, int type);
drvError_t subscribe_queue(unsigned int dev_id, unsigned int qid, struct sub_info sub_info, int type);
drvError_t sub_f_to_nf_event(unsigned int dev_id, unsigned int qid, struct sub_info sub_event);
drvError_t queue_front(unsigned int dev_id, unsigned int qid, Mbuf **mbuf);
void queue_un_front(Mbuf *mbuf);
void queue_update_time(unsigned int dev_id, unsigned int qid, unsigned int host_time, unsigned int msg_type);
void send_queue_event(unsigned int dev_id, struct queue_manages *manage);
unsigned int queue_get_dst_engine(unsigned int dev_id, unsigned int group_id);
drvError_t queue_init_local(unsigned int dev_id);
drvError_t queue_sub_event_local(struct QueueSubPara *sub_para);
drvError_t queue_unsub_event_local(struct QueueUnsubPara *unsub_para);
drvError_t queue_attach_local(unsigned int dev_id, unsigned int qid, int time_out);
drvError_t queue_en_queue_local(unsigned int dev_id, unsigned int qid, void *mbuf);
void send_f_to_nf_event(unsigned int dev_id, struct queue_manages *que_manage);
drvError_t queue_reset_local(unsigned int dev_id, unsigned int qid);
drvError_t queue_query_info_local(unsigned int dev_id, unsigned int qid, QueueInfo *que_info);
drvError_t queue_get_status_local(unsigned int dev_id, unsigned int qid, QUEUE_QUERY_ITEM query_item,
    unsigned int len, void *data);
drvError_t queue_set_local(unsigned int dev_id, QueueSetCmdType cmd, QueueSetInputPara *input);
#endif
