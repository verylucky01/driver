/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef UT_TEST
#include "esched_user_interface.h"
#include "prof_event_comm.h"
#include "prof_urma.h"

drvError_t prof_urma_get_channels(uint32_t dev_id, struct prof_channel_list *channels)
{
    struct prof_event_para event_para;
    drvError_t ret;

    prof_event_event_para_comm_pack(&event_para, dev_id, EVENT_DRV_MSG_EX, DRV_SUBEVENT_PROF_GET_CHAN_LIST_MSG, 0);
    prof_event_event_para_msg_send_pack(&event_para, NULL, 0);
    prof_event_event_para_msg_recv_pack(&event_para, (char *)channels, sizeof(struct prof_channel_list));

    ret = prof_event_submit_event_sync(&event_para);
    if (ret != DRV_ERROR_NONE) {
        PROF_ERR("Failed to send get channel list event. (dev_id=%u, ret=%d)\n", dev_id, ret);
    }

    return ret;
}

static drvError_t prof_urma_fill_start_event_msg(struct prof_urma_info *urma_info, struct prof_urma_chan_info *urma_chan_info,
    uint32_t chan_id, struct prof_user_start_para *para, struct prof_start_urma_msg *event_msg)
{
    int ret = DRV_ERROR_NONE;

    event_msg->jetty_id = urma_chan_info->jetty->jetty_id.id;
    event_msg->data_buff = urma_chan_info->user_buff;
    event_msg->data_buff_len = urma_chan_info->user_buff_len;

    event_msg->token_id = urma_info->token_id->token_id;
    event_msg->token_val = urma_info->token.token;
    event_msg->eid_index = urma_info->eid_index;
    ret = memcpy_s(event_msg->eid_raw, PROF_EID_SIZE, &(urma_info->urma_ctx->eid), PROF_EID_SIZE);
    if (ret != 0) {
        PROF_ERR("Failed to memcpy. (ret=%d)\n", ret);
        return DRV_ERROR_INNER_ERR;
    }

    event_msg->channel_id = chan_id;
    event_msg->sample_period = ((para->remote_pid == 0) ? para->sample_period : 0);
    event_msg->user_data_size = para->user_data_size;
    if (para->user_data_size != 0) {
        ret = memcpy_s(event_msg->user_data, PROF_USER_DATA_LEN, para->user_data, para->user_data_size);
        if (ret != DRV_ERROR_NONE) {
            PROF_ERR("Failed to memcpy user data. (user_data_size=%u)\n", para->user_data_size);
            return DRV_ERROR_INNER_ERR;
        }
    }
    return (drvError_t)ret;
}

static drvError_t prof_urma_send_stop_event(uint32_t dev_id, uint32_t chan_id, struct prof_user_stop_para *para,
    struct prof_urma_chan_info *urma_chan_info, struct prof_urma_info *urma_info)
{
    struct prof_stop_urma_msg stop_event_msg = {0};
    struct prof_event_para event_para;
    drvError_t ret;

    stop_event_msg.channel_id = chan_id;

    prof_event_event_para_comm_pack(&event_para, dev_id, EVENT_DRV_MSG_EX, DRV_SUBEVENT_PROF_STOP_MSG, 0);
    prof_event_event_para_msg_send_pack(&event_para, (char *)&stop_event_msg, sizeof(struct prof_stop_urma_msg));
    prof_event_event_para_msg_recv_pack(&event_para, NULL, sizeof(struct prof_stop_sync_msg));

    ret = prof_event_submit_event_sync(&event_para);
    if (ret != DRV_ERROR_NONE) {
        PROF_ERR("Failed to send stop event. (dev_id=%u, ret=%d)\n", dev_id, (int)ret);
    }

    return ret;
}

static drvError_t prof_urma_send_start_event(uint32_t dev_id, uint32_t chan_id, struct prof_user_start_para *para,
    struct prof_urma_chan_info *urma_chan_info, struct prof_urma_info *urma_info)
{
    struct prof_start_urma_msg start_event_msg = {0};
    struct prof_user_stop_para stop_para = {0};
    struct prof_start_sync_msg sync_msg;
    struct prof_event_para event_para;
    drvError_t ret;

    ret = prof_urma_fill_start_event_msg(urma_info, urma_chan_info, chan_id, para, &start_event_msg);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    prof_event_event_para_comm_pack(&event_para, dev_id, EVENT_DRV_MSG_EX, DRV_SUBEVENT_PROF_START_MSG, 0);
    prof_event_event_para_msg_send_pack(&event_para, (char *)&start_event_msg, sizeof(struct prof_start_urma_msg));
    prof_event_event_para_msg_recv_pack(&event_para, (char *)&sync_msg, sizeof(struct prof_start_sync_msg));

    ret = prof_event_submit_event_sync(&event_para);
    if (ret != DRV_ERROR_NONE) {
        PROF_ERR("Failed to send start event. (dev_id=%u, ret=%d)\n", dev_id, (int)ret);
        return ret;
    }

    ret = prof_urma_remote_info_import(urma_info, urma_chan_info, &sync_msg);
    if (ret != DRV_ERROR_NONE) {
        PROF_ERR("Failed to import remote urma. (dev_id=%u, chan_id=%u)\n", dev_id, chan_id);
        (void)prof_urma_send_stop_event(dev_id, chan_id, &stop_para, urma_chan_info, urma_info);
    }

    return ret;
}

drvError_t prof_urma_start(uint32_t dev_id, uint32_t chan_id, struct prof_user_start_para *para,
    struct prof_urma_start_para *urma_start_para, struct prof_urma_chan_info *urma_chan_info)
{
    struct prof_urma_info *urma_info;
    struct process_sign sign_info = {0};
    drvError_t ret;

    urma_info = prof_get_urma_info(dev_id);
    if (urma_info == NULL) {
        PROF_ERR("Failed to get prof urma info. (dev_id=%u)\n", dev_id);
        return DRV_ERROR_UNINIT;
    }

    ret = drvGetProcessSign(&sign_info);
    if (ret != DRV_ERROR_NONE) {
        PROF_ERR("Failed to get process sign . (dev_id=%u, chan_id=%u, ret=%d).\n", dev_id, chan_id, (int)ret);
        return ret;
    }

    ret = prof_urma_local_seg_register(urma_start_para, urma_info, urma_chan_info);
    if (ret != DRV_ERROR_NONE) {
        PROF_ERR("Failed to register. (dev_id=%u, chan_id=%u)\n", dev_id, chan_id);
        return ret;
    }

    ret = prof_urma_send_start_event(dev_id, chan_id, para, urma_chan_info, urma_info);
    if (ret != 0) {
        PROF_ERR("Failed to start prof. (ret=%d, dev_id=%u, chan_id=%u)\n", (int)ret, dev_id, chan_id);
        prof_urma_local_seg_unregister(urma_chan_info);
        return ret;
    }

    return DRV_ERROR_NONE;
}

drvError_t prof_urma_stop(uint32_t dev_id, uint32_t chan_id, struct prof_user_stop_para *para,
    struct prof_urma_chan_info *urma_chan_info)
{
    struct prof_urma_info *urma_info = prof_get_urma_info(dev_id);
    drvError_t ret;

    if (urma_info == NULL) {
        PROF_ERR("Failed to get prof urma info. (dev_id=%u)\n", dev_id);
        return DRV_ERROR_UNINIT;
    }

    ret = prof_urma_send_stop_event(dev_id, chan_id, para, urma_chan_info, urma_info);
    if (ret != DRV_ERROR_NONE) {
        PROF_ERR("Failed to stop prof. (ret=%d, dev_id=%u, chan_id=%u)\n", (int)ret, dev_id, chan_id);
        return ret;
    }

    prof_urma_remote_info_unimport(urma_info, urma_chan_info);
    prof_urma_local_seg_unregister(urma_chan_info);

    return DRV_ERROR_NONE;
}

drvError_t prof_urma_flush(uint32_t dev_id, uint32_t chan_id)
{
    struct prof_flush_urma_msg flush_event_msg = {0};
    struct prof_event_para event_para;
    drvError_t ret;

    flush_event_msg.channel_id = chan_id;

    prof_event_event_para_comm_pack(&event_para, dev_id, EVENT_DRV_MSG_EX, DRV_SUBEVENT_PROF_FLUSH_MSG, 0);
    prof_event_event_para_msg_send_pack(&event_para, (char *)&flush_event_msg, sizeof(struct prof_flush_urma_msg));
    prof_event_event_para_msg_recv_pack(&event_para, NULL, sizeof(struct prof_flush_sync_msg));

    ret = prof_event_submit_event_sync(&event_para);
    if ((ret != DRV_ERROR_NONE) && (ret != DRV_ERROR_NOT_SUPPORT)) {
        PROF_ERR("Failed to send flush event. (dev_id=%u, ret=%d)\n", dev_id, (int)ret);
        return ret;
    }

    PROF_INFO("Sample flush success. (dev_id=%u, chan_id=%u)\n", dev_id, chan_id);
    return ret;
}

drvError_t prof_urma_write_remote_r_ptr(uint32_t dev_id, uint32_t chan_id, struct prof_urma_chan_info *urma_chan_info)
{
    return prof_urma_write_remote_rptr(dev_id, chan_id, urma_chan_info);
}

#else
int prof_urma_ut_test(void)
{
    return 0;
}
#endif