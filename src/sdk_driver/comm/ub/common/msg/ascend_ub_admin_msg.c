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

#include "ka_memory_pub.h"
#include "ka_system_pub.h"
#include "ka_errno_pub.h"
#include "ka_task_pub.h"
#include "ka_compiler_pub.h"
#include "ka_kernel_def_pub.h"
#include "ubcore_types.h"
#include "ubcore_opcode.h"
#include "ubcore_uapi.h"
#include "ubcore_api.h"

#include "pbl/pbl_soc_res_sync.h"
#include "ascend_ub_common.h"
#include "ascend_ub_jetty.h"
#include "ascend_ub_msg.h"
#include "ascend_ub_dev.h"
#include "ascend_ub_main.h"
#include "ascend_ub_non_trans_chan.h"
#include "ascend_ub_urma_chan.h"
#include "ascend_ub_pair_dev_info.h"
#include "ascend_ub_proc_fs.h"
#include "ascend_ub_hotreset.h"
#include "ascend_ub_admin_msg.h"

#define UBDRV_ADMIN_OPER_FUNC_MAX 2
static int (*ubdrv_admin_oper_func[UBDRV_ADMIN_MSG_MAX][UBDRV_ADMIN_OPER_FUNC_MAX])
    (struct ascend_ub_msg_dev *msg_dev, void *data) = {
    [UBDRV_CREATE_MSG_QUEUE] = {ubdrv_msg_alloc_msg_queue, NULL},
    [UBDRV_FREE_MSG_QUEUE] = {ubdrv_msg_free_msg_queue, NULL},
    [UBDRV_ENABLE_MSG_QUEUE] = {ubdrv_enable_device_msg_chan, NULL},
    [UBDRV_CREATE_URMA_CHAN] = {ubdrv_device_alloc_urma_chan, NULL},
    [UBDRV_FREE_URMA_CHAN] = {ubdrv_device_free_urma_chan, NULL},
    [UBDRV_VFE_ONLINE] = {ubdrv_vfe_online, NULL},
    [UBDRV_VFE_OFFLINE] = {ubdrv_vfe_offline, NULL},
    [UBDRV_SYNC_RES_INFO] = {ubdrv_sync_res_info, NULL},
    [UBDRV_SYNC_DFX_INFO] = {ubdrv_procfs_device_dfx_info, NULL},
    [UBDRV_HOST_HOT_RESET] = {ubdrv_proc_host_hot_reset, ubdrv_set_hot_reset_msg_finish},
    [UBDRV_GET_TOKEN_VAL] = {ubdrv_msg_get_token_val, NULL},
};

STATIC void ubdrv_admin_jfce_recv_handle(struct ubcore_jfc *jfc)
{
    struct ascend_ub_admin_chan *msg_chan;
    struct ascend_ub_jetty_ctrl *cfg;
    struct ubdrv_msg_chan_stat *stat;

    if ((jfc == NULL) || (jfc->jfc_cfg.jfc_context == NULL)) {
        ubdrv_err("Jfce is null in admin recv jfce handle.\n");
        return;
    }

    cfg = (struct ascend_ub_jetty_ctrl*)jfc->jfc_cfg.jfc_context;
    msg_chan = (struct ascend_ub_admin_chan*)cfg->msg_chan;
    if (msg_chan == NULL) {
        ubdrv_warn("Admin chan is null in jfce handle.\n");
        return;
    }
    ka_base_atomic_add(1, &msg_chan->user_cnt);
    if (msg_chan->valid != UB_ADMIN_MSG_ENABLE) {
        ka_base_atomic_sub(1, &msg_chan->user_cnt);
        ubdrv_warn("Chan is unexpected state in jfce handle. (status=%d;dev_id=%u)\n",
            msg_chan->valid, msg_chan->dev_id);
        return;
    }

    stat = &msg_chan->chan_stat;
    stat->rx_total++;
    stat->rx_work_stamp = (u64)ka_jiffies;
    ka_task_queue_work(msg_chan->work_queue, &msg_chan->recv_work.work);
    ka_base_atomic_sub(1, &msg_chan->user_cnt);
    return;
}

int ubdrv_create_basic_jetty(struct ascend_ub_sync_jetty *jetty, u32 jfr_id)
{
    struct ascend_ub_jetty_ctrl *send_jetty;
    struct ascend_ub_jetty_ctrl *recv_jetty;
    int ret;

    send_jetty = &(jetty->send_jetty);
    recv_jetty = &(jetty->recv_jetty);
    ret = ubdrv_sync_send_jetty_init(send_jetty, NULL);
    if (ret != 0) {
        ubdrv_err("Ascend sync send jetty init failed. (ret=%d)\n", ret);
        return ret;
    }

    ret = ubdrv_sync_recv_jetty_init(recv_jetty, jfr_id, ubdrv_admin_jfce_recv_handle);
    if (ret != 0) {
        ubdrv_err("Ascend sync recv jetty init failed. (ret=%d)\n", ret);
        ubdrv_sync_send_jetty_uninit(send_jetty);
        return ret;
    }
    ubdrv_sync_jetty_init(jetty);
    return 0;
}

void ubdrv_delete_basic_jetty(struct ascend_ub_sync_jetty *jetty)
{
    ubdrv_sync_jetty_uninit(jetty);
    ubdrv_sync_recv_jetty_uninit(&jetty->recv_jetty);
    ubdrv_sync_send_jetty_uninit(&jetty->send_jetty);
    return;
}

STATIC int ubdrv_admin_jetty_cfg_init(u32 dev_id, struct ub_idev *idev, struct ascend_ub_sync_jetty *cfg)
{
    u32 token;
    int ret;

    ret = ubdrv_get_local_token(dev_id, &token);
    if (ret != 0) {
        return ret;
    }

    cfg->recv_jetty.ubc_dev = idev->ubc_dev;
    cfg->recv_jetty.eid_index = idev->eid_index;
    cfg->recv_jetty.send_msg_len = ASCEND_UB_ADMIN_SEND_SEG_SIZE;
    cfg->recv_jetty.recv_msg_len = ASCEND_UB_ADMIN_SEND_SEG_SIZE;
    cfg->recv_jetty.jfs_msg_depth = ASCEND_UB_ADMIN_SEND_SEG_COUNT;
    cfg->recv_jetty.jfr_msg_depth = ASCEND_UB_ADMIN_RECV_SEG_COUNT;
    cfg->recv_jetty.access = MEM_ACCESS_LOCAL;
    cfg->recv_jetty.token_value = token;
    cfg->recv_jetty.chan_mode = UBDRV_MSG_CHAN_FOR_ADMIN;

    cfg->send_jetty.ubc_dev = idev->ubc_dev;
    cfg->send_jetty.eid_index = idev->eid_index;
    cfg->send_jetty.send_msg_len = ASCEND_UB_ADMIN_SEND_SEG_SIZE;
    cfg->send_jetty.recv_msg_len = ASCEND_UB_ADMIN_SEND_SEG_SIZE;
    cfg->send_jetty.jfs_msg_depth = ASCEND_UB_ADMIN_SEND_SEG_COUNT;
    cfg->send_jetty.jfr_msg_depth = ASCEND_UB_ADMIN_RECV_SEG_COUNT;
    cfg->send_jetty.access = MEM_ACCESS_LOCAL;
    cfg->send_jetty.token_value = token;
    cfg->send_jetty.chan_mode = UBDRV_MSG_CHAN_FOR_ADMIN;
    return 0;
}

STATIC void ubdrv_admin_jetty_cfg_uninit(struct ascend_ub_sync_jetty *cfg)
{
    cfg->send_jetty.ubc_dev = NULL;
    cfg->send_jetty.eid_index = 0;
    cfg->send_jetty.send_msg_len = 0;
    cfg->send_jetty.recv_msg_len = 0;
    cfg->send_jetty.jfs_msg_depth = 0;
    cfg->send_jetty.jfr_msg_depth = 0;
    cfg->send_jetty.access = 0;
    cfg->send_jetty.token_value = 0;
    cfg->send_jetty.chan_mode = UBDRV_MSG_CHAN_FOR_MAX;

    cfg->recv_jetty.ubc_dev = NULL;
    cfg->recv_jetty.eid_index = 0;
    cfg->recv_jetty.recv_msg_len = 0;
    cfg->recv_jetty.send_msg_len = 0;
    cfg->recv_jetty.jfs_msg_depth = 0;
    cfg->recv_jetty.jfr_msg_depth = 0;
    cfg->recv_jetty.access = 0;
    cfg->recv_jetty.token_value = 0;
    cfg->recv_jetty.chan_mode = UBDRV_MSG_CHAN_FOR_MAX;
    return;
}

int ubdrv_create_admin_jetty(struct ub_idev *idev, u32 dev_id, u32 jfr_id)
{
    struct ascend_ub_jetty_ctrl *send_jetty;
    struct ascend_ub_jetty_ctrl *recv_jetty;
    struct ascend_ub_link_res *link_res;
    int ret;

    link_res = ubdrv_get_link_res_by_devid(dev_id);
    if (link_res->admin_jetty != NULL) {
        ubdrv_err("Admin jetty has been initialized, reinit fail. (dev_id=%u)\n", dev_id);
        return -EALREADY;
    }
    link_res->admin_jetty = ubdrv_kzalloc(sizeof(struct ascend_ub_sync_jetty), KA_GFP_KERNEL);
    if (link_res->admin_jetty == NULL) {
        ubdrv_err("Admin jetty alloc mem fail. (dev_id=%u)\n", dev_id);
        return -ENOMEM;
    }

    send_jetty = &(link_res->admin_jetty->send_jetty);
    recv_jetty = &(link_res->admin_jetty->recv_jetty);
    ret = ubdrv_admin_jetty_cfg_init(dev_id, idev, link_res->admin_jetty);
    if (ret != 0) {
        goto free_link_res;
    }
    ret = ubdrv_create_basic_jetty(link_res->admin_jetty, jfr_id);
    if (ret != 0) {
        ubdrv_err("Ascend admin jetty init failed. (ret=%d;dev_id=%u)\n", ret, dev_id);
        goto jetty_cfg_uninit;
    }
    ubdrv_info("Admin jetty create success. (dev_id=%u;send_jfr_id=%u; rcv_jfc_id=%u)\n",
        dev_id, send_jetty->jfr->jfr_id.id, recv_jetty->jfs_jfc->id);

    return 0;

jetty_cfg_uninit:
    ubdrv_admin_jetty_cfg_uninit(link_res->admin_jetty);
free_link_res:
    ubdrv_kfree(link_res->admin_jetty);
    link_res->admin_jetty = NULL;
    return ret;
}

void ubdrv_delete_admin_jetty(u32 dev_id)
{
    struct ascend_ub_jetty_ctrl *send_jetty;
    struct ascend_ub_jetty_ctrl *recv_jetty;
    struct ascend_ub_link_res *link_res;
    int ret;

    link_res = ubdrv_get_link_res_by_devid(dev_id);
    if (link_res->admin_jetty == NULL) {
        ubdrv_warn("Admin jetty is null, can't del. (dev_id=%u)\n", dev_id);
        return;
    }

    send_jetty = &(link_res->admin_jetty->send_jetty);
    recv_jetty = &(link_res->admin_jetty->recv_jetty);
    ret = ubdrv_clear_jetty(send_jetty->jfs);
    if (ret != 0) {
        ubdrv_err("Flush send jetty failed. (dev_id=%u;ret=%d)\n", dev_id, ret);
    }
    ret = ubdrv_clear_jetty(recv_jetty->jfs);
    if (ret != 0) {
        ubdrv_err("Flush recv jetty failed. (dev_id=%u;ret=%d)\n", dev_id, ret);
    }

    ubdrv_delete_basic_jetty(link_res->admin_jetty);
    ubdrv_admin_jetty_cfg_uninit(link_res->admin_jetty);
    ubdrv_kfree(link_res->admin_jetty);
    link_res->admin_jetty = NULL;
    return;
}

int ubdrv_admin_chan_import_jfr(struct ub_idev *idev, struct ascend_ub_admin_chan *msg_chan,
    struct jetty_exchange_data *data, u32 dev_id, enum ubdrv_log_level log_level)
{
    struct ubcore_tjetty *tjetty;
    u32 eid_index = idev->eid_index;

    tjetty = ascend_import_jfr(idev->ubc_dev, eid_index, data);
    if (KA_IS_ERR_OR_NULL(tjetty)) {
        UBDRV_LOG_LEVEL(log_level, "ascend_import_jfr unsuccessful. (dev_id=%u;ret=%ld)\n", dev_id, KA_PTR_ERR(tjetty));
        return -ETIMEDOUT;
    }
    msg_chan->tjetty = tjetty;
    return 0;
}

void ubdrv_admin_chan_unimport_jfr(struct ascend_ub_admin_chan *msg_chan, u32 dev_id)
{
    int ret;

    if (msg_chan->tjetty != NULL) {
        ret = ubcore_unimport_jfr(msg_chan->tjetty);
        if (ret != 0) {
            ubdrv_err("ubcore_unimport_jfr failed. (ret=%d;dev_id=%u)\n", ret, dev_id);
        }
    }
    msg_chan->tjetty = NULL;
    return;
}

int ubdrv_create_admin_msg_chan(u32 dev_id, struct ascend_ub_msg_dev *msg_dev)
{
    struct ascend_ub_admin_chan *msg_chan;
    struct ascend_ub_sync_jetty *admin_jetty;
    u32 mem_len;

    msg_chan = &(msg_dev->admin_msg_chan);
    admin_jetty = msg_chan->admin_jetty;
    mem_len = admin_jetty->recv_jetty.recv_msg_len;
    msg_chan->msg_desc = ubdrv_kzalloc(mem_len, KA_GFP_KERNEL);
    if (msg_chan->msg_desc == NULL) {
        ubdrv_err("Alloc chan msg desc mem fail. (dev_id=%u;mem_len=%u)\n", dev_id, mem_len);
        return -ENOMEM;
    }
    msg_chan->work_queue = ka_task_create_workqueue("ub_admin_workqueue");
    if (msg_chan->work_queue == NULL) {
        ubdrv_err("Create admin chan workqueue fail. (dev_id=%u)\n", dev_id);
        ubdrv_kfree(msg_chan->msg_desc);
        msg_chan->msg_desc = NULL;
        return -ENOMEM;
    }
    admin_jetty->recv_jetty.msg_chan = msg_chan;
    admin_jetty->send_jetty.msg_chan = msg_chan;
    msg_chan->recv_work.jfc = admin_jetty->recv_jetty.jfr_jfc;
    msg_chan->recv_work.stat = &msg_chan->chan_stat;
    KA_TASK_INIT_WORK(&msg_chan->recv_work.work, ubdrv_jfce_recv_work);
    ka_base_atomic_set(&msg_chan->msg_num, 0);
    ka_base_atomic_set(&msg_chan->user_cnt, 0);
    ka_task_mutex_lock(&msg_chan->mutex_lock);
    msg_chan->valid = UB_ADMIN_MSG_ENABLE;
    ka_task_mutex_unlock(&msg_chan->mutex_lock);
    ubdrv_record_chan_jetty_info(&msg_chan->chan_stat, admin_jetty);
    ubdrv_info("Create admin chan success. (dev_id=%u)\n", dev_id);
    return 0;
}

void ubdrv_del_admin_msg_chan(struct ascend_ub_msg_dev *msg_dev, u32 dev_id)
{
    struct ascend_ub_admin_chan *msg_chan;

    msg_chan = &(msg_dev->admin_msg_chan);
    ubdrv_clear_chan_dfx(&msg_chan->chan_stat);
    ka_task_mutex_lock(&msg_chan->mutex_lock);
    msg_chan->valid = UB_ADMIN_MSG_DISABLE;
    ka_task_mutex_unlock(&msg_chan->mutex_lock);
    ubdrv_wait_chan_jfce_user_cnt(&msg_chan->user_cnt, dev_id, 0);
    ka_task_cancel_work_sync(&msg_chan->recv_work.work);
    ka_base_atomic_set(&msg_chan->user_cnt, 0);
    ka_base_atomic_set(&msg_chan->msg_num, 0);
    ka_task_destroy_workqueue(msg_chan->work_queue);
    msg_chan->work_queue = NULL;
    ubdrv_kfree(msg_chan->msg_desc);
    msg_chan->msg_desc = NULL;
    return;
}

STATIC struct ascend_ub_admin_chan *ubdrv_get_admin_msg_chan(u32 dev_id)
{
    struct ascend_ub_admin_chan *msg_chan;
    struct ascend_ub_msg_dev *msg_dev;

    msg_dev = ubdrv_get_msg_dev_by_devid(dev_id);
    if (msg_dev == NULL) {
        ubdrv_err("Invalid msg_dev. (dev_id=%u)\n", dev_id);
        return NULL;
    }

    msg_chan = &(msg_dev->admin_msg_chan);
    ka_task_mutex_lock(&msg_chan->mutex_lock);
    if (msg_chan->valid == UB_ADMIN_MSG_DISABLE) {
        ubdrv_err("Msg_chan is invalid. (dev_id=%u)\n", dev_id);
        ka_task_mutex_unlock(&msg_chan->mutex_lock);
        msg_chan = NULL;
    }

    return msg_chan;
}

STATIC void ubdrv_put_admin_msg_chan(struct ascend_ub_admin_chan *msg_chan)
{
    ka_task_mutex_unlock(&msg_chan->mutex_lock);
    return;
}

STATIC void ubdrv_clear_user_data(struct ascend_ub_admin_chan *msg_chan,
    struct ascend_ub_msg_desc *desc)
{
    u32 buf_len = msg_chan->admin_jetty->send_jetty.send_msg_len;
    u32 len = buf_len - ASCEND_UB_MSG_DESC_LEN;

    (void)memset_s(&(desc->user_data), len, 0, len);
    return;
}

STATIC void ubdrv_prepare_send_wr(struct send_wr_cfg *wr_cfg, struct ascend_ub_admin_chan *msg_chan,
    struct ascend_ub_msg_desc *desc, u32 size)
{
    wr_cfg->user_ctx = desc->seg_id;
    wr_cfg->tjetty = msg_chan->tjetty;
    wr_cfg->sva = (u64)desc;
    wr_cfg->sseg = msg_chan->admin_jetty->send_jetty.send_seg;
    wr_cfg->tseg = NULL; // Not used in send opcode
    wr_cfg->jfs = msg_chan->admin_jetty->send_jetty.jfs;
    wr_cfg->len = size;
}

STATIC int ubdrv_prepare_send_data(struct ascend_ub_msg_desc *desc, struct ascend_ub_user_data *data)
{
    int ret = 0;
    u32 len = data->size + ASCEND_UB_MSG_DESC_LEN;

    len = (len > ASCEND_UB_MAX_SEND_LIMIT ? ASCEND_UB_MAX_SEND_LIMIT : len);
    desc->opcode = data->opcode;
    desc->in_data_len = data->size;
    desc->out_data_len = data->reply_size;
    if ((data->size != 0) && (data->cmd != NULL)) {
        ret = ubdrv_copy_user_send(desc, data->cmd, data->size, len);
        if (ret != 0) {
            ubdrv_err("ubdrv_copy_user_send failed. (ret=%d)\n", ret);
        }
    }
    return ret;
}

STATIC int ubdrv_check_amdin_msg(u32 dev_id, struct ascend_ub_user_data *data)
{
    if (dev_id >= ASCEND_UB_DEV_MAX_NUM) {
        ubdrv_err("Check dev_id failed. (dev_id=%u)\n", dev_id);
        return -EINVAL;
    }
    if (data == NULL) {
        ubdrv_err("Check data failed. (dev_id=%u)\n", dev_id);
        return -EINVAL;
    }
    if (data->size > ASCEND_UB_ADMIN_MAX_SEND_LEN) {
        ubdrv_err("Check send len failed. (dev_id=%u;send_size=%u)\n", dev_id, data->size);
        return -EINVAL;
    }
    return 0;
}

STATIC int ubdrv_prepare_send(struct ascend_ub_admin_chan *msg_chan, struct ascend_ub_msg_desc *desc,
    struct send_wr_cfg *wr_cfg, struct ascend_ub_user_data *data, u32 dev_id)
{
    int ret;

    ret = ubdrv_prepare_send_data(desc, data);
    if (ret != 0) {
        ubdrv_err("ubdrv_prepare_send_data failed. (dev_id=%u;ret=%d)\n", dev_id, ret);
        return ret;
    }
    ubdrv_prepare_send_wr(wr_cfg, msg_chan, desc, data->size);
    return 0;
}

int ubdrv_basic_chan_send(u32 dev_id, struct ascend_ub_user_data *data,
    struct ascend_ub_admin_chan *msg_chan)
{
    struct ascend_ub_jetty_ctrl *jetty_ctrl;
    struct ascend_ub_msg_desc *rqe_desc;
    struct ubdrv_msg_chan_stat *stat;
    struct ascend_ub_msg_desc *desc;
    struct send_wr_cfg wr_cfg = {0};
    struct ubcore_cr cr = {0};
    int ta_timeout_cnt = 0;
    u32 msg_num;
    int ret;

    msg_num = (u32)ka_base_atomic_inc_return(&msg_chan->msg_num);
    desc = ubdrv_alloc_sync_send_seg(msg_chan->admin_jetty, dev_id, msg_num,
        UBDRV_ALLOC_SEG_TRY_CNT, UBDRV_ALLOC_SEG_WAIT_PER_US);
    if (desc == NULL) {
        ubdrv_err("ubdrv_alloc_sync_send_seg failed. (dev_id=%u)\n", dev_id);
        return -ENOMEM;
    }

    ret = ubdrv_prepare_send(msg_chan, desc, &wr_cfg, data, dev_id);
    if (ret != 0) {
        ubdrv_err("ubdrv_prepare_send_data failed. (dev_id=%u;ret=%d)\n", dev_id, ret);
        goto free_send_seg;
    }
    stat = &msg_chan->chan_stat;
    stat->tx_total++;
admin_msg_tatimeout:
    ret = ubdrv_post_send_wr(&wr_cfg, dev_id);
    if (ret != 0) {
        ubdrv_err("ubcore_post_jfs_wr failed. (seg_id=%u;dev_id=%u)\n",
            desc->seg_id, dev_id);
        stat->tx_post_send_err++;
        goto clear_user_data;
    }
    jetty_ctrl = &msg_chan->admin_jetty->send_jetty;
    ret = ubdrv_interval_poll_send_jfs_jfc(jetty_ctrl->jfs_jfc, (u64)desc->seg_id, MSG_MAX_WAIT_CNT, &cr, stat, true);
    if ((ret == 0) && (cr.status == UBCORE_CR_ACK_TIMEOUT_ERR) && (ta_timeout_cnt < ASCEND_TATIMEOUT_RETRY_CNT)) {
        ubdrv_rebuild_chan_send_jetty(dev_id, 0, stat, jetty_ctrl, &wr_cfg);
        ta_timeout_cnt++;
        goto admin_msg_tatimeout;
    } else if ((ret != 0) || (cr.status != UBCORE_CR_SUCCESS)) {
        ubdrv_warn("Admin chan send unsuccess. (ret=%d;cr_status=%d;dev_id=%u;dev_name=%s)\n",
            ret, cr.status, dev_id, jetty_ctrl->ubc_dev->dev_name);
        ret = ((cr.status != 0) ? cr.status : ret);
        goto clear_user_data;
    }

    rqe_desc = ubdrv_wait_sync_msg_rqe(dev_id, jetty_ctrl, msg_num, MSG_MAX_WAIT_CNT, stat);
    if (KA_IS_ERR(rqe_desc)) {
        ret = KA_PTR_ERR(rqe_desc);
        goto clear_user_data;
    } else if (rqe_desc->status != UB_MSG_PROCESS_SUCCESS) {
        goto repost_jfr;
    }
    ret = ubdrv_copy_rqe_data_to_user(rqe_desc, data, stat, jetty_ctrl->recv_msg_len);
    if (ret != 0) {
        ubdrv_err("Copy rqe data failed. (dev_id=%u;ret=%u)\n", dev_id, ret);
    }

repost_jfr:
    ret = ubdrv_msg_result_process(ret, (int)rqe_desc->status, UBDRV_NONTRANS_TYPE_CNT);
    (void)ubdrv_post_jfr_wr(jetty_ctrl->jfr, jetty_ctrl->recv_seg, rqe_desc,
        jetty_ctrl->recv_msg_len, rqe_desc->seg_id);
clear_user_data:
    ubdrv_clear_user_data(msg_chan, desc);
free_send_seg:
    ubdrv_free_sync_send_seg(msg_chan->admin_jetty, desc);
    return ret;
}

int ubdrv_admin_send_msg(u32 dev_id, struct ascend_ub_user_data *data)
{
    struct ascend_ub_admin_chan *msg_chan;
    int ret;

    ret = ubdrv_check_amdin_msg(dev_id, data);
    if (ret != 0) {
        ubdrv_err("Check send param failed. (ret=%d;dev_id=%u)\n", ret, dev_id);
        return -EINVAL;
    }

    if (ka_unlikely(ubdrv_get_device_status(dev_id) == UBDRV_DEVICE_DEAD)) {
        ubdrv_warn("Device status is dead. (dev_id=%u)\n", dev_id);
        return -ENODEV;
    }

    msg_chan = ubdrv_get_admin_msg_chan(dev_id);
    if (msg_chan == NULL) {
        ubdrv_err("ubdrv_get_admin_msg_chan failed. (dev_id=%u)\n", dev_id);
        return -EINVAL;
    }

    ret = ubdrv_basic_chan_send(dev_id, data, msg_chan);
    if (ret != 0) {
        ubdrv_err("Send admin msg fail. (dev_id=%u;ret=%d)\n", dev_id, ret);
    }
    ubdrv_put_admin_msg_chan(msg_chan);
    return ret;
}
KA_EXPORT_SYMBOL_GPL(ubdrv_admin_send_msg);

STATIC int ubdrv_get_udevid_by_phydevid(u32 phy_dev_id, u32 ue_idx, u32 *udevid)
{
    struct uda_mia_dev_para mia_para = {0};
    int ret;

    mia_para.phy_devid = phy_dev_id;
    mia_para.sub_devid = ue_idx - 1;
    ret = uda_mia_devid_to_udevid(&mia_para, udevid);
    if (ret != 0) {
        ubdrv_err("Get udevid failed. (dev_id=%u;ue_idx=%u;ret=%d)\n", phy_dev_id, ue_idx, ret);
        return ret;
    }
    return 0;
}

int ubdrv_vfe_online(struct ascend_ub_msg_dev *msg_dev, void *data)
{
    struct ascend_ub_msg_desc *desc = (struct ascend_ub_msg_desc *)data;
    struct ubdrv_jetty_exchange_data *cmd = (struct ubdrv_jetty_exchange_data *)&desc->user_data;
    struct ascend_ub_link_res *link_res;
    struct ub_idev *idev = msg_dev->idev;
    u32 udevid = 0;
    u32 token;
    int ret;

    if (desc->in_data_len != sizeof(struct ubdrv_jetty_exchange_data)) {
        ubdrv_err("Recv data len invalid. (dev_id=%u;len=%u)\n", msg_dev->dev_id, desc->in_data_len);
        return -EINVAL;
    }
    desc->real_data_len = sizeof(struct ubdrv_jetty_exchange_data);
    ret = ubdrv_get_udevid_by_phydevid(msg_dev->dev_id, cmd->ue_idx, &udevid);
    if (ret != 0) {
        ubdrv_err("Get udevid failed. (dev_id=%u;ue_idx=%u;ret=%d)\n", msg_dev->dev_id, idev->ue_idx, ret);
        return ret;
    }

    ubdrv_print_exchange_data(cmd);
    idev = ubdrv_find_idev_by_udevid(udevid);
    if (idev == NULL) {
        ubdrv_err("Get idev failed. (dev_id=%u;ue_idx=%u)\n", msg_dev->dev_id, cmd->ue_idx);
        return -EINVAL;
    }
    ret = ubdrv_init_h2d_eid_index(udevid);
    if (ret != 0) {
        return ret;
    }
    ret = ubdrv_davinci_bind_fe(idev, udevid);
    if (ret != 0) {
        goto vf_uninit_eid_index;
    }
    if (ubdrv_get_token_val(udevid, &token) != 0) {
        ubdrv_err("Get token val failed.(vf_dev_id=%u)\n", udevid);
        goto unbind_vfe;
    }
    ubdrv_set_local_token(udevid, token, ASCEND_VALID);
    ret = ubdrv_create_admin_jetty(idev, udevid, UBDRV_DEFAULT_JETTY_ID);
    if (ret != 0) {
        ubdrv_err("Create admin jetty failed. (dev_id=%u;ue_idx=%u;ret=%d)\n", msg_dev->dev_id, idev->ue_idx, ret);
        goto set_token_invalid;
    }

    ret = ubdrv_add_msg_device(udevid, 0, idev->idev_id, idev->ue_idx, &cmd->admin_jetty_info);
    if (ret != 0) {
        ubdrv_err("Add msg device failed.(dev_id=%u;ue_idx=%u;ret=%d)\n", msg_dev->dev_id, idev->ue_idx, ret);
        goto free_jetty;
    }
    link_res = ubdrv_get_link_res_by_devid(udevid);
    ret = ubdrv_get_recv_jetty_info(&link_res->admin_jetty->recv_jetty, &cmd->admin_jetty_info);
    if (ret != 0) {
        ubdrv_err("Get recv jetty info failed. (udevid=%u;ret=%d)\n", udevid, ret);
        goto del_msg_dev;
    }
    ubdrv_print_exchange_data(cmd);

    ret = ubdrv_add_davinci_dev(udevid, UDA_VIRTUAL);
    if (ret != 0) {
        ubdrv_err("Add uda dev failed. (udevid=%u;ret=%d)\n", udevid, ret);
        goto del_msg_dev;
    }
    return 0;

del_msg_dev:
    ubdrv_del_msg_device(udevid, UBDRV_DEVICE_UNINIT);
free_jetty:
    ubdrv_delete_admin_jetty(udevid);
set_token_invalid:
    ubdrv_set_local_token(udevid, 0, ASCEND_INVALID);
unbind_vfe:
    ubdrv_davinci_unbind_fe(idev, udevid);
vf_uninit_eid_index:
    ubdrv_uninit_h2d_eid_index(udevid);
    return ret;
}

int ubdrv_vfe_offline(struct ascend_ub_msg_dev *msg_dev, void *data)
{
    struct ascend_ub_msg_desc *desc = (struct ascend_ub_msg_desc *)data;
    struct ubdrv_jetty_exchange_data *cmd = (struct ubdrv_jetty_exchange_data *)&desc->user_data;
    struct ub_idev *idev;
    u32 udevid = 0;
    int ret;

    desc->real_data_len = sizeof(struct ubdrv_jetty_exchange_data);
    ret = ubdrv_get_udevid_by_phydevid(msg_dev->dev_id, cmd->ue_idx, &udevid);
    if (ret != 0) {
        ubdrv_err("Get udevid failed. (dev_id=%u;ue_idx=%u;ret=%d)\n", msg_dev->dev_id, cmd->ue_idx, ret);
        return ret;
    }
    idev = ubdrv_find_idev_by_udevid(udevid);
    if (idev == NULL) {
        ubdrv_err("Get idev failed. (dev_id=%u;ue_idx=%u)\n", msg_dev->dev_id, cmd->ue_idx);
        return -EINVAL;
    }

    ubdrv_remove_davinci_dev(udevid, UDA_VIRTUAL);
    ubdrv_del_msg_device(udevid, UBDRV_DEVICE_UNINIT);
    ubdrv_delete_admin_jetty(udevid);
    ubdrv_set_local_token(udevid, 0, ASCEND_INVALID);
    ubdrv_davinci_unbind_fe(idev, udevid);
    ubdrv_uninit_h2d_eid_index(udevid);
    return 0;
}

int ubdrv_soc_res_addr_encode(u32 udevid, u64 addr, u64 len, u64 *encode_addr)
{
    /* not support iodecoder */
    return -EFAULT;
}

int ubdrv_sync_res_info(struct ascend_ub_msg_dev *msg_dev, void *data)
{
    struct ascend_ub_msg_desc *desc = (struct ascend_ub_msg_desc *)data;
    struct res_sync_target *target = (struct res_sync_target *)&desc->user_data;
    char *buf = (char *)&desc->user_data + sizeof(u32); /* first 4 bytes store out len */
    u32 buf_len = desc->out_data_len - sizeof(u32);
    u32 dev_id = msg_dev->dev_id;
    int ret;

    if (desc->in_data_len != sizeof(struct res_sync_target)) {
        ubdrv_err("Recv data len invalid, when sync res info. (dev_id=%u;len=%u)\n", dev_id, desc->in_data_len);
        return -EINVAL;
    }
    ret = soc_res_extract(dev_id, target, buf, &buf_len, ubdrv_soc_res_addr_encode);
    if (ret == 0) {
        *(u32 *)&desc->user_data = buf_len;
        desc->real_data_len = buf_len + sizeof(u32);
    }

    return ret;
}

STATIC int ubdrv_admin_recv_process(struct ascend_ub_jetty_ctrl *cfg, struct ascend_ub_msg_desc *desc, u32 seg_id)
{
    struct ascend_ub_admin_chan *chan = cfg->msg_chan;
    int ret;

    (void)seg_id;

    if (desc->opcode >= UBDRV_ADMIN_MSG_MAX) {
        ubdrv_err("Invalid admin msg opcode. (opcode=%u;msg_num=%u)\n", desc->opcode, desc->msg_num);
        desc->status = UB_MSG_INVALID_VALUE;
        return -EINVAL;
    }

    ret = ubdrv_wait_add_davinci_dev();
    if (ret != 0) {
        ubdrv_err("Wait davinci dev online error. (opcode=%u;ret=%d)\n", desc->opcode, ret);
        desc->status = UB_MSG_NULL_PROCSESS;
        return -EINVAL;
    }

    ret = ubdrv_admin_oper_func[desc->opcode][0](chan->msg_dev, desc);
    if (ret != 0) {
        ubdrv_err("Admin msg process error. (opcode=%u;msg_num=%u;ret=%d)\n", desc->opcode, desc->msg_num, ret);
        desc->status = UB_MSG_PROCESS_FAILED;
        return ret;
    }
    desc->status = UB_MSG_PROCESS_SUCCESS;
    return 0;
}

void ubdrv_admin_recv_finish_process(struct ascend_ub_jetty_ctrl *cfg,
                                     struct ascend_ub_msg_desc *desc)
{
    int ret;
    struct ascend_ub_admin_chan *chan = cfg->msg_chan;

    if (desc->opcode >= UBDRV_ADMIN_MSG_MAX) {
        ubdrv_err("Invalid admin msg opcode. (opcode=%u;msg_num=%u)\n", desc->opcode, desc->msg_num);
        return;
    }

    if (ubdrv_admin_oper_func[desc->opcode][1] == NULL) {
        return;
    }

    ret = ubdrv_admin_oper_func[desc->opcode][1](chan->msg_dev, desc);
    if (ret != 0) {
        ubdrv_err("Admin msg process error. (opcode=%u;msg_num=%u;ret=%d)\n", desc->opcode, desc->msg_num, ret);
    }

    return;
}

STATIC int ubdrv_admin_recv_check_parm(struct ascend_ub_msg_desc *desc)
{
    if (desc->in_data_len >= ASCEND_UB_ADMIN_MAX_SEND_LEN) {
        ubdrv_err("In_data_len over len. (len=%u)\n", desc->in_data_len);
        return -EINVAL;
    }
    if (desc->opcode >= UBDRV_ADMIN_MSG_MAX) {
        ubdrv_err("Invalid opcode. (opcode=%u)\n", desc->opcode);
        return -EINVAL;
    }
    return 0;
}

STATIC int ubdrv_send_admin_reply_msg(u32 dev_id, struct ascend_ub_jetty_ctrl *cfg,
    struct ascend_ub_msg_desc *src_desc, struct ubdrv_msg_chan_stat *stat)
{
    struct ascend_ub_admin_chan *admin_chan;
    struct ascend_ub_jetty_ctrl *recv_jetty;
    struct ascend_ub_msg_desc *jfs_desc;
    struct send_wr_cfg wr_cfg = {0};
    u32 seg_id = 0;
    int ret;

    admin_chan = (struct ascend_ub_admin_chan*)cfg->msg_chan;
    recv_jetty = &admin_chan->admin_jetty->recv_jetty;
    jfs_desc = ubdrv_get_send_desc(recv_jetty, seg_id);  // jfs seg_id is 0
    ret = memcpy_s(jfs_desc, recv_jetty->send_msg_len, src_desc, recv_jetty->recv_msg_len);
    if (ret != 0) {
        ubdrv_err("Copy result data to jfs_desc failed. (ret=%d;dst_len=%u;src_len=%u)",
            ret, recv_jetty->send_msg_len, recv_jetty->recv_msg_len);
        return ret;
    }
    wr_cfg.user_ctx = src_desc->msg_num;
    wr_cfg.tjetty = admin_chan->tjetty;
    wr_cfg.sva = (u64)jfs_desc;
    wr_cfg.sseg = recv_jetty->send_seg;
    wr_cfg.tseg = NULL; // Not used in send opcode
    wr_cfg.jfs = recv_jetty->jfs;
    wr_cfg.len = (recv_jetty->send_msg_len - ASCEND_UB_MSG_DESC_LEN);
    return ubdrv_send_reply_msg(dev_id, 0, &wr_cfg, cfg, stat);
}

void ubdrv_admin_recv_prepare_process(struct ascend_ub_jetty_ctrl *cfg,
                                      struct ascend_ub_msg_desc *desc, u32 seg_id)
{
    struct ascend_ub_admin_chan *chan = (struct ascend_ub_admin_chan*)cfg->msg_chan;
    struct ubdrv_msg_chan_stat *stat = &chan->chan_stat;
    u32 cost_time;
    int ret;

    ret = ubdrv_admin_recv_check_parm(desc);
    if (ret != 0) {
        ubdrv_err("Param check failed. (ret=%d)\n", ret);
        desc->status = UB_MSG_INVALID_VALUE;
        goto reply_admin_msg;
    }

    stat->rx_stamp = (u64)ka_jiffies;
    ret = ubdrv_admin_recv_process(cfg, desc, seg_id);
    if (ret != 0) {
        ubdrv_err("Recv process failed. (ret=%d)\n", ret);
        stat->rx_process_err++;
        goto reply_admin_msg;
    }
    cost_time = ubdrv_record_resq_time(stat->rx_stamp, "admin recv prepare process stamp", UBDRV_SCEH_RESP_TIME);
    if (cost_time > stat->rx_max_time) {
        stat->rx_max_time = cost_time;
    }

reply_admin_msg:
    ret = ubdrv_send_admin_reply_msg(chan->dev_id, cfg, desc, stat);
    if (ret != 0) {
        ubdrv_err("Admin send reply msg failed. (ret=%d)\n", ret);
    }

    if ((ret == 0) && (desc->status == UB_MSG_PROCESS_SUCCESS)) {
        ubdrv_admin_recv_finish_process(cfg, desc);
    }
    return;
}
