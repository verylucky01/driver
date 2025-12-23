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
#include "ka_kernel_def_pub.h"
#include "trs_pub_def.h"
#include "soc_adapt.h"
#include "trs_mbox.h"

static struct trs_mbox_send_ops *g_trs_soft_mbox_send_ops[TRS_DEV_MAX_NUM] = {NULL};
static struct trs_mbox_send_ops *g_trs_hard_mbox_send_ops[TRS_DEV_MAX_NUM] = {NULL};

void trs_register_soft_mbox_send_ops(u32 devid, struct trs_mbox_send_ops *ops)
{
    g_trs_soft_mbox_send_ops[devid] = ops;
}

void trs_unregister_soft_mbox_send_ops(u32 devid)
{
    g_trs_soft_mbox_send_ops[devid] = NULL;
}

void trs_register_hard_mbox_send_ops(u32 devid, struct trs_mbox_send_ops *ops)
{
    g_trs_hard_mbox_send_ops[devid] = ops;
}

void trs_unregister_hard_mbox_send_ops(u32 devid)
{
    g_trs_hard_mbox_send_ops[devid] = NULL;
}

int trs_mbox_send(struct trs_id_inst *inst, u32 chan_id, void *data, size_t size, int timeout)
{
    static struct trs_mbox_send_ops **g_trs_mbox_send_ops = NULL;

    if ((trs_id_inst_check(inst) != 0) || (data == NULL)) {
        return -EINVAL;
    }

    if (trs_soc_is_support_soft_mbox(inst)) {
        g_trs_mbox_send_ops = g_trs_soft_mbox_send_ops;
    } else {
        g_trs_mbox_send_ops = g_trs_hard_mbox_send_ops;
    }

    if ((g_trs_mbox_send_ops[inst->devid] != NULL) &&
        (g_trs_mbox_send_ops[inst->devid]->mbox_send != NULL)) {
        return g_trs_mbox_send_ops[inst->devid]->mbox_send(inst, chan_id, data, size, timeout);
    }

    trs_err("Not init mbox send ops.\n");
    return -ENODEV;
}
KA_EXPORT_SYMBOL_GPL(trs_mbox_send);

int trs_mbox_send_ex(struct trs_id_inst *inst, u32 chan_id, void *data, size_t size, int timeout)
{
    int ret = -ENODEV;

    if ((trs_id_inst_check(inst) != 0) || (data == NULL)) {
        return -EINVAL;
    }

    if ((g_trs_soft_mbox_send_ops[inst->devid] != NULL) && (g_trs_soft_mbox_send_ops[inst->devid]->mbox_send != NULL)) {
        ret = g_trs_soft_mbox_send_ops[inst->devid]->mbox_send(inst, chan_id, data, size, timeout);
        if (ret != 0 ) {
            trs_err("Soft mbox send failed.\n");
            return ret;
        }
    }

    if ((g_trs_hard_mbox_send_ops[inst->devid] != NULL) && (g_trs_hard_mbox_send_ops[inst->devid]->mbox_send != NULL)) {
        ret = g_trs_hard_mbox_send_ops[inst->devid]->mbox_send(inst, chan_id, data, size, timeout);
        if (ret != 0 ) {
            trs_err("Hard mbox send failed.\n");
            return ret;
        }
    }

    return ret;
}
KA_EXPORT_SYMBOL_GPL(trs_mbox_send_ex);

int trs_mbox_send_rpc_call_msg(struct trs_id_inst *inst, u32 chan_id, void *data, size_t size, int timeout)
{
    static struct trs_mbox_send_ops **g_trs_mbox_send_ops = NULL;

    if ((trs_id_inst_check(inst) != 0) || (data == NULL)) {
        return -EINVAL;
    }

    if (trs_soc_is_support_soft_mbox(inst)) {
        g_trs_mbox_send_ops = g_trs_soft_mbox_send_ops;
    } else {
        g_trs_mbox_send_ops = g_trs_hard_mbox_send_ops;
    }

    if ((g_trs_mbox_send_ops[inst->devid] != NULL) &&
        (g_trs_mbox_send_ops[inst->devid]->mbox_send_rpc_call_msg != NULL)) {
        return g_trs_mbox_send_ops[inst->devid]->mbox_send_rpc_call_msg(inst, chan_id, data, size, timeout);
    }

    trs_err("Not init mbox send ops.\n");
    return -ENODEV;
}
KA_EXPORT_SYMBOL_GPL(trs_mbox_send_rpc_call_msg);
