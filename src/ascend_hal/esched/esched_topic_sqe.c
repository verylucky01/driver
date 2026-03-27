/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>

#include "securec.h"
#include "ascend_hal.h"
#include "event_sched.h"
#include "drv_type.h"
#include "esched_user_interface.h"
#include "esched_drv_interface.h"
#include "topic_sched_drv_interface.h"

void *esched_alloc_ext_msg(void)
{
    return (void *)calloc(1, sizeof(struct sched_trs_chan_ext_msg));
}

void esched_free_ext_msg(void *ext_msg)
{
    free(ext_msg);
}

void esched_fill_sqcq_alloc_info(unsigned int sqe_depth, unsigned int cqe_depth, struct halSqCqInputInfo *in, void *ext_msg_t)
{
    in->type = DRV_NORMAL_TYPE;
    in->tsId = 0; /* keep same as esched_drv_set_chan_create_para */ 

    in->sqeSize = TOPIC_SCHED_TASK_SQE_SIZE;
    in->cqeSize = TOPIC_SCHED_TASK_CQE_SIZE;
    in->sqeDepth = sqe_depth + 1; /* stars restrictions (depth - 1) % 2^3 = 0 */
    in->cqeDepth = cqe_depth + 1; /* stars restrictions (depth - 1) % 2^3 = 0 */

    in->grpId = 0; /* runtime thread identifier,normal : 0 */
    in->flag = 0;
    in->cqId = 0; /* if flag bit 0 is 0, don't care about it */
    in->sqId = 0; /* if flag bit 1 is 0, don't care about it */

    in->info[0] = 0xffff;   /* 0 : streamid */
    in->info[1] = TOPIC_SCHED_RTSQ_PRI;   /* 1 : rtsq pri */
    in->info[2] = 0;   /* 2 : ssid */
    in->info[3] = TOPIC_SCHED_ACPU_POOL_ID;   /* 3 : pool_id */

    in->ext_info = ext_msg_t;
    in->ext_info_len = sizeof(struct sched_trs_chan_ext_msg);

    struct sched_trs_chan_ext_msg *ext_msg = (struct sched_trs_chan_ext_msg *)ext_msg_t;
    /* in ascend_kernel_hal.h #define CHAN_SUB_TYPE_HW_TOPIC_SCHED    1 */
    ext_msg->msg_header.type = 1;
    ext_msg->topic_ext_info.mb_spec = 0;
}

void *esched_alloc_topic_sqe(void)
{
    return (void *)calloc(1, sizeof(struct topic_sched_sqe));
}

void esched_free_topic_sqe(void *sqe)
{
    free(sqe);
}

int esched_fill_topic_sqe(unsigned int devid, struct event_summary *event, void *sqe_t)
{
    int ret;
    struct topic_sched_sqe *sqe = (struct topic_sched_sqe *)sqe_t;

    sqe->type = TOPIC_SCHED_SQE_TYPE_DEVICE;
    sqe->ie = 0;
    sqe->pre_p = 0;
    sqe->post_p = 0;
    sqe->wr_cqe = 0;
    sqe->ptr_mode = 0;
    sqe->ptt_mode = 0;
    sqe->head_update = 0;
    sqe->blk_dim = 1;

    sqe->rt_streamid = 0;
    sqe->task_id = 0;

    sqe->kernel_type = TOPIC_SCHED_DEFAULT_KERNEL_TYPE;
    sqe->batch_mode = 0; /* no use. */
    sqe->topic_type = TOPIC_TYPE_AICPU_DEVICE_ONLY; /* no use. */
    sqe->qos = 0; /* no use. */

    sqe->kernel_credit = TOPIC_SCHED_SQE_KERNEL_CREDIT;
    sqe->sqe_len = 0;

    ret = memcpy_s(&sqe->user_data[0], sizeof(typeof(sqe->user_data)), event->msg, event->msg_len);
    if (ret != EOK) {
        sched_err("Sqe data copy fail. (ret=%d; msg_len=%ld)\n", ret, event->msg_len);
        return DRV_ERROR_MEMORY_OPT_FAIL;
    }

    sqe->subtopic_id = event->subevent_id;
    sqe->topic_id = event->event_id;
    sqe->gid = event->grp_id;
    sqe->user_data_len = event->msg_len;

    sqe->pid = event->pid;

    sched_debug("Fill sqe event info. (pid=%d; gid=%u; eventid=%u; dst_engine=%d; policy=%u)\n",
        event->pid, event->grp_id, event->event_id, event->dst_engine, event->policy);
    return DRV_ERROR_NONE;
}
