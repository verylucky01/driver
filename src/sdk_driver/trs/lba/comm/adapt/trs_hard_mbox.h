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
#ifndef TRS_HARD_MBOX_H
#define TRS_HARD_MBOX_H

#include <linux/types.h>

#include "trs_pub_def.h"

struct trs_mbox_ops {
    void (* mbox_release)(void *priv);
    int (* trigger_irq)(void *priv);
    void (* free_irq)(void *priv);
};

enum trs_mbox_mem_type {
    TRS_MBOX_MEM_TYPE_DEVICE,
    TRS_MBOX_MEM_TYPE_NORMAL
};

struct trs_mbox_chan_attr {
    phys_addr_t base;
    size_t size;

    void *priv;
    struct trs_mbox_ops ops;
    enum trs_mbox_mem_type mem_type;
    int cont_tx_timeout_max;  /* ms */
};

void *trs_mbox_chan_init(struct trs_id_inst *inst, struct trs_mbox_chan_attr *attr);
void trs_mbox_chan_uninit(struct trs_id_inst *inst);

int trs_hard_mbox_send(struct trs_id_inst *inst, u32 chan_id, void *data, size_t size, int timeout);
int trs_hard_mbox_send_rpc_call_msg(struct trs_id_inst *inst, u32 chan_id, void *data, size_t size, int timeout);

void trs_mbox_chan_txdone(void *mbox_chan);
int trs_mbox_get_chan_num(struct trs_id_inst *inst);

#endif /* TRS_HARD_MBOX_H */
