/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef QUE_COMM_EVENT_H
#define QUE_COMM_EVENT_H
 
#include <sys/types.h>
#include "ascend_hal.h"
#include "ascend_hal_define.h"
#include "esched_user_interface.h"
#include "queue_interface.h"

#if (defined CFG_PLATFORM_FPGA)
#define QUE_EVENT_TIMEOUT_MS 5000000  /* always wait */
#define QUE_EVENT_MAX_WAIT_10S 10000000  /* always wait */
#else
#define QUE_EVENT_TIMEOUT_MS   5000 /* Defualt event timeout */
#define QUE_EVENT_MAX_WAIT_10S 10000   /* 10s */
#endif
#define QUE_SEND_NORMAL  0
#define QUE_SEND_WITH_RETRY 1

#define SCHED_INVALID_TID 0xFFFFFFFFU

struct que_event_msg {
    char *in;
    size_t in_len;
    char *out;
    size_t out_len;
};
 
struct que_event_attr {
    unsigned int devid;
    unsigned int sub_event;
    pid_t hostpid;
    pid_t devpid;
    unsigned int retry_flg;
};

struct que_comm_event_attr {
    unsigned int devid;
    unsigned int sub_event;
    pid_t local_pid;
    pid_t remote_pid;
    unsigned int remote_devid;
    unsigned int remote_grpid;
};

struct que_f2nf_res {
    int res_alloc_flag;
    int pid;
    unsigned int dst_engine;
    struct event_res f2nf_event_res;
};

unsigned int que_get_sched_mode(uint32_t devid);
unsigned int que_get_sched_engine_type(uint32_t devid);
void que_comm_event_sum_uninit(struct event_summary *event);
int que_event_reply_init(struct que_event_msg *msg, struct event_reply *reply);
void que_event_reply_uninit(struct event_reply *reply);
int que_event_get_result(struct que_event_msg *msg, struct event_reply *reply);
drvError_t check_event_and_reply(struct event_summary *event, struct event_reply *reply);
drvError_t wait_event_and_check(uint32_t devid, struct event_res *res, struct event_info *back_event_info,
    int32_t timeout, unsigned long timestamp);
int que_inter_dev_get_remote_pid(unsigned int remote_devid, pid_t *remote_pid);
int que_inter_dev_get_remote_grpid(unsigned int devid, unsigned int remote_devid, pid_t remote_pid, unsigned int *grpid);
int que_get_local_devid_grpid(unsigned int devid, pid_t *devpid, unsigned int *grpid);
int que_comm_event_send(struct que_comm_event_attr *attr, struct que_event_msg *msg, int timeout_ms);
int que_comm_event_send_ex(unsigned int devid, unsigned int qid, unsigned int sub_event, struct que_event_msg *msg,
    int timeout_ms);
int que_inter_dev_get_info(unsigned int devid, unsigned int remote_devid, pid_t *remote_devpid, unsigned int *remote_grpid);
int que_comm_event_send_d2d(unsigned int devid, unsigned int remote_devid, unsigned int sub_event, struct que_event_msg *msg,
    int timeout_ms);
void que_get_sched_event_type(uint32_t devid, int32_t *event_type);
#endif
