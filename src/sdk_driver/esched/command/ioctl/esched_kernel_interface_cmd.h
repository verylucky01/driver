/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */

#ifndef ESCHED_KERNEL_INTERFACE_CMD_H
#define ESCHED_KERNEL_INTERFACE_CMD_H

#define SCHED_MAX_DEFAULT_GRP_NUM 32U
#define SCHED_MAX_EX_GRP_NUM  32
#define SCHED_MAX_GRP_NUM (SCHED_MAX_DEFAULT_GRP_NUM + SCHED_MAX_EX_GRP_NUM)
#define SCHED_MAX_SYNC_GRP_NUM               16U
#define SCHED_MAX_SYNC_THREAD_NUM_PER_GRP    16U
#ifdef STATIC_SKIP
#  define STATIC
#else
#  define STATIC    static
#endif
#define SCHED_SYNC_START_EVENT_ID            28U
#define SCHED_SYNC_MAX_SUB_EVENT_ID          2000U
#define SCHED_INVALID_TID 0xFFFFFFFFU
#define SCHED_INVALID_GID 0xFFFFFFFFU
#define SCHED_THREAD_SWAPOUT   (-2)
#define SCHED_WAIT_TIMEOUT_GIVEUP   SCHED_THREAD_SWAPOUT
#define SCHED_WAIT_TIMEOUT_SWAPOUT  SCHED_THREAD_SWAPOUT    /* GIVEUP and SWAPOUT is same */
#define SCHED_INVALID_DEVID 0xFFFFFFFFU
#ifdef CFG_ENV_HOST
#define SCHED_DEFAULT_THREAD_NUM_IN_GRP 256
#else
#define SCHED_DEFAULT_THREAD_NUM_IN_GRP 16
#endif
#define SCHED_WAIT_TIMEOUT_ALWAYS_WAIT (-1)

typedef enum {
    SCHED_TRACE_TIME_ENUQE_START,
    SCHED_TRACE_TIME_EVENT_SEND,
    SCHED_TRACE_TIME_EVENT_PUBLISH_USER,
    SCHED_TRACE_TIME_EVENT_PUBLISH_KERNEL,
    SCHED_TRACE_TIME_EVENT_PUBLISH_FINISH,
    SCHED_TRACE_TIME_THREAD_WAIT,
    SCHED_TRACE_TIME_PUBLISH_WAKEUP,
    SCHED_TRACE_TIME_THREAD_WAKED,
    SCHED_TRACE_TIME_ITEM_MAX
} SchedTraceTimeItem;

typedef struct {
    unsigned long time_stamp[SCHED_TRACE_TIME_ITEM_MAX];
} sched_trace_time_info;

struct sched_published_event_info {
    unsigned int dst_engine;
    unsigned int policy;
    int pid;
    unsigned int gid;
    unsigned int tid;
    unsigned int event_id;
    unsigned int subevent_id;
    unsigned int dst_devid;
    unsigned int msg_len;
    char *msg;
    unsigned int event_num;
    unsigned long long publish_timestamp;
    unsigned long long publish_timestamp_of_day;
    void *priv; /* Private event info for ack/finish. */
};

struct sched_subscribed_event {
    int pid;
    int host_pid;
    unsigned int gid;
    unsigned int event_id;
    unsigned int subevent_id;
    unsigned int msg_len; /* input msg buff size */
    char *msg;
    unsigned long long publish_user_timestamp;
    unsigned long long publish_kernel_timestamp;
    unsigned long long publish_finish_timestamp;
    unsigned long long publish_wakeup_timestamp;
    unsigned long long subscribe_timestamp;
};


struct sched_sync_event_trace {
    unsigned int gid;
    unsigned int event_id;
    unsigned int subevent_id;
    unsigned long long src_submit_user_timestamp;
    unsigned long long src_submit_kernel_timestamp;
    unsigned long long dst_publish_timestamp;
    unsigned long long dst_wait_start_timestamp;
    unsigned long long dst_wait_end_timestamp;
    unsigned long long dst_submit_user_timestamp;
    unsigned long long dst_submit_kernel_timestamp;
    unsigned long long src_publish_timestamp;
    unsigned long long src_wait_start_timestamp;
    unsigned long long src_wait_end_timestamp;
};

#endif