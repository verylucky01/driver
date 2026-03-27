/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include <unistd.h>

#include "ascend_hal.h"
#include "esched_user_interface.h"
#include "hdc_cmn.h"
#include "hdc_epoll.h"
#include "hdc_ub_drv.h"

#define HDC_LOCAL_SIDE_PRINT 0
#define HDC_REMOTE_SIDE_PRINT 1

void hdc_ub_print_dfx_info(hdc_ub_dbg_stat_t *dbg_stat, hdcdrv_jetty_info_t *jetty_info,
    hdc_ub_send_recv_info_t *sr_info, int flag, struct hdc_ub_session *session)
{
    if (flag == HDC_LOCAL_SIDE_PRINT) {
        HDC_RUN_LOG_INFO("local side dfx info (l_id=%u; r_id=%u; service_type=\"%s\") "
            "jfc_recv=%u; jfc_send=%u; jfr_id=%u; jfs_id=%u; tx=%llu; tx_byte=%llu; tx_full=%llu; tx_fail_hdc=%llu; "
            "tx_fail_ub=%llu; rx=%llu; rx_byte=%llu; remote_rx_full=%llu; remote_rx_fail=%llu\n",
            session->local_id, session->remote_id, hdc_get_sevice_str(session->service_type),
            jetty_info->session_jfc_recv_id, jetty_info->session_jfc_send_id, jetty_info->session_jfr_id,
            jetty_info->session_jfs_id, dbg_stat->tx, dbg_stat->tx_bytes, dbg_stat->tx_full, dbg_stat->tx_fail_hdc,
            dbg_stat->tx_fail_ub, dbg_stat->rx, dbg_stat->rx_bytes, dbg_stat->remote_rx_full, dbg_stat->remote_rx_fail);
        HDC_RUN_LOG_INFO("local side recv_send_cost info (l_id=%u; r_id=%u; service_type=\"%s\") "
            "s_find_block:%llu; s_memcpy=%llu; s_post_jfs_wr=%llu; s_poll_jfc:%llu; s_ack_rearm_jfc=%llu; s_time=%llu; "
            "r_poll_jfc:%llu; r_ack_rearm_jfc=%llu; r_memcpy=%llu; r_post_jfr_wr=%llu; r_peek_time=%llu; r_time=%llu "
            "send_exceed_times=%llu; recv_exceed_times=%llu\n",
            session->local_id, session->remote_id, hdc_get_sevice_str(session->service_type),
            sr_info->timecost1, sr_info->timecost2, sr_info->timecost3, sr_info->timecost4, sr_info->timecost5,
            sr_info->timecost_send, sr_info->timecost6, sr_info->timecost7, sr_info->timecost8, sr_info->timecost9,
            sr_info->timecost_recv_peek, sr_info->timecost_recv, sr_info->timecost_exceed_cnt_send,
            sr_info->timecost_exceed_cnt_recv);
    } else {
        HDC_RUN_LOG_INFO("remote side dfx info (l_id=%u; r_id=%u; service_type=\"%s\") "
            "jfc_recv=%u; jfc_send=%u; jfr_id=%u; jfs_id=%u; tx=%llu; tx_byte=%llu; tx_full=%llu; tx_fail_hdc=%llu; "
            "tx_fail_ub=%llu; rx=%llu; rx_byte=%llu; remote_rx_full=%llu; remote_rx_fail=%llu\n",
            session->remote_id, session->local_id, hdc_get_sevice_str(session->service_type),
            jetty_info->session_jfc_recv_id, jetty_info->session_jfc_send_id, jetty_info->session_jfr_id,
            jetty_info->session_jfs_id, dbg_stat->tx, dbg_stat->tx_bytes, dbg_stat->tx_full, dbg_stat->tx_fail_hdc,
            dbg_stat->tx_fail_ub, dbg_stat->rx, dbg_stat->rx_bytes, dbg_stat->remote_rx_full, dbg_stat->remote_rx_fail);
        // RPINT INFO
        HDC_RUN_LOG_INFO("remote side dfx info (l_id=%u; r_id=%u; service_type=\"%s\") "
            "s_find_block:%llu; s_memcpy=%llu; s_post_jfs_wr=%llu; s_poll_jfc:%llu; s_ack_rearm_jfc=%llu; s_time=%llu; "
            "r_poll_jfc:%llu; r_ack_rearm_jfc=%llu; r_memcpy=%llu; r_post_jfr_wr=%llu; r_peek_time=%llu; r_time=%llu "
            "send_exceed_times=%llu; recv_exceed_times=%llu\n",
            session->remote_id, session->local_id, hdc_get_sevice_str(session->service_type),
            sr_info->timecost1, sr_info->timecost2, sr_info->timecost3, sr_info->timecost4, sr_info->timecost5,
            sr_info->timecost_send, sr_info->timecost6, sr_info->timecost7, sr_info->timecost8, sr_info->timecost9,
            sr_info->timecost_recv_peek, sr_info->timecost_recv, sr_info->timecost_exceed_cnt_send,
            sr_info->timecost_exceed_cnt_recv);
    }

    return;
}

STATIC int hdc_fill_msg_dfx(struct hdcdrv_sync_event_msg *msg, struct hdc_ub_session *session, struct hdc_query_info *info,
    unsigned int gid, struct event_summary *event_submit)
{
    int ret;
    int tid;

    ret = hdc_fill_event_for_drv_grp(event_submit, msg, session, HDCDRV_NOTIFY_MSG_DFX, DRV_SUBEVENT_HDC_CLOSE_LINK_MSG);
    if (ret != 0) {
        return ret;
    }

    tid = hdc_alloc_tid();
    if (tid < 0) {
        HDC_LOG_ERR("No enough tid for dfx, please check thread num.(dev_id=%d; session_id=%u; service_type=\"%s\")\n",
            session->dev_id, session->local_id, hdc_get_sevice_str(session->service_type));
        return DRV_ERROR_NO_RESOURCES;
    }

    msg->data.dfx_msg.l_session_id = session->local_id;
    msg->data.dfx_msg.r_session_id = session->remote_id;
    msg->data.dfx_msg.grp_id = gid;
    msg->data.dfx_msg.tid = (unsigned int)tid;
    msg->data.dfx_msg.para_info.unique_val = session->unique_val;
    msg->data.type = HDCDRV_NOTIFY_MSG_DFX;

    return DRV_ERROR_NONE;
}

STATIC void hdc_clear_msg_dfx(unsigned int tid)
{
    hdc_free_tid((int)tid);
}

int hdc_ub_get_session_dfx(unsigned int dev_id, struct hdc_ub_session *session)
{
    struct hdcdrv_sync_event_msg dfx_event = {0};
    struct event_summary event_submit = {0};
    struct event_info event_back = {0};
    unsigned int grp_id, tid;
    signed int ret;
    esched_event_buffer *event_buffer = (esched_event_buffer *)event_back.priv.msg;
    struct hdc_query_info info = {0};
    struct hdc_event_wait_info wait_info;

    hdc_ub_print_dfx_info(&session->dbg_stat, &session->jetty_info, &session->send_recv_info,
        HDC_LOCAL_SIDE_PRINT, session);

    ret = hdc_event_thread_init(dev_id, false);
    if (ret != DRV_ERROR_NONE) {
        HDC_LOG_ERR("hdc query and create local grp_id failed.(ret=%d; dev_id=%u)\n", ret, dev_id);
        return ret;
    }

    (void)hdc_fill_query_info(&info, getpid(), QUERY_TYPE_LOCAL_GRP_ID, HDC_OWN_GROUP_FLAG);
    ret = hdc_event_query_gid(dev_id, &grp_id, &info);
    if (ret != DRV_ERROR_NONE) {
        HDC_LOG_ERR("hdc query local grp_id failed.(ret=%d; dev_id=%u)\n", ret, dev_id);
        goto dfx_thread_uninit;
    }

    ret = hdc_fill_msg_dfx(&dfx_event, session, &info, grp_id, &event_submit);
    if (ret != DRV_ERROR_NONE) {
        HDC_LOG_ERR("hdc_fill_msg_dfx failed.(ret=%d; dev_id=%u)\n", ret, dev_id);
        goto dfx_thread_uninit;
    }

    tid = dfx_event.data.dfx_msg.tid;

    HDC_LOG_DEBUG("halEschedSubmitEvent for dfx (pid=%d; gid=%d; msg_len=%d; event_id=%u; sub_eventid=%u)\n",
        event_submit.pid, event_submit.grp_id, event_submit.msg_len, event_submit.event_id, event_submit.subevent_id);
    ret = halEschedSubmitEvent(dev_id, &event_submit);
    if (ret != DRV_ERROR_NONE) {
        HDC_LOG_ERR("SubmitEvent failed, only print local side dfx.(ret=%d; dev_id=%u; session_id=%u; gid=%u; "
            "service_type=\"%s\"; remote_pid=%llu)\n", ret, dev_id, session->local_id, event_submit.grp_id,
            hdc_get_sevice_str(session->service_type), session->peer_create_pid);
        goto dfx_clear;
    }

    // dfx_event will store remote dfx_info after esched_wait_event_ex
    event_buffer->msg = (char *)&dfx_event;
    event_buffer->msg_len = sizeof(struct hdcdrv_sync_event_msg);
    wait_info.grp_id = grp_id;
    wait_info.tid = tid;
    ret = hdc_ub_wait_reply(&event_back, &dfx_event, session, HDCDRV_NOTIFY_MSG_DFX, &wait_info);
    if (ret != DRV_ERROR_NONE) {
        HDC_LOG_ERR("eSchedWaitEventEx failed, only print local side dfx.(ret=%d; dev_id=%u; session_id=%u; gid=%u; "
            "service_type=\"%s\")\n", ret, dev_id, session->local_id, grp_id, hdc_get_sevice_str(session->service_type));
        goto dfx_clear;
    }

    HDC_LOG_INFO("Get remote side dfx info.\n");
    hdc_ub_print_dfx_info(&dfx_event.data.dfx_msg_reply.dfx_info, &dfx_event.data.dfx_msg_reply.remote_jetty_info,
        &dfx_event.data.dfx_msg_reply.send_recv_info, HDC_REMOTE_SIDE_PRINT, session);

dfx_clear:
    hdc_clear_msg_dfx(tid);

dfx_thread_uninit:
    hdc_event_thread_uninit(dev_id);

    return ret;
}

// The following functions are used for dfx processing, which has been registered to the public drv thread
void hdc_ub_fill_dfx_info(hdc_ub_dbg_stat_t *dbg_stat, struct hdc_ub_session *session)
{
    dbg_stat->tx = session->dbg_stat.tx;
    dbg_stat->tx_bytes = session->dbg_stat.tx_bytes;
    dbg_stat->rx = session->dbg_stat.rx;
    dbg_stat->rx_bytes = session->dbg_stat.rx_bytes;
    dbg_stat->tx_full = session->dbg_stat.tx_full;
    dbg_stat->tx_fail_hdc = session->dbg_stat.tx_fail_hdc;
    dbg_stat->tx_fail_ub = session->dbg_stat.tx_fail_ub;
    dbg_stat->remote_rx_full = session->dbg_stat.remote_rx_full;
    dbg_stat->remote_rx_fail = session->dbg_stat.remote_rx_fail;

    return;
}

// The following functions are used for dfx processing, which has been registered to the public drv thread
void hdc_ub_fill_send_recv_info(hdc_ub_send_recv_info_t *send_recv_info, struct hdc_ub_session *session)
{
    send_recv_info->timecost1 = session->send_recv_info.timecost1;
    send_recv_info->timecost2 = session->send_recv_info.timecost2;
    send_recv_info->timecost3 = session->send_recv_info.timecost3;
    send_recv_info->timecost4 = session->send_recv_info.timecost4;
    send_recv_info->timecost5 = session->send_recv_info.timecost5;
    send_recv_info->timecost6 = session->send_recv_info.timecost6;
    send_recv_info->timecost7 = session->send_recv_info.timecost7;
    send_recv_info->timecost8 = session->send_recv_info.timecost8;
    send_recv_info->timecost9 = session->send_recv_info.timecost9;
    send_recv_info->timecost_exceed_cnt_send = session->send_recv_info.timecost_exceed_cnt_send;
    send_recv_info->timecost_exceed_cnt_recv = session->send_recv_info.timecost_exceed_cnt_recv;
    send_recv_info->timecost_send = session->send_recv_info.timecost_send;
    send_recv_info->timecost_recv = session->send_recv_info.timecost_recv;
    send_recv_info->timecost_recv_peek = session->send_recv_info.timecost_recv_peek;

    return;
}

STATIC void hdc_fill_msg_dfx_reply(struct hdcdrv_sync_event_msg *msg, struct hdc_ub_session *session,
    struct event_summary *event_submit, struct hdcdrv_event_dfx *remote_info)
{
    event_submit->pid = (int)session->peer_create_pid;
    event_submit->tid = remote_info->tid;
    event_submit->grp_id = remote_info->grp_id;

    hdc_fill_event_for_own_grp(event_submit, msg, HDCDRV_NOTIFY_MSG_DFX_REPLY, DRV_SUBEVENT_HDC_CLOSE_LINK_MSG);
}

int hdc_dfx_query_handle(int dev_id, struct hdc_ub_session *session, struct hdcdrv_event_dfx *dfx_msg)
{
    struct hdcdrv_sync_event_msg msg = {0};
    struct event_summary event_submit = {0};
    int ret;

    msg.data.dfx_msg_reply.l_session_id = dfx_msg->l_session_id;
    msg.data.dfx_msg_reply.r_session_id = dfx_msg->r_session_id;
    hdc_ub_fill_dfx_info(&msg.data.dfx_msg_reply.dfx_info, session);
    hdc_ub_fill_jetty_info(&msg.data.dfx_msg_reply.remote_jetty_info, session);
    hdc_ub_fill_send_recv_info(&msg.data.dfx_msg_reply.send_recv_info, session);
    hdc_fill_msg_dfx_reply(&msg, session, &event_submit, dfx_msg);
    ret = halEschedSubmitEventToThread((unsigned int)dev_id, &event_submit);
    if (ret != DRV_ERROR_NONE) {
        HDC_LOG_ERR("halEschedSubmitEventToThread failed, ret = %d\n", ret);
    }

    return ret;
}

signed int hdc_ub_get_peer_devId(mmProcess handle, signed int dev_id, signed int *peer_dev_id)
{
    signed int ret;
    union hdcdrv_cmd hdc_cmd;

    hdc_cmd.get_peer_dev_id.dev_id = dev_id;

    ret = hdc_ub_ioctl(handle, HDCDRV_GET_PEER_DEV_ID, &hdc_cmd);

    *peer_dev_id = hdc_cmd.get_peer_dev_id.peer_dev_id;

    return ret;
}