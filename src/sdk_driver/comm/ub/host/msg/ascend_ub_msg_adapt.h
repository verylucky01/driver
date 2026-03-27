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
#ifndef _ASCEND_UB_MSG_ADAPT_H_
#define _ASCEND_UB_MSG_ADAPT_H_

#include "ascend_ub_dev.h"

struct ubdrv_common_msg {
    void *msg_chan[ASCEND_UB_DEV_MAX_NUM];
    int (*common_fun[ASCEND_UB_DEV_MAX_NUM][DEVDRV_COMMON_MSG_TYPE_MAX])(u32 devid,
        void *data, u32 in_data_len, u32 out_data_len, u32 *real_out_len);
    struct ubdrv_common_msg_stat com_msg_stat[ASCEND_UB_DEV_MAX_NUM][DEVDRV_COMMON_MSG_TYPE_MAX];
    ka_rw_semaphore_t rwlock[ASCEND_UB_DEV_MAX_NUM];
};

int devdrv_ub_register_common_msg_client(const struct devdrv_common_msg_client *msg_client);
int devdrv_ub_unregister_common_msg_client(u32 devid, const struct devdrv_common_msg_client *msg_client);
int devdrv_ub_common_msg_send(u32 devid, void *data, u32 in_data_len, u32 out_data_len, u32 *real_out_len,
    enum devdrv_common_msg_type msg_type);
int devdrv_ub_msg_alloc_non_trans_queue_process(struct ubdrv_non_trans_chan *chan,
    struct ubdrv_create_non_trans_cmd *cmd);
int devdrv_ub_msg_free_non_trans_queue_process(struct ubdrv_non_trans_chan *chan, struct ubdrv_free_non_trans_cmd *cmd);
void *devdrv_ub_msg_alloc_non_trans_queue(u32 dev_id, struct devdrv_non_trans_msg_chan_info *chan_info);
int devdrv_ub_msg_free_non_trans_queue(void *msg_chan);
int devdrv_ub_sync_msg_send(void *msg_chan, void *data, u32 in_data_len, u32 out_data_len, u32 *real_out_len);

#endif