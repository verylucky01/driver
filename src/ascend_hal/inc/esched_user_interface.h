/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef ESCHED_USER_INTERFACE_H
#define ESCHED_USER_INTERFACE_H

#if defined(__arm__) || defined(__aarch64__)
#else
#include <time.h>
#endif

#include "ascend_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRV_EVENT_REPLY_BUFFER_RET(ptr)         (*((int *)(ptr)))
#define DRV_EVENT_REPLY_BUFFER_DATA_PTR(ptr)    (((char *)(ptr)) + sizeof(int))

struct drv_event_proc_rsp {
    void *rsp_data_buf;
    int rsp_data_buf_len;
    int real_rsp_data_len;
    bool need_rsp;
};

struct drv_event_proc {
    drvError_t (*proc_func)(unsigned int devId, const void *msg, int msg_len, struct drv_event_proc_rsp *rsp);
    unsigned proc_size;
    const char *proc_name;
};

void drv_registert_event_proc(DRV_SUBEVENT_ID id, struct drv_event_proc *event_proc);

#ifdef __cplusplus
}
#endif

#define USEC_PER_SEC 1000000
#define NSEC_PER_USEC 1000
#ifdef USER_EVENT_SCHED_UT
#define SCHED_GET_CUR_SYSTEM_COUNTER(cnt) (cnt = 1000000)
#define SCHED_GET_SYSTEM_FREQ(cnt) (cnt = 1000000)
#else
#define SCHED_GET_CUR_SYSTEM_COUNTER(cnt) asm volatile("mrs %0, CNTVCT_EL0" : "=r"(cnt) :)
#define SCHED_GET_SYSTEM_FREQ(cnt) asm volatile("mrs %0, CNTFRQ_EL0" : "=r"(cnt) :)
#endif
struct event_res {
    unsigned int gid;
    unsigned int tid;
    unsigned int event_id;
    unsigned int subevent_id : 12; /* same as struct event_sync_msg */
};

enum sync_event_type {
    NORMAL_EVENT,
    QUEUE_EVENT,
    F2NF_EVENT,
    E2NE_EVENT,
    COMM_EVENT_TYPE_MAX,
    QUEUE_ACPU_EVENT = COMM_EVENT_TYPE_MAX,// only device support
    DRV_INNER_EVENT,
    EVENT_TYPE_BUTT
};

typedef struct {
    unsigned int event_id;
    unsigned int subevent_id;
    unsigned int msg_len;
    char msg[EVENT_MAX_MSG_LEN];
} esched_event_info;

#pragma pack(4)
typedef struct {
    unsigned int msg_len;
    char *msg;
} esched_event_buffer;
#pragma pack()

/* inline */
/* get cpu tick */
static inline unsigned long long esched_get_cur_cpu_tick(void)
{
#if defined(__arm__) || defined(__aarch64__) || defined(USER_EVENT_SCHED_UT)
    unsigned long long cnt = 0;
    SCHED_GET_CUR_SYSTEM_COUNTER(cnt);
    return cnt;
#else
    struct timespec timestamp;
    (void)clock_gettime(CLOCK_MONOTONIC, &timestamp);
    return (unsigned long long)((timestamp.tv_sec * USEC_PER_SEC) + (timestamp.tv_nsec / NSEC_PER_USEC));
#endif
}

static inline unsigned long long esched_get_cur_cpu_timestamp(void)
{
    return esched_get_cur_cpu_tick();
}

static inline unsigned long long esched_get_sys_freq(void)
{
#if defined(__arm__) || defined(__aarch64__) || defined(EVENT_SCHED_UT)
    unsigned long long cnt = 0;
    SCHED_GET_SYSTEM_FREQ(cnt);
    return cnt;
#else
    return USEC_PER_SEC; /* sched_get_cur_cpu_tick return microsecond */
#endif
}

static inline unsigned long long tickToMicrosecond(unsigned long long tick)
{
    unsigned long long freq = esched_get_sys_freq();
    // tickFreq is record by second, to microsecond need multiply 1000000
    if ((freq / USEC_PER_SEC) != 0) {
        return tick / (freq / USEC_PER_SEC);
    }

    return 0;
}
drvError_t halEschedQueryInfoEx(unsigned int devId, unsigned int dstDevId, ESCHED_QUERY_TYPE type,
    struct esched_input_info *inPut, struct esched_output_info *outPut);
drvError_t esched_wait_event_ex(uint32_t dev_id, uint32_t grp_id,
    uint32_t thread_id, int32_t timeout, struct event_info *event);
drvError_t esched_device_open(uint32_t devid, halDevOpenIn *in, halDevOpenOut *out);
drvError_t esched_device_close(uint32_t devid, halDevCloseIn *in);
drvError_t esched_device_close_user(uint32_t devid, halDevCloseIn *in);
drvError_t halEschedSubmitEventEx(uint32_t devId, uint32_t dstDevId, struct event_summary *event);
int esched_alloc_event_res(uint32_t dev_id, int32_t event_type, struct event_res *e_res);
void esched_free_event_res(uint32_t dev_id, int32_t event_type, const struct event_res *e_res);
drvError_t esched_query_grp_type(uint32_t dev_id, uint32_t grp_id, GROUP_TYPE *type);
void esched_fill_sync_msg(struct event_summary *event, const struct event_res *res);
void esched_fill_sync_msg_for_peer_que(uint32_t dev_id, uint32_t phy_dev_id, struct event_summary *event, const struct event_res *res);
drvError_t esched_register_finish_func_ex(unsigned int grp_id, unsigned int event_id,
    void (*finish_func)(unsigned int dev_id, unsigned int grp_id, esched_event_info event_info));
drvError_t esched_create_extend_grp(uint32_t dev_id, unsigned int grp_id, struct esched_grp_para *grp_para);
unsigned int esched_get_cpu_mode(uint32_t devid);
#endif
