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

#ifndef TRS_GDB_SQCQ_H
#define TRS_GDB_SQCQ_H

#include "ascend_hal_define.h"
#include "trs_proc.h"

struct trs_core_ts_inst;

int trs_gdb_sqcq_send(struct trs_proc_ctx *proc_ctx, struct trs_core_ts_inst *ts_inst, struct halTaskSendInfo *para);
int trs_gdb_sqcq_recv(struct trs_proc_ctx *proc_ctx, struct trs_core_ts_inst *ts_inst, struct halReportRecvInfo *para);

int trs_gdb_sqcq_alloc(struct trs_proc_ctx *proc_ctx, struct trs_core_ts_inst *ts_inst, struct halSqCqInputInfo *para);
int trs_gdb_sqcq_free(struct trs_proc_ctx *proc_ctx, struct trs_core_ts_inst *ts_inst, struct halSqCqFreeInfo *para);
void trs_maint_sqcq_recycle(struct trs_proc_ctx *proc_ctx, struct trs_core_ts_inst *ts_inst, int res_type, u32 res_id);

int trs_maint_sqcq_init(struct trs_core_ts_inst *ts_inst);
void trs_maint_sqcq_uninit(struct trs_core_ts_inst *ts_inst);

#endif
