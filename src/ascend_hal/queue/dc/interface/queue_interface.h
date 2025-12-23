/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef QUEUE_INTERFACE_H
#define QUEUE_INTERFACE_H

#include "ascend_hal.h"
#include "ascend_hal_define.h"
#include "esched_user_interface.h"

#define CLIENT_QID_OFFSET MAX_SURPORT_QUEUE_NUM

#define CLOSE_ALL_RES 0
#define CLOSE_USER_RES 1

typedef enum queue_deployment_type {
    CLIENT_QUEUE = CLIENT_QUEUE_DEPLOY,
    LOCAL_QUEUE = LOCAL_QUEUE_DEPLOY,
    DEPLOYMENT_EXTER_MAX_TYPE,
    INTER_DEV_QUEUE = DEPLOYMENT_EXTER_MAX_TYPE,
    INVALID_INTER_DEV_QUEUE,
    DEPLOYMENT_MAX_TYPE,
} QUEUE_DEPLOYMENT_TYPE;

enum que_inter_dev_state {
    QUEUE_STATE_DISABLED = 0,
    QUEUE_STATE_EXPORTED,
    QUEUE_STATE_IMPORTED,
    QUEUE_STATE_UNEXPORTED,
    QUEUE_STATE_UNIMPORTED,
    QUEUE_STATE_BUSY,
    QUEUE_STATE_BUTT,
};

/* QUEUE_DEPLOYMENT_TYPE common interface_list */
struct queue_comm_interface_list {
    drvError_t (*queue_dc_init)(unsigned int dev_id);
    void (*queue_uninit)(unsigned int dev_id, unsigned int scene);
    drvError_t (*queue_create)(unsigned int dev_id, const QueueAttr *que_attr, unsigned int *qid);
    drvError_t (*queue_grant)(unsigned int dev_id, unsigned int qid, int pid, QueueShareAttr attr);
    drvError_t (*queue_attach)(unsigned int dev_id, unsigned int qid, int time_out);
    drvError_t (*queue_destroy)(unsigned int dev_id, unsigned int qid);
    drvError_t (*queue_reset)(unsigned int dev_id, unsigned int qid);
    drvError_t (*queue_en_queue)(unsigned int dev_id, unsigned int qid, void *mbuf);
    drvError_t (*queue_de_queue)(unsigned int dev_id, unsigned int qid, void **mbuf);
    drvError_t (*queue_subscribe)(unsigned int dev_id, unsigned int qid, unsigned int group_id, int type);
    drvError_t (*queue_unsubscribe)(unsigned int dev_id, unsigned int qid);
    drvError_t (*queue_sub_f_to_nf_event)(unsigned int dev_id, unsigned int qid, unsigned int group_id);
    drvError_t (*queue_unsub_f_to_nf_event)(unsigned int dev_id, unsigned int qid);
    drvError_t (*queue_sub_event)(struct QueueSubPara *sub_para);
    drvError_t (*queue_unsub_event)(struct QueueUnsubPara *unsub_para);
    drvError_t (*queue_ctrl_event)(struct QueueSubscriber *subscriber, QUE_EVENT_CMD cmd_type);
    drvError_t (*queue_query_info)(unsigned int dev_id, unsigned int qid, QueueInfo *que_info);
    drvError_t (*queue_get_status)(unsigned int dev_id, unsigned int qid, QUEUE_QUERY_ITEM query_item,
        unsigned int len, void *data);
    drvError_t (*queue_get_qid_by_name)(unsigned int dev_id, const char *name, unsigned int *qid);
    drvError_t (*queue_get_qids_by_pid)(unsigned int dev_id, unsigned int pid, unsigned int max_que_size,
        QidsOfPid *info);
    drvError_t (*queue_query)(unsigned int dev_id, QueueQueryCmdType cmd, QueueQueryInputPara *in_put,
        QueueQueryOutputPara *out_put);
    drvError_t (*queue_peek_data)(unsigned int dev_id, unsigned int qid, unsigned int flag, QueuePeekDataType type,
        void **mbuf);
    drvError_t (*queue_set)(unsigned int dev_id, QueueSetCmdType cmd, QueueSetInputPara *input);
    void (*queue_finish_cb)(unsigned int dev_id, unsigned int qid, unsigned int grp_id, unsigned int event_id);
    drvError_t (*queue_export)(unsigned int dev_id, unsigned int qid, struct shareQueInfo *que_info);
    drvError_t (*queue_unexport)(unsigned int dev_id, unsigned int qid, struct shareQueInfo *que_info);
    drvError_t (*queue_import)(unsigned int dev_id, struct shareQueInfo *que_info, unsigned int* qid);
    drvError_t (*queue_unimport)(unsigned int dev_id, unsigned int qid, struct shareQueInfo *que_info);
};

static inline void que_get_time(struct timeval *tv) {
  struct timespec ts;
  (void)clock_gettime(CLOCK_MONOTONIC, &ts);
  tv->tv_sec  = ts.tv_sec;
  tv->tv_usec = ts.tv_nsec / NSEC_PER_USEC;
}

void queue_finish_callback(unsigned int devid, unsigned int grp_id, esched_event_info event_info);
void queue_set_comm_interface(QUEUE_DEPLOYMENT_TYPE type, struct queue_comm_interface_list *interface);
unsigned int queue_get_virtual_qid(unsigned int actual_qid, QUEUE_DEPLOYMENT_TYPE type);
unsigned int queue_get_actual_qid(unsigned int virtual_qid);
QUEUE_DEPLOYMENT_TYPE queue_get_deployment_type_by_qid(unsigned int qid);
int que_get_inter_dev_deploy_type(unsigned int dev_id, unsigned int qid, QUEUE_DEPLOYMENT_TYPE *type);
#ifdef CFG_FEATURE_QUE_SUPPORT_UB
int que_get_inter_dev_status(unsigned int dev_id, unsigned int qid, int *inter_dev_state, int *peer_deploy_flag, char *share_que_name);
#endif
int que_get_inter_dev_que_type(unsigned int dev_id, unsigned int qid, QUEUE_DEPLOYMENT_TYPE *type);
void clear_inter_dev_queue(unsigned int dev_id);
drvError_t queue_register_callback(unsigned int groupid);
unsigned int halGetHostDevid(void);
bool queue_inter_devid_invalid(unsigned int dev_id);
unsigned int que_get_unified_devid(unsigned int dev_id);
#endif
