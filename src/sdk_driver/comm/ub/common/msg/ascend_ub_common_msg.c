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
#include "comm_kernel_interface.h"
#include "ascend_ub_common.h"
#include "ascend_ub_main.h"
#include "ascend_ub_dev.h"
#include "ascend_ub_msg.h"
#include "ascend_ub_non_trans_chan.h"
#include "ascend_ub_common_msg.h"

int ubdrv_rx_msg_common_msg_process(void *msg_chan, void *data, u32 in_data_len, u32 out_data_len, u32 *real_out_len)
{
    struct ubdrv_common_msg_stat *stat;
    struct ascend_ub_msg_desc *desc;
    u64 pre_stamp;
    u32 dev_id;
    int ret = 0;

    dev_id = ubdrv_get_devid_by_non_trans_handle(msg_chan);
    if (dev_id >= ASCEND_UB_DEV_MAX_NUM) {
        ubdrv_err("Invalid chan. (dev_id=%u)\n", dev_id);
        return -EINVAL;
    }
    desc = ka_container_of(data, struct ascend_ub_msg_desc, user_data);
    if (desc->client_type >= (u32)DEVDRV_COMMON_MSG_TYPE_MAX) {
        ubdrv_err("Msg client type is not support yet. (dev_id=%u;msg_client_type=%u)\n", dev_id, desc->client_type);
        return -EOPNOTSUPP;
    }
    stat = ubdrv_get_common_stat_dfx(dev_id, desc->client_type);
    stat->rx_total++;

    pre_stamp = ka_jiffies;
    ret = ubdrv_rx_msg_common_msg_process_proc(desc, stat, dev_id, data, in_data_len, out_data_len, real_out_len);
    (void)ubdrv_record_resq_time(pre_stamp, "rx common msg call back process stamp", UBDRV_SCEH_RESP_TIME);
    if (ret != 0) {
        ubdrv_err("Common rx_process error. (dev_id=%u;msg_type=%u;ret=%d)\n",
            dev_id, desc->client_type, ret);
        stat->rx_cb_process_err++;
    }
    stat->rx_finish++;
    return ret;
}

int ubdrv_comm_msg_send_check(u32 dev_id, void *data, u32 *real_out_len)
{
    if (dev_id >= ASCEND_UB_DEV_MAX_NUM) {
        ubdrv_err("Invalid dev_id. (dev_id=%u)\n", dev_id);
        return -EINVAL;
    }
    if (data == NULL) {
        ubdrv_err("Input parameter is invalid. (dev_id=%u)\n", dev_id);
        return -EINVAL;
    }
    if (real_out_len == NULL) {
        ubdrv_err("Input parameter is invalid. (dev_id=%u)\n", dev_id);
        return -EINVAL;
    }
    return 0;
}

STATIC void ubdrv_record_common_client_dfx(struct ubdrv_common_msg_stat *stat, int ret)
{
    if (ret == -ENODEV) {
        stat->tx_enodev_err++;
    } else {
        stat->tx_chan_err++;
    }
    return;
}

void ubdrv_common_msg_send_ret_process(int ret, u32 dev_id,
    u32 type, struct ubdrv_common_msg_stat *stat)
{
    if (ret == 0) {
        return;
    } else if (ret != -ENODEV) {
        ubdrv_err("Sync common msg send inner failed. (ret=%d;dev_id=%u;msg_type=%u)\n", ret, dev_id, type);
    } else {
        ubdrv_warn("Sync common msg send unsuccessful, device status is normal. (ret=%d;dev_id=%u;msg_type=%u)\n",
            ret, dev_id, type);
    }
    ubdrv_record_common_client_dfx(stat, ret);
    return;
}