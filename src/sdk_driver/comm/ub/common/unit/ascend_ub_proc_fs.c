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

#include "ka_fs_pub.h"
#include "ka_compiler_pub.h"
#include "ka_system_pub.h"
#include "ka_kernel_def_pub.h"
#include "ubcore_types.h"
#include "pbl/pbl_feature_loader.h"
#include "ascend_ub_main.h"
#include "ascend_ub_link_chan.h"
#include "ascend_ub_admin_msg.h"
#include "ascend_ub_mem_decoder.h"
#include "ascend_ub_pair_dev_info.h"
#include "ascend_ub_link_chan.h"
#include "ascend_ub_proc_fs.h"

ka_atomic_t fs_valid = KA_BASE_ATOMIC_INIT(0);

STATIC struct ubdrv_procfs_entry g_procfs_entry = {
    .dev_id = NULL,
    .pair_info = NULL,
    .admin_stat = NULL,
    .nontrans_stat = NULL,
    .common_stat = NULL,
    .mem_decoder = NULL,
    .host_mem_cfg = NULL,
};

STATIC struct ubdrv_user_input g_user_data = {
    .dev_id = 0,
};

struct ubdrv_user_input *get_global_user_data(void)
{
    return &g_user_data;
}

STATIC int ubdrv_procfs_pair_info(ka_inode_t *inode, ka_file_t *file)
{
#ifdef CFG_FEATURE_SUPPORT_RUN_INSTALL
    return ka_fs_single_open(file, ubdrv_procfs_pair_info_show, pde_data(inode));
#else
    return -EOPNOTSUPP;
#endif
}

STATIC int ubdrv_procfs_get_device_admin_chan_dfx(struct ubdrv_msg_chan_stat *stat_buf, u32 chan_num,
    struct ascend_ub_msg_dev *msg_dev, struct ascend_ub_msg_desc *desc)
{
    size_t len = sizeof(struct ubdrv_msg_chan_stat);
    int ret;
    (void)chan_num;

    ret = memcpy_s(stat_buf, len, &msg_dev->admin_msg_chan.chan_stat, len);
    if (ret != 0) {
        ubdrv_err("Memcpy admin stat dfx error.(dev_id=%u;ret=%d)\n", msg_dev->dev_id, ret);
        return ret;
    }

    return 0;
}

STATIC int ubdrv_procfs_get_device_nontrans_chan_dfx(struct ubdrv_msg_chan_stat *stat_buf, u32 chan_num,
    u32 chan_id, struct ascend_ub_msg_dev *msg_dev, struct ascend_ub_msg_desc *desc)
{
    size_t len = sizeof(struct ubdrv_msg_chan_stat);
    int ret;

    if (chan_id >= chan_num) {
        ubdrv_err("Check chan_id fail.(chan_id=%u;chan_num=%u)\n", chan_id, chan_num);
        return -EINVAL;
    }

    ret = memcpy_s(stat_buf, len, &msg_dev->non_trans_chan[chan_id].chan_stat, len);
    if (ret != 0) {
        ubdrv_err("Memcpy chan stat dfx error.(dev_id=%u;ret=%d)\n", msg_dev->dev_id, ret);
        return ret;
    }

    return 0;
}

STATIC int ubdrv_procfs_get_device_common_chan_dfx(struct ubdrv_common_msg_stat *stat_buf, u32 chan_num,
    u32 dev_id, struct ascend_ub_msg_desc *desc)
{
    size_t len = sizeof(struct ubdrv_common_msg_stat);
    struct ubdrv_common_msg_stat *stat;
    int ret;
    u32 i;

    for (i = 0; i < chan_num; i++) {
        stat = ubdrv_get_common_stat_dfx(dev_id ,i);
        ret = memcpy_s(stat_buf, len, stat, len);
        if (ret != 0) {
            ubdrv_err("Memcpy client stat dfx error.(dev_id=%d;ret=%u)\n", dev_id, ret);
            return ret;
        }
        stat_buf += 1;
    }

    return 0;
}

int ubdrv_procfs_device_dfx_info(struct ascend_ub_msg_dev *msg_dev, void *data)
{
    struct ascend_ub_msg_desc *desc = (struct ascend_ub_msg_desc *)data;
    struct ubdrv_chan_dfx_cmd *data_buf = (struct ubdrv_chan_dfx_cmd *)&desc->user_data;
    u32 len = sizeof(struct ubdrv_chan_dfx_cmd);
    u32 dev_id = msg_dev->dev_id;

    if (desc->in_data_len != len) {
        ubdrv_err("Recv data len invalid, when get dfx info. (dev_id=%u;len=%u;expect_len=%u)\n",
            dev_id, desc->in_data_len, len);
        return -EINVAL;
    }

    desc->real_data_len = (u32)sizeof(struct ubdrv_chan_dfx_cmd);
    if (data_buf->opcode == UBDRV_PROCFS_ADMIN_CHAN_CMD) {
        // admin only have one chan
        return ubdrv_procfs_get_device_admin_chan_dfx(&data_buf->stat.chan, 1, msg_dev, desc);
    } else if (data_buf->opcode == UBDRV_PROCFS_NONTRANS_CHAN_CMD) {
        return ubdrv_procfs_get_device_nontrans_chan_dfx(&data_buf->stat.chan, msg_dev->chan_cnt,
            data_buf->chan_id, msg_dev, desc);
    } else if (data_buf->opcode == UBDRV_PROCFS_COMMON_CHAN_CMD) {
        return ubdrv_procfs_get_device_common_chan_dfx(data_buf->stat.client,
            UBDRV_COMMON_CLIENT_CNT, dev_id, desc);
    } else {
        ubdrv_err("Recv data sub cmd invalid. (dev_id=%u;sub_cmd=%d)\n", dev_id, data_buf->opcode);
    }
    return 0;
}

void ubdrv_procfs_show_chan_dfx(ka_seq_file_t *seq, void *offset,
    struct ubdrv_msg_chan_stat *stat, u32 chan_id, char *name)
{
    ka_fs_seq_printf(seq, "|%-8u|%-10s|%-9llu|%-7llu|%-7llu|%-15llu|%-16llu"
        "|%-11llu|%-10llu|%-17llu|%-9llu|%-18llu|%-13llu|%-14llu"
        "|%-9llu|%-8llu|%-16llu|%-11llu|%-12llu|%-13llu|%-10llu|%-16llu"
        "|%-10llu|%-19llu|%-18llu|%-10llu|%-14llu|%-14llu"
        "|%-12u|%-17u"
        "|%-9u|%-7u|%-11u|%-7u|%-11u|%-7u|%-11u|%-7u|%-10u|\n",
        chan_id, name, stat->tx_total, stat->tx_post_send_err,  stat->tx_cqe, stat->tx_cqe_timeout, stat->tx_poll_cqe_err,
        stat->tx_cqe_status_err, stat->tx_rebuild_jfs, stat->tx_recv_cqe_timeout, stat->tx_recv_cqe, stat->tx_recv_poll_cqe_err, stat->tx_recv_cqe_status_err, stat->tx_recv_data_err,
        stat->rx_total, stat->rx_work, stat->rx_poll_cqe_err, stat->rx_cqe_status_err, stat->rx_process_err, stat->rx_null_process,
        stat->rx_work_finish, stat->rx_post_jfr_err,
        stat->rx_tx_post_err, stat->rx_tx_poll_cqe_err, stat->rx_tx_cqe_timeout, stat->rx_tx_cqe, stat->rx_tx_cqe_status_err, stat->rx_tx_rebuild_jfs,
        stat->rx_max_time, stat->rx_work_max_time,
        stat->jfae_err, stat->tx_jfr_id, stat->tx_jfr_jfc_id, stat->tx_jfs_id, stat->tx_jfs_jfc_id, stat->rx_jfr_id,
        stat->rx_jfr_jfc_id, stat->rx_jfs_id, stat->rx_jfs_jfc_id);
    return;
}

void ubdrv_procfs_show_common_chan_dfx_head(ka_seq_file_t *seq)
{
    ka_fs_seq_printf(seq, "| client_type | tx_total | tx_check_data_err | tx_chan_null_err | tx_enodev_err "
        "| tx_chan_err | rx_total | rx_cb_process_err | rx_null_cb_err | rx_finish |\n");
    return;
}

void ubdrv_procfs_show_common_chan_dfx(ka_seq_file_t *seq, void *offset,
    struct ubdrv_common_msg_stat *stat, char *str)
{
    ka_fs_seq_printf(seq, "|%-13s|%-10llu|%-19llu|%-18llu|%-15llu|%-13llu|%-10llu|%-19llu|%-16llu|%-11llu|\n",
        str, stat->tx_total, stat->tx_check_data_err, stat->tx_chan_null_err, stat->tx_enodev_err,
        stat->tx_chan_err, stat->rx_total, stat->rx_cb_process_err, stat->rx_null_cb_err, stat->rx_finish);
    return;
}

int ubdrv_procfs_send_admin_msg(struct ubdrv_chan_dfx_cmd *cmd, u32 dev_id, u32 sub_cmd, u32 chan_id)
{
    struct ascend_ub_user_data user_desc = {0};

    cmd->opcode = sub_cmd;
    cmd->chan_id = chan_id;
    user_desc.opcode = UBDRV_SYNC_DFX_INFO;
    user_desc.size = (u32)sizeof(struct ubdrv_chan_dfx_cmd);
    user_desc.reply_size = (u32)sizeof(struct ubdrv_chan_dfx_cmd);
    user_desc.cmd = cmd;
    user_desc.reply = cmd;
    return ubdrv_admin_send_msg(dev_id, &user_desc);
}

STATIC_PROCFS_FILE_FUNC_OPS(pair_info, ubdrv_procfs_pair_info, NULL);

STATIC int ubdrv_proc_create_data(ka_proc_dir_entry_t *top_entry)
{
    g_procfs_entry.pair_info = ka_fs_proc_create_data("pair_info", KA_S_IRUSR, top_entry, &pair_info, &g_user_data.dev_id);
    if (g_procfs_entry.pair_info == NULL) {
        ubdrv_err("Creat proc pair_info data error.\n");
        return -EBADR;
    }

    return ubdrv_proc_create_data_proc(top_entry, &g_procfs_entry);
}

int ubdrv_procfs_init(void)
{
    ka_proc_dir_entry_t *top_entry = NULL;
    int ret;

    top_entry = ka_fs_proc_mkdir("asdrv_ub", NULL);
    if (top_entry == NULL) {
        ubdrv_err("Creat proc dir error.\n");
        return -EINVAL;
    }
    ret = ubdrv_proc_create_data(top_entry);
    if (ret != 0) {
        ubdrv_err("Creat proc dir error.\n");
        (void)ka_fs_remove_proc_subtree("asdrv_ub", NULL);
        return ret;
    }
    ka_base_atomic_set(&fs_valid, UBDRV_PROCFS_VALID);

    return 0;
}
DECLAER_FEATURE_AUTO_INIT(ubdrv_procfs_init, FEATURE_LOADER_STAGE_5);

void ubdrv_procfs_uninit(void)
{
    if (ka_base_atomic_read(&fs_valid) == UBDRV_PROCFS_VALID) {
        (void)ka_fs_remove_proc_subtree("asdrv_ub", NULL);
    }
    return;
}
DECLAER_FEATURE_AUTO_UNINIT(ubdrv_procfs_uninit, FEATURE_LOADER_STAGE_5);
