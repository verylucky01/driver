/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include <stdint.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include "securec.h"
#include "ascend_hal.h"
#include "ascend_hal_external.h"
#include "ascend_hal_error.h"
#include "ascend_hal_define.h"
#include "esched_user_interface.h"
#include "dms_user_interface.h"
#include "pbl_uda_user.h"

#include "queue_client_comm.h"
#include "queue_interface.h"
#include "queue.h"
#include "que_compiler.h"
#include "que_ub_msg.h"
#include "que_inter_dev_tgt.h"
#include "que_event.h"
#include "que_ctx.h"
#include "que_comm_chan.h"
#include "que_comm_ctx.h"
#include "que_comm_thread.h"
#include "que_clt_ub.h"
#ifndef EMU_ST

static pthread_mutex_t g_queue_mutex = PTHREAD_MUTEX_INITIALIZER;
static int g_queue_tgid = -1;
static pthread_mutex_t g_clt_queue_mutex = PTHREAD_MUTEX_INITIALIZER;
static int g_queue_clt_init_status[MAX_DEVICE] = {0};

static int que_clt_get_hostpid(pid_t *hostpid)
{
    if (g_queue_tgid == -1) {
        g_queue_tgid = drvDeviceGetBareTgid();
        QUEUE_RUN_LOG_INFO("query tgid. (tgid=%d)\n", g_queue_tgid);
    }
    if (que_unlikely(g_queue_tgid == -1)) {
        return DRV_ERROR_INNER_ERR;
    }

    *hostpid = g_queue_tgid;
    return DRV_ERROR_NONE;
}

static pid_t que_clt_query_proc_hostpid(pid_t hostpid)
{
    unsigned int chip_id, vfid, cp_type;
    pid_t hostpid_;
    int ret;

    ret = drvQueryProcessHostPid(hostpid, &chip_id, &vfid, (unsigned int *)(uintptr_t)&hostpid_, &cp_type);
    if (ret != DRV_ERROR_NONE) {
        hostpid_ = hostpid;
    }
    return hostpid_;
}

static int que_clt_get_devpid(unsigned int devid, pid_t *devpid)
{
    struct halQueryDevpidInfo info;
    pid_t hostpid = getpid();
    pid_t hostpid_;
    int ret;

    hostpid_ = que_clt_query_proc_hostpid(hostpid);
    info.proc_type = DEVDRV_PROCESS_CP1;
    info.hostpid = hostpid_; /* docker pid */
    info.devid = devid;
    info.vfid = 0;
    ret = halQueryDevpid(info, devpid);
    if (que_unlikely(ret != DRV_ERROR_NONE)) {
        QUEUE_LOG_ERR("Query devpid failed. (ret=%d; dev_id=%u; hostpid=%d).\n", ret, devid, hostpid);
    }
    return ret;
}

static int que_clt_get_pids(unsigned int devid, pid_t *hostpid, pid_t *devpid)
{
    int ret;

    ret = que_clt_get_hostpid(hostpid);
    if (que_unlikely(ret != DRV_ERROR_NONE)) {
        QUEUE_LOG_ERR("get hostpid fail. (ret=%d; devid=%u)\n", ret, devid);
        return ret;
    }
    ret = que_clt_get_devpid(devid, devpid);
    if (que_unlikely(ret != DRV_ERROR_NONE)) {
        QUEUE_LOG_ERR("get devpid fail. (ret=%d; devid=%u)\n", ret, devid);
        return ret;
    }

    return DRV_ERROR_NONE;
}

static int que_thread_create(unsigned int devid)
{
    int ret;
    if (devid == halGetHostDevid()) {
        return que_create_wait_f2nf_thread(devid);
    }

    ret = que_create_poll_thread(devid);
    if (que_unlikely(ret != DRV_ERROR_NONE)) {
        return ret;
    }
    ret = que_create_recycle_thread(devid);
    if (que_unlikely(ret != DRV_ERROR_NONE)) {
        que_thread_cancle(devid);
        return ret;
    }

    return ret;
}

static drvError_t que_clt_ub_res_init(unsigned int devid)
{
    pid_t hostpid = 0, devpid = 0;
    int ret;

    (void)pthread_mutex_lock(&g_queue_mutex);
    ret = queue_clt_init_check(devid);
    if (que_unlikely(ret != DRV_ERROR_NONE)) {
        goto mutex_unlock;
    }

    if (devid != halGetHostDevid()) {
        ret = que_clt_get_pids(devid, &hostpid, &devpid);
        if (que_unlikely(ret != DRV_ERROR_NONE)) {
            QUEUE_LOG_ERR("get hostpid & devpid fail. (ret=%d; devid=%u)\n", ret, devid);
            goto mutex_unlock;
        }
    }

    ret = que_ctx_create(devid, hostpid, devpid);
    if (que_unlikely(ret != DRV_ERROR_NONE)) {
        QUEUE_LOG_ERR("que ctx create fail. (ret=%d; devid=%u; hostpid=%d; devpid=%d)\n", ret, devid, hostpid, devpid);
        goto mutex_unlock;
    }

    ret = que_thread_create(devid);
    if (que_unlikely(ret != DRV_ERROR_NONE)) {
        que_ctx_destroy(devid);
        QUEUE_LOG_ERR("que thread create fail. (devid=%u)\n", devid);
        goto mutex_unlock;
    }

    (void)pthread_mutex_unlock(&g_queue_mutex);
    return DRV_ERROR_NONE;

mutex_unlock:
    (void)pthread_mutex_unlock(&g_queue_mutex);
    return ret;
}

static void que_clt_ub_res_uninit(unsigned int dev_id, bool uninit_flag)
{
    if (uninit_flag) {
        que_thread_cancle(dev_id);
        que_ctx_destroy(dev_id);
    }
}

static drvError_t que_clt_mem_alloc(void **addr, unsigned long long size)
{
    int ret;
    void *addr_tmp = valloc(size);
    if (que_unlikely(addr_tmp == NULL)) {
        QUEUE_LOG_ERR("que clt mem alloc fail. (size=%llu)\n", size);
        return DRV_ERROR_OUT_OF_MEMORY;
    }

    ret = memset_s(addr_tmp, size, 0, size);
    if (que_unlikely(ret != DRV_ERROR_NONE)) {
        QUEUE_LOG_ERR("memset fail. (ret=%d)\n", ret);
        free(addr_tmp);
        return DRV_ERROR_QUEUE_INNER_ERROR;
    }

    *addr = addr_tmp;
    return DRV_ERROR_NONE;
}

static void que_clt_mem_free(void *addr)
{
    if (addr != NULL) {
        free(addr);
    }
}

static drvError_t que_clt_init(unsigned int dev_id)
{
    struct que_init_in_msg in;
    struct que_init_out_msg out;
    struct que_event_msg event_msg = {.in = (char *)&in, .in_len = sizeof(struct que_init_in_msg), .out = (char *)&out, .out_len = sizeof(struct que_init_out_msg)};
    struct que_event_attr attr = {.devid = dev_id, .sub_event = DRV_SUBEVENT_QUEUE_INIT_MSG, .retry_flg = QUE_SEND_WITH_RETRY};
    int ret;

    ret = que_clt_ub_res_init(dev_id);
    if (que_unlikely((ret != DRV_ERROR_NONE) && (ret != DRV_ERROR_REPEATED_INIT))) {
        QUEUE_LOG_ERR("que clt ub res init fail. (ret=%d; devid=%u)\n", ret, dev_id);
        return ret;
    }

    (void)pthread_mutex_lock(&g_clt_queue_mutex);
    if (g_queue_clt_init_status[dev_id] == QUEUE_INITED) {
       (void)pthread_mutex_unlock(&g_clt_queue_mutex);
        return DRV_ERROR_REPEATED_INIT;
    }

    ret = que_h2d_info_fill(dev_id, &attr, &in);
    if (que_unlikely(ret != DRV_ERROR_NONE)) {
        QUEUE_LOG_ERR("que h2d info fill fail. (ret=%d; devid=%u)\n", ret, dev_id);
        goto mutex_unlock;
    }

    que_update_ini_basetime(dev_id);
    ret = que_event_send(&attr, &event_msg, QUE_EVENT_MAX_WAIT_10S);
    if (que_unlikely(ret != DRV_ERROR_NONE)) {
        QUEUE_LOG_ERR("que init event send fail. (ret=%d; devid=%u)\n", ret, dev_id);
        goto mutex_unlock;
    }

    ret = que_ctx_h2d_init(dev_id, &out.tjfr_id, &out.token);
    if (que_unlikely(ret != DRV_ERROR_NONE)) {
        goto mutex_unlock;
    }
    QUEUE_RUN_LOG_INFO("que init device. (devid=%u)\n", dev_id);
    g_queue_clt_init_status[dev_id] = QUEUE_INITED;

mutex_unlock:
    (void)pthread_mutex_unlock(&g_clt_queue_mutex);
    return ret;
}

static void que_clt_uninit(unsigned int dev_id, unsigned int scene)
{
    int ret;
    int qid_idx;
    struct que_ctx *ctx = NULL;
    (void)scene;

    ctx = que_ctx_get(dev_id);
    if (que_likely(ctx == NULL)) {
        return;
    }
    que_thread_cancle(dev_id);
    /* free all chan for client queue */
    for (qid_idx = 0; qid_idx < CLIENT_QID_OFFSET; qid_idx++) {
        ret = que_ctx_chan_destroy(dev_id, qid_idx);
        if (que_unlikely(ret != DRV_ERROR_NONE)) {
            QUEUE_RUN_LOG_INFO("que destroy chan. (ret=%d; devid=%u; qid=%u)\n", ret, dev_id, qid_idx);
        }
    }

    /* Free all of queue */
    que_ctx_destroy(dev_id);

    que_ctx_put(ctx);

    que_urma_ctx_put_ex(dev_id);

    /* clear grp id for cp process */
    que_init_grpid_by_dev(dev_id);
    g_queue_clt_init_status[dev_id] = 0;
    g_queue_tgid = -1;
}

static int que_clt_create_send(unsigned int devid, const QueueAttr *que_attr, unsigned int *qid, unsigned long *create_time)
{
    struct que_create_in_msg in;
    struct que_create_out_msg out;
    struct que_event_msg event_msg = {.in = (char *)&in, .in_len = sizeof(struct que_create_in_msg),
        .out = (char *)&out, .out_len = sizeof(struct que_create_out_msg)};
    int ret;

    ret = memcpy_s((void *)&in.que_attr, sizeof(QueueAttr), (void *)que_attr, sizeof(QueueAttr));
    if (que_unlikely(ret != EOK)) {
        return DRV_ERROR_MEMORY_OPT_FAIL;
    }

    ret = que_event_send_ex(devid, QUE_SEND_NORMAL, DRV_SUBEVENT_CREATE_MSG, &event_msg, QUE_EVENT_MAX_WAIT_10S);
    if (que_unlikely(ret != DRV_ERROR_NONE)) {
        QUEUE_LOG_ERR("que create event send fail. (ret=%d; devid=%u)\n", ret, devid);
        return ret;
    }

    *qid = out.qid;
    *create_time = out.create_time;
    return DRV_ERROR_NONE;
}

static int que_clt_destroy_send(unsigned int devid, unsigned int qid)
{
    struct que_destoy_in_msg in = {.qid = qid};
    struct que_event_msg event_msg = {.in = (char *)&in, .in_len = sizeof(struct que_destoy_in_msg),
            .out = NULL, .out_len = 0};
    int ret;

    ret = que_event_send_ex(devid, QUE_SEND_NORMAL, DRV_SUBEVENT_DESTROY_MSG, &event_msg, QUE_EVENT_MAX_WAIT_10S);
    if (que_unlikely(ret != DRV_ERROR_NONE)) {
        QUEUE_LOG_ERR("event send fail. (ret=%d; devid=%u; qid=%u\n", ret, devid, qid);
        return ret;
    }

    return DRV_ERROR_NONE;
}


static void queue_sub_init(unsigned int dev_id, unsigned int qid)
{
    struct que_ctx* ctx = NULL;
    struct queue_sub_flag *subflag = NULL;

    ctx = que_ctx_get(dev_id);
    if (que_unlikely(ctx == NULL)) {
        QUEUE_LOG_ERR("que ctx get fail. (devid=%u; qid=%u)\n", dev_id, qid);
        return;
    }

    subflag = ctx->sub_flag;
    if (que_unlikely(subflag == NULL)) {
        QUEUE_LOG_ERR("que subflag get fail. (devid=%u; qid=%u)\n", dev_id, qid);
        que_ctx_put(ctx);
        return;
    }

    subflag[qid].en_que = false;
    subflag[qid].f2nf = false;
    que_ctx_put(ctx);
    return;
}

static drvError_t que_clt_create(unsigned int dev_id, const QueueAttr *que_attr, unsigned int *qid)
{
    unsigned int qid_;
    unsigned long create_time;
    int ret;

    ret = que_clt_create_send(dev_id, que_attr, &qid_, &create_time);
    if (que_unlikely(ret != DRV_ERROR_NONE)) {
        QUEUE_LOG_ERR("que create event send fail. (ret=%d; devid=%u)\n", ret, dev_id);
        return ret;
    }

    if (qid_ >= MAX_SURPORT_QUEUE_NUM) {
        QUEUE_LOG_ERR("que qid is invalid. (qid=%u; devid=%u)\n", qid_, dev_id);
        return DRV_ERROR_INVALID_VALUE;
    }
    queue_sub_init(dev_id, qid_);

    ret = que_ctx_chan_create(dev_id, qid_, CHAN_CREATE, create_time, TRANS_D2H_H2D);
    if (que_unlikely(ret != DRV_ERROR_NONE)) {
        (void)que_clt_destroy_send(dev_id, qid_);
        return ret;
    }
    *qid = qid_;

    return DRV_ERROR_NONE;
}

static drvError_t que_clt_grant(unsigned int dev_id, unsigned int qid, int pid, QueueShareAttr attr)
{
    QueueGrantPara in = {.devid = dev_id, .qid = qid, .pid = pid, .attr = attr};
    struct que_event_msg event_msg = {.in = (char *)&in, .in_len = sizeof(QueueGrantPara),
            .out = NULL, .out_len = 0};
    int ret;

    ret = que_event_send_ex(dev_id, QUE_SEND_WITH_RETRY, DRV_SUBEVENT_GRANT_MSG, &event_msg, QUE_EVENT_MAX_WAIT_10S);
    if (que_unlikely(ret != DRV_ERROR_NONE)) {
        QUEUE_LOG_ERR("grant event send fail. (ret=%d; devid=%u; qid=%u; pid=%d)\n",
            ret, dev_id, qid, pid);
        return ret;
    }

    return DRV_ERROR_NONE;
}

static drvError_t que_clt_attach(unsigned int dev_id, unsigned int qid, int time_out)
{
    struct que_attach_in_msg in = {.qid = qid, .timeout = time_out};
    struct que_attach_out_msg out;
    struct que_event_msg event_msg = {.in = (char *)&in, .in_len = sizeof(struct que_attach_in_msg),
            .out = (char *)&out, .out_len = sizeof(struct que_attach_out_msg)};
    int ret;

    ret = que_event_send_ex(dev_id, QUE_SEND_NORMAL, DRV_SUBEVENT_ATTACH_MSG, &event_msg, QUE_EVENT_MAX_WAIT_10S);
    if (que_unlikely(ret != DRV_ERROR_NONE)) {
        QUEUE_LOG_ERR("attach event send fail. (ret=%d; devid=%u; qid=%u)\n", ret, dev_id, qid);
        return ret;
    }

    ret = que_ctx_chan_check(dev_id, qid, out.create_time);
    if (que_unlikely((ret != DRV_ERROR_NONE))) {
        if (ret == DRV_ERROR_QUEUE_REPEEATED_INIT) {
            return DRV_ERROR_NONE;
        }
        return ret;
    }

    ret = que_ctx_chan_create(dev_id, qid, CHAN_ATTACH, out.create_time, TRANS_D2H_H2D);
    if (que_unlikely(ret == DRV_ERROR_QUEUE_REPEEATED_INIT)) {
        return DRV_ERROR_NONE;
    }
    return ret;
}

static drvError_t que_clt_destroy(unsigned int dev_id, unsigned int qid)
{
    int ret;

    ret = que_clt_destroy_send(dev_id, qid);
    if (que_unlikely(ret != DRV_ERROR_NONE)) {
        QUEUE_LOG_ERR("destroy msg send fail. (ret=%d; devid=%u; qid=%u)\n", ret, dev_id, qid);
        return ret;
    }

    ret = que_ctx_chan_destroy(dev_id, qid);
    if (que_unlikely(ret != DRV_ERROR_NONE)) {
        QUEUE_LOG_ERR("que ctx queue destroy fail. (ret=%d; devid=%u; qid=%u)\n", ret, dev_id, qid);
        return ret;
    }

    return DRV_ERROR_NONE;
}

static drvError_t que_clt_reset(unsigned int dev_id,  unsigned int qid)
{
    struct que_reset_in_msg in = {.qid = qid};
    struct que_event_msg event_msg = {.in = (char *)&in, .in_len = sizeof(struct que_reset_in_msg),
            .out = NULL, .out_len = 0};
    int ret;

    ret = que_event_send_ex(dev_id, QUE_SEND_NORMAL, DRV_SUBEVENT_QUEUE_RESET_MSG, &event_msg, QUE_EVENT_MAX_WAIT_10S);
    if (que_unlikely(ret != DRV_ERROR_NONE)) {
        QUEUE_LOG_ERR("que reset event send fail. (ret=%d, devid=%u, qid=%u)\n", ret, dev_id, qid);
        return ret;
    }

    return DRV_ERROR_NONE;
}

static drvError_t que_clt_enque(unsigned int dev_id,  unsigned int qid, void *mbuf)
{
    return DRV_ERROR_NOT_SUPPORT;
}

static drvError_t que_clt_deque(unsigned int dev_id, unsigned int qid, void **mbuf)
{
    return DRV_ERROR_NOT_SUPPORT;
}

static drvError_t que_clt_subscribe(unsigned int dev_id, unsigned int qid, unsigned int group_id, int type)
{
    return DRV_ERROR_NOT_SUPPORT;
}

static drvError_t que_clt_unsubscribe(unsigned int dev_id, unsigned int qid)
{
    return DRV_ERROR_NOT_SUPPORT;
}

static drvError_t que_clt_subf2nf_event(unsigned int dev_id, unsigned int qid, unsigned int group_id)
{
    return DRV_ERROR_NOT_SUPPORT;
}

static drvError_t que_clt_unsubf2nf_event(unsigned int dev_id, unsigned int qid)
{
    return DRV_ERROR_NOT_SUPPORT;
}

static drvError_t que_clt_sub_event(struct QueueSubPara *sub_para)
{
    struct que_sub_event_in_msg in = {.qid = sub_para->qid, .grp_id = sub_para->groupId};
    struct que_event_msg event_msg = {.in = (char *)&in, .in_len = sizeof(struct que_sub_event_in_msg),
            .out = NULL, .out_len = 0};
    struct que_event_attr attr = {.devid = sub_para->devId};
    unsigned int dst_phy_dev_id = QUEUE_INVALID_VALUE;
    pid_t hostpid, devpid;
    int ret;

    ret = que_ctx_get_pids(sub_para->devId, &hostpid, &devpid);
    if (que_unlikely(ret != DRV_ERROR_NONE)) {
        return ret;
    }

    if ((sub_para->flag & QUEUE_SUB_FLAG_SPEC_DST_DEVID) != 0) {
        ret = uda_get_udevid_by_devid(sub_para->dstDevId, &dst_phy_dev_id);
        if (que_unlikely(ret != DRV_ERROR_NONE)) {
            QUEUE_LOG_ERR("get udevid failed. (ret=%d; devid=%u, qid=%u; hostpid=%d; devpid=%d)\n",
                ret, sub_para->devId, sub_para->qid, hostpid, devpid);
            return ret;
        }
    }

    in.pid = (int)hostpid;
    in.tid = ((sub_para->flag & QUEUE_SUB_FLAG_SPEC_THREAD) != 0) ? sub_para->threadId : QUEUE_INVALID_VALUE;
    in.event_id = (sub_para->eventType == QUEUE_F2NF_EVENT) ? EVENT_QUEUE_FULL_TO_NOT_FULL : EVENT_QUEUE_ENQUEUE;
    in.dst_phy_devid = dst_phy_dev_id;

    attr.sub_event = (sub_para->eventType == QUEUE_F2NF_EVENT) ? DRV_SUBEVENT_SUBF2NF_MSG : DRV_SUBEVENT_SUBE2NE_MSG;
    attr.hostpid = hostpid;
    attr.devpid = devpid;
    attr.retry_flg = QUE_SEND_NORMAL;
    ret = que_event_send(&attr, &event_msg, QUE_EVENT_MAX_WAIT_10S);
    if (que_unlikely(ret != DRV_ERROR_NONE)) {
        QUEUE_LOG_ERR("sub event send fail. (ret=%d; devid=%u; qid=%u; hostpid=%d; devpid=%d)\n",
            ret, sub_para->devId, in.qid, hostpid, devpid);
        return ret;
    }

    ret = que_sub_status_set(sub_para, sub_para->devId, sub_para->qid);
    if (que_unlikely(ret != DRV_ERROR_NONE)) {
        QUEUE_LOG_ERR("que sub status set fail. (ret=%d; devid=%u; qid=%u)\n",
            ret, sub_para->devId, in.qid);
        return ret;
    }

    (void)queue_register_callback(sub_para->groupId);
    QUEUE_LOG_INFO("sub event. (dev_id=%u; qid=%u; event_type=%d)\n", sub_para->devId, sub_para->qid, sub_para->eventType);
    return DRV_ERROR_NONE;
}

static drvError_t que_clt_unsub_event(struct QueueUnsubPara *unsub_para)
{
    struct que_unsub_event_in_msg in = {.qid = unsub_para->qid};
    struct que_event_msg event_msg = {.in = (char *)&in, .in_len = sizeof(struct que_unsub_event_in_msg),
            .out = NULL, .out_len = 0};
    unsigned int sub_event;
    pid_t hostpid, devpid;
    int ret;

    ret = que_ctx_get_pids(unsub_para->devId, &hostpid, &devpid);
    if (que_unlikely(ret != DRV_ERROR_NONE)) {
        return ret;
    }

    ret = que_clt_get_devpid(unsub_para->devId, &devpid);
    if (que_unlikely(ret != DRV_ERROR_NONE)) {
        return DRV_ERROR_INNER_ERR;
    }

    sub_event = (unsub_para->eventType == QUEUE_F2NF_EVENT) ? DRV_SUBEVENT_UNSUBF2NF_MSG : DRV_SUBEVENT_UNSUBE2NE_MSG;
    ret = que_event_send_ex(unsub_para->devId, QUE_SEND_NORMAL, sub_event, &event_msg, QUE_EVENT_MAX_WAIT_10S);
    if (que_unlikely(ret != DRV_ERROR_NONE)) {
        QUEUE_LOG_ERR("unsub event send fail. (ret=%d; devid=%u; qid=%u; sub_event=%u)\n",
            ret, unsub_para->devId, in.qid, sub_event);
        return ret;
    }

    ret = que_unsub_status_set(unsub_para, unsub_para->devId, unsub_para->qid);
    if (que_unlikely(ret != DRV_ERROR_NONE)) {
        QUEUE_LOG_ERR("que sub status set fail. (ret=%d; devid=%u; qid=%u)\n",
            ret, unsub_para->devId, in.qid);
        return ret;
    }

    QUEUE_LOG_INFO("unsub event. (dev_id=%u; qid=%u; event_type=%d)\n",
        unsub_para->devId, unsub_para->qid, unsub_para->eventType);
    return DRV_ERROR_NONE;
}

static drvError_t que_clt_ctrl_event(struct QueueSubscriber *subscriber, QUE_EVENT_CMD cmd_type)
{
    return DRV_ERROR_NOT_SUPPORT;
}

static drvError_t que_clt_query_info(unsigned int dev_id, unsigned int qid, QueueInfo *que_info)
{
    struct que_query_info_in_msg in = {.qid = qid};
    struct que_query_info_out_msg out;
    struct que_event_msg event_msg = {.in = (char *)&in, .in_len = sizeof(struct que_query_info_in_msg),
        .out = (char *)&out, .out_len = sizeof(struct que_query_info_out_msg)};
    int ret;

    ret = que_event_send_ex(dev_id, QUE_SEND_WITH_RETRY, DRV_SUBEVENT_QUERY_MSG, &event_msg, QUE_EVENT_MAX_WAIT_10S);
    if (que_unlikely(ret != DRV_ERROR_NONE)) {
        QUEUE_LOG_ERR("query info send fail. (ret=%d; devid=%u; qid=%u)\n", ret, dev_id, qid);
        return ret;
    }

    que_info->size = out.size;
    QUEUE_LOG_DEBUG("size=%d.\n", que_info->size);
    return DRV_ERROR_NONE;
}

static drvError_t que_clt_get_status(unsigned int dev_id, unsigned int qid, QUEUE_QUERY_ITEM query_item,
    unsigned int len, void *data)
{
    struct que_get_status_in_msg in = {.qid = qid, .query_item = query_item, .out_len = len};
    struct que_get_status_out_msg out;
    struct que_event_msg event_msg = {.in = (char *)&in, .in_len = sizeof(struct que_get_status_in_msg),
        .out = (char *)&out, .out_len = sizeof(struct que_get_status_out_msg)};
    int ret;

    ret = que_event_send_ex(dev_id, QUE_SEND_NORMAL, DRV_SUBEVENT_GET_QUEUE_STATUS_MSG, &event_msg, QUE_EVENT_TIMEOUT_MS);
    if (que_unlikely(ret != DRV_ERROR_NONE)) {
        QUEUE_LOG_ERR("get que status event send fail. (ret=%d; devid=%u; qid=%u)\n", ret, dev_id, qid);
        return ret;
    }

    ret = memcpy_s(data, len, out.data, len);
    if (que_unlikely(ret != EOK)) {
        QUEUE_LOG_ERR("get que status copy fail. (ret=%d; dev_id=%u; qid=%u; len=%u)\n", ret, dev_id, qid, len);
        return DRV_ERROR_INVALID_VALUE;
    }

    return DRV_ERROR_NONE;
}

static drvError_t que_clt_get_qid_by_name(unsigned int dev_id, const char *name, unsigned int *qid)
{
    return DRV_ERROR_NOT_SUPPORT;
}

static drvError_t que_clt_get_qid_by_pid(unsigned int dev_id, unsigned int pid,
    unsigned int max_que_size, QidsOfPid *info)
{
    return DRV_ERROR_NOT_SUPPORT;
}

static drvError_t que_clt_query_max_iovec_num(unsigned int dev_id, QueueQueryInputPara *in_put,
    QueueQueryOutputPara *out_put)
{
    QueueQueryOutput *out_buff = (QueueQueryOutput *)(out_put->outBuff);

    if (que_unlikely(out_put->outLen < sizeof(QueQueryMaxIovecNum))) {
        QUEUE_LOG_ERR("Input para error. (out_len=%u)\n", out_put->outLen);
        return DRV_ERROR_INVALID_VALUE;
    }

    out_buff->queQueryMaxIovecNum.count = QUEUE_MAX_IOVEC_NUM;

    return DRV_ERROR_NONE;
}

static drvError_t (*g_queue_query[QUEUE_QUERY_CMD_MAX])
    (unsigned int dev_id, QueueQueryInputPara *in_put, QueueQueryOutputPara *out_put) = {
        [QUEUE_QUERY_MAX_IOVEC_NUM] = que_clt_query_max_iovec_num,
};

static drvError_t que_clt_query(unsigned int dev_id, QueueQueryCmdType cmd,
    QueueQueryInputPara *in_put, QueueQueryOutputPara *out_put)
{
    if (g_queue_query[cmd] == NULL) {
        return DRV_ERROR_NOT_SUPPORT;
    }

    return g_queue_query[cmd](dev_id, in_put, out_put);
}

static drvError_t que_clt_peek_data(unsigned int dev_id, unsigned int qid, unsigned int flag, QueuePeekDataType type,
    void **mbuf)
{
    return DRV_ERROR_NOT_SUPPORT;
}

static drvError_t que_clt_set(unsigned int dev_id, QueueSetCmdType cmd, QueueSetInputPara *input)
{
    return DRV_ERROR_NOT_SUPPORT;
}

static void que_clt_finish_cb(unsigned int dev_id, unsigned int qid, unsigned int grp_id, unsigned int event_id)
{
    struct que_finish_cb_in_msg in = {.qid = qid, .grp_id = grp_id, .event_id = event_id};
    struct que_event_msg event_msg = {.in = (char *)&in, .in_len = sizeof(struct que_finish_cb_in_msg),
        .out = NULL, .out_len = 0};
    int ret;

    ret = que_event_send_ex(dev_id, QUE_SEND_NORMAL, DRV_SUBEVENT_FINISH_CALLBACK_MSG, &event_msg, QUE_EVENT_TIMEOUT_MS);
    if (que_unlikely(ret != DRV_ERROR_NONE)) {
        QUEUE_LOG_ERR("finish cb send fail. (ret=%d; devid=%u; qid=%u; grp_id=%u)\n", ret, dev_id, qid, grp_id);
    }
}

static drvError_t que_clt_api_peek(unsigned int dev_id, unsigned int qid, uint64_t *buf_len, int timeout)
{
    struct que_peek_in_msg in = {.qid = qid};
    struct que_peek_out_msg out;
    struct que_event_msg event_msg = {.in = (char *)&in, .in_len = sizeof(struct que_peek_in_msg),
            .out = (char *)&out, .out_len = sizeof(struct que_peek_out_msg)};
    int ret;

    ret = que_clt_send_event_with_wait(dev_id, qid, DRV_SUBEVENT_PEEK_MSG, &event_msg, timeout);
    if (que_unlikely(ret == DRV_ERROR_NONE)) {
        *buf_len = (uint64_t)out.buf_len;
        QUEUE_LOG_DEBUG("peek mbuf len. (buf_len=%llu; devid=%u; qid=%u)\n", out.buf_len, dev_id, in.qid);
        return ret;
    }

    return ret;
}

static drvError_t que_clt_api_enque(unsigned int dev_id, unsigned int qid, struct buff_iovec *vector, int timeout)
{
    int ret;

    ret = que_ctx_enque(dev_id, qid, vector, timeout);
    if (que_unlikely(ret != DRV_ERROR_NONE)) {
        return ret;
    }

    return DRV_ERROR_NONE;
}

static drvError_t que_clt_api_deque(unsigned int dev_id, unsigned int qid, struct buff_iovec *vector, int timeout)
{
    int ret;

    ret = que_ctx_deque(dev_id, qid, vector, timeout);
    if (que_unlikely(ret != DRV_ERROR_NONE)) {
        return ret;
    }

    return DRV_ERROR_NONE;
}

static drvError_t que_clt_api_subscribe(unsigned int dev_id, unsigned int qid,
    unsigned int subevent_id, struct event_res *res)
{
    struct que_sub_event_in_msg in = {.qid = qid, .grp_id = res->gid, .tid = QUEUE_INVALID_VALUE,
        .event_id = res->event_id, .dst_phy_devid = QUEUE_INVALID_VALUE};
    struct que_event_msg event_msg = {.in = (char *)&in, .in_len = sizeof(struct que_sub_event_in_msg),
            .out = NULL, .out_len = 0};
    struct que_event_attr attr = {.devid = dev_id, .sub_event = subevent_id};
    pid_t hostpid, devpid;
    int ret;

    ret = que_ctx_get_pids(dev_id, &hostpid, &devpid);
    if (que_unlikely(ret != DRV_ERROR_NONE)) {
        QUEUE_LOG_ERR("que ctx get pids fail. (ret=%d; devid=%u; qid=%u)\n", ret, dev_id, qid);
        return ret;
    }

    in.pid = (int)hostpid;
    in.inner_sub_flag = QUEUE_INNER_SUB_FLAG;

    attr.hostpid = hostpid;
    attr.devpid = devpid;
    attr.retry_flg = QUE_SEND_NORMAL;

    ret = que_event_send(&attr, &event_msg, QUE_EVENT_MAX_WAIT_10S);
    if (que_unlikely(ret != DRV_ERROR_NONE)) {
        QUEUE_LOG_ERR("que sub event send fail. (ret=%d; devid=%u; qid=%u; hostpid=%d; devpid=%d)\n",
            ret, dev_id, in.qid, hostpid, devpid);
        return ret;
    }

    return DRV_ERROR_NONE;
}

static drvError_t que_clt_api_unsubscribe(unsigned int dev_id, unsigned int qid, unsigned int subevent_id)
{
    struct que_unsub_event_in_msg in = {.qid = qid};
    struct que_event_msg event_msg = {.in = (char *)&in, .in_len = sizeof(struct que_unsub_event_in_msg),
            .out = NULL, .out_len = 0};
    int ret;

    ret = que_event_send_ex(dev_id, QUE_SEND_NORMAL, subevent_id, &event_msg, QUE_EVENT_MAX_WAIT_10S);
    if (que_unlikely(ret != DRV_ERROR_NONE)) {
        QUEUE_LOG_ERR("que unsub event send fail. (ret=%d; devid=%u; qid=%u)\n", ret, dev_id, qid);
        return ret;
    }

    return DRV_ERROR_NONE;
}

drvError_t que_clt_query_que_alive(unsigned int dev_id, struct que_query_alive_msg *qid_list)
{
    struct que_event_msg event_msg = {.in = (char *)qid_list, .in_len = sizeof(struct que_query_alive_msg),
        .out = (char *)qid_list, .out_len =sizeof(struct que_query_alive_msg)};
    int ret;

    ret = que_event_send_ex(dev_id, QUE_SEND_NORMAL, DRV_SUBEVENT_QUEUE_ALIVE_MSG, &event_msg, QUE_EVENT_TIMEOUT_MS);
    if (que_unlikely(ret != DRV_ERROR_NONE)) {
        QUEUE_LOG_ERR("recycle query not success. (ret=%d; devid=%u)\n", ret, dev_id);
        return ret;
    }
    return DRV_ERROR_NONE;
}

static struct queue_comm_interface_list g_que_clt_ub_intf = {
    .queue_dc_init = que_clt_init,
    .queue_uninit = que_clt_uninit,
    .queue_create = que_clt_create,
    .queue_grant = que_clt_grant,
    .queue_attach = que_clt_attach,
    .queue_destroy = que_clt_destroy,
    .queue_reset = que_clt_reset,
    .queue_en_queue = que_clt_enque,
    .queue_de_queue = que_clt_deque,
    .queue_subscribe = que_clt_subscribe,
    .queue_unsubscribe = que_clt_unsubscribe,
    .queue_sub_f_to_nf_event = que_clt_subf2nf_event,
    .queue_unsub_f_to_nf_event = que_clt_unsubf2nf_event,
    .queue_sub_event = que_clt_sub_event,
    .queue_unsub_event = que_clt_unsub_event,
    .queue_ctrl_event = que_clt_ctrl_event,
    .queue_query_info = que_clt_query_info,
    .queue_get_status = que_clt_get_status,
    .queue_get_qid_by_name = que_clt_get_qid_by_name,
    .queue_get_qids_by_pid = que_clt_get_qid_by_pid,
    .queue_query = que_clt_query,
    .queue_peek_data = que_clt_peek_data,
    .queue_set = que_clt_set,
    .queue_finish_cb = que_clt_finish_cb,
    .queue_export = NULL,
    .queue_unexport = NULL,
    .queue_import = NULL,
    .queue_unimport = NULL,
};

static struct que_clt_api g_clt_ub_api = {
    .api_peek = que_clt_api_peek,
    .api_enque_buf = que_clt_api_enque,
    .api_deque_buf = que_clt_api_deque,
    .api_subscribe = que_clt_api_subscribe,
    .api_unsubscribe = que_clt_api_unsubscribe
};

struct que_clt_api *que_clt_ub_get_api(void)
{
    queue_set_comm_interface(CLIENT_QUEUE, &g_que_clt_ub_intf);
    return &g_clt_ub_api;
}

static void que_inter_dev_event_init(void)
{
    enum drv_subevent_id subevent = DRV_SUBEVENT_QUEUE_COMM_MSG_START;

    for (; subevent < DRV_SUBEVENT_QUEUE_COMM_MSG_BUTT; subevent++) {
        drv_registert_event_proc(subevent, que_get_comm_subevent_proc(subevent));
    }

    QUEUE_LOG_DEBUG("que inter dev init.\n");
}

void que_clt_ub_var_init(void)
{
    que_init_grpid();
    que_inter_dev_event_init();
}

static int __attribute__((constructor)) que_clt_res_init(void)
{
    struct que_agent_interface_list *list = que_get_agent_interface();
    list->que_ub_res_init = que_clt_ub_res_init;
    list->que_ub_res_uninit = que_clt_ub_res_uninit;
    list->que_mem_alloc = que_clt_mem_alloc;
    list->que_mem_free = que_clt_mem_free;
    return DRV_ERROR_NONE;
}
#else /* EMU_ST */

void que_clt_ub_emu_test(void)
{
}

#endif /* EMU_ST */
