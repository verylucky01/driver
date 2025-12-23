/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef HDC_UB_DRV_H
#define HDC_UB_DRV_H

#include <semaphore.h>

#include "hdc_cmn.h"
#include "hdcdrv_cmd_ioctl.h"
#include "esched_user_interface.h"

#define HDC_MODULE_NAME "HISI_HDC"
#define HDC_MSG_GRP_NAME "hdc_msg_grp"
#define HDC_SESSION_STATUS_IDLE 0
#define HDC_SESSION_STATUS_CONN 1
#define HDC_SESSION_STATUS_REMOTE_CLOSE 2

#define HDCDRV_STR_NAME_LEN     32

#define HDC_EVENT_TID_ACCEPT 0
#define HDC_TID_MAX_NUM 1024

#define HDCDRV_INVALID_PID 0x7FFFFFFEULL

#define HDC_SCHED_WAIT_TIMEOUT_ALWAYS_WAIT (-1)

#define HDC_OWN_GROUP_FLAG 0        // hdc thread flag
#define HDC_DRV_GROUP_FLAG 1        // Drv thread flag

struct hdc_query_info {
    int pid;            // input: The process id for query
    int query_type;     // input: query local_gid or remote_gid
    int grp_type_flag;  // input: query for drv_thread or hdc_thread
};

#if defined(CFG_PLATFORM_ESL) || defined(CFG_PLATFORM_FPGA)
#define HDC_CONN_WAIT_TIMEOUT 100000                   // 100s
#define HDC_SET_SESSION_OWNER_WAIT_TIMEOUT 600000   // 600s
#define HDC_SESSION_CLOSE_WAIT_TIMEOUT 15000        // 15s
#define HDC_TIMEOUT_CQE_STATUS URMA_CR_REM_ACCESS_ABORT_ERR
#else
#define HDC_CONN_WAIT_TIMEOUT 30000                    // 30s
#define HDC_SET_SESSION_OWNER_WAIT_TIMEOUT 60000    // 60s
#define HDC_SESSION_CLOSE_WAIT_TIMEOUT 5000         // 5s
#define HDC_TIMEOUT_CQE_STATUS URMA_CR_RNR_RETRY_CNT_EXC_ERR
#endif

#define HDC_UB_VALID 1
#define HDC_UB_INVALID 0

#define HDC_TIMEOUT_RETRY (-1)
#define HDC_NON_BLOCK_RETRY (-2)
#define HDC_TIMEOUT_REBUILD (-3)
#define HDC_RETRY_MAX_TIME 5

#ifdef CFG_FEATURE_SUPPORT_UB
#define SESSION_REMOTE_CLOSED_TIME_US (3 * CONVERT_S_TO_US)   // 3s
#include "urma_api.h"
#include "urma_log.h"

typedef struct hdc_urma_info {
    urma_context_t *urma_ctx;
    urma_device_attr_t dev_attr;
    urma_token_id_t *token_id;
    int cnt;
} hdc_urma_info_t;

typedef struct hdc_urma_jfc {
    urma_jfce_t *jfce;
    urma_jfc_t *jfc;
} hdc_urma_jfc_t;

#define HDC_URMA_MAX_JFR_DEPTH 16
#define HDC_URMA_MAX_JFS_DEPTH 16
typedef struct hdc_urma_jfr {
    urma_jfr_t *jfr;
    uint64_t seg_len;
} hdc_urma_jfr_t;

#define HDC_MEM_BLOCK_SIZE 4096
// If the value of this macro needs to be changed, the macro with the same name in kernel mode also needs to be changed.
#define HDCDRV_UB_MEM_POOL_LEN (4 * 1024 * 16 * 2) // 4K(block_size) * 16(block_num) * 2(pool num)
typedef struct hdc_urma_jfs {
    urma_jfs_t *jfs;                                        // use urma_post_jfs_wr to send
    int jfs_tseg_flag[HDC_URMA_MAX_JFS_DEPTH];
    sem_t tseg_sema;
} hdc_urma_jfs_t;

struct hdc_ub_rx_buf {
    urma_target_seg_t *tseg;
    uint64_t addr;
    uint32_t len;
    uint64_t idx;   // record mem idx in tseg; 0-15 used for jfs, 16-31 used for jfr
    struct hdc_time_record_for_single_recv recv_record;
};

typedef struct hdc_ub_context {
    urma_context_t *urma_ctx;

    hdc_urma_jfc_t jfc_send;
    hdc_urma_jfc_t jfc_recv;

    hdc_urma_jfs_t jfs;

    // local JFRs
    hdc_urma_jfr_t jfr;

    // Remote JFRs
    urma_target_jetty_t *tjfr; // use urma_import_jfr to get info,  The input parameter needs remote EID jfr_id uasid

    urma_target_seg_t *tseg;    // used for urma_register_seg

    urma_token_t token;
    urma_token_id_t *token_id;
    uint32_t uasid;
    uint32_t devid;
    uint32_t session_fd;
    mmMutex_t mutex_jfs;
    mmMutex_t mutex_jfr;
} hdc_ub_context_t;

// Used to exchange information between two sides.
typedef struct hdc_jfr_id_info {
    urma_eid_t eid;
    uint32_t uasid;
    /* Send local two JFR info to remote */
    uint32_t jfr_id;
    urma_token_t token_val;
} hdc_jfr_id_info_t;

struct hdc_remote_close_thread_para {
    unsigned int dev_id;
    unsigned int local_session;
    unsigned int remote_session;
    int session_close_state;
    unsigned int unique_val;
};

#define HDC_RX_LIST_LEN (HDC_URMA_MAX_JFR_DEPTH + 1)
struct hdc_ub_epoll_node {
    pthread_cond_t cond;
    pthread_mutex_t mutex;
    int ref;        // record how many ref for node, if ref is not equal to 0, this node can not be free
    int pkt_num;    // num of cqe waiting for recv
    struct hdc_ub_rx_buf rx_list[HDC_RX_LIST_LEN];
    hdc_ub_context_t *ctx;
    uint32_t head;
    uint32_t tail;
    void *session;
};

static inline void hdc_pack_jetty_info(hdc_ub_context_t *ctx, hdc_jfr_id_info_t *info)
{
    info->eid = ctx->urma_ctx->eid;
    info->uasid = ctx->urma_ctx->uasid;
    info->jfr_id = ctx->jfr.jfr->jfr_id.id;
    info->token_val = ctx->token;
}

static inline bool hdc_use_kernel_pool(int service_type)
{
    if (service_type == HDC_SERVICE_TYPE_DMP) {
        return true;
    }

    return false;
}

static inline bool hdc_rx_list_is_full(struct hdc_ub_epoll_node *node)
{
    if ((node->tail + 1) % HDC_RX_LIST_LEN == node->head) {
        return true;
    }

    return false;
}

static inline bool hdc_rx_list_is_empty(struct hdc_ub_epoll_node *node)
{
    if (node->tail == node->head) {
        return true;
    }

    return false;
}

static inline bool hdc_service_type_vaild(int service_type)
{
    if ((service_type >= HDC_SERVICE_TYPE_MAX) || (service_type < 0)) {
        return false;
    }

    return true;
}

struct hdcdrv_sync_event_msg {
    struct event_sync_msg head;
    struct hdcdrv_event_msg data;
};

#define HDC_UB_TX 0
#define HDC_UB_RX 1

enum drv_hdc_ub_op {
    HDC_URMA_WAIT_FAIL = 0,                     // fail by urma_wait_jfc
    HDC_URMA_POLL_FAIL = 1,                     // fail by urma_poll_jfc
    HDC_URMA_POLL_FAIL_BY_REM_ACESS_ABORT = 2,  // fail by urma_poll_jfc, with URMA_CR_REM_ACCESS_ABORT_ERR
    HDC_URMA_REARM_FAIL = 3,                    // faile by urma_rearm_jfc
    HDC_URMA_OP_FAIL_MAX
};

struct hdc_mem_res_info {
    mmProcess bind_fd;
    int service_type;
    unsigned long long user_va;
};

struct hdc_event_wait_info {
    unsigned int grp_id;
    unsigned int tid;
};

int hdc_fill_event_for_drv_grp(struct event_summary *event_submit, struct hdcdrv_sync_event_msg *msg,
    struct hdc_ub_session *session, enum hdcdrv_notify_type notify_type, DRV_SUBEVENT_ID subevent_id);

void hdc_fill_event_for_own_grp(struct event_summary *event_submit, struct hdcdrv_sync_event_msg *msg,
    enum hdcdrv_notify_type notify_type, DRV_SUBEVENT_ID subevent_id);

void hdc_ub_fill_dfx_info(hdc_ub_dbg_stat_t *dbg_stat, struct hdc_ub_session *session);
void hdc_jfc_dbg_fill(int tx_rx_flag, struct hdc_ub_session *session, enum drv_hdc_ub_op op);
void hdc_ub_init_dfx_info(hdc_ub_dbg_stat_t *dbg_stat);
int hdc_dfx_query_handle(int devId, struct hdc_ub_session *session, struct hdcdrv_event_dfx *dfx_msg);
unsigned int hdc_get_jfc_id_by_type(hdcdrv_jetty_info_t *jetty_info, int tx_rx_flag);

void hdc_touch_close_notify(int dev_id, unsigned long long peer_pid, int service_type);
void hdc_touch_connect_notify(int dev_id, unsigned long long peer_pid, int service_type);
void hdc_touch_data_in_notify(int dev_id, int service_type);
int hdc_session_alive_check(int devId, int l_id, unsigned int unique_val);
bool hdc_has_data_in_thread(int service_type);
void hdc_ub_notiy_init(struct hdcConfig *hdcConfig);
void hdc_ack_jfc(hdc_urma_jfc_t *hdc_jfc);
int hdc_rearm_jfc(hdc_urma_jfc_t *hdc_jfc, struct hdc_ub_session *session, int tx_rx_flag);

void hdc_get_time_record(struct timeval *time_val, int *fail_count);
unsigned long long hdc_get_time_cost(struct timeval *start_tval, struct timeval *end_tval);
void hdc_get_recv_time_cost(struct hdc_time_record_for_single_recv *recv_record, struct hdc_ub_session *session);
void hdc_get_send_time_cost(struct hdc_time_record_for_single_send *send_record, struct hdc_ub_session *session);
void hdc_get_accept_time_cost(struct hdc_time_record_for_accept *accept_record, struct hdc_ub_session *session);
int hdc_ub_wait_reply(struct event_info *event_back, struct hdcdrv_sync_event_msg *wait_event,
    struct hdc_ub_session *session, int msg_type, struct hdc_event_wait_info *wait_info);

void hdc_get_ub_res_info(hdc_ub_context_t *ctx, hdc_ub_res_info_t *ub_res_info);
void hdc_update_session_recv_record(struct hdc_ub_rx_buf *buf, struct hdc_time_record_for_single_recv *recv_record);

#define HDC_PERF_LOG_LIMIT_TIME 60000     /* 60s */
#define HDC_PERF_LOG_LIMIT_BRANCH_RATE 1 /* print 1 counts per 60s */
#define HDC_PERF_LOG_LIMIT_TIME_ENV "ASCEND_HDC_PERF_LOG_LIMIT"

#ifdef CFG_BUILD_DEBUG
#define HDC_PERF_LOG_RUN_INFO_LIMIT(format, ...)                                                          \
    do {                                                                                                  \
        static int __drv_log_cnt = 0;                                                                     \
        if (getenv(HDC_PERF_LOG_LIMIT_TIME_ENV) != NULL ||                                                \
            !drv_log_rate_limit(&__drv_log_cnt, HDC_PERF_LOG_LIMIT_BRANCH_RATE, HDC_PERF_LOG_LIMIT_TIME)) \
            DRV_RUN_INFO(HAL_MODULE_TYPE_HDC, format, ##__VA_ARGS__);                                     \
    } while (0)
#else
#define HDC_PERF_LOG_RUN_INFO_LIMIT(format, ...)                                                          \
    do {                                                                                                  \
        static int __drv_log_cnt = 0;                                                                     \
        if (!drv_log_rate_limit(&__drv_log_cnt, HDC_PERF_LOG_LIMIT_BRANCH_RATE, HDC_PERF_LOG_LIMIT_TIME)) \
            DRV_RUN_INFO(HAL_MODULE_TYPE_HDC, format, ##__VA_ARGS__);                                     \
    } while (0)
#endif

void hdc_recv_data_in_event_handle(struct hdc_ub_epoll_node *node);
int hdc_ub_epoll_thread_init(void);
void hdc_ub_epoll_thread_uninit(void);
int hdc_ub_add_ctl_to_thread_epoll(struct hdc_ub_session *session);
void hdc_ub_del_ctl_to_thread_epoll(struct hdc_ub_session *session);
int hdc_poll_jfc(hdc_urma_jfc_t *hdc_jfc, urma_cr_t *cr, struct hdc_ub_session *session, int tx_rx_flag,
    int service_type);
void hdc_ack_jfc(hdc_urma_jfc_t *hdc_jfc);
uint8_t hdcUbGetJfsPriorityByType(int service_type);
// used for EMU_ST
#ifdef EMU_ST
int hdc_mem_res_init_stub(int fd, unsigned long long *user_va, unsigned long size);
void hdc_mem_res_uninit_stub(int fd, unsigned long long *user_va);
#endif

#endif // CFG_FEATURE_SUPPORT_UB

signed int __attribute__((weak)) hdc_ub_get_session_attr(mmProcess handle,
    const struct hdc_session *pSession, int attr, int *value);
signed int __attribute__((weak)) hdc_ub_server_create(mmProcess handle, signed int devId, signed int serviceType,
    unsigned int *grpId, struct hdc_server_head *pHead);
hdcError_t __attribute__((weak)) hdc_ub_server_destroy(struct hdc_server_head *pServ,
    signed int devId, signed int serviceType);
signed int __attribute__((weak)) hdc_ub_client_destroy(mmProcess handle, signed int devId, signed int serviceType);
signed int __attribute__((weak)) hdc_ub_accept(struct hdc_server_head *pServ, signed int devId, signed int serviceType,
    struct hdc_session *pSession);
signed int __attribute__((weak)) hdc_ub_connect(signed int devId, struct hdc_client_head *pHead, signed int peerpid,
    struct hdc_session *pSession);
mmProcess __attribute__((weak)) hdc_ub_open(void);
void __attribute__((weak)) hdc_ub_close(mmProcess handle);
signed int __attribute__((weak)) hdc_ub_session_close(unsigned int devId, struct hdc_session *pSession, int close_state,
    int close_flag);
signed int __attribute__((weak)) hdc_ub_send(const struct hdc_session *pSession,
    struct drvHdcMsg *pMsg, signed int wait, unsigned int timeout);
signed int __attribute__((weak)) hdc_ub_recv_peek(const struct hdc_session *pSession, signed int *len,
    struct hdc_recv_config *recvConfig);
signed int __attribute__((weak)) hdc_ub_recv(const struct hdc_session *pSession,
    char *buf, signed int len, signed int *outLen, struct hdc_recv_config *recvConfig);
signed int __attribute__((weak)) hdc_ub_set_session_owner(const struct hdc_session *pSession);
signed int __attribute__((weak)) HdcUbGetSessionUid(mmProcess handle,
    const struct hdc_session *pSession, int *root_privilege);

void __attribute__((weak)) hdc_ub_init(struct hdcConfig *hdcConfig);
void __attribute__((weak)) hdc_ub_uninit(struct hdcConfig *hdcConfig);
drvError_t __attribute__((weak)) hdc_sync_event_proc(uint32_t devId, const void *msg, int msg_len,
    struct drv_event_proc_rsp *rsp);
drvError_t __attribute__((weak)) hdc_connect_event_proc(uint32_t devId, const void *msg, int msg_len,
    struct drv_event_proc_rsp *rsp);
drvError_t __attribute__((weak)) hdc_dfx_query_event_proc(uint32_t devId, const void *msg, int msg_len,
    struct drv_event_proc_rsp *rsp);
void __attribute__((weak)) hdc_release_remote_session(void);
signed int __attribute__((weak)) hdc_event_thread_init(unsigned int devId, bool isServer);
void __attribute__((weak)) hdc_event_thread_uninit(unsigned int devId);
signed int __attribute__((weak)) hdc_tid_pool_init(void);
signed int __attribute__((weak)) hdc_link_event_pre_init(unsigned int devId, bool isServer);
void __attribute__((weak)) hdc_link_event_pre_uninit(signed int devId, bool isServer);
const char* __attribute__((weak)) hdc_get_sevice_str(int service_type);
drvError_t __attribute__((weak)) hdc_remote_close_proc(void *msg, uint32_t devId);
int __attribute__((weak)) hdc_get_lock_index(int devId, int sessionId);
struct hdc_ub_session* __attribute__((weak)) hdc_find_session_in_list(unsigned int fd, int dev_id, uint32_t unique_val);
int __attribute__((weak)) hdc_ub_get_session_dfx(unsigned int devId, struct hdc_ub_session *session);
void __attribute__((weak)) hdc_fill_query_info(struct hdc_query_info *info, int pid, int query_type, int grp_type_flag);
int __attribute__((weak)) hdc_event_query_gid(unsigned int devId, unsigned int *grpId, struct hdc_query_info *info);
signed int __attribute__((weak)) hdc_alloc_tid(void);
signed int __attribute__((weak)) hdc_free_tid(int tid);
void __attribute__((weak)) hdc_ub_fill_jetty_info(hdcdrv_jetty_info_t *info, struct hdc_ub_session *session);
hdcError_t __attribute__((weak)) hdc_ub_notify_register(int service_type, struct HdcSessionNotify *notify);
void __attribute__((weak)) hdc_ub_notify_unregister(int service_type);
signed int __attribute__((weak)) hdc_ub_get_peer_devId(mmProcess handle, signed int devId, signed int *peerDevId);
signed int __attribute__((weak)) hdc_ub_ioctl(mmProcess handle, signed int ioctl_code, union hdcdrv_cmd *hdcCmd);
#endif
