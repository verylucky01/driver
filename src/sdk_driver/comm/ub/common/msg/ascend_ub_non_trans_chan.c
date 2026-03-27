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
#include "ka_system_pub.h"
#include "ka_memory_pub.h"
#include "ka_errno_pub.h"
#include "ka_task_pub.h"
#include "ubcore_uapi.h"
#include "ubcore_types.h"
#include "comm_kernel_interface.h"
#include "ascend_ub_common.h"
#include "ascend_ub_dev.h"
#include "ascend_ub_main.h"
#include "ascend_ub_msg.h"
#include "ascend_ub_admin_msg.h"
#include "ascend_ub_msg_def.h"
#include "ascend_ub_jetty.h"
#include "ascend_ub_non_trans_chan.h"

struct ubdrv_non_trans_msg_client_ctrl g_non_trans_msg_client_ctrl[agentdrv_msg_client_max] = {0};

struct ubdrv_non_trans_msg_client_ctrl* get_global_non_trans_msg_client_ctrl(void)
{
    return g_non_trans_msg_client_ctrl;
}

void *ubdrv_get_non_trans_chan_handle(const struct ubdrv_non_trans_chan *chan)
{
    UBDRV_MSG_HANDLE msg_handle;

    msg_handle.bits.dev_id = chan->msg_dev->dev_id;
    msg_handle.bits.chan_id = chan->chan_id;
    msg_handle.bits.magic = UBDRV_MSG_MAGIC;
    msg_handle.bits.reserved = 0;

    return (void *)(uintptr_t)msg_handle.value;
}

u32 ubdrv_get_devid_by_non_trans_handle(const void *chan)
{
    const UBDRV_MSG_HANDLE *msg_handle = (UBDRV_MSG_HANDLE*)&chan;
    return msg_handle->bits.dev_id;
}

int ubdrv_get_devid_by_chan_pointer(const struct ubdrv_non_trans_chan *chan, u32 *dev_id)
{
    if ((chan == NULL) || (chan->msg_dev == NULL) || (dev_id == NULL)) {
        ubdrv_err("Get dev id failed, chan or dev_id is null.\n");
        return -EINVAL;
    }
    if (chan->msg_dev->dev_id >= ASCEND_UB_DEV_MAX_NUM) {
        ubdrv_err("Invalid dev id. (dev_id=%u)\n", chan->msg_dev->dev_id);
        return -EINVAL;
    }
    *dev_id = chan->msg_dev->dev_id;
    return 0;
}

int ubdrv_get_msg_chan_devid(void *msg_chan)
{
    if (msg_chan == NULL) {
        ubdrv_err("Check msg chan failed, msg chan is null.");
        return -EINVAL;
    }
    return ubdrv_get_devid_by_non_trans_handle(msg_chan);
}

void* ubdrv_get_msg_chan_priv(void *msg_chan)
{
    u32 dev_id = ubdrv_get_devid_by_non_trans_handle(msg_chan);
    struct ubdrv_non_trans_chan* chan = NULL;
    void *priv = NULL;
    int ret;

    ret = ubdrv_add_device_status_ref(dev_id);
    if (ret != 0) {
        return NULL;
    }

    chan = ubdrv_find_msg_chan_by_handle(msg_chan);
    if (chan == NULL) {
        ubdrv_sub_device_status_ref(dev_id);
        ubdrv_err("Check msg chan failed, invalid msg_chan handle.");
        return NULL;
    }
    priv = chan->priv;
    ubdrv_sub_device_status_ref(dev_id);
    return priv;
}

int ubdrv_set_msg_chan_priv(void *msg_chan, void *priv)
{
    u32 dev_id = ubdrv_get_devid_by_non_trans_handle(msg_chan);
    struct ubdrv_non_trans_chan* chan = NULL;
    int ret;

    ret = ubdrv_add_device_status_ref(dev_id);
    if (ret != 0) {
        return ret;
    }

    chan = ubdrv_find_msg_chan_by_handle(msg_chan);
    if (chan == NULL) {
        ubdrv_sub_device_status_ref(dev_id);
        ubdrv_err("Check msg chan failed, invalid msg_chan handle.");
        return -EINVAL;
    }
    chan->priv = priv;
    ubdrv_sub_device_status_ref(dev_id);
    return 0;
}

void ubdrv_non_trans_msg_chan_init(struct ascend_ub_msg_dev *msg_dev)
{
    struct ubdrv_non_trans_chan *chan;
    u32 chan_num;
    u32 i;

    chan_num = msg_dev->chan_cnt;
    for (i = 0; i < chan_num; ++i) {
        chan = &msg_dev->non_trans_chan[i];
        chan->chan_id = i;
        chan->dev_id = msg_dev->dev_id;
        chan->status = UBDRV_CHAN_IDLE;
        chan->msg_dev = msg_dev;
        chan->priv = NULL;
        chan->chan_mode = UBDRV_MSG_CHAN_FOR_NON_TRANS;
        chan->chan_stat.dev_id = msg_dev->dev_id;
        chan->chan_stat.chan_id = i;
        ka_task_mutex_init(&chan->tx_mutex);
        ka_task_mutex_init(&chan->rx_mutex);
    }
}

void ubdrv_non_trans_msg_chan_uninit(struct ascend_ub_msg_dev *msg_dev)
{
    struct ubdrv_non_trans_chan *chan;
    u32 chan_num;
    u32 i;

    chan_num = msg_dev->chan_cnt;
    for (i = 0; i < chan_num; ++i) {
        chan = &msg_dev->non_trans_chan[i];
        ka_task_mutex_destroy(&chan->rx_mutex);
        ka_task_mutex_destroy(&chan->tx_mutex);
        chan->chan_id = 0;
        chan->dev_id = 0;
        chan->status = 0;
        chan->msg_dev = NULL;
    }
}

void ubdrv_rao_msg_chan_init(struct ascend_ub_msg_dev *msg_dev)
{
    struct ubdrv_non_trans_chan *chan;
    u32 i;

    for (i = 0; i < DEVDRV_RAO_CLIENT_MAX; ++i) {
        chan = &msg_dev->rao.rao_msg_chan[i];
        chan->chan_id = i;
        chan->dev_id = msg_dev->dev_id;
        chan->status = UBDRV_CHAN_IDLE;
        chan->msg_dev = msg_dev;
        chan->priv = NULL;
        chan->chan_mode = UBDRV_MSG_CHAN_FOR_RAO;
        chan->chan_stat.dev_id = msg_dev->dev_id;
        chan->chan_stat.chan_id = i;
        ka_task_mutex_init(&chan->tx_mutex);
        ka_task_mutex_init(&chan->rx_mutex);
    }
}

void ubdrv_rao_msg_chan_uninit(struct ascend_ub_msg_dev *msg_dev)
{
    struct ubdrv_non_trans_chan *chan;
    u32 i;

    for (i = 0; i < DEVDRV_RAO_CLIENT_MAX; ++i) {
        chan = &msg_dev->rao.rao_msg_chan[i];
        chan->chan_id = 0;
        chan->dev_id = 0;
        chan->status = UBDRV_CHAN_IDLE;
        chan->msg_dev = NULL;
        chan->priv = NULL;
        ka_task_mutex_destroy(&chan->rx_mutex);
        ka_task_mutex_destroy(&chan->tx_mutex);
    }
}


STATIC int ubdrv_set_non_trans_cfg(struct ubdrv_non_trans_chan *chan,
    struct ascend_ub_sync_jetty *msg_jetty, const struct ubdrv_create_non_trans_cmd *cmd)
{
    struct ubcore_device *ubc_dev = chan->msg_dev->ubc_dev;
    u32 token;
    int ret;

    ret = ubdrv_get_local_token(chan->dev_id, &token);
    if (ret != 0) {
        return ret;
    }
    msg_jetty->send_jetty.jfr_msg_depth = cmd->cq_depth;
    msg_jetty->send_jetty.jfs_msg_depth = cmd->sq_depth;
    msg_jetty->send_jetty.recv_msg_len = cmd->cq_size;
    msg_jetty->send_jetty.send_msg_len = cmd->sq_size;
    msg_jetty->send_jetty.ubc_dev = ubc_dev;
    msg_jetty->send_jetty.eid_index = chan->msg_dev->idev->eid_index;
    msg_jetty->send_jetty.msg_chan = (void*)chan;
    msg_jetty->send_jetty.chan_mode = cmd->chan_mode;
    msg_jetty->send_jetty.rao_info = &chan->rao_info;
    msg_jetty->send_jetty.token_value = token;

    msg_jetty->recv_jetty.jfr_msg_depth = cmd->cq_depth;
    msg_jetty->recv_jetty.jfs_msg_depth = cmd->sq_depth;
    msg_jetty->recv_jetty.recv_msg_len = cmd->cq_size;
    msg_jetty->recv_jetty.send_msg_len = cmd->sq_size;
    msg_jetty->recv_jetty.ubc_dev = ubc_dev;
    msg_jetty->recv_jetty.eid_index = chan->msg_dev->idev->eid_index;
    msg_jetty->recv_jetty.msg_chan = (void*)chan;
    msg_jetty->recv_jetty.chan_mode = cmd->chan_mode;
    msg_jetty->recv_jetty.rao_info = &chan->rao_info;
    msg_jetty->recv_jetty.token_value = token;
    if (cmd->chan_mode == UBDRV_MSG_CHAN_FOR_RAO) {
        msg_jetty->send_jetty.access = MEM_ACCESS_ALL;
        msg_jetty->recv_jetty.access = MEM_ACCESS_ALL;
    } else {
        msg_jetty->send_jetty.access = MEM_ACCESS_LOCAL;
        msg_jetty->recv_jetty.access = MEM_ACCESS_LOCAL;
    }
    return 0;
}

STATIC int ubdrv_send_nontrans_reply_msg(u32 dev_id, struct ubdrv_non_trans_chan *chan, struct ascend_ub_jetty_ctrl *jetty_ctrl,
    struct ascend_ub_msg_desc *src_desc, struct ubdrv_msg_chan_stat *stat)
{
    struct ascend_ub_msg_desc *jfs_desc;
    struct send_wr_cfg wr_cfg = {0};
    u32 seg_id = 0;
    int ret;

    jfs_desc = ubdrv_get_send_desc(jetty_ctrl, seg_id);  // jfs seg_id is 0
    ret = memcpy_s(jfs_desc, jetty_ctrl->send_msg_len, src_desc, jetty_ctrl->recv_msg_len);
    if (ret != 0) {
        ubdrv_err("Copy result data to jfs_desc failed. (ret=%d;dst_len=%u;src_len=%u)",
            ret, jetty_ctrl->send_msg_len, jetty_ctrl->recv_msg_len);
        return ret;
    }
    wr_cfg.user_ctx = src_desc->msg_num;
    wr_cfg.tjetty = chan->s_tjetty;
    wr_cfg.sva = (u64)jfs_desc;
    wr_cfg.sseg = jetty_ctrl->send_seg;
    wr_cfg.tseg = NULL; // Not used in send opcode
    wr_cfg.jfs = jetty_ctrl->jfs;
    wr_cfg.len = (jetty_ctrl->send_msg_len - ASCEND_UB_MSG_DESC_LEN);
    return ubdrv_send_reply_msg(dev_id, chan->chan_id, &wr_cfg, jetty_ctrl, stat);
}

void ubdrv_non_trans_recv_prepare_process(struct ascend_ub_jetty_ctrl *cfg, struct ascend_ub_msg_desc *desc, u32 seg_id)
{
    struct ubdrv_non_trans_chan *chan = (struct ubdrv_non_trans_chan*)cfg->msg_chan;
    u32 max_send_len = chan->msg_jetty.send_jetty.send_msg_len - sizeof(struct ascend_ub_msg_desc);
    struct ubdrv_msg_chan_stat *stat = &chan->chan_stat;
    u32 dev_id = chan->msg_dev->dev_id;
    u32 cost_time;
    int ret = 0;

    if (ka_unlikely(ubdrv_get_device_status(dev_id)) == UBDRV_DEVICE_DEAD) {
        desc->status =  UB_MSG_RECV_ABORT;
        ubdrv_warn("Device is dead. (dev_id=%u;chan_id=%u;msg_type=%u)\n", dev_id,
            chan->chan_id, chan->msg_type);
        return;
    }

    ka_task_mutex_lock(&chan->rx_mutex);
    if (chan->rx_msg_process == NULL) {
        ka_task_mutex_unlock(&chan->rx_mutex);
        ubdrv_err("Process func is NULL. (dev_id=%u;chan_id=%u;msg_type=%u)\n", dev_id,
            chan->chan_id, chan->msg_type);
        desc->status = UB_MSG_NULL_PROCSESS;
        stat->rx_null_process++;
        goto reply_non_trans_msg;
    }
    stat->rx_stamp = (u64)ka_jiffies;
    ret = chan->rx_msg_process(ubdrv_get_non_trans_chan_handle(chan), (void*)desc->user_data,
        desc->in_data_len, desc->out_data_len, &desc->real_data_len);
    ka_task_mutex_unlock(&chan->rx_mutex);
    cost_time = ubdrv_record_resq_time(stat->rx_stamp, "non trans recv prepare process stamp", UBDRV_SCEH_RESP_TIME);
    if (cost_time > stat->rx_max_time) {
        stat->rx_max_time = cost_time;
    }
    if ((ret != 0) || (desc->real_data_len > max_send_len)) {
        desc->status = UB_MSG_PROCESS_FAILED;
        stat->rx_process_err++;
        ubdrv_err("Non trans process func error. (dev_id=%u;ret=%d;real_len=%u;max_len=%u;chan_id=%u;chan_type=%u)\n",
            dev_id, ret, desc->real_data_len, max_send_len, chan->chan_id, chan->msg_type);
        goto reply_non_trans_msg;
    }
    desc->status = UB_MSG_PROCESS_SUCCESS;

reply_non_trans_msg:
    ret = ubdrv_send_nontrans_reply_msg(dev_id, chan, cfg, desc, stat);
    if (ret != 0) {
        ubdrv_err("Send non trans reply msg failed. (ret=%d;dev_id=%u;chan_id=%u;chan_type=%u;msg_num=%u)\n",
            ret, dev_id, chan->chan_id, chan->msg_type, desc->msg_num);
    }
    return;
}

STATIC struct ubdrv_non_trans_chan* ubdrv_get_non_trans_chan_by_jfc(const struct ubcore_jfc *jfc)
{
    struct ascend_ub_jetty_ctrl *jetty;
    struct ubdrv_non_trans_chan *chan;

    jetty = (struct ascend_ub_jetty_ctrl *)jfc->jfc_cfg.jfc_context;
    chan = (struct ubdrv_non_trans_chan*)jetty->msg_chan;
    return chan;
}

void ubdrv_non_trans_jfce_recv_handle(struct ubcore_jfc *jfc)
{
    struct ubdrv_non_trans_chan *chan = NULL;
    struct ubdrv_msg_chan_stat *stat = NULL;

    if ((jfc == NULL) || (jfc->jfc_cfg.jfc_context == NULL)) {
        ubdrv_err("Jfce is null in non trans recv jfce handle.\n");
        return;
    }

    chan = ubdrv_get_non_trans_chan_by_jfc(jfc);
    if (chan == NULL) {
        ubdrv_warn("Non trans chan is null in jfce handle.\n");
        return;
    }
    ka_base_atomic_add(1, &chan->user_cnt);
    if (chan->status != UBDRV_CHAN_ENABLE) {
        ka_base_atomic_sub(1, &chan->user_cnt);
        ubdrv_warn("Chan is unexpected status in jfce handle. (status=%d;dev_id=%u;chan_id=%u;chan_type=%u)\n",
            chan->status, chan->dev_id, chan->chan_id, chan->msg_type);
        return;
    }

    stat = &chan->chan_stat;
    stat->rx_total++;
    stat->rx_work_stamp = (u64)ka_jiffies;
    ka_task_queue_work(chan->work_queue, &chan->non_trans_r_work.work);
    ka_base_atomic_sub(1, &chan->user_cnt);
    return;
}

int ubdrv_create_non_trans_jetty(struct ubcore_device *ubc_dev,
    struct ubdrv_non_trans_chan *chan, struct ubdrv_create_non_trans_cmd *cmd)
{
    struct ascend_ub_sync_jetty *msg_jetty = &(chan->msg_jetty);
    int ret;

    ret = ubdrv_set_non_trans_cfg(chan, msg_jetty, cmd);
    if (ret != 0) {
        return ret;
    }
    chan->status = UBDRV_CHAN_OCCUPY;
    ret = ubdrv_sync_send_jetty_init(&msg_jetty->send_jetty, chan->send_jfce_handler);
    if (ret != 0) {
        ubdrv_err("Non trans send jetty init failed. (ret=%d;chan_id=%u)\n", ret, chan->chan_id);
        goto set_idle;
    }
    ret = ubdrv_sync_recv_jetty_init(&msg_jetty->recv_jetty, 0, chan->recv_jfce_handler);
    if (ret != 0) {
        ubdrv_err("Non trans recv jetty init failed. (ret=%d;chan_id=%u)\n", ret, chan->chan_id);
        goto send_jetty_uninit;
    }
    chan->work_queue = ka_task_create_workqueue("ub_msg_workqueue");
    if (chan->work_queue == NULL) {
        ubdrv_err("Create non trans msg chan workqueue fail. (dev_id=%u;chan_id=%u)\n",
            chan->dev_id, chan->chan_id);
        ret = -ENOMEM;
        goto recv_jetty_uninit;
    }
    ubdrv_sync_jetty_init(msg_jetty);
    chan->non_trans_r_work.jfc = msg_jetty->recv_jetty.jfr_jfc;
    chan->non_trans_r_work.stat = &chan->chan_stat;
    KA_TASK_INIT_WORK(&chan->non_trans_r_work.work, ubdrv_jfce_recv_work);
    ubdrv_debug("Non trans jetty. (chan_id=%u; send.jfr_id=%u)\n", chan->chan_id, msg_jetty->send_jetty.jfr->jfr_id.id);
    return 0;

recv_jetty_uninit:
    ubdrv_sync_recv_jetty_uninit(&msg_jetty->recv_jetty);
send_jetty_uninit:
    ubdrv_sync_send_jetty_uninit(&msg_jetty->send_jetty);
set_idle:
    chan->status = UBDRV_CHAN_IDLE;
    return ret;
}

void ubdrv_delete_non_trans_jetty(struct ubdrv_non_trans_chan* chan)
{
    struct ascend_ub_sync_jetty *msg_jetty = &(chan->msg_jetty);
    int ret;

    ret = ubdrv_clear_jetty(msg_jetty->send_jetty.jfs);
    if (ret != 0) {
        ubdrv_err("Flush send jetty failed. (ret=%d;dev_id=%u;chan_id=%u)\n",
            ret, chan->msg_dev->dev_id, chan->chan_id);
    }
    ret = ubdrv_clear_jetty(msg_jetty->recv_jetty.jfs);
    if (ret != 0) {
        ubdrv_err("Flush recv jetty failed. (ret=%ddev_id=%u;chan_id=%u)\n",
            ret, chan->msg_dev->dev_id, chan->chan_id);
    }
    ka_task_cancel_work_sync(&chan->non_trans_r_work.work);
    chan->non_trans_r_work.jfc = NULL;
    msg_jetty->send_jetty.msg_chan = NULL;
    msg_jetty->recv_jetty.msg_chan = NULL;
    ubdrv_sync_jetty_uninit(msg_jetty);
    ka_task_destroy_workqueue(chan->work_queue);
    chan->work_queue = NULL;
    ubdrv_sync_recv_jetty_uninit(&msg_jetty->recv_jetty);
    ubdrv_sync_send_jetty_uninit(&msg_jetty->send_jetty);
    chan->status = UBDRV_CHAN_IDLE;
}

STATIC int ubdrv_alloc_non_trans_chan_msg_desc(struct ubdrv_non_trans_chan *chan, u32 len)
{
    if (chan->chan_mode != UBDRV_MSG_CHAN_FOR_NON_TRANS) {
        return 0;
    }
    chan->msg_desc = ubdrv_kzalloc(len, KA_GFP_KERNEL);
    if (chan->msg_desc == NULL) {
        ubdrv_err("Alloc non trans chan mem failed. (dev_id=%u;chan_id=%u)\n", chan->dev_id, chan->chan_id);
        return -ENOMEM;
    }
    return 0;
}

STATIC void ubdrv_free_non_trans_chan_msg_desc(struct ubdrv_non_trans_chan *chan)
{
    if (chan->chan_mode == UBDRV_MSG_CHAN_FOR_NON_TRANS) {
        ubdrv_kfree(chan->msg_desc);
        chan->msg_desc = NULL;
    }
    return;
}

int ubdrv_init_non_trans_chan(struct ubdrv_non_trans_chan *chan, struct ubcore_device *ubc_dev,
    struct ubdrv_create_non_trans_cmd *cmd, u32 dev_id, u32 msg_type)
{
    struct jetty_exchange_data *s_data = &cmd->s_jetty_data;
    struct jetty_exchange_data *r_data = &cmd->r_jetty_data;
    u32 eid_index = chan->msg_dev->idev->eid_index;
    struct ubcore_target_seg *tseg;
    struct ubcore_tjetty *tjetty;

    if (ubdrv_alloc_non_trans_chan_msg_desc(chan, cmd->cq_size) != 0) {
        return -ENOMEM;
    }
    tjetty = ascend_import_jfr(ubc_dev, eid_index, s_data);
    if (KA_IS_ERR_OR_NULL(tjetty)) {
        ubdrv_err("Import sjetty failed. (dev_id=%u;chan_id=%u;errno=%ld)\n", dev_id, chan->chan_id, KA_PTR_ERR(tjetty));
        goto free_msg_desc;
    }
    chan->s_tjetty = tjetty;
    if (s_data->seg.len != 0) {
        tseg = ascend_import_seg(ubc_dev, s_data);
        if (KA_IS_ERR_OR_NULL(tseg)) {
            ubdrv_err("Import seg failed. (dev_id=%u;chan_id=%u;errno=%ld)\n", dev_id, chan->chan_id, KA_PTR_ERR(tseg));
            goto unimport_r_jetty;
        }
        chan->s_tseg = tseg;
    }

    tjetty = ascend_import_jfr(ubc_dev, eid_index, r_data);
    if (KA_IS_ERR_OR_NULL(tjetty)) {
        ubdrv_err("Import rjetty failed. (dev_id=%u;chan_id=%u;errno=%ld)\n", dev_id, chan->chan_id, KA_PTR_ERR(tjetty));
        goto unimport_r_tseg;
    }
    chan->r_tjetty = tjetty;
    chan->msg_type = msg_type;
    chan->status = UBDRV_CHAN_ENABLE;
    ka_base_atomic_set(&chan->user_cnt, 0);
    ka_base_atomic_set(&chan->msg_num, 0);
    return 0;

unimport_r_tseg:
    if (chan->s_tseg != NULL) {
        (void)ubcore_unimport_seg(chan->s_tseg);
        chan->s_tseg = NULL;
    }
unimport_r_jetty:
    (void)ubcore_unimport_jfr(chan->s_tjetty);
    chan->s_tjetty = NULL;
free_msg_desc:
    ubdrv_free_non_trans_chan_msg_desc(chan);
    return -EINVAL;
}

void ubdrv_uninit_non_trans_chan(struct ubdrv_non_trans_chan *chan, u32 dev_id)
{
    int ret;

    chan->status = UBDRV_CHAN_DISABLE;
    ubdrv_wait_chan_jfce_user_cnt(&chan->user_cnt, dev_id, chan->chan_id);
    ret = ubcore_unimport_jfr(chan->s_tjetty);
    if (ret != 0) {
        ubdrv_err("ubcore_unimport_jfr failed. (ret=%d;chan_id=%u;dev_id=%u)\n",
            ret, chan->chan_id, dev_id);
    }
    chan->s_tjetty = NULL;
    ret = ubcore_unimport_jfr(chan->r_tjetty);
    if (ret != 0) {
        ubdrv_err("ubcore_unimport_jfr failed. (ret=%d;chan_id=%u;dev_id=%u)\n",
            ret, chan->chan_id, dev_id);
    }
    chan->r_tjetty = NULL;
    if (chan->s_tseg != NULL) {
        ret = ubcore_unimport_seg(chan->s_tseg);
        if (ret != 0) {
            ubdrv_err("ubcore_unimport_seg failed. (ret=%d;chan_id=%u;dev_id=%u)\n",
                ret, chan->chan_id, dev_id);
        }
    }
    chan->s_tseg = NULL;
    ka_base_atomic_set(&chan->user_cnt, 0);
    ka_base_atomic_set(&chan->msg_num, 0);
    ubdrv_free_non_trans_chan_msg_desc(chan);
    return;
}

int ubdrv_prepare_non_trans_jetty_info(struct ubdrv_non_trans_chan* chan,
    struct ubdrv_create_non_trans_cmd* cmd)
{
    int ret;

    ret = ubdrv_get_send_jetty_info(&chan->msg_jetty.send_jetty, &cmd->s_jetty_data);
    if (ret != 0) {
        ubdrv_err("Get send jetty info failed. (ret=%d;chan_id=%u;dev_id=%u)\n",
            ret, chan->chan_id, chan->msg_dev->dev_id);
        return ret;
    }
    ret = ubdrv_get_recv_jetty_info(&chan->msg_jetty.recv_jetty, &cmd->r_jetty_data);
    if (ret != 0) {
        ubdrv_err("Get recv jetty info failed. (ret=%d;chan_id=%u;dev_id=%u)\n",
            ret, chan->chan_id, chan->msg_dev->dev_id);
        return ret;
    }
    return ret;
}

STATIC bool ubdrv_check_msg_chan_handle_valid(const void *chan_handle)
{
    const UBDRV_MSG_HANDLE *msg_handle;
    struct ascend_ub_msg_dev *msg_dev;
    u32 dev_id;
    u32 chan_id;
    u64 magic;

    msg_handle = (const UBDRV_MSG_HANDLE *)&chan_handle;
    dev_id = msg_handle->bits.dev_id;
    if (dev_id >= ASCEND_UB_DEV_MAX_NUM) {
        ubdrv_err("Invalid chan handle. (dev_id=%u)\n", dev_id);
        return false;
    }
    msg_dev = ubdrv_get_msg_dev_by_devid(dev_id);
    if (msg_dev == NULL) {
        ubdrv_err("Get msg dev by devid failed, msg_dev is NULL. (dev_id=%u)\n", dev_id);
        return false;
    }
    chan_id = msg_handle->bits.chan_id;
    if (chan_id >= msg_dev->chan_cnt) {
        ubdrv_err("Invalid chan handle. (dev_id=%u;chan_id=%u)\n", dev_id, chan_id);
        return false;
    }
    magic = msg_handle->bits.magic;
    if (magic != UBDRV_MSG_MAGIC) {
        ubdrv_err("Invalid chan handle. (dev_id=%u;chan_id=%u;magic=%llu)\n",
            dev_id, chan_id, magic);
        return false;
    }
    return true;
}

struct ubdrv_non_trans_chan* ubdrv_find_msg_chan_by_handle(void *chan_handle)
{
    struct ubdrv_non_trans_chan *chan = NULL;
    struct ascend_ub_msg_dev *msg_dev;
    const UBDRV_MSG_HANDLE *msg_handle;
    u32 dev_id;
    u32 chan_id;

    if (ubdrv_check_msg_chan_handle_valid(chan_handle) == false) {
        ubdrv_err("Check msg chan handle failed.");
        return NULL;
    }

    msg_handle = (const UBDRV_MSG_HANDLE *)&chan_handle;
    dev_id = msg_handle->bits.dev_id;
    msg_dev = ubdrv_get_msg_dev_by_devid(dev_id);
    chan_id = msg_handle->bits.chan_id;
    chan = &msg_dev->non_trans_chan[chan_id];
    return chan;
}

struct agentdrv_non_trans_msg_client *ubdrv_find_non_trans_client_by_type(u32 type)
{
    if (type >= agentdrv_msg_client_max) {
        ubdrv_err("Msg client type is not support yet. (msg_client_type=%u)\n", type);
        return NULL;
    }

    if (g_non_trans_msg_client_ctrl[type].status == UBDRV_NON_TRANS_CLIENT_DISABLE) {
        ubdrv_err("Non_trans client type is not registered. (type=%u)\n", type);
        return NULL;
    }
    return &g_non_trans_msg_client_ctrl[type].non_trans_msg_client;
}

STATIC int ubdrv_non_trans_client_init(struct ubdrv_non_trans_chan *chan, u32 msg_type)
{
    struct agentdrv_non_trans_msg_client *non_trans_client = NULL;
    int ret;

    non_trans_client = ubdrv_find_non_trans_client_by_type(msg_type);
    if (non_trans_client == NULL) {
        ubdrv_err("Find non trans message client failed. (dev_id=%d;type=%u;chan_id=%u)\n",
            chan->msg_dev->dev_id, msg_type, chan->chan_id);
        return -EINVAL;
    }
    if (non_trans_client->init_non_trans_msg_chan != NULL) {
        ret = non_trans_client->init_non_trans_msg_chan(ubdrv_get_non_trans_chan_handle(chan));
        if (ret != 0) {
            ubdrv_err("Call non trans client init failed. (dev_id=%d;ret=%d;type=%u;chan_id=%u)\n",
                chan->msg_dev->dev_id, ret, msg_type, chan->chan_id);
            return ret;
        }
    } else {
        ubdrv_warn("Non trans client init func is NULL. (dev_id=%d;type=%u;chan_id=%u)\n",
            chan->msg_dev->dev_id, msg_type, chan->chan_id);
    }
    chan->rx_msg_process = non_trans_client->non_trans_msg_process;
    return 0;
}

STATIC int ubdrv_non_trans_client_uninit(struct ubdrv_non_trans_chan *chan)
{
    struct agentdrv_non_trans_msg_client *non_trans_client = NULL;
    u32 msg_type = chan->msg_type;

    non_trans_client = ubdrv_find_non_trans_client_by_type(msg_type);
    if (non_trans_client == NULL) {
        ubdrv_err("Find non trans message client failed. (dev_id=%d;type=%u;chan_id=%u)\n",
            chan->msg_dev->dev_id, msg_type, chan->chan_id);
        return -EINVAL;
    }
    if (non_trans_client->uninit_non_trans_msg_chan != NULL) {
        non_trans_client->uninit_non_trans_msg_chan(ubdrv_get_non_trans_chan_handle(chan));
    } else {
        ubdrv_warn("Non trans client uninit func is NULL. (dev_id=%d;type=%u;chan_id=%u)\n",
            chan->msg_dev->dev_id, msg_type, chan->chan_id);
    }
    chan->rx_msg_process = NULL;
    chan->msg_type = 0;
    return 0;
}

STATIC void ubdrv_rao_chan_jfce_send_handle(struct ubcore_jfc *jfc)
{
    return;
}

STATIC void ubdrv_rao_chan_jfce_recv_handle(struct ubcore_jfc *jfc)
{
    return;
}

int ubdrv_msg_alloc_msg_queue_process(struct ubdrv_non_trans_chan *chan, struct ubdrv_create_non_trans_cmd *cmd)
{
    struct ascend_ub_msg_dev *msg_dev = chan->msg_dev;
    int ret;

    ret = ubdrv_create_non_trans_jetty(msg_dev->ubc_dev, chan, cmd);
    if (ret != 0) {
        ubdrv_err("Create non trans jetty failed. (ret=%d;dev_id=%u;chan_id=%u;client_type=%u)\n",
            ret, msg_dev->dev_id, chan->chan_id, cmd->msg_type);
        return -EINVAL;
    }
    ret = ubdrv_init_non_trans_chan(chan, msg_dev->ubc_dev, cmd, msg_dev->dev_id, cmd->msg_type);
    if (ret != 0) {
        ubdrv_err("Init non trans chan failed. (ret=%d;dev_id=%u;chan_id=%u;client_type=%u)\n",
            ret, msg_dev->dev_id, chan->chan_id, cmd->msg_type);
        goto delet_jetty;
    }
    /* must rearm jfc at last */
    ret = ubdrv_rearm_sync_jfc(&chan->msg_jetty);
    if (ret != 0) {
        ubdrv_err("Non trans chan rearm jfc failed. (dev_id=%u;chan_id=%u)\n", msg_dev->dev_id, chan->chan_id);
        goto get_info_err;
    }

    ret = ubdrv_prepare_non_trans_jetty_info(chan, cmd);
    if (ret != 0) {
        ubdrv_err("Prepare non trans jetty info failed. (ret=%d;dev_id=%u)\n", ret, msg_dev->dev_id);
        goto get_info_err;
    }
    ubdrv_record_chan_jetty_info(&chan->chan_stat, &chan->msg_jetty);
    return 0;
get_info_err:
    ubdrv_uninit_non_trans_chan(chan, msg_dev->dev_id);
delet_jetty:
    ubdrv_delete_non_trans_jetty(chan);
    return ret;
}

int ubdrv_msg_alloc_msg_queue_para_check(struct ascend_ub_msg_dev *msg_dev, struct ascend_ub_msg_desc *desc)
{
    struct ubdrv_create_non_trans_cmd *cmd;
    struct ubdrv_non_trans_chan *chan;

    if (desc->in_data_len != sizeof(struct ubdrv_create_non_trans_cmd)) {
        ubdrv_err("Recv data len invalid. (dev_id=%u;len=%u)\n",
            msg_dev->dev_id, desc->in_data_len);
        return -EINVAL;
    }
    cmd = (struct ubdrv_create_non_trans_cmd*)&desc->user_data;
    if (cmd->chan_mode == UBDRV_MSG_CHAN_FOR_RAO) {
        if (cmd->chan_id >= DEVDRV_RAO_CLIENT_MAX) {
            ubdrv_err("Chan id is invalid. (dev_id=%u;client_type=%u;chan_id=%u)\n",
                msg_dev->dev_id, cmd->msg_type, cmd->chan_id);
            return -EINVAL;
        }
        chan = &msg_dev->rao.rao_msg_chan[cmd->chan_id];
        /* rao chan addr info from user */
        if ((chan->rao_info.len == 0) || chan->rao_info.va == 0) {
            ubdrv_err("RAO user va or len invalid. (dev_id=%u;client_type=%u;chan_id=%u;len=0x%llx)\n",
                msg_dev->dev_id, cmd->msg_type, cmd->chan_id, chan->rao_info.len);
            return -EINVAL;
        }
        cmd->sq_size = (u32)chan->rao_info.len;
        cmd->cq_size = (u32)chan->rao_info.len;
    } else {
        if (cmd->chan_id >= msg_dev->chan_cnt) {
            ubdrv_err("Chan id is invalid. (dev_id=%u;client_type=%u;chan_id=%u)\n",
                msg_dev->dev_id, cmd->msg_type, cmd->chan_id);
            return -EINVAL;
        }
        chan = &msg_dev->non_trans_chan[cmd->chan_id];
    }

    if ((cmd->sq_size != cmd->cq_size) || (cmd->sq_size <= ASCEND_UB_MSG_DESC_LEN)) {
        ubdrv_err("Chan info is invalid. (dev_id=%u;client_type=%u;chan_id=%u;sq_size=%u;cq_size=%u)\n",
            msg_dev->dev_id, cmd->msg_type, cmd->chan_id, cmd->sq_size, cmd->cq_size);
        return -EINVAL;
    }
    if (chan->status != UBDRV_CHAN_IDLE) {
        ubdrv_err("Chan status is error. (dev_id=%u;client_type=%u;chan_id=%u;chan_status=%u)\n",
            msg_dev->dev_id, cmd->msg_type, cmd->chan_id, chan->status);
        return -EINVAL;
    }

    return 0;
}

/* device interface alloc non_trans_chan */
int ubdrv_msg_alloc_msg_queue(struct ascend_ub_msg_dev *msg_dev, void *data)
{
    struct ascend_ub_msg_desc *desc = (struct ascend_ub_msg_desc*)data;
    struct ubdrv_create_non_trans_cmd *cmd;
    struct ubdrv_non_trans_chan *chan;
    int ret;

    ret = ubdrv_msg_alloc_msg_queue_para_check(msg_dev, desc);
    if (ret != 0) {
        ubdrv_err("Device alloc msg chan para check failed. (ret=%d;dev_id=%u)\n", ret, msg_dev->dev_id);
        return ret;
    }
    cmd = (struct ubdrv_create_non_trans_cmd*)&desc->user_data;
    if (cmd->chan_mode == UBDRV_MSG_CHAN_FOR_RAO) {
        chan = &msg_dev->rao.rao_msg_chan[cmd->chan_id];
        chan->send_jfce_handler = ubdrv_rao_chan_jfce_send_handle;
        chan->recv_jfce_handler = ubdrv_rao_chan_jfce_recv_handle;
    } else {
        chan = &msg_dev->non_trans_chan[cmd->chan_id];
        chan->send_jfce_handler = NULL;
        chan->recv_jfce_handler = ubdrv_non_trans_jfce_recv_handle;
    }

    ret = ubdrv_msg_alloc_msg_queue_process(chan, cmd);
    if (ret != 0) {
        ubdrv_err("Device alloc msg chan failed. (ret=%d;dev_id=%u)\n", ret, msg_dev->dev_id);
        return ret;
    }

    desc->real_data_len = sizeof(struct ubdrv_create_non_trans_cmd);
    ubdrv_info("Device alloc msg chan success. (dev_id=%u;chan_id=%u;msg_type=%u)\n",
        msg_dev->dev_id, chan->chan_id, chan->msg_type);
    return 0;
}

int ubdrv_enable_device_msg_chan(struct ascend_ub_msg_dev *msg_dev, void *data)
{
    struct ascend_ub_msg_desc *desc = (struct ascend_ub_msg_desc*)data;
    struct ubdrv_create_non_trans_cmd *cmd = (struct ubdrv_create_non_trans_cmd*)&desc->user_data;
    struct ubdrv_non_trans_chan *chan;
    u32 len = sizeof(struct ubdrv_create_non_trans_cmd);
    u32 dev_id = msg_dev->dev_id;
    u32 chan_id;
    int ret;

    if (desc->in_data_len != len) {
        ubdrv_err("Recv data len invalid, when enable device chan. (dev_id=%u;len=%u)\n", dev_id, desc->in_data_len);
        return -EINVAL;
    }
    if (cmd->chan_id >= msg_dev->chan_cnt) {
        ubdrv_err("Invalid chan id, when enable device chan. (dev_id=%u;chan_id=%u)\n", dev_id, cmd->chan_id);
        return -EINVAL;
    }
    chan = &msg_dev->non_trans_chan[cmd->chan_id];
    chan_id = cmd->chan_id;
    ret = ubdrv_non_trans_client_init(chan, cmd->msg_type);
    if (ret != 0) {
        ubdrv_err("Device trans init client failed. (dev_id=%u;chan_id=%u;ret=%d;msg_type=%u)\n",
            dev_id, chan_id, ret, cmd->msg_type);
        return ret;
    }
    ubdrv_info("Enable non trans msg chan success. (dev_id=%u;chan_id=%u;msg_type=%u)\n",
        dev_id, chan_id, chan->msg_type);
    return 0;
}

int ubdrv_free_non_trans_chan_process(struct ubdrv_non_trans_chan *chan, enum ubdrv_msg_chan_mode chan_mode,
    u32 dev_id, u32 chan_id)
{
    u32 msg_type = chan->msg_type;
    int ret = 0;

    ka_task_mutex_lock(&chan->tx_mutex);
    if (chan->status != UBDRV_CHAN_ENABLE) {
        ubdrv_warn("Chan status is not enable. (dev_id=%u;chan_id=%u;chan_status=%u)\n",
            dev_id, chan->chan_id, chan->status);
        ka_task_mutex_unlock(&chan->tx_mutex);
        return 0;
    }
    if (chan_mode != UBDRV_MSG_CHAN_FOR_RAO) {
        ret = ubdrv_non_trans_client_uninit(chan);
        if (ret != 0) {
            ubdrv_warn("ubdrv_non_trans_client_uninit failed. (ret=%d;dev_id=%u;chan_id=%u)\n",
                ret, dev_id, chan->chan_id);
        }
    }
    ubdrv_uninit_non_trans_chan(chan, dev_id);
    ubdrv_delete_non_trans_jetty(chan);
    ka_task_mutex_unlock(&chan->tx_mutex);
    ubdrv_clear_chan_dfx(&chan->chan_stat);
    ubdrv_info("Device free msg chan success. (dev_id=%u;chan_id=%u;msg_type=%u)\n",
        dev_id, chan->chan_id, msg_type);
    return 0;
}

int ubdrv_msg_free_msg_queue(struct ascend_ub_msg_dev *msg_dev, void *data)
{
    struct ascend_ub_msg_desc *desc = (struct ascend_ub_msg_desc*)data;
    struct ubdrv_free_non_trans_cmd *cmd = (struct ubdrv_free_non_trans_cmd*)desc->user_data;
    struct ubdrv_non_trans_chan *chan;
    u32 max_chan_cnt;

    if (desc->in_data_len != sizeof(struct ubdrv_free_non_trans_cmd)) {
        ubdrv_err("Recv data len invalid. (dev_id=%u;chan_id=%u;len=%u)\n",
            msg_dev->dev_id, cmd->chan_id, desc->in_data_len);
        return -EINVAL;
    }
    if (cmd->chan_mode == UBDRV_MSG_CHAN_FOR_RAO) {
        max_chan_cnt = DEVDRV_RAO_CLIENT_MAX;
    } else {
        max_chan_cnt = msg_dev->chan_cnt;
    }
    if (cmd->chan_id >= max_chan_cnt) {
        ubdrv_err("Invalid chan_id. (chan_id=%u)\n", cmd->chan_id);
        return -EINVAL;
    }
    if (cmd->chan_mode == UBDRV_MSG_CHAN_FOR_RAO) {
        chan = &msg_dev->rao.rao_msg_chan[cmd->chan_id];
    } else {
        chan = &msg_dev->non_trans_chan[cmd->chan_id];
    }
    desc->real_data_len = 0;

    return ubdrv_free_non_trans_chan_process(chan, cmd->chan_mode, msg_dev->dev_id, cmd->chan_id);
}

STATIC int ubdrv_non_trans_prepare_send(struct ubdrv_non_trans_chan *chan, struct ascend_ub_msg_desc *desc,
    struct send_wr_cfg *wr_cfg, struct ascend_ub_user_data *user_data)
{
    u32 chan_len = chan->msg_jetty.send_jetty.send_msg_len;
    u32 max_send_len = (chan_len > ASCEND_UB_MAX_SEND_LIMIT ? ASCEND_UB_MAX_SEND_LIMIT : chan_len) -
        ASCEND_UB_MSG_DESC_LEN;
    int ret;

    if (user_data->size > max_send_len) {
        ubdrv_err("Send msg over len. (real_size=%u;max_size=%u)\n", user_data->size, max_send_len);
        return -EINVAL;
    }
    desc->in_data_len = user_data->size;
    desc->out_data_len = user_data->reply_size;
    desc->client_type = user_data->msg_type;
    if ((user_data->size != 0) && (user_data->cmd != NULL)) {
        ret = ubdrv_copy_user_send(desc, user_data->cmd, user_data->size, max_send_len);
        if (ret != 0) {
            ubdrv_err("ubdrv_copy_user_send failed. (ret=%d)\n", ret);
            return ret;
        }
    }

    wr_cfg->user_ctx = desc->seg_id;
    wr_cfg->tjetty = chan->r_tjetty;
    wr_cfg->sva = (u64)(uintptr_t)desc;
    wr_cfg->sseg = chan->msg_jetty.send_jetty.send_seg;
    wr_cfg->tseg = NULL; // Not used in send opcode
    wr_cfg->jfs = chan->msg_jetty.send_jetty.jfs;
    wr_cfg->len = user_data->size;
    return 0;
}

STATIC void ubdrv_non_trans_clear_user_data(struct ascend_ub_msg_desc *desc,
    struct ubdrv_non_trans_chan *chan)
{
    size_t len = chan->msg_jetty.send_jetty.send_msg_len - ASCEND_UB_MSG_DESC_LEN;
    (void)memset_s(&(desc->user_data), len, 0, len);
}

void ubdrv_sync_send_prepare_user_data(struct ascend_ub_user_data *user_data, void *data,
    u32 in_data_len, u32 out_data_len, u32 msg_type)
{
    user_data->cmd = data;
    user_data->reply = data;
    user_data->size = in_data_len;
    user_data->reply_size = out_data_len;
    user_data->msg_type = msg_type;
}

STATIC void ubdrv_msg_tx_dfx_info(struct ubdrv_msg_chan_stat *stat)
{
    ubdrv_err("Msg chan id info. (dev_id=%u;chan_id=%u;tx_jfr=%u,tx_jfr_jfc=%u;tx_jfs=%u;tx_jfs_jfc=%u;rx_jfr=%u;\
        rx_jfr_jfc=%u;rx_jfs=%u;rx_jfs_jfc=%u)\n", stat->dev_id, stat->chan_id, stat->tx_jfr_id, stat->tx_jfr_jfc_id,
        stat->tx_jfs_id, stat->tx_jfs_jfc_id, stat->rx_jfr_id, stat->rx_jfr_jfc_id, stat->rx_jfs_id,
        stat->rx_jfs_jfc_id);
    ubdrv_err("Msg chan tx dfx info. (tx_total=%llu,post_err=%llu;cqe_cnt=%llu;cqe_timeout=%llu)\n",
        stat->tx_total, stat->tx_post_send_err, stat->tx_cqe, stat->tx_cqe_timeout);
    ubdrv_err("Msg chan tx dfx info. (poll_cqe_err=%llu,cqe_status_err=%llu;rebuild_jfs=%llu;recv_cqe_timeout=%llu)\n",
        stat->tx_poll_cqe_err, stat->tx_cqe_status_err, stat->tx_rebuild_jfs, stat->tx_recv_cqe_timeout);
    ubdrv_err("Msg chan tx dfx info. (recv_cqe_cnt=%llu,recv_poll_cqe_err=%llu;recv_cqe_status_err=%llu;\
        recv_data_err=%llu)\n", stat->tx_recv_cqe, stat->tx_recv_poll_cqe_err, stat->tx_recv_cqe_status_err,
        stat->tx_recv_data_err);
    return;
}

int devdrv_sync_msg_send_inner(u32 dev_id, void *msg_chan, struct ascend_ub_user_data *user_data, u32 *real_out_len)
{
    struct ubdrv_non_trans_chan *chan;
    struct ascend_ub_msg_desc *rqe_desc;
    struct ascend_ub_msg_desc *desc;
    struct ubdrv_msg_chan_stat *stat;
    struct send_wr_cfg wr_cfg = {0};
    struct ascend_ub_jetty_ctrl *jetty_ctrl;
    struct ubcore_cr cr = {0};
    int ta_timeout_cnt = 0;
    u32 msg_num;
    int ret = -EINVAL;

    ret = ubdrv_add_device_status_ref(dev_id);
    if (ret != 0) {
        return ret;
    }
    chan = ubdrv_find_msg_chan_by_handle(msg_chan);
    if (chan == NULL) {
        ubdrv_err("Can't find chan. (dev_id=%u)\n", dev_id);
        goto sub_ref;
    }
    ka_task_mutex_lock(&chan->tx_mutex);
    if (chan->status != UBDRV_CHAN_ENABLE) {
        ubdrv_err("Chan is invalid. (status=%u;dev_id=%u;chan_id=%u;chan_type=%u)\n",
            chan->status, dev_id, chan->chan_id, chan->msg_type);
        goto chan_unlock;
    }
    msg_num = (u32)ka_base_atomic_inc_return(&chan->msg_num);
    stat = &chan->chan_stat;
    desc = ubdrv_alloc_sync_send_seg(&chan->msg_jetty, dev_id, msg_num,
        UBDRV_ALLOC_SEG_TRY_CNT, UBDRV_ALLOC_SEG_WAIT_PER_US);
    if (desc == NULL) {
        ubdrv_err("Alloc send seg failed. (dev_id=%u;chan_id=%u;chan_type=%u)\n",
            dev_id, chan->chan_id, chan->msg_type);
        goto chan_unlock;
    }
    ret = ubdrv_non_trans_prepare_send(chan, desc, &wr_cfg, user_data);
    if (ret != 0) {
        ubdrv_err("ubdrv_prepare_send_data failed. (dev_id=%u;ret=%d;chan_id=%u;chan_type=%u)\n",
            dev_id, ret, chan->chan_id, chan->msg_type);
        goto free_send_seg;
    }
    stat->tx_total++;
nontrans_msg_tatimeout:
    ret = ubdrv_post_send_wr(&wr_cfg, dev_id);
    if (ret != 0) {
        stat->tx_post_send_err++;
        ubdrv_err("Post send wr failed. (ret=%d;seg_id=%u;dev_id=%u;chan_id=%u;chan_type=%u;msg_sqe=%u)\n",
        ret, desc->seg_id, dev_id, chan->chan_id, chan->msg_type, msg_num);
        goto clear_user_data;
    }
    jetty_ctrl = &chan->msg_jetty.send_jetty;
    ret = ubdrv_interval_poll_send_jfs_jfc(jetty_ctrl->jfs_jfc, (u64)desc->seg_id, MSG_MAX_WAIT_CNT, &cr, stat, true);
    if ((ret == 0) && (cr.status == UBCORE_CR_ACK_TIMEOUT_ERR) && (ta_timeout_cnt < ASCEND_TATIMEOUT_RETRY_CNT)) {
        ubdrv_rebuild_chan_send_jetty(dev_id, chan->chan_id, stat, jetty_ctrl, &wr_cfg);
        ta_timeout_cnt++;
        goto nontrans_msg_tatimeout;
    } else if ((ret != 0) || (cr.status != UBCORE_CR_SUCCESS)) {
        ubdrv_warn("Non trans chan send unsuccess. (ret=%d;cr_status=%d;dev_id=%u;chan_id=%u;msg_type=%u;dev_name=%s)\n",
            ret, cr.status, dev_id, chan->chan_id, chan->msg_type, jetty_ctrl->ubc_dev->dev_name);
        ret = ((ret != 0) ? ret : cr.status);
        goto clear_user_data;
    }

    rqe_desc = ubdrv_wait_sync_msg_rqe(dev_id, jetty_ctrl, msg_num, MSG_MAX_WAIT_CNT, stat);
    if (KA_IS_ERR(rqe_desc)) {
        ret = KA_PTR_ERR(rqe_desc);
        ubdrv_msg_tx_dfx_info(stat);
        goto clear_user_data;
    } else if (rqe_desc->status != UB_MSG_PROCESS_SUCCESS) {
        goto nontrans_repost_jfr;
    }
    ret = ubdrv_copy_rqe_data_to_user(rqe_desc, user_data, stat, jetty_ctrl->recv_msg_len);
    if (ret != 0) {
        ubdrv_err("Copy rqe data failed. (ret=%u;dev_id=%u;chan_id=%u;msg_type=%u;dev_name=%s)\n",
            ret, dev_id, chan->chan_id, chan->msg_type, jetty_ctrl->ubc_dev->dev_name);
    }
    *real_out_len = rqe_desc->real_data_len;
nontrans_repost_jfr:
    ret = ubdrv_msg_result_process(ret, (int)rqe_desc->status, chan->msg_type);
    (void)ubdrv_post_jfr_wr(jetty_ctrl->jfr, jetty_ctrl->recv_seg, rqe_desc,
        jetty_ctrl->recv_msg_len, rqe_desc->seg_id);
clear_user_data:
    ubdrv_non_trans_clear_user_data(desc, chan);
free_send_seg:
    ubdrv_free_sync_send_seg(&chan->msg_jetty, desc);
chan_unlock:
    ka_task_mutex_unlock(&chan->tx_mutex);
sub_ref:
    ubdrv_sub_device_status_ref(dev_id);
    return ret;
}

int ubdrv_sync_send_check_param(void *msg_chan, void *data, u32 *real_out_len)
{
    const UBDRV_MSG_HANDLE *msg_handle;
    u32 chan_id;

    if (msg_chan == NULL) {
        ubdrv_err("Sync msg send failed, msg_chan is NULL.\n");
        return -EINVAL;
    }
    msg_handle = (const UBDRV_MSG_HANDLE *)&msg_chan;
    chan_id = msg_handle->bits.chan_id;
    if (data == NULL) {
        ubdrv_err("Sync msg send failed, data is NULL. (chan_id=%u)\n", chan_id);
        return -EINVAL;
    }
    if (real_out_len == NULL) {
        ubdrv_err("Sync msg send failed, real_out_len is NULL. (chan_id=%u)\n", chan_id);
        return -EINVAL;
    }
    if (msg_handle->bits.dev_id >= ASCEND_UB_DEV_MAX_NUM) {
        ubdrv_err("Sync msg send failed, dev id is invalid. (dev_id=%u)\n", msg_handle->bits.dev_id);
        return -EINVAL;
    }
    return 0;
}