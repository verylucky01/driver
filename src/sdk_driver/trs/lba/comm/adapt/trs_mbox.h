/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
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
#ifndef TRS_MBOX_H
#define TRS_MBOX_H
#include "trs_pub_def.h"

struct trs_mbox_send_ops {
    int (*mbox_send)(struct trs_id_inst *inst, u32 chan_id, void *data, size_t size, int timeout);
    int (*mbox_send_rpc_call_msg)(struct trs_id_inst *inst, u32 chan_id, void *data, size_t size, int timeout);
};

void trs_register_soft_mbox_send_ops(u32 devid, struct trs_mbox_send_ops *ops);
void trs_unregister_soft_mbox_send_ops(u32 devid);

void trs_register_hard_mbox_send_ops(u32 devid, struct trs_mbox_send_ops *ops);
void trs_unregister_hard_mbox_send_ops(u32 devid);

int trs_mbox_send(struct trs_id_inst *inst, u32 chan_id, void *data, size_t size, int timeout);
int trs_mbox_send_ex(struct trs_id_inst *inst, u32 chan_id, void *data, size_t size, int timeout);
int trs_mbox_send_rpc_call_msg(struct trs_id_inst *inst, u32 chan_id, void *data, size_t size, int timeout);

#endif