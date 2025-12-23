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
#include "trs_id.h"
#include "ascend_kernel_hal.h"
#include "trs_chan_near_ops_id.h"

int trs_chan_ops_sqcq_speified_id_alloc(struct trs_id_inst *inst, int type, u32 flag, u32 *id, void *para)
{
    return trs_id_alloc_ex(inst, type, flag, id, 1);
}
KA_EXPORT_SYMBOL_GPL(trs_chan_ops_sqcq_speified_id_alloc);

int trs_chan_ops_sqcq_speified_id_free(struct trs_id_inst *inst, int type, u32 flag, u32 id)
{
    return trs_id_free_ex(inst, type, flag, id);
}
KA_EXPORT_SYMBOL_GPL(trs_chan_ops_sqcq_speified_id_free);
