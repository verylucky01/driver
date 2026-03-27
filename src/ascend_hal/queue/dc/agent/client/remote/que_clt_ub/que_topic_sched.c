/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include <stdlib.h>
#include <pthread.h>
#include <limits.h>

#include "ascend_hal_error.h"
#include "queue.h"
#include "que_compiler.h"
#include "esched_topic_sqe.h"
#include "que_ctx.h"
#include "que_ub_msg.h"
#include "que_topic_sched.h"

#define QUE_TOPIC_SQ_DEPTH 256
#define QUE_TOPIC_CQ_DEPTH 256

static inline void que_fill_sqcq_free_info(struct que_sqcq_info *sqcq_info, struct halSqCqFreeInfo *free_info)
{
    free_info->type = sqcq_info->type;
    free_info->tsId = sqcq_info->ts_id;
    free_info->sqId = sqcq_info->sq_id;
    free_info->cqId = sqcq_info->cq_id;
    free_info->flag = sqcq_info->flag;
    return;
}

static inline void que_fill_sqcq_info(struct halSqCqInputInfo *in, struct halSqCqOutputInfo *out,
    struct que_sqcq_info *sqcq_info)
{
    sqcq_info->type = in->type;
    sqcq_info->ts_id = in->tsId;
    sqcq_info->sq_id = out->sqId;
    sqcq_info->cq_id = out->cqId;
    sqcq_info->flag = out->flag;
    return;
}

int que_topic_rtsq_init(unsigned int devid, struct que_sqcq_info *sqcq_info)
{
    int ret;
    struct halSqCqInputInfo in = {0};
    struct halSqCqOutputInfo out = {0};
    void *ext_msg = NULL;

    struct drvDevInfo devinfo;
    ret = drvDeviceOpen((void *)&devinfo, devid);
    if ((ret != DRV_ERROR_NONE) && (ret != DRV_ERROR_REPEATED_INIT)) {
        QUEUE_LOG_ERR("open drv failed. (ret=%d; devid=%u)\n", ret, devid);
        return DRV_ERROR_INNER_ERR;
    }

    ext_msg = esched_alloc_ext_msg();
    if (que_unlikely(ext_msg == NULL)) {
        QUEUE_LOG_ERR("que alloc ext_msg fail. (devid=%u)\n", devid);
        return DRV_ERROR_INNER_ERR;
    }

    esched_fill_sqcq_alloc_info(QUE_TOPIC_SQ_DEPTH, QUE_TOPIC_CQ_DEPTH, &in, ext_msg);
    ret = halSqCqAllocate(devid, &in, &out);
    if (que_unlikely(ret != DRV_ERROR_NONE)) {
        esched_free_ext_msg(ext_msg);
        QUEUE_LOG_ERR("que sqcq alloc fail. (ret=%d; devid=%u)\n", ret, devid);
        return ret;
    }

    que_fill_sqcq_info(&in, &out, sqcq_info);
    esched_free_ext_msg(ext_msg);
    return DRV_ERROR_NONE;
}

void que_topic_rtsq_uninit(unsigned int devid, struct que_sqcq_info *sqcq_info)
{
    int ret;
    struct halSqCqFreeInfo free_info = {0};
    que_fill_sqcq_free_info(sqcq_info, &free_info);
    ret = halSqCqFree(devid, &free_info);
    if (que_unlikely(ret != DRV_ERROR_NONE)) {
        QUEUE_LOG_ERR("que sqcq free fail. (ret=%d; devid=%u; ts_id=%u; type=%u; sq_id=%u)\n",
            ret, devid, sqcq_info->ts_id, sqcq_info->type, sqcq_info->sq_id);
    }
    sqcq_info->sq_id = UINT_MAX;
    sqcq_info->cq_id = UINT_MAX;
}

static inline void que_fill_task_send_info(struct que_sqcq_info *sqcq_info, void *sqe, int time_out,
    struct halTaskSendInfo *sendinfo)
{
    sendinfo->type = sqcq_info->type;
    sendinfo->tsId = sqcq_info->ts_id;
    sendinfo->sqId = sqcq_info->sq_id;

    sendinfo->sqe_num = 1;
    sendinfo->sqe_addr = (unsigned char *)sqe;
    sendinfo->timeout = time_out;
    return;
}

static int event_fill_topic_sqe(unsigned int devid, unsigned int sub_event, struct que_event_msg *event_msg, void *sqe)
{
    struct que_event_attr attr = {.devid = devid, .sub_event = sub_event};
    struct event_summary event_sum = {0};
    int ret;

    ret = que_ctx_get_pids(devid, &attr.hostpid, &attr.devpid);
    if (que_unlikely(ret != DRV_ERROR_NONE)) {
        QUEUE_LOG_ERR("que ctx get pids fail. (ret=%d; devid=%u; sub_event=%u)\n",
            ret, devid, attr.sub_event);
        return ret;
    }

    ret = que_event_sum_init(&attr, event_msg, &event_sum);
    if (que_unlikely(ret != DRV_ERROR_NONE)) {
        QUEUE_LOG_ERR("event summary init fail. (ret=%d; devid=%u; hostpid=%d; devpid=%d; sub_event=%u)\n",
            ret, attr.devid, attr.hostpid, attr.devpid, attr.sub_event);
        return ret;
    }

    ret = esched_fill_topic_sqe(devid, &event_sum, sqe);
    if (que_unlikely(ret != DRV_ERROR_NONE)) {
        QUEUE_LOG_ERR("topic sqe fill fail. (ret=%d; devid=%u; sub_event=%u)\n", ret, attr.devid, attr.sub_event);
    }

    que_event_sum_uninit(&event_sum);
    return ret;
}

static int que_send_topic_sqe(unsigned int devid, unsigned int sub_event, struct que_event_msg *event_msg,
    struct que_sqcq_info *sqcq_info)
{
    int ret;
    void *sqe_addr = NULL;
    struct halTaskSendInfo sendinfo = {0};

    sqe_addr = esched_alloc_topic_sqe();
    if (que_unlikely(sqe_addr == NULL)) {
        QUEUE_LOG_ERR("que sqe alloc fail. (devid=%u; sub_event=%u)\n", devid, sub_event);
        return DRV_ERROR_INNER_ERR;
    }

    ret = event_fill_topic_sqe(devid, sub_event, event_msg, sqe_addr);
    if (que_unlikely(ret != DRV_ERROR_NONE)) {
        QUEUE_LOG_ERR("que fill topic sqe fail. (ret=%d; devid=%u; sq_id=%u)\n", ret, devid, sqcq_info->sq_id);
        goto free_proc;
    }

    que_fill_task_send_info(sqcq_info, sqe_addr, QUE_EVENT_TIMEOUT_MS, &sendinfo);
    ret = halSqTaskSend(devid,  &sendinfo);
    if (que_unlikely(ret != DRV_ERROR_NONE)) {
        QUEUE_LOG_ERR("que sqe task send fail. (ret=%d; devid=%u; sq_id=%u)\n", ret, devid, sqcq_info->sq_id);
        goto free_proc;
    }

free_proc:
    esched_free_topic_sqe(sqe_addr);
    return ret;
}

int que_enque_notify_proc(unsigned int devid, unsigned int qid, struct que_sqcq_info *sqcq_info)
{
    struct que_enque_in_msg in = {.qid = qid};
    struct que_event_msg event_msg = {.in = (char *)&in, .in_len = sizeof(struct que_enque_in_msg),
            .out = NULL, .out_len = 0};

    int ret;
    ret = que_send_topic_sqe(devid, DRV_SUBEVENT_ENQUEUE_MSG, &event_msg, sqcq_info);
    if (que_unlikely(ret != DRV_ERROR_NONE)) {
        QUEUE_LOG_ERR("que tx send fail. (ret=%d; devid=%u; qid=%u)\n", ret, devid, qid);
    }
    return ret;
}