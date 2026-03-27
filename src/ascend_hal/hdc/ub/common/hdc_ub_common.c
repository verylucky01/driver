/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <sys/time.h>

#include "ascend_hal.h"
#include "esched_user_interface.h"
#include "hdc_cmn.h"
#include "hdc_ub_drv.h"
#include "hdc_core.h"

// basic interface
int hdc_get_lock_index(int dev_id, int session_id)
{
    return (dev_id * HDCDRV_UB_SINGLE_DEV_MAX_SESSION + session_id);
}

void hdc_fill_query_info(struct hdc_query_info *info, int pid, int query_type, int grp_type_flag)
{
    info->pid = pid;
    info->query_type = query_type;
    info->grp_type_flag = grp_type_flag;

    return;
}

void hdc_ub_fill_jetty_info(hdcdrv_jetty_info_t *info, struct hdc_ub_session *session)
{
    info->session_jfc_recv_id = session->ctx->jfc_recv.jfc->jfc_id.id;
    info->session_jfc_send_id = session->ctx->jfc_send.jfc->jfc_id.id;
    info->session_jfs_id = session->ctx->jfs.jfs->jfs_id.id;
    info->session_jfr_id = session->ctx->jfr.jfr->jfr_id.id;

    return;
}

void hdc_ub_init_dfx_info(hdc_ub_dbg_stat_t *dbg_stat)
{
    dbg_stat->tx = 0;
    dbg_stat->tx_bytes = 0;
    dbg_stat->rx = 0;
    dbg_stat->rx_bytes = 0;
    dbg_stat->tx_full = 0;
    dbg_stat->tx_fail_hdc = 0;
    dbg_stat->tx_fail_ub = 0;
    dbg_stat->remote_rx_full = 0;
    dbg_stat->remote_rx_fail = 0;

    return;
}

unsigned int hdc_get_jfc_id_by_type(hdcdrv_jetty_info_t *jetty_info, int tx_rx_flag)
{
    if (tx_rx_flag == HDC_UB_TX) {
        return jetty_info->session_jfc_send_id;
    }

    return jetty_info->session_jfc_recv_id;
}

// Fill common msg for Event send to HDC group
void hdc_fill_event_for_own_grp(struct event_summary *event_submit, struct hdcdrv_sync_event_msg *msg,
    enum hdcdrv_notify_type notify_type, DRV_SUBEVENT_ID subevent_id)
{
    msg->data.type = notify_type;
    event_submit->event_id = EVENT_DRV_MSG;
    event_submit->subevent_id = subevent_id;
    event_submit->msg_len = sizeof(struct hdcdrv_sync_event_msg);
    event_submit->msg = (char *)msg;
    hdc_fill_event_summary_dst_engine(event_submit);
}

// Fill common msg for Event send to DRV group
int hdc_fill_event_for_drv_grp(struct event_summary *event_submit, struct hdcdrv_sync_event_msg *msg,
    struct hdc_ub_session *session, enum hdcdrv_notify_type notify_type, DRV_SUBEVENT_ID subevent_id)
{
    int ret;
    struct hdc_query_info info = {0};

    event_submit->pid = (int)session->peer_create_pid;

    hdc_fill_query_info(&info, (int)session->peer_create_pid, QUERY_TYPE_REMOTE_GRP_ID, HDC_DRV_GROUP_FLAG);
    // drv thread don't care serviceType, last para use zero
    ret = hdc_event_query_gid((unsigned int)session->dev_id, &event_submit->grp_id, &info);
    if (ret != 0) {
        HDC_LOG_ERR("hdc_event_query_gid for drv_grp failed(ret=%d; dev_id=%d).\n", ret, session->dev_id);
        return ret;
    }
    event_submit->msg_len = sizeof(struct hdcdrv_sync_event_msg);
    event_submit->msg = (char *)msg;

    event_submit->event_id = EVENT_DRV_MSG;
    event_submit->subevent_id = subevent_id;
    hdc_fill_event_summary_dst_engine(event_submit);
    hdc_fill_event_msg_dst_engine(msg);
    return 0;
}

// hdc fill dfx
void hdc_jfc_dbg_fill(int tx_rx_flag, struct hdc_ub_session *session, enum drv_hdc_ub_op op)
{
    if (tx_rx_flag == HDC_UB_TX) {
        switch (op) {
            case HDC_URMA_POLL_FAIL_BY_REM_ACESS_ABORT:
                session->dbg_stat.remote_rx_full++;
                break;
            case HDC_URMA_WAIT_FAIL:
            case HDC_URMA_POLL_FAIL:
            case HDC_URMA_REARM_FAIL:
                session->dbg_stat.tx_fail_ub++;
                break;
            default:
                break;
        }
    } else {
        session->dbg_stat.rx_fail_ub++;
    }
}

int hdc_session_alive_check(int dev_id, int l_id, unsigned int unique_val)
{
    int idx;
    int ret = 0;

    if ((l_id < 0) || (l_id >= HDCDRV_UB_SINGLE_DEV_MAX_SESSION)) {
        HDC_LOG_ERR("session para invalid.(id=%d; dev_id=%d)\n", l_id, dev_id);
        return DRV_ERROR_INVALID_VALUE;
    }

    if ((dev_id < 0) || (dev_id >= hdc_get_max_device_num())) {
        HDC_LOG_ERR("session para invalid.(id=%d; dev_id=%d)\n", l_id, dev_id);
        return DRV_ERROR_INVALID_VALUE;
    }

    idx = hdc_get_lock_index(dev_id, l_id);
    if ((g_hdcConfig.status_list[idx].status == HDC_SESSION_STATUS_IDLE) ||
        (g_hdcConfig.status_list[idx].unique_val != unique_val)) {
        ret = -HDCDRV_SESSION_HAS_CLOSED;
    }

    return ret;
}

void hdc_get_time_record(struct timespec *time_val, int *fail_count)
{
    int ret;
    int count = *fail_count;

    ret = clock_gettime(CLOCK_MONOTONIC, time_val);
    if (ret != 0) {
        count++;
    }

    *fail_count = count;

    return;
}

unsigned long long hdc_get_time_cost(struct timespec *start_tval, struct timespec *end_tval)
{
    unsigned long long cost_s, cost_ns;

    cost_s = (unsigned long long)(end_tval->tv_sec - start_tval->tv_sec);
    cost_ns = (unsigned long long)((CONVERT_S_TO_NS + end_tval->tv_nsec - start_tval->tv_nsec) % CONVERT_S_TO_NS);

    if (end_tval->tv_nsec < start_tval->tv_nsec) {
        cost_s--;
    }

    return (cost_s * CONVERT_S_TO_US) + (cost_ns / CONVERT_US_TO_NS);
}

STATIC void hdc_get_time_record_for_single_urma_init(struct hdc_time_record_for_urma_init *record,
    hdc_urma_init_info_t *urma_timecost)
{
    if (record->urma_record.need_init) {
        urma_timecost->attr_calloc = hdc_get_time_cost(&record->urma_record.hdc_init_urma_start,
            &record->urma_record.attr_calloc);
        urma_timecost->get_dev_info = hdc_get_time_cost(&record->urma_record.attr_calloc,
            &record->dms_record.get_dev_info);
        urma_timecost->create_ctx = hdc_get_time_cost(&record->dms_record.get_dev_info,
            &record->urma_record.create_ctx);
        urma_timecost->alloc_token_id = hdc_get_time_cost(&record->urma_record.create_ctx,
            &record->urma_record.alloc_token_id);
        urma_timecost->register_share_seg = hdc_get_time_cost(&record->urma_record.alloc_token_id,
            &record->urma_record.urma_init_end);
    }

    return;
}

STATIC void hdc_get_time_record_for_urma_res_init(struct hdc_time_record_for_urma_init *record,
    struct timespec *import_jetty_start, hdc_urma_init_info_t *urma_timecost)
{
    urma_timecost->urma_init_end = hdc_get_time_cost(&record->urma_record.urma_init_start,
        &record->urma_record.urma_init_end);
    urma_timecost->ctx_calloc = hdc_get_time_cost(&record->urma_record.ctx_calloc,
        &record->dms_record.get_token_val);
    urma_timecost->get_token_val = hdc_get_time_cost(&record->dms_record.get_token_val,
        &record->urma_record.urma_init_start);
    urma_timecost->register_own_seg = hdc_get_time_cost(&record->urma_record.res_init_start,
        &record->urma_record.register_own_seg);
    urma_timecost->create_jfc = hdc_get_time_cost(&record->urma_record.register_own_seg,
        &record->urma_record.create_jfc);
    urma_timecost->create_jfs = hdc_get_time_cost(&record->urma_record.create_jfc,
        &record->urma_record.create_jfs);
    urma_timecost->create_jfr = hdc_get_time_cost(&record->urma_record.create_jfs,
        &record->urma_record.create_jfr);
    urma_timecost->post_jfr_wr = hdc_get_time_cost(&record->urma_record.create_jfr,
        &record->urma_record.post_jfr_wr);
    urma_timecost->res_init_end = hdc_get_time_cost(&record->urma_record.urma_init_end,
        &record->urma_record.res_init_end);
    urma_timecost->lock_init = hdc_get_time_cost(&record->urma_record.res_init_end,
        &record->urma_record.lock_init);
    urma_timecost->get_tp_list = hdc_get_time_cost(import_jetty_start,
        &record->urma_record.get_tp_list);
    urma_timecost->import_jfr = hdc_get_time_cost(&record->urma_record.get_tp_list,
        &record->urma_record.import_jfr);

    return;
}

void hdc_get_urma_init_time_cost(struct hdc_time_record_for_urma_init *record, struct hdc_ub_session *session,
    struct timespec *import_jetty_start, hdc_urma_init_info_t *urma_timecost)
{
    if (record->fail_times > 0) {
        HDC_PERF_LOG_RUN_INFO_LIMIT("hdc perf data not get successfully.(dev_id=%d; l_id=%d; type=\'%s\'\n",
            session->dev_id, session->local_id, hdc_get_sevice_str(session->service_type));
    }

    hdc_get_time_record_for_single_urma_init(record, urma_timecost);
    hdc_get_time_record_for_urma_res_init(record, import_jetty_start, urma_timecost);

    if (record->urma_record.need_init) {
        HDC_PERF_LOG_RUN_INFO_LIMIT("hdc urma_cost info.(dev_id=%d; l_id=%u; type=%d; "
            "ctx_c=%llu; get_token=%llu; attr_c:%llu; dev_info=%llu; urma_ctx=%llu; alloc_id=%llu; share_seg=%llu; "
            "urma_init=%llu; own_seg=%llu; jfc=%llu; jfs=%llu; jfr:%llu; post_wr:%llu; res_init=%llu; lock_init=%llu; "
            "get_tp=%llu; import=%llu)\n",
            session->dev_id, session->local_id, session->service_type, urma_timecost->ctx_calloc,
            urma_timecost->get_token_val, urma_timecost->attr_calloc, urma_timecost->get_dev_info,
            urma_timecost->create_ctx, urma_timecost->alloc_token_id, urma_timecost->register_share_seg,
            urma_timecost->urma_init_end, urma_timecost->register_own_seg, urma_timecost->create_jfc,
            urma_timecost->create_jfs, urma_timecost->create_jfr, urma_timecost->post_jfr_wr,
            urma_timecost->res_init_end, urma_timecost->lock_init, urma_timecost->get_tp_list,
            urma_timecost->import_jfr);
    } else {
        HDC_PERF_LOG_RUN_INFO_LIMIT("hdc urma_cost info.(dev_id=%d; l_id=%u; type=%d; "
            "ctx_c=%llu; get_token=%llu; urma_init=%llu; own_seg=%llu; jfc=%llu; jfs=%llu; jfr:%llu; post_wr:%llu; "
            "res_init=%llu;lock_init=%llu; get_tp=%llu; import=%llu\n",
            session->dev_id, session->local_id, session->service_type,
            urma_timecost->ctx_calloc, urma_timecost->get_token_val, urma_timecost->urma_init_end,
            urma_timecost->register_own_seg, urma_timecost->create_jfc, urma_timecost->create_jfs,
            urma_timecost->create_jfr, urma_timecost->post_jfr_wr, urma_timecost->res_init_end,
            urma_timecost->lock_init, urma_timecost->get_tp_list, urma_timecost->import_jfr);
    }
}

void hdc_get_connect_time_cost(struct hdc_time_record_for_connect *connect_record, struct hdc_ub_session *session)
{
    hdc_ub_connect_info_t timecost = {0};
    hdc_urma_init_info_t urma_timecost = {0};

    if (connect_record->fail_times > 0) {
        HDC_PERF_LOG_RUN_INFO_LIMIT("hdc perf data not get successfully.(dev_id=%d; l_id=%d; type=\'%s\'\n",
            session->dev_id, session->local_id, hdc_get_sevice_str(session->service_type));
        return;
    }

    // cost of connect
    timecost.connect_end = hdc_get_time_cost(&connect_record->connect_start, &connect_record->connect_end);
    if (timecost.connect_end <= HDC_ACCEPT_PERF_STANDER) {
        return;
    }

    // cost of link_pre_init
    timecost.link_pre_init = hdc_get_time_cost(&connect_record->connect_start, &connect_record->link_pre_init);
    // cost of gid_query conn_wait
    timecost.gid_query = hdc_get_time_cost(&connect_record->link_pre_init, &connect_record->gid_query);
    // cost of mem_res_init
    timecost.mem_res_init = hdc_get_time_cost(&connect_record->gid_query, &connect_record->mem_res_init);
    // cost of alloc session in kernel
    timecost.alloc_session = hdc_get_time_cost(&connect_record->mem_res_init, &connect_record->alloc_session);
    // cost of session_pre_init
    timecost.pre_init = hdc_get_time_cost(&connect_record->alloc_session, &connect_record->pre_init);
    // cost of alloc_tid
    timecost.alloc_tid = hdc_get_time_cost(&connect_record->pre_init, &connect_record->alloc_tid);
    // cost of create_ub_ctx
    timecost.create_ub_ctx = hdc_get_time_cost(&connect_record->alloc_tid, &connect_record->create_ub_ctx);
    // cost of get_res_info_and_add_ctrl
    timecost.get_res_info_and_add_ctrl = hdc_get_time_cost(&connect_record->create_ub_ctx,
        &connect_record->get_res_info_and_add_ctrl);
    // cost of submit_event
    timecost.submit_event = hdc_get_time_cost(&connect_record->get_res_info_and_add_ctrl,
        &connect_record->submit_event);
    // cost of wait_reply
    timecost.wait_reply = hdc_get_time_cost(&connect_record->submit_event, &connect_record->wait_reply);
    // cost of check_reply
    timecost.check_reply = hdc_get_time_cost(&connect_record->wait_reply, &connect_record->check_reply);
    // cost of free_tid_and_fill_jetty_info
    timecost.free_tid_and_fill_jetty_info = hdc_get_time_cost(&connect_record->urma_func_cost.urma_record.import_jfr,
        &connect_record->free_tid_and_fill_jetty_info);

    // get all urma res timecost
    hdc_get_urma_init_time_cost(&connect_record->urma_func_cost, session, &connect_record->check_reply, &urma_timecost);
    HDC_PERF_LOG_RUN_INFO_LIMIT("hdc connect time_cost:%llu, exceed %d us.(dev_id=%d; l_id=%u; type=%d; "
        "link_pre=%llu; query_gid:%llu; res_init=%llu; alloc_session=%llu; pre_init=%llu; alloc_tid=%llu; "
        "ub_ctx=%llu; get_res_ctrl=%llu; fill_msg=%llu; submit=%llu; wait=%llu; check=%llu; free_fill=%llu; "
        "import_jetty=%llu)\n",
        timecost.connect_end, HDC_ACCEPT_PERF_STANDER, session->dev_id, session->local_id, session->service_type,
        timecost.link_pre_init, timecost.gid_query, timecost.mem_res_init, timecost.alloc_session, timecost.pre_init,
        timecost.alloc_tid, timecost.create_ub_ctx, timecost.get_res_info_and_add_ctrl, timecost.fill_event_msg,
        timecost.submit_event, timecost.wait_reply, timecost.check_reply, timecost.free_tid_and_fill_jetty_info,
        (urma_timecost.get_tp_list + urma_timecost.import_jfr));
}

void hdc_get_accept_time_cost(struct hdc_time_record_for_accept *accept_record, struct hdc_ub_session *session,
    bool succ_flag)
{
    hdc_ub_accept_info_t accept_time_cost = {0};
    hdc_urma_init_info_t urma_timecost = {0};

    if (accept_record->fail_times > 0) {
        HDC_PERF_LOG_RUN_INFO_LIMIT("hdc perf data not get successfully.(dev_id=%d; l_id=%d; type=\'%s\'\n",
            session->dev_id, session->local_id, hdc_get_sevice_str(session->service_type));
        return;
    }

    if (succ_flag) {
        // cost of accept
        accept_time_cost.accept = hdc_get_time_cost(&accept_record->conn_wait, &accept_record->accept_end);
    } else {
        // cost of accept
        accept_time_cost.accept = hdc_get_time_cost(&accept_record->conn_wait, &accept_record->submit_event);
    }

    if (accept_time_cost.accept <= HDC_ACCEPT_PERF_STANDER) {
        return;
    }

    // cost of event_query_gid
    accept_time_cost.query_gid = hdc_get_time_cost(&accept_record->query_gid_start, &accept_record->query_gid_end);
    // cost of mmReadFile conn_wait
    accept_time_cost.conn_wait = hdc_get_time_cost(&accept_record->query_gid_end, &accept_record->conn_wait);
    // cost of mem_res_init
    accept_time_cost.res_init = hdc_get_time_cost(&accept_record->conn_wait, &accept_record->res_init);
    // cost of alloc session in kernel
    accept_time_cost.alloc_session = hdc_get_time_cost(&accept_record->res_init, &accept_record->alloc_session);
    // cost of session_pre_init
    accept_time_cost.pre_init = hdc_get_time_cost(&accept_record->alloc_session, &accept_record->pre_init);
    // cost of urma_init
    accept_time_cost.urma_init = hdc_get_time_cost(&accept_record->pre_init, &accept_record->create_ub_ctx);
    // cost of add_ctrl
    accept_time_cost.add_ctrl = hdc_get_time_cost(&accept_record->create_ub_ctx, &accept_record->add_ctrl);
    // cost of submit_event
    accept_time_cost.submit_event = hdc_get_time_cost(&accept_record->urma_func_cost.urma_record.import_jfr,
        &accept_record->submit_event);

    // time_cost exceed ACCEPT_PERF_STANDER, print log
    hdc_get_urma_init_time_cost(&accept_record->urma_func_cost, session,
        &accept_record->add_ctrl, &urma_timecost);
    HDC_PERF_LOG_RUN_INFO_LIMIT("hdc accept time_cost:%llu, exceed %d us.(dev_id=%d; l_id=%u; type=%d; "
        "query_gid=%llu; conn_wait=%llu; res_init=%llu; alloc_session=%llu; pre_init=%llu; urma_init=%llu; "
        "add_ctrl=%llu; import_jetty=%llu; submit_event=%llu)\n",
        accept_time_cost.accept, HDC_ACCEPT_PERF_STANDER, session->dev_id, session->local_id, session->service_type,
        accept_time_cost.query_gid, accept_time_cost.conn_wait, accept_time_cost.res_init,
        accept_time_cost.alloc_session, accept_time_cost.pre_init, accept_time_cost.urma_init,
        accept_time_cost.add_ctrl, (urma_timecost.get_tp_list + urma_timecost.import_jfr),
        accept_time_cost.submit_event);

        return;
}

void hdc_get_recv_time_cost(struct hdc_time_record_for_single_recv *recv_record, struct hdc_ub_session *session)
{
    unsigned long long recv_total, recv_record_now;
    hdc_ub_send_recv_info_t recv_time_cost = {0};

    if (!recv_record->recv_peek_flag) {
        return;
    }

    if (recv_record->fail_times > 0) {
        HDC_PERF_LOG_RUN_INFO_LIMIT("hdc perf data not get successfully.(dev_id=%d; l_id=%d; type=\'%s\'\n",
            session->dev_id, session->local_id, hdc_get_sevice_str(session->service_type));
    }

    // cost of wait_poll_jfc in recv
    recv_time_cost.timecost6 = hdc_get_time_cost(&recv_record->ub_recv_peek_start, &recv_record->poll_jfc_recv);
    // cost of ack + rearm in recv
    recv_time_cost.timecost7 = hdc_get_time_cost(&recv_record->poll_jfc_recv, &recv_record->ack_and_rearm_jfc);
    // cost of total recv_peek
    recv_time_cost.timecost_recv_peek =
        hdc_get_time_cost(&recv_record->ub_recv_peek_start, &recv_record->ub_recv_peek_end);
    // cost of total recv
    recv_time_cost.timecost_recv = hdc_get_time_cost(&recv_record->ub_recv_start, &recv_record->ub_recv_end);

    // cost of memcpy in recv
    recv_time_cost.timecost8 = hdc_get_time_cost(&recv_record->ub_recv_start, &recv_record->copy_buf_to_user);
    // cost of urma_post_jfr_wr in recv
    recv_time_cost.timecost9 = hdc_get_time_cost(&recv_record->copy_buf_to_user, &recv_record->ub_recv_end);

    // cost of total recv
    recv_time_cost.timecost_recv = hdc_get_time_cost(&recv_record->ub_recv_start, &recv_record->ub_recv_end);

    // if time_cost exceed PERF_STANDER, print log and add count
    recv_total = recv_time_cost.timecost_recv_peek + recv_time_cost.timecost_recv;
    if (recv_total > HDC_PERF_STANDER) {
        session->send_recv_info.timecost_exceed_cnt_recv++;
        HDC_PERF_LOG_RUN_INFO_LIMIT("hdc recv exceed %d us.(dev_id=%d; l_id=%d; type=\'%s\'; jfr_id=%u; jfc_r=%u;"
            " wait_poll_jfc:%llu; ack_rearm_jfc=%llu; memcpy=%llu; post_jfr_wr=%llu; recv_peek=%llu;"
            " recv=%llu; exceed_times=%llu\n", HDC_PERF_STANDER, session->dev_id, session->local_id,
            hdc_get_sevice_str(session->service_type), session->jetty_info.session_jfr_id,
            session->jetty_info.session_jfc_recv_id, recv_time_cost.timecost6,
            recv_time_cost.timecost7, recv_time_cost.timecost8, recv_time_cost.timecost9,
            recv_time_cost.timecost_recv_peek, recv_time_cost.timecost_recv,
            session->send_recv_info.timecost_exceed_cnt_recv);
    }

    // if time_cost exceed current max val, update max val
    recv_record_now = session->send_recv_info.timecost_recv_peek + session->send_recv_info.timecost_recv;
    if (recv_total > recv_record_now) {
        session->send_recv_info.timecost6 = recv_time_cost.timecost6;
        session->send_recv_info.timecost7 = recv_time_cost.timecost7;
        session->send_recv_info.timecost8 = recv_time_cost.timecost8;
        session->send_recv_info.timecost9 = recv_time_cost.timecost9;
        session->send_recv_info.timecost_recv_peek = recv_time_cost.timecost_recv_peek;
        session->send_recv_info.timecost_recv = recv_time_cost.timecost_recv;
    }

    return;
}

void hdc_get_send_time_cost(struct hdc_time_record_for_single_send *send_record, struct hdc_ub_session *session)
{
    hdc_ub_send_recv_info_t send_time_cost = {0};
    unsigned long long timecost_wait_jfc;
    unsigned long long timecost_post_jfs_wr;

    if (send_record->fail_times > 0) {
        HDC_PERF_LOG_RUN_INFO_LIMIT("hdc perf data not get successfully.(dev_id=%d; l_id=%d; type=\'%s\'\n",
            session->dev_id, session->local_id, hdc_get_sevice_str(session->service_type));
    }

    // cost of find_idle_block in send
    send_time_cost.timecost1 = hdc_get_time_cost(&send_record->ub_send_start, &send_record->find_idle_block);
    // cost of memcpy in send
    send_time_cost.timecost2 = hdc_get_time_cost(&send_record->find_idle_block, &send_record->fill_jfs_wr);
    // cost of urma_post_jfs_wr in send
    send_time_cost.timecost3 = hdc_get_time_cost(&send_record->post_jfs_wr_start, &send_record->wait_jfc_send_finish);
    // cost of poll_jfc in send
    send_time_cost.timecost4 = hdc_get_time_cost(&send_record->wait_jfc_send_finish, &send_record->poll_jfc_send);
    // cost of ack + rearm in send
    send_time_cost.timecost5 = hdc_get_time_cost(&send_record->poll_jfc_send, &send_record->ub_send_end);
    // send may retry, so use sum of all time instead of send timecost
    send_time_cost.timecost_send = send_time_cost.timecost1 + send_time_cost.timecost2 + send_time_cost.timecost3 +
        send_time_cost.timecost4 + send_time_cost.timecost5;
    // cost of total send. If time_cost exceed PERF_STANDER, print log and add count
    if (send_time_cost.timecost_send > HDC_PERF_STANDER) {
        session->send_recv_info.timecost_exceed_cnt_send++;
        timecost_post_jfs_wr = hdc_get_time_cost(&send_record->post_jfs_wr_start, &send_record->wait_jfc_send_start);
        timecost_wait_jfc = hdc_get_time_cost(&send_record->wait_jfc_send_start, &send_record->wait_jfc_send_finish);
        HDC_PERF_LOG_RUN_INFO_LIMIT("hdc send exceed %d us.(dev_id=%d; l_id=%u; type=\'%s\'; jfs_id=%u; jfc_s=%u; "
            "find_block:%llu; memcpy=%llu; post_jfs_wr=%llu; wait_jfc=%llu; poll_jfc:%llu; ack_rearm_jfc=%llu; "
            "send=%llu; exceed_times=%llu\n", HDC_PERF_STANDER, session->dev_id, session->local_id,
            hdc_get_sevice_str(session->service_type), session->jetty_info.session_jfs_id,
            session->jetty_info.session_jfc_send_id, send_time_cost.timecost1, send_time_cost.timecost2,
            timecost_post_jfs_wr, timecost_wait_jfc, send_time_cost.timecost4, send_time_cost.timecost5,
            send_time_cost.timecost_send, session->send_recv_info.timecost_exceed_cnt_send);
    }

    // if time_cost exceed current max val, update max val
    if (send_time_cost.timecost_send > session->send_recv_info.timecost_send) {
        session->send_recv_info.timecost1 = send_time_cost.timecost1;
        session->send_recv_info.timecost2 = send_time_cost.timecost2;
        session->send_recv_info.timecost3 = send_time_cost.timecost3;
        session->send_recv_info.timecost4 = send_time_cost.timecost4;
        session->send_recv_info.timecost5 = send_time_cost.timecost5;
        session->send_recv_info.timecost_send = send_time_cost.timecost_send;
    }

    return;
}

STATIC void hdc_get_urma_uninit_time_cost(struct hdc_time_record_for_close *close_record,
    hdc_ub_close_info_t *time_cost)
{
    struct hdc_time_record_for_urma_uninit *uninit_record = &close_record->urma_uninit;
    time_cost->del_jfr = hdc_get_time_cost(&close_record->unimport_jetty, &uninit_record->del_jfr);
    time_cost->wait_data_fin = hdc_get_time_cost(&uninit_record->del_jfr, &uninit_record->wait_data_fin);
    time_cost->del_jfcr = hdc_get_time_cost(&uninit_record->wait_data_fin, &uninit_record->del_jfcr);
    time_cost->del_jfs_1 = hdc_get_time_cost(&uninit_record->del_jfcr, &uninit_record->del_jfs_1);
    time_cost->del_jfcs_1 = hdc_get_time_cost(&uninit_record->del_jfs_1, &uninit_record->del_jfcs_1);
    time_cost->own_unreg = hdc_get_time_cost(&uninit_record->del_jfcs_1, &uninit_record->own_unreg);
    if (uninit_record->need_uninit) {
        time_cost->share_unreg = hdc_get_time_cost(&uninit_record->own_unreg, &uninit_record->share_unreg);
        time_cost->share_unreg = hdc_get_time_cost(&uninit_record->own_unreg, &uninit_record->share_unreg);
        time_cost->free_token_id = hdc_get_time_cost(&uninit_record->share_unreg, &uninit_record->free_token_id);
        time_cost->del_ctx = hdc_get_time_cost(&uninit_record->free_token_id, &uninit_record->del_ctx);
    }
}

STATIC void hdc_print_close_time_cost(struct hdc_time_record_for_close *close_record, hdc_ub_close_info_t *time_cost)
{
    if (close_record->close_type == HDCDRV_CLOSE_TYPE_USER) {
        HDC_PERF_LOG_RUN_INFO_LIMIT("hdc urma_uninit_cost info. dev_id=%d; l_id=%u; type=\'%s\'; "
            "unimport_jetty=%llu; del_jfr:%llu; del_jfcr=%llu; del_jfs=%llu; del_jfcs=%llu; own_unreg=%llu; "
            "(urma_uninit_flag=%d; share_unreg=%llu; free_token_id=%llu; del_ctx=%llu)\n",
            close_record->dev_id, close_record->local_id, hdc_get_sevice_str(close_record->service_type),
            time_cost->unimport_jetty, time_cost->del_jfr, time_cost->del_jfcr, time_cost->del_jfs_1,
            time_cost->del_jfcs_1, time_cost->own_unreg, close_record->urma_uninit.need_uninit, time_cost->share_unreg,
            time_cost->free_token_id, time_cost->del_ctx);
        HDC_PERF_LOG_RUN_INFO_LIMIT("hdc close time_cost:%llu, exceed %d us.(dev_id=%d; l_id=%u; type=\'%s\'; "
            "del_recv_epoll:%llu; sub_event=%llu; del_data_epoll=%llu; wait_data_fin=%llu; del_urma=%llu; "
            "close_kernel=%llu; mem_uninit=%llu; wake_recv=%llu; close_notify=%llu; close_record_free=%llu; "
            "del_close_epoll=%llu\n",
            time_cost->timecost_close, HDC_PERF_STANDER, close_record->dev_id, close_record->local_id,
            hdc_get_sevice_str(close_record->service_type), time_cost->del_recv_epoll, time_cost->sub_close_event,
            time_cost->del_data_epoll, time_cost->wait_data_fin, time_cost->del_urma, time_cost->close_kernel,
            time_cost->mem_uninit, time_cost->wake_recv, time_cost->close_notify, time_cost->session_free,
            time_cost->del_close_epoll);
    } else if (close_record->close_type == HDCDRV_CLOSE_TYPE_REMOTE_CLOSED_POST) {
        HDC_PERF_LOG_RUN_INFO_LIMIT("hdc urma_uninit_cost info. dev_id=%d; l_id=%u; type=\'%s\'; del_jfs=%llu; "
            "del_jfcs=%llu; unimport_jetty=%llu; del_jfr:%llu; del_jfcr=%llu; del_jfs_1=%llu; del_jfcs_1=%llu; "
            "own_unreg=%llu; (urma_uninit_flag=%d; share_unreg=%llu; free_token_id=%llu; del_ctx=%llu)\n",
            close_record->dev_id, close_record->local_id, hdc_get_sevice_str(close_record->service_type),
            time_cost->del_jfs, time_cost->del_jfcs, time_cost->unimport_jetty, time_cost->del_jfr, time_cost->del_jfcr,
            time_cost->del_jfs_1, time_cost->del_jfcs_1, time_cost->own_unreg, close_record->urma_uninit.need_uninit,
            time_cost->share_unreg, time_cost->free_token_id, time_cost->del_ctx);
        HDC_PERF_LOG_RUN_INFO_LIMIT("hdc close time_cost:%llu, exceed %d us.(dev_id=%d; l_id=%u; type=\'%s\'; "
            "del_recv_epoll:%llu; sub_event=%llu; del_data_epoll=%llu; wait_data_fin=%llu; del_urma=%llu; "
            "close_kernel=%llu; mem_uninit=%llu; wake_recv=%llu; close_notify=%llu; write_file=%llu;\n",
            time_cost->timecost_close, HDC_PERF_STANDER, close_record->dev_id, close_record->local_id,
            hdc_get_sevice_str(close_record->service_type), time_cost->del_recv_epoll, time_cost->sub_close_event,
            time_cost->del_data_epoll, time_cost->wait_data_fin, time_cost->del_urma, time_cost->close_kernel,
            time_cost->mem_uninit, time_cost->wake_recv, time_cost->close_notify, time_cost->write_file);
    }
}

void hdc_get_close_time_cost(struct hdc_time_record_for_close *close_record)
{
    hdc_ub_close_info_t time_cost = {0};
    struct hdc_time_record_for_remote_close *proc_record = close_record->remote_close;
    struct hdc_time_record_for_urma_uninit *urma_uninit_record = &close_record->urma_uninit;
    int fail_times;
    if (close_record->close_type == HDCDRV_CLOSE_TYPE_USER) {
        fail_times = close_record->fail_times + urma_uninit_record->fail_times;
    } else if (close_record->close_type == HDCDRV_CLOSE_TYPE_REMOTE_CLOSED_POST) {
        fail_times = close_record->fail_times + proc_record->fail_times + urma_uninit_record->fail_times;
    } else {
        return; // session has closed
    }

    if (fail_times > 0) {
        HDC_PERF_LOG_RUN_INFO_LIMIT("hdc perf data not get successfully.(dev_id=%d; l_id=%d; type=\'%s\'\n",
            close_record->dev_id, close_record->local_id, hdc_get_sevice_str(close_record->service_type));
        return;
    }

    if (close_record->close_type == HDCDRV_CLOSE_TYPE_USER) {
        time_cost.timecost_close = hdc_get_time_cost(&close_record->start_close_handle, &close_record->del_close_epoll);
    } else if (close_record->close_type == HDCDRV_CLOSE_TYPE_REMOTE_CLOSED_POST) {
        time_cost.timecost_close = hdc_get_time_cost(&proc_record->close_proc, &proc_record->del_jfcs) +
            hdc_get_time_cost(&close_record->start_close_handle, &close_record->write_file);
    }

    if (time_cost.timecost_close <= HDC_CLOSE_PERF_STANDER) {
        return;
    }
    if (close_record->close_type == HDCDRV_CLOSE_TYPE_USER) {
        time_cost.session_free = hdc_get_time_cost(&close_record->close_notify, &close_record->session_free);
        time_cost.del_close_epoll = hdc_get_time_cost(&close_record->session_free, &close_record->del_close_epoll);
    } else if (close_record->close_type == HDCDRV_CLOSE_TYPE_REMOTE_CLOSED_POST) {
        time_cost.del_jfs = hdc_get_time_cost(&proc_record->close_proc, &proc_record->del_jfs);
        time_cost.del_jfcs = hdc_get_time_cost(&proc_record->del_jfs, &proc_record->del_jfcs);
        time_cost.write_file = hdc_get_time_cost(&close_record->close_notify, &close_record->write_file);
    }
    time_cost.del_recv_epoll = hdc_get_time_cost(&close_record->start_close_handle, &close_record->del_recv_epoll);
    time_cost.sub_close_event = hdc_get_time_cost(&close_record->del_recv_epoll, &close_record->sub_close_event);
    time_cost.del_data_epoll = hdc_get_time_cost(&close_record->sub_close_event, &close_record->del_data_epoll);
    time_cost.unimport_jetty = hdc_get_time_cost(&close_record->del_data_epoll, &close_record->unimport_jetty);
    time_cost.del_urma = hdc_get_time_cost(&close_record->del_data_epoll, &close_record->del_ub_ctx);
    time_cost.close_kernel = hdc_get_time_cost(&close_record->del_ub_ctx, &close_record->close_kernel);
    time_cost.mem_uninit = hdc_get_time_cost(&close_record->close_kernel, &close_record->mem_uninit);
    time_cost.wake_recv = hdc_get_time_cost(&close_record->mem_uninit, &close_record->wake_recv);
    time_cost.close_notify = hdc_get_time_cost(&close_record->wake_recv, &close_record->close_notify);
    hdc_get_urma_uninit_time_cost(close_record, &time_cost);
    hdc_print_close_time_cost(close_record, &time_cost);
}

STATIC void hdc_update_session_recv_peek_record(struct hdc_ub_rx_buf *buf, struct hdc_time_record_for_single_recv *recv_record)
{
    if (recv_record->fail_times > 0) {
        buf->recv_record.recv_peek_flag = false;
        return;
    }

    buf->recv_record.ub_recv_peek_start = recv_record->ub_recv_peek_start;
    buf->recv_record.wait_jfc_recv = recv_record->wait_jfc_recv;
    buf->recv_record.poll_jfc_recv = recv_record->poll_jfc_recv;
    buf->recv_record.ack_and_rearm_jfc = recv_record->ack_and_rearm_jfc;
    buf->recv_record.ub_recv_peek_end = recv_record->ub_recv_peek_end;

    return;
}

void hdc_update_session_recv_record(struct hdc_ub_rx_buf *buf, struct hdc_time_record_for_single_recv *recv_record)
{
    if (recv_record->fail_times > 0) {
        buf->recv_record.recv_peek_flag = false;
        return;
    }

    buf->recv_record.ub_recv_start = recv_record->ub_recv_start;
    buf->recv_record.copy_buf_to_user = recv_record->copy_buf_to_user;
    buf->recv_record.ub_recv_end = recv_record->ub_recv_end;

    return;
}

STATIC bool hdc_ub_dfx_reply_check(struct hdcdrv_event_msg *msg_back, struct hdc_ub_session *session)
{
    struct hdcdrv_event_dfx_reply *msg_reply = &msg_back->dfx_msg_reply;

    if (msg_back->type != HDCDRV_NOTIFY_MSG_DFX_REPLY) {
        return false;
    }

    if (msg_reply->l_session_id != session->local_id) {
        return false;
    }

    if (msg_reply->para_info.unique_val != session->unique_val) {
        return false;
    }

    return true;
}

STATIC bool hdc_ub_conn_reply_check(struct hdcdrv_event_msg *msg_back, struct hdc_ub_session *session)
{
    struct hdcdrv_event_connect_reply *msg_reply = &msg_back->connect_msg_reply;

    if (msg_back->type != HDCDRV_NOTIFY_MSG_CONNECT_REPLY) {
        return false;
    }

    if (msg_reply->client_session != session->local_id) {
        return false;
    }

    if (msg_reply->unique_val != session->unique_val) {
        return false;
    }

    return true;
}

STATIC bool hdc_reply_type_check(struct hdcdrv_event_msg *msg_back, struct hdc_ub_session *session, int msg_type) {
    if (msg_type == HDCDRV_NOTIFY_MSG_CONNECT) {
        return hdc_ub_conn_reply_check(msg_back, session);
    } else if (msg_type == HDCDRV_NOTIFY_MSG_DFX) {
        return hdc_ub_dfx_reply_check(msg_back, session);
    }

    return false;
}

// Used for connect/dfx query to wait reply from remote
int hdc_ub_wait_reply(struct event_info *event_back, struct hdcdrv_sync_event_msg *wait_event,
    struct hdc_ub_session *session, int msg_type, struct hdc_event_wait_info *wait_info)
{
    esched_event_buffer *event_buffer = (esched_event_buffer *)event_back->priv.msg;
    unsigned long long timecost = HDC_CONN_WAIT_TIMEOUT;
    struct hdcdrv_event_msg *msg_back = NULL;
    int timeout = HDC_CONN_WAIT_TIMEOUT;
    struct timespec start_time, end_time;
    int ret;

    event_buffer->msg = (char *)wait_event;
    event_buffer->msg_len = sizeof(struct hdcdrv_sync_event_msg);

    (void)clock_gettime(CLOCK_MONOTONIC, &start_time);
    do {
        ret = esched_wait_event_ex((uint32_t)session->dev_id, wait_info->grp_id, wait_info->tid, timeout, event_back);
        if (ret != DRV_ERROR_NONE) {
            HDC_LOG_ERR("halEschedWaitEvent failed.(ret=%d; dev_id=%d; session_id=%d; gid=%u; service_type=\"%s\")\n",
                ret, session->dev_id, session->local_id, session->local_gid, hdc_get_sevice_str(session->service_type));
            return ret;
        }
        (void)clock_gettime(CLOCK_MONOTONIC, &end_time);
        msg_back = &wait_event->data;
        if (hdc_reply_type_check(msg_back, session, msg_type)) {
            break;
        }

        timecost = hdc_get_time_cost(&start_time, &end_time) / CONVERT_MS_TO_US;
        if (timecost < HDC_CONN_WAIT_TIMEOUT) {
            HDC_LOG_WARN("Wait connect reply is not expect, retry.\n");
            timeout = HDC_CONN_WAIT_TIMEOUT - (int)timecost;
            continue;
        } else {
            HDC_LOG_ERR("Wait connect reply timeout.\n");
            return DRV_ERROR_WAIT_TIMEOUT;
        }
    } while(1);

    return 0;
}

void hdc_get_ub_res_info(hdc_ub_context_t *ctx, hdc_ub_res_info_t *ub_res_info)
{
    ub_res_info->l_jfr_id = ctx->jfr.jfr->jfr_id.id;
    ub_res_info->l_jfs_id = ctx->jfs.jfs->jfs_id.id;
    ub_res_info->l_jfc_r_id = ctx->jfc_recv.jfc->jfc_id.id;
    ub_res_info->l_jfc_s_id = ctx->jfc_send.jfc->jfc_id.id;
    ub_res_info->l_jfce_r_fd = ctx->jfc_recv.jfce->fd;
}

STATIC int hdc_update_rx_list(struct hdc_ub_epoll_node *node, struct hdc_ub_session *session,
    urma_cr_t *cr, urma_target_seg_t *tseg, struct hdc_time_record_for_single_recv *recv_record)
{
    if ((cr->completion_len > HDC_MEM_BLOCK_SIZE) || (cr->completion_len <= 0) ||
        (cr->user_ctx >= (HDC_URMA_MAX_JFR_DEPTH + HDC_URMA_MAX_JFS_DEPTH))) {
        HDC_LOG_ERR("Recv buf len overflow. (dev_id=%d; recv len=%u; l_id=%u; r_id=%u; user_ctx=%lu)\n",
            session->dev_id, cr->completion_len, session->local_id, session->remote_id, cr->user_ctx);
        session->dbg_stat.rx_fail_hdc++;
        return DRV_ERROR_TRANS_LINK_ABNORMAL;
    }

    node->rx_list[node->tail].tseg = tseg;
    node->rx_list[node->tail].addr = session->user_va + cr->user_ctx * HDC_MEM_BLOCK_SIZE;
    node->rx_list[node->tail].len = cr->completion_len;
    node->rx_list[node->tail].idx = cr->user_ctx;

    hdc_get_time_record(&recv_record->ub_recv_peek_end, &recv_record->fail_times);
    hdc_update_session_recv_peek_record(&node->rx_list[node->tail], recv_record);
    node->tail = (node->tail + 1) % HDC_RX_LIST_LEN;

    return DRV_ERROR_NONE;
}

STATIC bool hdc_check_wait_recv_jfc_ret(hdc_ub_context_t *ctx, struct hdc_ub_session *session)
{
    int cnt;
    urma_jfc_t *ev_jfc;
    bool flag = true;

    cnt = urma_wait_jfc(ctx->jfc_recv.jfce, 1, 1, &ev_jfc);
    if ((cnt != 1) || (ctx->jfc_recv.jfc != ev_jfc)) {
        /* if cnt is 0, means no cqe come; if cnt is -1, means error. For some services that frequently time out
            (like log, bbox), no need to print. Otherwise, the log will be refreshed. */
        HDC_LOG_ERR("Wait jfc has problem. (cnt=%d; jfc_id=%u; l_id=%u; r_id=%u; dev_id=%d;"
                "service_type=\"%s\")\n", cnt, hdc_get_jfc_id_by_type(&session->jetty_info, HDC_UB_RX), session->local_id,
                session->remote_id, session->dev_id, hdc_get_sevice_str(session->service_type));
        hdc_jfc_dbg_fill(HDC_UB_RX, session, HDC_URMA_WAIT_FAIL);
        flag = false;
    }

    // If wait_jfc receives cqe, it must be paired with an ack; otherwise, the delete_jfc interface will be blocked
    if (cnt > 1) {
        urma_ack_jfc((urma_jfc_t **)&ev_jfc, (uint32_t *)&cnt, 1);
        (void)urma_rearm_jfc(ctx->jfc_recv.jfc, false);
    }

    return flag;
}

void hdc_recv_data_in_event_handle(struct hdc_ub_epoll_node *node)
{
    int service_type, dev_id;
    struct hdc_time_record_for_single_recv recv_record = {0};
    struct hdc_ub_session *session = (struct hdc_ub_session *)node->session;
    hdc_ub_context_t *ctx = NULL;
    urma_target_seg_t *tseg = NULL;
    urma_cr_t cr = {0};
    uint64_t inc = 1;
    mmSsize_t ret;

    pthread_mutex_lock(&node->mutex);
    // Add a session status check
    if ((session == NULL) || (session->status == HDC_SESSION_STATUS_IDLE)) {
        goto recv_abnormal_exit;
    }

    if (hdc_rx_list_is_full(node)) {
        goto recv_abnormal_exit;
    }

    hdc_get_time_record(&recv_record.ub_recv_peek_start, &recv_record.fail_times);
    node->pkt_num++;
    ctx = node->ctx;
    tseg = ctx->tseg;
    dev_id = session->dev_id;
    service_type = session->service_type;

    if (!hdc_check_wait_recv_jfc_ret(ctx, session)) {
        goto recv_abnormal_exit;
    }
    wmb();
    hdc_get_time_record(&recv_record.wait_jfc_recv, &recv_record.fail_times);

    if (hdc_poll_jfc(&ctx->jfc_recv, &cr, session, HDC_UB_RX, session->service_type) != 0) {
        hdc_ack_jfc(&ctx->jfc_recv);
        (void)hdc_rearm_jfc(&ctx->jfc_recv, session, HDC_UB_RX);
        goto recv_abnormal_exit;
    }
    wmb();
    hdc_get_time_record(&recv_record.poll_jfc_recv, &recv_record.fail_times);

    hdc_ack_jfc(&ctx->jfc_recv);
    if (hdc_rearm_jfc(&ctx->jfc_recv, session, HDC_UB_RX) != 0) {
        goto recv_abnormal_exit;
    }
    wmb();
    hdc_get_time_record(&recv_record.ack_and_rearm_jfc, &recv_record.fail_times);

    if (hdc_update_rx_list(node, session, &cr, tseg, &recv_record) != DRV_ERROR_NONE) {
        goto recv_abnormal_exit;
    }
    wmb();

    pthread_cond_signal(&node->cond);
    pthread_mutex_unlock(&node->mutex);
    hdc_touch_data_in_notify(dev_id, service_type);
    if (session->recv_eventfd != -1) {
        ret = mm_write_file(session->recv_eventfd, &inc, sizeof(inc));
        if (ret != (mmSsize_t)sizeof(inc)) {
            HDC_LOG_WARN("send close event not success.(ret=%d; id=%d; dev=%d)\n", ret,
                session->local_id, session->dev_id);
        }
    }
    return;

recv_abnormal_exit:
    pthread_mutex_unlock(&node->mutex);
    return;
}

uint8_t hdc_ub_get_jfs_priority_by_type(int service_type)
{
    uint8_t priority_level;

    switch (service_type) {
        case HDC_SERVICE_TYPE_PROFILING:
        case HDC_SERVICE_TYPE_IDE1:
        case HDC_SERVICE_TYPE_IDE2:
        case HDC_SERVICE_TYPE_LOG:
        case HDC_SERVICE_TYPE_BBOX:
        case HDC_SERVICE_TYPE_IDE_FILE_TRANS:
        case HDC_SERVICE_TYPE_DUMP:
            priority_level = HDC_URMA_PRIORITY_LOW;
            break;
        default:
            priority_level = HDC_URMA_PRIORITY_HIGH;
            break;
    }

    return priority_level;
}
