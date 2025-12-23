/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef UT_TEST
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>

#include "securec.h"

#include "prof_communication.h"
#include "prof_h2d_kernel_msg.h"
#include "prof_hdc_comm.h"
#include "prof_hdc.h"

#define PROF_HDC_FLUSH_MAXTIME 20 /* s */
#define PROF_HDC_RESPOND_MAXTIME 10 /* s */
#define PROF_HDC_SESSION_CLOSE_MAXTIME 50 /* s */
#define PROF_HDC_TRY_COUNT 60000
#define PROF_HDC_TRY_WAIT_TIME 1000 /* us */
#define PROF_HDC_GET_CHANNELS_IDLE 0
#define PROF_HDC_GET_CHANNELS_OCCUPY 1

struct prof_hdc_info {
    pthread_mutex_t mutex;
    uint32_t verify;
    struct prof_channel_list *channels;
    sem_t sem;
    int status;
};

static struct prof_hdc_info g_prof_hdc_info[DEV_NUM];

STATIC void prof_hdc_info_init(uint32_t dev_id)
{
    struct prof_hdc_info *info = &g_prof_hdc_info[dev_id];

    (void)pthread_mutex_init(&info->mutex, NULL);
    info->verify = 0;
    info->channels = NULL;
    ATOMIC_SET(&info->status, PROF_HDC_GET_CHANNELS_IDLE);
    (void)sem_init(&info->sem, 0, 0);
}

STATIC struct prof_hdc_info *prof_hdc_get_info(uint32_t dev_id)
{
    static pthread_mutex_t init_mutex = PTHREAD_MUTEX_INITIALIZER;
    static int init_flag[DEV_NUM] = {0};

    if (init_flag[dev_id] != 0) {
        return &g_prof_hdc_info[dev_id];
    }

    (void)pthread_mutex_lock(&init_mutex);
    if (init_flag[dev_id] == 0) {
        prof_hdc_info_init(dev_id);
        ATOMIC_SET(&init_flag[dev_id], 1);
    }
    (void)pthread_mutex_unlock(&init_mutex);

    return &g_prof_hdc_info[dev_id];
}

struct prof_hdc_chan_info {
    int ret_val;
    sem_t sem;
};

static struct prof_hdc_chan_info *g_prof_hdc_chan[DEV_NUM] = {NULL};

STATIC drvError_t prof_hdc_chan_info_init(uint32_t dev_id)
{
    struct prof_hdc_chan_info *chan_info = NULL;
    int i;

    chan_info = (struct prof_hdc_chan_info *)calloc(PROF_CHANNEL_NUM_MAX, sizeof(*chan_info));
    if (chan_info == NULL) {
        PROF_ERR("Failed to alloc hdc_chan_info. (dev_id=%u)\n", dev_id);
        return DRV_ERROR_MALLOC_FAIL;
    }

    for (i = 0; i < PROF_CHANNEL_NUM_MAX; i++) {
        (void)sem_init(&chan_info[i].sem, 0, 0);
    }

    g_prof_hdc_chan[dev_id] = chan_info;
    return DRV_ERROR_NONE;
}

STATIC struct prof_hdc_chan_info *prof_hdc_get_chan_info(uint32_t dev_id, uint32_t chan_id)
{
    static pthread_mutex_t init_mutex = PTHREAD_MUTEX_INITIALIZER;
    static int init_flag[DEV_NUM] = {0};
    drvError_t ret;

    if (init_flag[dev_id] != 0) {
        return &g_prof_hdc_chan[dev_id][chan_id];
    }

    (void)pthread_mutex_lock(&init_mutex);
    if (init_flag[dev_id] == 0) {
        ret = prof_hdc_chan_info_init(dev_id);
        if (ret != DRV_ERROR_NONE) {
            (void)pthread_mutex_unlock(&init_mutex);
            PROF_ERR("Failed to init hdc_chan_info. (dev_id=%u, ret=%d)\n", dev_id, (int)ret);
            return NULL;
        }

        ATOMIC_SET(&init_flag[dev_id], 1);
    }
    (void)pthread_mutex_unlock(&init_mutex);

    return &g_prof_hdc_chan[dev_id][chan_id];
}

STATIC drvError_t prof_hdc_chan_sem_init(uint32_t dev_id, uint32_t chan_id)
{
    struct prof_hdc_chan_info *chan_info = prof_hdc_get_chan_info(dev_id, chan_id);

    if (chan_info == NULL) {
        PROF_ERR("Failed to get hdc_chan_info. (dev_id=%u, chan_id=%u)\n", dev_id, chan_id);
        return DRV_ERROR_MALLOC_FAIL;
    }

    (void)sem_init(&chan_info->sem, 0, 0);
    return DRV_ERROR_NONE;
}

STATIC drvError_t prof_hdc_get_ret(uint32_t dev_id, uint32_t chan_id, int timeout, uint32_t msg_type)
{
    sem_t *sem = NULL;
    uint32_t wait_count = 0;
    uint32_t wait_count_max = (uint32_t)(timeout * 1000000 / 500);
    int ret;
    drvError_t drv_ret;

    if (msg_type == PROF_HDC_CMD_GET_CHANNEL) {
        sem = &(prof_hdc_get_info(dev_id)->sem);
    } else {
        sem = &(prof_hdc_get_chan_info(dev_id, chan_id)->sem);
    }

    ret = sem_trywait(sem);
    while (ret != 0) {
        if (wait_count == wait_count_max) {
            PROF_ERR("Waiting for device response was timeout.(dev_id=%u, chan_id=%u, msg_type=%u)\n",
                dev_id, chan_id, msg_type);
            goto destroy_session;
        }
        (void)usleep(500); /* 500 */
        wait_count++;
        ret = sem_trywait(sem);
    }

    if (msg_type == (uint32_t)PROF_HDC_CMD_GET_CHANNEL) {
        return DRV_ERROR_NONE;
    } else {
        return prof_comm_errcode_convert(prof_hdc_get_chan_info(dev_id, chan_id)->ret_val);
    }

destroy_session:
    if (msg_type == (uint32_t)PROF_HDC_CMD_GET_CHANNEL) {
        prof_hdc_get_info(dev_id)->verify++;
    }

    drv_ret = prof_session_destroy(dev_id, chan_id);
    if ((drv_ret != DRV_ERROR_NONE) && (drv_ret != (int)DRV_ERROR_NOT_SUPPORT)) {
        PROF_ERR("Failed to destroy hdc session client. (ret=%d)\n", (int)drv_ret);
    }

    return DRV_ERROR_WAIT_TIMEOUT;
}

STATIC drvError_t prof_hdc_get_channels_msg_send(uint32_t dev_id, struct prof_hdc_info *info)
{
    struct prof_hdc_msg *msg = NULL;
    uint32_t msg_len = sizeof(struct prof_hdc_msg) + sizeof(struct prof_channel_list);
    drvError_t ret;

    msg = (struct prof_hdc_msg *)calloc(1, msg_len);
    if (msg == NULL) {
        PROF_ERR("Failed to alloc hdc_msg. (dev_id=%u, len=%u)\n", dev_id, msg_len);
        return DRV_ERROR_MALLOC_FAIL;
    }

    msg->msg_type = (int)PROF_HDC_CMD_GET_CHANNEL;
    msg->cmd_verify = info->verify;
    msg->data_len = sizeof(struct prof_channel_list);

    ret = prof_hdc_msg_send(dev_id, 0, (uint8_t *)msg, msg_len);
    if (ret != DRV_ERROR_NONE) {
        PROF_ERR("Failed to send message to device. (dev_id=%u, ret=%d)\n", dev_id, (int)ret);
    }

    free(msg);
    return ret;
}

STATIC drvError_t prof_hdc_start_msg_send(uint32_t dev_id, uint32_t chan_id, struct prof_user_start_para *para)
{
    struct prof_hdc_msg *msg = NULL;
    struct prof_hdc_start_para *start = NULL;
    uint32_t msg_len = sizeof(struct prof_hdc_msg) + sizeof(struct prof_hdc_start_para);
    int ret;
    drvError_t drv_ret;

    msg = (struct prof_hdc_msg *)calloc(1, msg_len);
    if (msg == NULL) {
        PROF_ERR("Failed to alloc hdc_msg. (dev_id=%u, chan_id=%u, len=%u)\n", dev_id, chan_id, msg_len);
        return DRV_ERROR_MALLOC_FAIL;
    }

    msg->msg_type = (int)PROF_HDC_CMD_START;
    msg->channel_id = chan_id;
    msg->data_len = sizeof(struct prof_hdc_start_para);
    start = (struct prof_hdc_start_para *)(msg->data);
    start->sample_period = ((para->remote_pid == 0) ? para->sample_period : 0);
    start->user_data_size = para->user_data_size;
    if ((para->user_data != NULL) && (para->user_data_size != 0)) {
        ret = memcpy_s(start->user_data, PROF_USER_DATA_LEN, para->user_data, para->user_data_size);
        if (ret != 0) {
            PROF_ERR("Failed to copy user_data. (dev_id=%u, chan_id=%u, ret=%d, data_size=%u)\n",
                dev_id, chan_id, ret, para->user_data_size);
            free(msg);
            return DRV_ERROR_MEMORY_OPT_FAIL;
        }
    }

    drv_ret = prof_hdc_msg_send(dev_id, chan_id, (uint8_t *)msg, msg_len);
    if (drv_ret != DRV_ERROR_NONE) {
        PROF_ERR("Failed to send message to device. (dev_id=%u, chan_id=%u, ret=%d)\n", dev_id, chan_id, (int)drv_ret);
    }

    free(msg);
    return drv_ret;
}

STATIC drvError_t prof_hdc_stop_msg_send(uint32_t dev_id, uint32_t chan_id)
{
    struct prof_hdc_msg *msg = NULL;
    uint32_t msg_len = sizeof(struct prof_hdc_msg) + sizeof(struct prof_kernel_dfx_info);
    drvError_t ret;

    msg = (struct prof_hdc_msg *)calloc(1, msg_len);
    if (msg == NULL) {
        PROF_ERR("Failed to alloc hdc_msg. (dev_id=%u, chan_id=%u, len=%u)\n", dev_id, chan_id, msg_len);
        return DRV_ERROR_MALLOC_FAIL;
    }

    msg->msg_type = (int)PROF_HDC_CMD_STOP;
    msg->channel_id = chan_id;
    msg->data_len = sizeof(struct prof_kernel_dfx_info);

    ret = prof_hdc_msg_send(dev_id, chan_id, (uint8_t *)msg, msg_len);
    if (ret != DRV_ERROR_NONE) {
        PROF_ERR("Failed to send message to device. (dev_id=%u, chan_id=%u, ret=%d)\n", dev_id, chan_id, (int)ret);
    }

    free(msg);
    return ret;
}

STATIC drvError_t prof_hdc_flush_msg_send(uint32_t dev_id, uint32_t chan_id)
{
    struct prof_hdc_msg *msg = NULL;
    uint32_t msg_len = sizeof(struct prof_hdc_msg) + sizeof(uint32_t);
    drvError_t ret;

    msg = (struct prof_hdc_msg *)calloc(1, msg_len);
    if (msg == NULL) {
        PROF_ERR("Failed to alloc hdc_msg. (dev_id=%u, chan_id=%u, len=%u)\n", dev_id, chan_id, msg_len);
        return DRV_ERROR_MALLOC_FAIL;
    }

    msg->msg_type = (int)PROF_HDC_DATA_FLUSH;
    msg->channel_id = chan_id;
    msg->data_len = sizeof(uint32_t);

    ret = prof_hdc_msg_send(dev_id, chan_id, (uint8_t *)msg, msg_len);
    if (ret != DRV_ERROR_NONE) {
        PROF_ERR("Failed to send message to device. (dev_id=%u, chan_id=%u, ret=%d)\n", dev_id, chan_id, (int)ret);
    }

    free(msg);
    return ret;
}

STATIC void prof_hdc_receive_channels(uint32_t dev_id, struct prof_hdc_msg *msg_head, uint32_t total_len)
{
    struct prof_hdc_info *info = prof_hdc_get_info(dev_id);
    uint32_t expect_len = sizeof(struct prof_hdc_msg) + sizeof(struct prof_channel_list);
    int ret;

    if (total_len != expect_len) {
        PROF_ERR("Invalid para. (dev_id=%u, total_len=%u, expect_len=%u)\n", dev_id, total_len, expect_len);
        return;
    }

    if (msg_head->ret_val != PROF_OK) {
        PROF_ERR("The returned value was not [PROF_OK]. (dev_id=%u, ret_val=%d)\n", dev_id, msg_head->ret_val);
        return;
    }

    (void)pthread_mutex_lock(&info->mutex);
    if (msg_head->cmd_verify != info->verify || info->channels == NULL) {
        PROF_ERR("Recv dev respond too late. (dev_id=%u, msg_verify=%u, expect_verify=%u)\n",
            dev_id, msg_head->cmd_verify, info->verify);
        (void)pthread_mutex_unlock(&info->mutex);
        return;
    }

    ret = memcpy_s(info->channels, sizeof(struct prof_channel_list), msg_head->data, sizeof(struct prof_channel_list));
    if (ret != 0) {
        PROF_ERR("Failed to copy channels. (dev_id=%u, ret=%d)\n", dev_id, ret);
        (void)pthread_mutex_unlock(&info->mutex);
        return;
    }
    (void)sem_post(&info->sem);
    (void)pthread_mutex_unlock(&info->mutex);

    ret = prof_session_destroy(dev_id, 0);
    if ((ret != PROF_OK) && (ret != (int)DRV_ERROR_NOT_SUPPORT)) {
        PROF_ERR("Failed to destroy hdc session client. (dev_id=%u, ret=%d)\n", dev_id, ret);
        return;
    }
}

STATIC void prof_hdc_receive_chan_respond(uint32_t dev_id, struct prof_hdc_msg *msg_head)
{
    struct prof_hdc_chan_info *chan_info = NULL;
    uint32_t chan_id = msg_head->channel_id;
    drvError_t ret;

    if (chan_id >= PROF_CHANNEL_NUM_MAX) {
        PROF_ERR("Invalid para. (chan_id=%u)\n", chan_id);
        return;
    }

    PROF_INFO("Received chan response. (msg_type=%d, dev_id=%u, chan_id=%u, ret_val=%d)\n",
        msg_head->msg_type, dev_id, chan_id, msg_head->ret_val);

    chan_info = prof_hdc_get_chan_info(dev_id, chan_id);
    if (chan_info == NULL) {
        PROF_ERR("Failed to get chan_info. (dev_id=%u, chan_id=%u)\n", dev_id, chan_id);
        return;
    }

    chan_info->ret_val = msg_head->ret_val;

    if ((msg_head->msg_type == (int)PROF_HDC_CMD_STOP) && (msg_head->ret_val == DRV_ERROR_NONE)) {
        ret = prof_session_destroy(dev_id, chan_id);
        if ((ret != DRV_ERROR_NONE) && (ret != DRV_ERROR_NOT_SUPPORT)) {
            PROF_ERR("Failed to destroy hdc session client. (dev_id=%u, chan_id=%u, ret=%d)\n", dev_id, chan_id, ret);
            return;
        }
    }

    (void)sem_post(&chan_info->sem);
}

STATIC void prof_hdc_receive_chan_data(uint32_t dev_id, struct prof_hdc_msg *msg_head, uint32_t total_len)
{
    struct prof_comm_core_notifier *notifier = prof_comm_get_notifier();

    if (total_len != (sizeof(struct prof_hdc_msg) + msg_head->data_len)) {
        PROF_ERR("Invalid para. (dev_id=%u, total_len=%u, head_size=%lu, data_len=%u)\n",
            dev_id, total_len, sizeof(struct prof_hdc_msg), msg_head->data_len);
        return;
    }

    if (msg_head->channel_id >= PROF_CHANNEL_NUM_MAX) {
        PROF_ERR("Invalid para. (dev_id=%u, chan_id=%u)\n", dev_id, msg_head->channel_id);
        return;
    }

    if (msg_head->data_len > 0) {
        notifier->chan_report(dev_id, msg_head->channel_id, msg_head->data, msg_head->data_len, false);
    }
}

void prof_hdc_msg_proc(uint32_t dev_id, void *msg, uint32_t len)
{
    struct prof_hdc_msg *msg_head = NULL;

    if ((dev_id >= (uint32_t)DEV_NUM) || (msg == NULL) || (len < sizeof(struct prof_hdc_msg))) {
        PROF_ERR("Invalid para. (dev_id=%u, msg_len=%u, min_len=%lu)\n", dev_id, len, sizeof(struct prof_hdc_msg));
        return;
    }

    msg_head = (struct prof_hdc_msg *)msg;
    PROF_DEBUG("Recv dev respond. (dev_id=%u, msg_type=%d)\n", dev_id, msg_head->msg_type);
    switch (msg_head->msg_type) {
        case PROF_HDC_CMD_GET_CHANNEL:
            prof_hdc_receive_channels(dev_id, msg_head, len);
            break;
        case PROF_HDC_CMD_START:
        case PROF_HDC_CMD_STOP:
        case PROF_HDC_DATA_FLUSH:
            prof_hdc_receive_chan_respond(dev_id, msg_head);
            break;
        case PROF_HDC_DATA:
            prof_hdc_receive_chan_data(dev_id, msg_head, len);
            break;
        default:
            PROF_ERR("Profile detected an unknown msg. (dev_id=%u, msg_type=%d)\n", dev_id, msg_head->msg_type);
            return;
    }
}

STATIC drvError_t prof_hdc_try_lock(struct prof_hdc_info *info)
{
    int wait_count = 0;
    drvError_t ret = DRV_ERROR_WAIT_TIMEOUT;

    while (wait_count < PROF_HDC_TRY_COUNT) {
        if (CAS(&info->status, PROF_HDC_GET_CHANNELS_IDLE, PROF_HDC_GET_CHANNELS_OCCUPY)) {
            ret = DRV_ERROR_NONE;
            break;
        }
        wait_count++;
        (void)usleep(PROF_HDC_TRY_WAIT_TIME);
    }

    return ret;
}

STATIC void prof_hdc_unlock(struct prof_hdc_info *info)
{
    ATOMIC_SET(&info->status, PROF_HDC_GET_CHANNELS_IDLE);
}

drvError_t prof_hdc_get_channels(uint32_t dev_id, struct prof_channel_list *channels)
{
    struct prof_hdc_info *info = prof_hdc_get_info(dev_id);
    drvError_t ret;

    ret = prof_hdc_try_lock(info);
    if (ret != DRV_ERROR_NONE) {
        PROF_ERR("Failed to get permission. (dev_id=%u, ret=%d)\n", dev_id, (int)ret);
        return ret;
    }

    (void)pthread_mutex_lock(&info->mutex);
    (void)sem_init(&info->sem, 0, 0);
    info->channels = channels;
    (void)pthread_mutex_unlock(&info->mutex);

    ret = prof_hdc_get_channels_msg_send(dev_id, info);
    if (ret != DRV_ERROR_NONE) {
        goto exit; 
    }

    ret = prof_hdc_get_ret(dev_id, 0, PROF_HDC_RESPOND_MAXTIME + PROF_HDC_SESSION_CLOSE_MAXTIME, PROF_HDC_CMD_GET_CHANNEL);
    if (ret != DRV_ERROR_NONE) {
        PROF_ERR("Failed to get channels. (dev_id=%u, ret=%d)\n", dev_id, (int)ret);
    }

exit:
    (void)pthread_mutex_lock(&info->mutex);
    info->channels = NULL;
    (void)pthread_mutex_unlock(&info->mutex);
    prof_hdc_unlock(info);

    return ret;
}

drvError_t prof_hdc_start(uint32_t dev_id, uint32_t chan_id, struct prof_user_start_para *para)
{
    struct process_sign sign_info = {0};
    drvError_t ret;

    ret = drvGetProcessSign(&sign_info);
    if (ret != DRV_ERROR_NONE) {
        PROF_ERR("Failed get process sign. (devid=%u, chan_id=%u, ret=%d).\n", dev_id, chan_id, (int)ret);
        return ret;
    }

    ret = prof_hdc_chan_sem_init(dev_id, chan_id);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    ret = prof_hdc_start_msg_send(dev_id, chan_id, para);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    return prof_hdc_get_ret(dev_id, chan_id, PROF_HDC_RESPOND_MAXTIME, PROF_HDC_CMD_START);
}

drvError_t prof_hdc_stop(uint32_t dev_id, uint32_t chan_id)
{
    drvError_t ret;

    ret = prof_hdc_chan_sem_init(dev_id, chan_id);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    ret = prof_hdc_stop_msg_send(dev_id, chan_id);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    return prof_hdc_get_ret(dev_id, chan_id, PROF_HDC_RESPOND_MAXTIME + PROF_HDC_SESSION_CLOSE_MAXTIME, PROF_HDC_CMD_STOP);
}

drvError_t prof_hdc_flush(uint32_t dev_id, uint32_t chan_id)
{
    drvError_t ret;

    ret = prof_hdc_chan_sem_init(dev_id, chan_id);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    ret = prof_hdc_flush_msg_send(dev_id, chan_id);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    return prof_hdc_get_ret(dev_id, chan_id, PROF_HDC_FLUSH_MAXTIME, PROF_HDC_DATA_FLUSH);
}

STATIC void __attribute__((constructor)) prof_hdc_proc_init(void)
{
    prof_hdc_register_msg_proc_func(prof_hdc_msg_proc);
    PROF_INFO("Register prof hdc msg proc successfully.\n");
}

#else
int prof_hdc_ut_test(void)
{
    return 0;
}
#endif
