/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef QUEUE_USER_MANAGE
#define QUEUE_USER_MANAGE

#include "ascend_hal.h"
#include "drv_user_common.h"
#include "atomic_lock.h"
#include "atomic_ref.h"
#ifdef CFG_FEATURE_QUE_SUPPORT_UB 
#include "urma_types.h"
#endif

#define MIN_VALID_QUEUE_DEPTH 2  /* min depth of queue */
#define MAX_QUEUE_DEPTH 8192

#if (defined CFG_PLATFORM_FPGA)
#define QUEUE_ALLOC_BUF_FLAG (XSMEM_BLK_NOT_AUTO_RECYCLE | BUFF_SP_HUGEPAGE_PRIOR)
#define MAX_SURPORT_QUEUE_NUM 128
#else
#define QUEUE_ALLOC_BUF_FLAG (XSMEM_BLK_NOT_AUTO_RECYCLE)
#define MAX_SURPORT_QUEUE_NUM 4096 /* kernel space should to be modified synchronously. */
#endif

#define MAX_SHARE_GRP  4  /* surport max sp-group in queue_manage, "zone" means same */

#ifndef MAX_STR_LEN
#define MAX_STR_LEN 128
#endif

#define INVALID_QUEUE_MERGE_IDX   (-1)
#define QUEUE_INVALID_VALUE 0xFFFFFFFFU

#define QUEUE_INNER_SUB_FLAG (1U)
#define QUEUE_INTER_SUB_FLAG (2U)

struct queue_merge_node {
    struct list_head list;
    unsigned int qid;
    int groupid;
};

struct event_stat {
    unsigned long long user_event_fail;         /* count of event callback times */
    unsigned long long user_event_succ;
    unsigned long long call_back;
};

struct group_merge {
    int idx;
    int pid;                        /* Process of subscribed to the queue */
    unsigned int groupid;           /* Group subscribed to the queue */
    volatile int atomic_flag;       /* Event merge flag */
    unsigned int ref_cnt;           /* Reference count, indicating how many queues a group is subscribed to */
    volatile int pause_flag;        /* ctrl a group queue do not publish event for a moment */
    unsigned int enque_event_ret;
    int rsv;
    struct event_stat event_stat;
    unsigned long long last_dequeue_time;
};

struct group_merge_list {
    struct list_head head;
    struct atomic_lock lock;
};

typedef struct {
    unsigned int blk_id;
    unsigned int rsv;
} queue_node_desp;

typedef struct {
    void *node;
    queue_node_desp node_desp;
} queue_entity_node;

struct queue_stats {
    unsigned long long enque_ok;               /* enqueue success */
    unsigned long long deque_num;              /* dequeue num */
    unsigned long long deque_ok;               /* dequeue success */
    unsigned long long enque_full;              /* enqueue failed */
    unsigned long long deque_empty;             /* dequeue failed */
    unsigned long long enque_event_ok;         /* enqueue event submit success */
    unsigned long long enque_event_fail;       /* enqueue event submit fail */
    unsigned long long f2nf_event_ok;          /* full to not full event submit success */
    unsigned long long f2nf_event_fail;        /* full to not full event submit fail */
    unsigned long long deque_drop;
    unsigned long long call_back;
    unsigned long enque_drop;
    unsigned long enque_none_subscrib;
    unsigned int enque_fail;
    unsigned int deque_fail;
    unsigned int deque_null;
    unsigned int enque_event_ret;
    struct timeval last_enque_time;
    struct timeval last_deque_time;
};

struct queue_permission {
    int pid;
    QueueShareAttr attr;
};

struct sub_info {
    unsigned int src_location; /* 0: device; 1: host */
    unsigned int src_udevid; /* set in msg when submit event */
    unsigned int dst_engine;
    int pid;
    unsigned int groupid;
    bool spec_thread;
    unsigned int tid;
    unsigned int eventid;
    unsigned int dst_devid; /* when sub event to local, this is local devid; else this is remote udevid */
    int sub_send;              /* Indicates whether to send events during subscription: 1 send, other not send */
    unsigned int inner_sub_flag;
};

struct atomic_queue_head_info {
    unsigned long long index : 32;
    unsigned long long head  : 32;
};
 
union atomic_queue_head {
    struct atomic_queue_head_info head_info;
    unsigned long long head_value;
};

struct queue_manages {
    unsigned int dev_id;
    unsigned int id;                     /* queue id */
    char name[MAX_STR_LEN];           /* queue's name */
    int creator_pid;
    int valid;                  /* Whether the queue is valid, 1 is valid, 0 is invalid */
    union atomic_queue_head queue_head;
    union atomic_status tail_status;
    int full_flag;
    int empty_flag;            /* value 1 indicates a failure to dequeue because the queue is empty */
    int event_flag;            /* Single queue sends enqueue event flags */
    int work_mode;             /* Queue work mode, push or pull, default push */
    int bind_type;             /* Supports group and single methods, send events as a group or a single queue */
    struct sub_info consumer;  /* Consumer event info */
    struct sub_info producer;  /* Producer event info */
    int enque_cas;
    int deque_cas;
    int fctl_flag;            /* flow control flag, */
    unsigned drop_time;
    int over_write;
    struct atomic_lock merge_atomic_lock;
    struct queue_stats stat;
    int merge_idx;
    int inter_dev_state;              /* 0: disabled; 1: exported; 2: imported; 3: unexported; 4: unimported */
    unsigned int remote_devid;        /* dev_id from inter dev */
    int remote_grpid;
    unsigned int remote_qid;          /* queue id from inter dev */
    int remote_devpid;                /* pid from inter dev mng proc */
    char share_queue_name[SHARE_QUEUE_NAME_MAX_LEN];     /* share queue's name */
#ifdef CFG_FEATURE_QUE_SUPPORT_UB 
    urma_jfr_id_t tjfr_id;
    unsigned int tjfr_valid_flag;  /* 0: invalid; 1: valid */
    urma_token_t token;
#endif
    unsigned int peer_deploy_flag; /* 0: local used; 1: remote used  */
};

struct queue_local_info {
    struct queue_manages *que_mng;
    queue_entity_node *entity;
    unsigned int depth;
    union atomic_status ref_status;
    bool detached;
};

struct que_chan_ctx_agent_list {
    void (*que_chan_cnt_info_print)(unsigned int, unsigned int);
    void (*que_ctx_cnt_info_print)(unsigned int);
};
void que_chan_cnt_info_print(unsigned int devid, unsigned int qid);
void que_ctx_cnt_info_print(unsigned int devid);
struct que_chan_ctx_agent_list *que_get_chan_ctx_agent(void);
drvError_t queue_dc_init(void);
drvError_t queue_create(unsigned int depth, unsigned int *qid, struct queue_local_info *local_info);
void queue_delete(unsigned int qid, void *que_entity);
drvError_t queue_attach(unsigned int qid, int pid, int timeout, struct queue_local_info *local_info);
drvError_t queue_merge_init(struct queue_manages *que_manage, unsigned int qid, int pid, unsigned int groupid);
drvError_t queue_merge_uninit(struct queue_manages *que_manage, unsigned int qid);
drvError_t queue_merge_ctrl(unsigned int dev_id, int pid, unsigned int groupid, QUE_EVENT_CMD cmd_type);
bool queue_is_init(void);
void *queue_get_que_addr(unsigned int qid);
void *queue_get_merge_addr(int qid);
void queue_set_local_info(unsigned int qid, struct queue_local_info *local_info);
void queue_clear_local_info(unsigned int qid);
struct queue_manages *queue_get_local_mng(unsigned int qid);
unsigned int queue_get_local_depth(unsigned int qid);
void queue_get_entity_and_depth(unsigned int qid, queue_entity_node **entity, unsigned int *depth);
bool queue_local_is_attached_and_set_detached(unsigned int qid);
bool queue_local_is_detached_and_se_attached(unsigned int qid);
bool queue_is_valid(unsigned int qid);
bool queue_get(unsigned int qid);
void queue_put(unsigned int qid);
void queue_finish_callback_local(unsigned int dev_id, unsigned int qid, unsigned int grp_id, unsigned int event_id);
void queue_set_local_depth(unsigned int qid, unsigned int depth);

static inline bool queue_is_merge_idx_valid(int merge_idx)
{
    return ((merge_idx >= 0) && (merge_idx < MAX_SURPORT_QUEUE_NUM));
}

static inline unsigned int queue_get_head(struct queue_manages *que_mng, unsigned int qid)
{
    return ((que_mng->queue_head.head_info.head) % queue_get_local_depth(qid));
}

static inline unsigned int queue_get_orig_tail(struct queue_manages *que_mng)
{
    return atomic_value_get(&que_mng->tail_status);
}

#endif
