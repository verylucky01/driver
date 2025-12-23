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

#ifndef TRS_CORE_ADAPT_H
#define TRS_CORE_ADAPT_H
#include "ka_fs_pub.h"

#include "ascend_kernel_hal.h"
#include "trs_proc.h"
#include "trs_ioctl.h"
#include "trs_pub_def.h"

static inline struct trs_core_ts_inst *trs_core_inst_get_for_res_map(struct trs_id_inst *id_inst, struct trs_proc_ctx *proc_ctx, struct trs_cmd_res_map para)
{
    (void)para;
    id_inst->devid = proc_ctx->devid;
    id_inst->tsid = 0;
    return trs_core_inst_get(proc_ctx->devid, id_inst->tsid);
}

static inline void trs_core_printf_avail_num(struct trs_core_ts_inst *ts_inst, ka_seq_file_t *seq, int i, u32 avail_num)
{
    ka_fs_seq_printf(seq, "idx %d res %s avail_num %u\n", i, trs_id_type_to_name(i), avail_num);
}

#endif

