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
#include <sys/epoll.h>
#include <stdint.h>
#include <sys/eventfd.h>

#include "ascend_hal.h"
#include "esched_user_interface.h"
#include "hdc_cmn.h"
#include "hdc_epoll.h"
#include "hdc_ub_drv.h"
#include "hdc_core.h"

// process hdc remote connect request, equal to crtl_msg_connect in pcie
drvError_t hdc_connect_event_proc(uint32_t dev_id, const void *msg, int msg_len, struct drv_event_proc_rsp *rsp)
{
    struct hdcdrv_event_msg *conn_msg = (struct hdcdrv_event_msg *)msg;
    struct hdc_server_head *p_server_head = NULL;
    int serviceType;
    mmSsize_t num;

    if ((msg == NULL) || (msg_len < sizeof(struct hdcdrv_event_msg))) {
        HDC_LOG_ERR("Invalid msg len for connect event. (len=%u)\n", msg_len);
        return DRV_ERROR_INVALID_VALUE;
    }
    serviceType = conn_msg->connect_msg.service_type;
    rsp->real_rsp_data_len = 0;
    rsp->need_rsp = false;

    HDC_LOG_DEBUG("event type=%d; peer_id=%llu; service_type=\"%s\"; client_pid=%llu; euid=%d; uid=%d; "
        "connect_tid=%u; connect_gid=%u\n", conn_msg->type, conn_msg->peer_pid,
        hdc_get_sevice_str(conn_msg->connect_msg.service_type),
        conn_msg->connect_msg.client_pid, conn_msg->connect_msg.euid, conn_msg->connect_msg.uid,
        conn_msg->connect_msg.connect_tid, conn_msg->connect_msg.connect_gid);

    if (!hdc_service_type_vaild(serviceType)) {
        HDC_LOG_ERR("service type is invalid.(service type=%d)\n", serviceType);
        return DRV_ERROR_INVALID_VALUE;
    }
    p_server_head = (struct hdc_server_head *)g_hdcConfig.server_list[dev_id][serviceType];
    if (p_server_head == NULL) {
        HDC_LOG_WARN("server not be found. maybe the server has destroyed.(service type=%d)\n", serviceType);
        return 0;
    }

    /* Send a receiving notification to the accept thread through the pipe. */
    if (p_server_head->conn_notify == -1) {
        HDC_LOG_WARN("maybe the server has destroyed.(dev_id=%d; service_type=\"%s\")\n", dev_id,
            hdc_get_sevice_str(serviceType));
        return HDCDRV_SESSION_HAS_CLOSED;
    }
    num = mm_write_file(p_server_head->conn_notify, (const void *)conn_msg, sizeof(struct hdcdrv_event_msg));
    if (num != (mmSsize_t)sizeof(struct hdcdrv_event_msg)) {
        HDC_LOG_ERR("send connection event message data failed.(dev_id=%d; service_type=\"%s\")\n", dev_id,
            hdc_get_sevice_str(serviceType));
        return HDCDRV_SEND_CTRL_MSG_FAIL;
    }
    HDC_LOG_DEBUG("received a connection. (device id=%u, serviceType=%d)\n", dev_id, serviceType);

    hdc_touch_connect_notify((int)dev_id, conn_msg->connect_msg.client_pid, serviceType);

    return 0;
}

// process hdc remote dfx query request
drvError_t hdc_dfx_query_event_proc(uint32_t dev_id, const void *msg, int msg_len, struct drv_event_proc_rsp *rsp)
{
    struct hdcdrv_event_msg *dfx_msg = NULL;
    int idx, ret;
    struct hdc_ub_session *session = NULL;

    if ((msg == NULL) || (msg_len < sizeof(struct hdcdrv_event_msg))) {
        HDC_LOG_ERR("Invalid msg len for dfx query. (len=%u)\n", msg_len);
        return DRV_ERROR_INVALID_VALUE;
    }

    dfx_msg = (struct hdcdrv_event_msg *)msg;

    if ((dfx_msg->dfx_msg.r_session_id >= HDCDRV_UB_SINGLE_DEV_MAX_SESSION) ||
        (dev_id >= (unsigned int)hdc_get_max_device_num())) {
        HDC_LOG_ERR("session para invalid.(id=%d)\n", dfx_msg->dfx_msg.r_session_id);
        return DRV_ERROR_NOT_SUPPORT;
    }

    idx = hdc_get_lock_index((int)dev_id, (int)dfx_msg->dfx_msg.r_session_id);
    (void)mmMutexLock(&g_hdcConfig.session_lock[idx]);

    session = hdc_find_session_in_list(dfx_msg->dfx_msg.r_session_id, (int)dev_id, dfx_msg->dfx_msg.para_info.unique_val);
    if ((session == NULL) || (session->status == HDC_SESSION_STATUS_IDLE)) {
        HDC_LOG_WARN("Can not find session, session may close.(local_session=%u, remote_session=%u, unique_val=%u)\n",
            dfx_msg->dfx_msg.r_session_id, dfx_msg->dfx_msg.l_session_id, dfx_msg->dfx_msg.para_info.unique_val);
        (void)mmMutexUnLock(&g_hdcConfig.session_lock[idx]);
        return -HDCDRV_SESSION_HAS_CLOSED;
    }

    ret = hdc_dfx_query_handle((int)dev_id, session, &dfx_msg->dfx_msg);

    (void)mmMutexUnLock(&g_hdcConfig.session_lock[idx]);

    rsp->real_rsp_data_len = 0;
    rsp->need_rsp = false;

    return ret;
}

// process hdc remote close request, equal to crtl_msg_close in pcie
drvError_t hdc_sync_event_proc(uint32_t dev_id, const void *msg, int msg_len, struct drv_event_proc_rsp *rsp)
{
    drvError_t ret;
    struct hdcdrv_event_msg *msg_para = (struct hdcdrv_event_msg *)msg;

    if ((msg == NULL) || (msg_len < sizeof(struct hdcdrv_event_msg))) {
        HDC_LOG_ERR("Invalid msg len. (len=%u)\n", msg_len);
        return DRV_ERROR_INVALID_VALUE;
    }

    if (dev_id >= (unsigned int)hdc_get_max_device_num()) {
        HDC_LOG_ERR("Invalid dev id. (dev_id=%u)\n", dev_id);
        return DRV_ERROR_INVALID_VALUE;
    }
    if (msg_para->type >= HDCDRV_NOTIFY_MSG_MAX) {
        HDC_LOG_ERR("Invalid msg id. (msg_id=%u)\n", msg_para->type);
        return DRV_ERROR_INVALID_VALUE;
    }

    if (msg_para->type == HDCDRV_NOTIFY_MSG_DFX) {
        ret = hdc_dfx_query_event_proc(dev_id, msg, msg_len, rsp);
    } else if (msg_para->type == HDCDRV_NOTIFY_MSG_CLOSE) {
        ret = hdc_remote_close_proc((void *)msg, dev_id);
    } else {
        ret = DRV_ERROR_INVALID_VALUE;
    }

    rsp->real_rsp_data_len = 0;
    rsp->need_rsp = false;

    return ret;
}