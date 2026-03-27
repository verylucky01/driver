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

#ifndef KMC_D2D_H
#define KMC_D2D_H

#include "svm_pub.h"
#include "kmc_msg.h"

struct svm_kmc_d2d_recv_handle {
    int (*func)(u32 udevid, void *msg, u32 *reply_len);
    u32 raw_msg_size;
    u32 extend_gran_size;
};

void svm_kmc_d2d_recv_handle_register(enum svm_kmc_msg_id msg_id, struct svm_kmc_d2d_recv_handle *handle);

/*
* src_udevid: view of local
* dst_udevid: view of host
* *reply_len[in/out]: expect_reply_len/actual_reply_len
*/
int svm_kmc_d2d_send(u32 src_udevid, u32 dst_udevid, void *msg, u32 msg_len, u32 *reply_len);

#endif
