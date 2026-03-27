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
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sched.h>
#include <pthread.h>
#include <securec.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/sysinfo.h>
#include <ascend_hal.h>
#include "utils.h"

typedef struct _s_sched_info {
    int dev_id;
    int grp_id;
    int cpu_id;
    int qid;
    int enq_qid;
    int depth;
    unsigned long long bitmap;
    int wait;
    int end_flg;
    int ready;
    int thd_id;
} s_sched_info;

static int buff_init(char *grp_name)
{
    GroupCfg grpacfg = {0};
    grpacfg.maxMemSize = 1024 * 1024 * 32;
    grpacfg.privMbufFlag = BUFF_ENABLE_PRIVATE_MBUF;
    GroupShareAttr attr;
    char *name_str = "buff_test";
    BuffCfg buffcfg = {0};
    memZoneCfg memzone_cfg = {0};
    buffcfg.cfg[0] = memzone_cfg;
    buffcfg.cfg[1] = memzone_cfg;

    attr.admin = 1;
    attr.alloc = 1;
    attr.write = 1;
    attr.read = 1;
    attr.rsv = 1;

    CHECK_ERROR(halGrpCreate(name_str, &grpacfg));
    CHECK_ERROR(halGrpAddProc(name_str, getpid(), attr));
    CHECK_ERROR(halGrpAttach(name_str, 1000));
    CHECK_ERROR(halBuffInit(&buffcfg));

    return 0;
}

static int queue_init_group(unsigned int devid, char *grp)
{
    CHECK_ERROR(halQueueSet(devid, QUEUE_ENABLE_LOCAL_QUEUE, NULL));
    CHECK_ERROR(buff_init(grp));
    CHECK_ERROR(halQueueInit(devid));
    CHECK_ERROR(halEschedAttachDevice(devid));

    return 0;
}

static int queue_create(unsigned int devid, char *name, int depth, int workmode, int flag,
                 int ftime, unsigned int type, unsigned int *qid)
{
    unsigned int name_len = 0;
    QueueAttr queAttr = {0};
    name_len = strlen(name);
    name_len = (sizeof(queAttr.name) >= name_len) ? name_len : sizeof(queAttr.name);
    memcpy_s(queAttr.name,sizeof(queAttr.name), name, name_len);
    queAttr.depth = depth;
    queAttr.workMode = workmode;
    queAttr.flowCtrlFlag = flag;
    queAttr.overWriteFlag = 0;
    queAttr.flowCtrlDropTime = ftime;
    queAttr.deploy_type = LOCAL_QUEUE_DEPLOY;

    CHECK_ERROR(halQueueCreate(devid, &queAttr, qid));
    return 0;
}

int queue_dequeue(unsigned int devid, unsigned int qid, int num)
{
    int ret = -1;
    int j = 0;
    void *mbuf = NULL;

    for (j = 0; j < num; j++) {
        ret = halQueueDeQueue(devid, qid, &mbuf);
        if (ret == 0) {
            halMbufFree(mbuf);
        } else if (ret == DRV_ERROR_QUEUE_EMPTY) {
            LOG_INFO("Queue is Empty, deqCnt = %d.\n", j);
            return 0;
        } else {
            if (ret != DRV_ERROR_BUSY) {
                LOG_ERR("Queue(%d) Dequeue fail cnt = %d, ret = %d.\n", qid, j, ret);
            }
            break;
        }
    }
    return ret;
}

static void *wait_event_do_cleanqueue(s_sched_info *info)
{
    struct event_info event = {0};
    int ret;
    int tid = info->thd_id;

    ret = halEschedSubscribeEvent(info->dev_id, info->grp_id, tid, info->bitmap);
    LOG_INFO("HalEschedSubscribeEvent: dev(%d) grp(%d) tid(%d) bitmap=0x%llx ret=%d.\n", \
                    info->dev_id, info->grp_id, tid, info->bitmap, ret);

    halEschedWaitEvent(info->dev_id, info->grp_id, tid, 0, &event);
    info->ready = 1;
    while(info->end_flg == 0) {
        ret = halEschedWaitEvent(info->dev_id, info->grp_id, tid, info->wait, &event);
        if (ret == 0) {
            // 数据出队
            usleep(100); // 保证先入队完成
            ret = queue_dequeue(info->dev_id, info->qid, info->depth);
        }else if (ret == DRV_ERROR_SCHED_WAIT_TIMEOUT) {
            continue;
        } else {
            break;
        }
    }
    LOG_INFO("Tid %d finish end.\n", tid);
    return NULL;
}

// 数据入队
int queue_enqueue(unsigned int devid, unsigned int qid, int num)
{
    int ret = -1;
    int j = 0;
    Mbuf *mbuf = NULL;

    for (j = 0; j < num; j++) {
        CHECK_ERROR(halMbufAlloc(sizeof(void *), &mbuf));
        ret = halQueueEnQueue(devid, qid, mbuf);
        if (ret == 0){
            mbuf = NULL;
        } else {
            if(ret == DRV_ERROR_QUEUE_FULL) {
                LOG_INFO("Queue is Full, enqCnt = %d, ret = %d.\n", j, ret);
            } else {
                LOG_ERR("Enqueue failed: devid=%d, qid=%d, enque cnt = %d, ret=%d.\n", \
                   devid, qid, j, ret);
            }
            halMbufFree(mbuf);
            mbuf = NULL;
            break;
        }
    }

    return ret;
}

// 订阅队列
int queue_subscribe(unsigned int devid, unsigned int qid, unsigned int groupid, int type)
{
    CHECK_ERROR(halQueueSubscribe(devid, qid, groupid, type));
    return 0;
}

// 查询队列状态
int queue_query(unsigned int devid, unsigned int qid, QUEUE_QUERY_ITEM queryItem, unsigned int len,
                void *data)
{
    CHECK_ERROR(halQueueGetStatus(devid, qid, queryItem, len, data));
    return 0;
}

// 查询队列信息
int queue_show_info(unsigned int devid, unsigned int qid)
{
    int ret;
    QueueInfo que_Info = {0};
    QUEUE_DROP_PKT_STAT drop = {0};
    LOG_INFO("*****************************[%d]Show queue info ***************************\n", getpid());
    ret = halQueueQueryInfo(devid, qid, &que_Info);
    if (ret == 0) {
        LOG_INFO("Queue [%s]information:\n", que_Info.name);
        LOG_INFO("Queue info     : id=%d, size=%d, depth=%d, mode=%d, type=%d, status=%d.\n", \
               que_Info.id, que_Info.size, que_Info.depth, que_Info.workMode, que_Info.type, que_Info.status);
        LOG_INFO("Queue sub info : grp_id=%d, pid=%d, f2nf_grp=%d, f2nf_pid=%d f2nf_evtOk=%lld, f2nf_evtMiss=%lld.\n", \
               que_Info.subGroupId, que_Info.subPid, que_Info.subF2NFGroupId, que_Info.subF2NFPid, \
               que_Info.stat.f2nfEventOk, que_Info.stat.f2nfEventFail);
        LOG_INFO("Queue enq info : enqOkCnt=%lld, enqErrCnt:%lld, evtOK=%lld, evtMiss=%lld.\n", \
               que_Info.stat.enqueCnt, que_Info.stat.enqueFailCnt, que_Info.stat.enqueEventOk, que_Info.stat.enqueEventFail);
        LOG_INFO("Queue deq info : deqOkCnt=%lld, deqErrCnt:%lld.\n", que_Info.stat.dequeCnt, que_Info.stat.dequeFailCnt);
    } else {
        LOG_INFO("Query queue info faild: devid=%d, qid=%d, ret=%d.\n", devid, qid, ret);
    }
    ret = queue_query(devid, qid, QUERY_QUEUE_DROP_STAT, sizeof(QUEUE_DROP_PKT_STAT), (void *)&drop);
    if (ret == 0) {
        LOG_INFO("Queue drop info: eqDropCnt=%lld, deqDropCnt=%lld.\n", drop.enqueDropCnt, drop.dequeDropCnt);
    } else {
        LOG_INFO("Query queue drop faild: devid=%d, qid=%d, ret=%d.\n", devid, qid, ret);
    }
    LOG_INFO("**************************************************************************\n");
    return ret;
}

// 删除队列
int queue_destroy(unsigned int devid, unsigned int qid)
{
    CHECK_ERROR(halQueueDestroy(devid, qid));
    return 0;
}

int32_t main(int argc, char const *argv[])
{
    unsigned int qid = -1;
    int group_id = 0;
    int ret;
    char *name = "queue_test";
    int depth = 100;
    int devid = 0;
    s_sched_info info = {0};
    pthread_t pthd_id;
    QueueInfo que_Info = {0};
    int status = -1;

    CHECK_ERROR(queue_init_group(devid, name));
    CHECK_ERROR(queue_create(devid, name, depth, QUEUE_MODE_PUSH, 0, 1, 1, &qid));
    CHECK_ERROR(halEschedCreateGrp(devid, group_id, GRP_TYPE_BIND_CP_CPU));

    info.dev_id = devid;
    info.grp_id = group_id;
    info.qid = qid;
    info.depth = depth;
    info.bitmap = 1ULL << EVENT_QUEUE_ENQUEUE;
    info.wait = 2000;
    info.cpu_id = 0;

    CHECK_ERROR(pthread_create(&pthd_id, NULL, (void* (*)(void*))wait_event_do_cleanqueue, &info));
    sleep(1);

    // 数据入队
    CHECK_ERROR(queue_enqueue(devid, qid, depth - 1));
    CHECK_ERROR(queue_subscribe(devid, qid, group_id, QUEUE_TYPE_SINGLE));

    sleep(5);
    CHECK_ERROR(queue_query(devid, qid, QUERY_QUEUE_STATUS, sizeof(QUEUE_STATUS), &status));
    if (status != QUEUE_EMPTY) {
        LOG_ERR("Queue_query status is not empty, status = %d.\n", status);
        return -1;
    }

    // 查询队列信息
    queue_show_info(devid, qid);

    halQueueQueryInfo(devid, qid, &que_Info);
    if ((que_Info.stat.enqueEventOk != 1) || (que_Info.stat.dequeCnt != depth - 1)) {
        LOG_ERR("Queue info err, enqueEventOk=%llu, dequeCnt=%llu.\n", que_Info.stat.enqueEventOk, que_Info.stat.dequeCnt);
        return -1;
    }

    info.end_flg = 1;
    usleep(info.wait * 1000);
    pthread_join(pthd_id, NULL);
    CHECK_ERROR(halEschedDettachDevice(devid));
    CHECK_ERROR(queue_destroy(devid, qid));

    LOG_INFO("Testcase_success.\n");
    return 0;
}