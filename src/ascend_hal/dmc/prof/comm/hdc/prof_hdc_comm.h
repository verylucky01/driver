/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef PROF_HDC_COMM_H
#define PROF_HDC_COMM_H
#include "ascend_inpackage_hal.h"

#define PROF_HDC_EVENT_NUM_MAX          64U
#define PROF_HDC_RECEIVE_THREAD_ENABLE  1U
#define PROF_HDC_RECEIVE_THREAD_DISABLE 0U
#define PROF_HDC_RECEIVE_THREAD_RUNNING 1U
#define PROF_HDC_RECEIVE_THREAD_STOP    0U

void prof_hdc_register_msg_proc_func(void (*func)(uint32_t dev_id, void *msg, uint32_t len));
drvError_t prof_session_destroy(uint32_t dev_id, uint32_t chan_id);
drvError_t prof_hdc_msg_send(uint32_t dev_id, uint32_t chan_id, unsigned char *buff_base, uint32_t buff_len);

struct prof_hdc_common_info {
    HDC_EPOLL epoll;
    HDC_CLIENT client;
    HDC_SESSION session[DEV_NUM];
    uint32_t session_count;
    pthread_t epoll_thread;
    int prof_channel_num_count[DEV_NUM];
    int prof_epoll_client_recv_flag;
    int recv_thread_run_flag;
    int prof_channel_enable_flag[DEV_NUM][PROF_CHANNEL_NUM_MAX];
    pthread_mutex_t prof_hdc_mutex;
};

#endif
