/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */
#include "ka_compiler_pub.h"
#include "ascend_ub_jetty.h"
#include "ascend_ub_admin_msg.h"
#include "ascend_ub_link_chan.h"
#include "ascend_ub_non_trans_chan.h"
#include "ascend_ub_main.h"
#include "ascend_ub_msg.h"

void ubdrv_wait_chan_jfce_user_cnt(ka_atomic_t *user_cnt, u32 dev_id, u32 chan_id)
{
    int i;

    for (i = 0; i < MSG_RX_USER_CNT_WAIT_CNT; i++) {
        if (ka_base_atomic_read(user_cnt) == 0) {
            return;
        }
        ka_system_usleep_range(1, 2);
    }
    ubdrv_warn("Wait msg chan jfce_recv user cnt timeout. (chan_id=%u;dev_id=%u)\n",
        chan_id, dev_id);
}

int ubdrv_copy_user_send(struct ascend_ub_msg_desc *desc, char *data, u32 len, u32 max_len)
{
    if (len > max_len) {
        return -EDOM;
    }
    return memcpy_s(&(desc->user_data), len, data, len);
}

int ubdrv_prepare_recv_data(struct ascend_ub_msg_desc *desc, struct ascend_ub_user_data *data,
    u32 msg_num, u32 max_len)
{
    u32 real_reply_size = desc->real_data_len;

    if (msg_num != desc->msg_num) {
        ubdrv_err("Msg num check failed. (seg_id=%u;msg_num=%u;real_msg_num=%u)\n",
            desc->seg_id, msg_num, desc->msg_num);
            return -EAGAIN;
    }
    if ((data->reply == NULL) || (real_reply_size == 0)) {
        return 0;
    } else {
        if ((real_reply_size > data->reply_size) || (real_reply_size > max_len)) {
            ubdrv_err("Reply data len invalid. (reply_size=%u;real_reply_size=%u)\n",
                data->reply_size, real_reply_size);
            return -EINVAL;
        }
        return memcpy_s(data->reply, data->reply_size, &(desc->user_data), real_reply_size);
    }
}

int ubdrv_copy_rqe_data_to_user(struct ascend_ub_msg_desc *rqe_desc, struct ascend_ub_user_data *data,
    struct ubdrv_msg_chan_stat *stat, u32 rqe_len)
{
    u32 real_reply_size = rqe_desc->real_data_len;
    u32 max_len;

    if ((data->reply == NULL) || (real_reply_size == 0)) {
        return 0;
    }
    if (rqe_len <= ASCEND_UB_MSG_DESC_LEN) {
        ubdrv_err("Rqe len invalid. (rqe_len=%u)\n", rqe_len);
        stat->tx_recv_data_err++;
        return -EINVAL;
    }
    max_len = (rqe_len - ASCEND_UB_MSG_DESC_LEN);
    if ((real_reply_size > data->reply_size) || (real_reply_size > max_len)) {
        ubdrv_err("Reply data len invalid. (expect_reply_size=%u;real_reply_size=%u;max_len=%u)\n",
            data->reply_size, real_reply_size, max_len);
            stat->tx_recv_data_err++;
        return -EINVAL;
    }
    return memcpy_s(data->reply, data->reply_size, rqe_desc->user_data, real_reply_size);
}

struct ascend_ub_msg_desc *ubdrv_alloc_sync_send_seg(struct ascend_ub_sync_jetty *sync_jetty,
    u32 dev_id, u32 msg_num, u32 try_cnt, u32 wait_per_us)
{
    u32 i;
    struct ascend_ub_jetty_ctrl *send_jetty;
    struct ascend_ub_msg_desc *desc = NULL;
    struct ascend_ub_msg_desc *tmp_desc = NULL;
    u32 seg_num;

retry:
    if (ka_unlikely(ubdrv_get_device_status(dev_id) == UBDRV_DEVICE_DEAD)) {
        ubdrv_warn("Device status is dead. (dev_id=%u)\n", dev_id);
        return NULL;
    }
    try_cnt--;
    send_jetty = &(sync_jetty->send_jetty);
    seg_num = send_jetty->jfs_msg_depth;
    ka_task_mutex_lock(&sync_jetty->mutex_lock);
    for (i = 0; i < seg_num; ++i) {
        desc = ubdrv_get_send_desc(send_jetty, i);
        if (desc->time_out == UB_MSG_TIME_OUT) {
            tmp_desc = desc;
            continue;
        }
        if ((desc->status == UB_MSG_INIT) || (desc->status == UB_MSG_IDLE)) {
            desc->status = UB_MSG_SENDING;
            desc->msg_num = msg_num;
            ka_task_mutex_unlock(&sync_jetty->mutex_lock);
            return desc;
        }
    }
    if (tmp_desc == NULL) {
        ka_task_mutex_unlock(&sync_jetty->mutex_lock);
        if (try_cnt > 0) {
            ka_system_usleep_range(wait_per_us, wait_per_us);
            goto retry;
        }
        return NULL;
    }
    tmp_desc->time_out = UB_MSG_TIME_NORMAL;
    tmp_desc->status = UB_MSG_SENDING;
    tmp_desc->msg_num = msg_num;
    ka_task_mutex_unlock(&sync_jetty->mutex_lock);
    ubdrv_warn("Will using a over time seg. (msg_num=%u;dev_id=%u)", msg_num, dev_id);
    return tmp_desc;
}

void ubdrv_free_sync_send_seg(struct ascend_ub_sync_jetty *sync_jetty,
    struct ascend_ub_msg_desc *desc)
{
    ka_task_mutex_lock(&sync_jetty->mutex_lock);
    desc->status = UB_MSG_IDLE;
    ka_task_mutex_unlock(&sync_jetty->mutex_lock);
    return;
}

void ubdrv_record_chan_jetty_info(struct ubdrv_msg_chan_stat *stat,
    struct ascend_ub_sync_jetty *sync_jetty)
{
    struct ascend_ub_jetty_ctrl *send_jetty = &sync_jetty->send_jetty;
    struct ascend_ub_jetty_ctrl *recv_jetty = &sync_jetty->recv_jetty;

    stat->tx_jfr_jfc_id = send_jetty->jfr_jfc->id;
    stat->tx_jfs_jfc_id = send_jetty->jfs_jfc->id;
    stat->tx_jfr_id = send_jetty->jfr->jfr_id.id;
    stat->tx_jfs_id = send_jetty->jfs->jfs_id.id;
    stat->rx_jfs_jfc_id = recv_jetty->jfs_jfc->id;
    stat->rx_jfr_jfc_id = recv_jetty->jfr_jfc->id;
    stat->rx_jfr_id = recv_jetty->jfr->jfr_id.id;
    stat->rx_jfs_id = recv_jetty->jfs->jfs_id.id;
    return;
}

void ubdrv_clear_chan_dfx(struct ubdrv_msg_chan_stat *stat)
{
    size_t len = sizeof(struct ubdrv_msg_chan_stat);

    (void)memset_s(stat, len, 0, len);
    return;
}

void ubdrv_recv_msg_call_process(struct ascend_ub_jetty_ctrl *cfg,
    struct ascend_ub_msg_desc *desc, u32 seg_id)
{
    if (cfg->chan_mode == UBDRV_MSG_CHAN_FOR_ADMIN) {
        ubdrv_admin_recv_prepare_process(cfg, desc, seg_id);
    } else if (cfg->chan_mode == UBDRV_MSG_CHAN_FOR_LINK) {
        ubdrv_link_chan_recv_prepare_process(cfg, desc, seg_id);
    } else if (cfg->chan_mode == UBDRV_MSG_CHAN_FOR_NON_TRANS) {
        ubdrv_non_trans_recv_prepare_process(cfg, desc, seg_id);
    } else {
        ubdrv_err("Invalid chan mode. (chan_mode=%d)\n", cfg->chan_mode);
    }
}

struct ascend_ub_msg_desc* ubdrv_copy_msg_from_rqe_to_chan(struct ascend_ub_jetty_ctrl *cfg,
    struct ascend_ub_msg_desc *src_desc, u32 seg_id)
{
    struct ubdrv_non_trans_chan *chan;
    struct ascend_ub_admin_chan *admin_chan;
    struct ascend_ub_msg_desc *dst_desc;
    u32 dev_id = 0, chan_id = 0;
    int ret;

     if ((cfg->chan_mode == UBDRV_MSG_CHAN_FOR_NON_TRANS) || ((cfg->chan_mode == UBDRV_MSG_CHAN_FOR_RAO))) {
        chan = cfg->msg_chan;
        dst_desc = chan->msg_desc;
        dev_id = chan->dev_id;
        chan_id = chan->chan_id;
    } else {
        admin_chan = cfg->msg_chan;
        dst_desc = admin_chan->msg_desc;
        dev_id = admin_chan->dev_id;
        chan_id = (u32)-1;
    }
    if (dst_desc->msg_num == src_desc->msg_num) {
        ubdrv_err("Recv repeat msg. (seg_id=%u;dev_id=%u;chan_id=%u;msg_num=%u)\n",
            seg_id, dev_id, chan_id, dst_desc->msg_num);
        return NULL;
    }
    ret = memcpy_s(dst_desc, cfg->recv_msg_len, src_desc, cfg->recv_msg_len);
    if (ret != 0) {
        ubdrv_err("Copy msg from rqe to chan fail. (seg_id=%u;dev_id=%u;chan_id=%u;ret=%d)\n",
            seg_id, dev_id, chan_id, ret);
        return NULL;
    }
    return dst_desc;
}

void ubdrv_rebuild_chan_send_jetty(u32 dev_id, u32 chan_id, struct ubdrv_msg_chan_stat *stat,
    struct ascend_ub_jetty_ctrl *jetty_ctrl, struct send_wr_cfg *wr_cfg)
{
    int ret;

    stat->tx_rebuild_jfs++;
    ret = ubdrv_rebuild_jfs_jfc(dev_id, jetty_ctrl);
    ubdrv_warn("Enter send jfs rebuild. (chan_id=%u;dev_id=%u;chan_mode=%u;dev_name=%s;ret=%d)\n",
        chan_id, dev_id, jetty_ctrl->chan_mode, jetty_ctrl->ubc_dev->dev_name, ret);
    if (ret != 0) {
        wr_cfg->jfs = NULL;
        return;
    }
    stat->tx_jfs_id = jetty_ctrl->jfs->jfs_id.id;
    stat->tx_jfs_jfc_id = jetty_ctrl->jfs_jfc->id;
    wr_cfg->jfs = jetty_ctrl->jfs;
    return;
}

STATIC void ubdrv_rebuild_chan_recv_jetty(u32 dev_id, u32 chan_id, struct ubdrv_msg_chan_stat *stat,
    struct ascend_ub_jetty_ctrl *jetty_ctrl, struct send_wr_cfg *wr_cfg)
{
    int ret;

    ret = ubdrv_rebuild_jfs_jfc(dev_id, jetty_ctrl);
    ubdrv_warn("Enter recv jfs rebuild(chan_id=%u;dev_id=%u;chan_mode=%u;dev_name=%s;ret=%d)\n",
        chan_id, dev_id, jetty_ctrl->chan_mode, jetty_ctrl->ubc_dev->dev_name, ret);
    if (ret != 0) {
        wr_cfg->jfs = NULL;
        return;
    }
    stat->rx_jfs_id = jetty_ctrl->jfs->jfs_id.id;
    stat->rx_jfs_jfc_id = jetty_ctrl->jfs_jfc->id;
    wr_cfg->jfs = jetty_ctrl->jfs;
    return;
}

int ubdrv_send_reply_msg(u32 dev_id, u32 chan_id, struct send_wr_cfg *wr_cfg,
    struct ascend_ub_jetty_ctrl *jetty_ctrl, struct ubdrv_msg_chan_stat *stat)
{
    struct ubcore_cr cr = {0};
    u32 retry_cnt = 0;
    int ret;

retry_recv_jfs_send:
    ret = ubdrv_post_send_wr(wr_cfg, dev_id);
    if (ret != 0) {
        ubdrv_err("Send reply result data failed. (ret=%d;dev_id=%u)", ret, dev_id);
        stat->rx_tx_post_err++;
        return ret;
    }

    ret = ubdrv_interval_poll_recv_jfs_jfc(jetty_ctrl->jfs_jfc, (u64)wr_cfg->user_ctx, MSG_MAX_WAIT_CNT, &cr, stat);
    if ((ret == 0) && (retry_cnt < ASCEND_TATIMEOUT_RETRY_CNT) && (cr.status == UBCORE_CR_ACK_TIMEOUT_ERR)) {
        retry_cnt++;
        stat->rx_tx_rebuild_jfs++;
        ubdrv_rebuild_chan_recv_jetty(dev_id, chan_id, stat, jetty_ctrl, wr_cfg);
        goto retry_recv_jfs_send;
    }
    return ((ret != 0) ? ret : cr.status);
}

int ubdrv_msg_result_process(int ret, int peer_status, u32 msg_type)
{
    if (ret != 0) {
        return ret;
    }
    if (peer_status == UB_MSG_PROCESS_SUCCESS) {
        return 0;
    } else if (peer_status == UB_MSG_PROCESS_FAILED) {
        ubdrv_warn("Msg send finish, process unsuccessful, Please check peer log. (process_ret=%d;msg_type=%u)\n",
            peer_status, msg_type);
        return peer_status;
    } else if (peer_status == UB_MSG_NULL_PROCSESS) {
        ubdrv_warn("Msg send finish, no process cb. (process_ret=%d;msg_type=%u)\n",
            peer_status, msg_type);
        return -EUNATCH;
    } else if (peer_status == UB_MSG_CHECKE_VERSION_FAILED) {
        ubdrv_warn("Msg send finish, perr check version not match. (process_ret=%d;msg_type=%u)\n",
            peer_status, msg_type);
        return peer_status;
    } else {
        ubdrv_warn("Msg send finish, invalid para. (process_ret=%d;msg_type=%u)\n",
            peer_status, msg_type);
        return -ETIMEDOUT;
    }
    return 0;
}