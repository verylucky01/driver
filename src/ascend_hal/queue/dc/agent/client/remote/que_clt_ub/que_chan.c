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

#include "securec.h"
#include "uref.h"
#include "queue.h"
#include "que_compiler.h"
#include "que_jetty.h"
#include "que_ini_proc.h"
#include "que_tgt_proc.h"
#include "que_comm_agent.h"
#include "que_comm_chan.h"
#include "que_chan.h"
#ifndef EMU_ST
static struct que_chan *g_chan[MAX_DEVICE][MAX_SURPORT_QUEUE_NUM] = {{NULL}};
static pthread_rwlock_t g_chan_lock = PTHREAD_RWLOCK_INITIALIZER;

static int _que_chan_add(struct que_chan *chan)
{
    unsigned int devid = chan->devid;
    unsigned int qid = chan->qid;

    if (que_unlikely(g_chan[devid][qid] != NULL)) {
        return DRV_ERROR_QUEUE_REPEEATED_INIT;
    }
    g_chan[devid][qid] = chan;
    return DRV_ERROR_NONE;
}

static int que_clt_chan_add(struct que_chan *chan)
{
    int ret;

    (void)pthread_rwlock_wrlock(&g_chan_lock);
    ret = _que_chan_add(chan);
    (void)pthread_rwlock_unlock(&g_chan_lock);

    if (que_unlikely(ret != DRV_ERROR_NONE)) {
        QUEUE_LOG_INFO("que chan repeat add. (ret=%d; devid=%u; qid=%u)\n", ret, chan->devid, chan->qid);
    }

    return ret;
}

static int _que_chan_del(struct que_chan *chan)
{
    unsigned int devid = chan->devid;
    unsigned int qid = chan->qid;

    if (que_unlikely((devid >= MAX_DEVICE) || (qid >= MAX_SURPORT_QUEUE_NUM))) {
        return DRV_ERROR_QUEUE_PARA_ERROR;
    }

    if (que_unlikely(g_chan[devid][qid] != chan)) {
        return DRV_ERROR_QUEUE_NOT_INIT;
    }
    g_chan[devid][qid] = NULL;

    return DRV_ERROR_NONE;
}

static int que_clt_chan_del(struct que_chan *chan)
{
    int ret;

    (void)pthread_rwlock_wrlock(&g_chan_lock);
    ret = _que_chan_del(chan);
    (void)pthread_rwlock_unlock(&g_chan_lock);

    if (que_unlikely(ret != DRV_ERROR_NONE)) {
        QUEUE_LOG_ERR("que chan del fail. (ret=%d; devid=%u; qid=%u)\n", ret, chan->devid, chan->qid);
    }

    return ret;
}

static void que_chan_release(struct uref *uref)
{
    struct que_chan *chan = container_of(uref, struct que_chan, ref);

    que_chan_uninit(chan);
    _que_chan_destroy(chan);
}

static struct que_chan *que_clt_chan_get(unsigned int devid, unsigned int qid)
{
    struct que_chan *chan = NULL;

    (void)pthread_rwlock_rdlock(&g_chan_lock);
    chan = g_chan[devid][qid];
    if (que_likely(chan != NULL)) {
        if (uref_get_unless_zero(&chan->ref) == 0) {
            (void)pthread_rwlock_unlock(&g_chan_lock);
            return NULL;
        }
    }
    (void)pthread_rwlock_unlock(&g_chan_lock);

    return chan;
}

static void que_clt_chan_put(struct que_chan *chan)
{
    uref_put(&chan->ref, que_chan_release);
}

void que_ini_timestamp_update(struct que_ini_proc *ini_proc, struct buff_iovec *vector, uint64_t *stamp)
{
    int ini_log_level = que_get_ini_log_level();
    unsigned int num = TRACE_INI_LEVLE0_BUTT + vector->count * (TRACE_INI_LEVLE1_BUTT - TRACE_INI_LEVLE1_START) * (unsigned int)ini_log_level;
    uint64_t *timestamp = calloc(num, sizeof(uint64_t));
    if (timestamp == NULL) {
        return;
    }
    timestamp[TRACE_INI_START] = stamp[TRACE_INI_START];
    timestamp[TRACE_UPDATE] = stamp[TRACE_UPDATE];
    ini_proc->total_iovec_num = vector->count;
    ini_proc->timestamp = timestamp;
}

int que_chan_pkt_send(unsigned int devid, unsigned int qid, QUEUE_AGENT_TYPE que_type, struct buff_iovec *vector, uint64_t *stamp)
{
    struct que_chan *chan = NULL;
    int ret;

    chan = que_chan_get(devid, qid);
    if (que_unlikely(chan == NULL)) {
        QUEUE_LOG_ERR("que chan get fail. (devid=%u; qid=%u)\n", devid, qid);
        return DRV_ERROR_QUEUE_NOT_CREATED;
    }
    que_ini_timestamp_update(chan->ini_proc[que_type], vector, stamp);

    ret = que_ini_pkt_send(chan->ini_proc[que_type], vector);
    if (ret == DRV_ERROR_NONE) {
        ATOMIC_INC((volatile int *)&chan->ini_proc[que_type]->cnt[INI_SEND_SUCCESS]);
    } else {
        ATOMIC_INC((volatile int *)&chan->ini_proc[que_type]->cnt[INI_SEND_FAIL]);
    }

    que_chan_put(chan);
    if (que_unlikely(ret != DRV_ERROR_NONE)) {
        QUEUE_LOG_ERR("que ini pkt send fail. (ret=%d; devid=%u; qid=%u)\n", ret, devid, qid);
    }
    return ret;
}

int que_chan_wait(unsigned int devid, unsigned int qid, QUEUE_AGENT_TYPE que_type, int timeout)
{
    struct que_chan *chan = NULL;
    int ret;

    chan = que_chan_get(devid, qid);
    if (que_unlikely(chan == NULL)) {
        QUEUE_LOG_ERR("que chan get fail. (devid=%u; qid=%u)\n", devid, qid);
        return DRV_ERROR_QUEUE_NOT_CREATED;
    }

    ret = que_ini_ack_wait(chan->ini_proc[que_type], timeout);
    if (ret == DRV_ERROR_NONE) {
        ATOMIC_INC((volatile int *)&chan->ini_proc[que_type]->cnt[INI_WAIT_SUCCESS]);
    } else {
        ATOMIC_INC((volatile int *)&chan->ini_proc[que_type]->cnt[INI_WAIT_FAIL]);
    }

    que_chan_put(chan);
    return ret;
}

int que_chan_ini_update(unsigned int devid, unsigned int qid, struct que_jfs_pool_info *jfs_info, struct que_jfr_pool_info *jfr_info,
    urma_target_jetty_t *tjetty, urma_token_t token, QUEUE_AGENT_TYPE que_type, unsigned int d2d_flag)
{
    struct que_chan *chan = que_chan_get(devid, qid);
    if (que_unlikely(chan == NULL)) {
        QUEUE_LOG_ERR("que chan is not init. (qid=%u)\n", qid);
        return DRV_ERROR_QUEUE_NOT_CREATED;
    }

    struct que_ini_proc *ini_proc = chan->ini_proc[que_type];
    ini_proc->devid = devid;
    ini_proc->qid = qid;
    ini_proc->que_type = que_type;
    ini_proc->d2d_flag = d2d_flag;

    ini_proc->pkt_send_jetty = jfs_info->qjfs;
    ini_proc->pkt_send_jetty->tjetty = tjetty;
    ini_proc->tx = NULL;

    ini_proc->imm_recv_jetty = jfr_info->qjfr;
    ini_proc->recv_para = jfr_info->recv_para;
    if (chan->token.token_id != NULL) {
        ini_proc->token_info.token = chan->token.token;
        ini_proc->token_info.token_id = chan->token.token_id;
    } else {
        ini_proc->token_info.token = token;
        ini_proc->token_info.token_id = NULL;
    }

    que_chan_put(chan);
    return DRV_ERROR_NONE;
}

static int __attribute__((constructor)) que_clt_chan_init(void)
{
    struct que_agent_interface_list *list = que_get_agent_interface();
    list->que_chan_get = que_clt_chan_get;
    list->que_chan_put = que_clt_chan_put;
    list->que_chan_add = que_clt_chan_add;
    list->que_chan_del = que_clt_chan_del;
    return DRV_ERROR_NONE;
}
#else   /* EMU_ST */

void que_clt_chan_emu_test(void)
{
}

#endif  /* EMU_ST */

