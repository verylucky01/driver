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

#ifndef __TRS_SHR_SQCQ_H__
#define __TRS_SHR_SQCQ_H__

#include "ascend_hal_define.h"

int trs_shr_ctx_mng_init(struct trs_id_inst *inst);
void trs_shr_ctx_mng_uninit(struct trs_id_inst *inst);

int trs_shr_sq_remap(struct trs_proc_ctx *proc_ctx, struct trs_core_ts_inst *ts_inst,
    struct halSqCqInputInfo *para, struct trs_sq_ctx *sq_ctx, struct trs_chan_sq_info *sq_info);
void trs_shr_sq_unmap(struct trs_proc_ctx *proc_ctx, struct trs_core_ts_inst *ts_inst,
    struct trs_sq_ctx *sq_ctx);

struct trs_shr_ctx_mng *trs_get_shr_ctx(struct trs_id_inst *inst);
void trs_shr_ctx_mng_release(struct kref_safe *kref);
void trs_put_shr_ctx(struct trs_shr_ctx_mng *shr_ctx);
void trs_fill_map_addr_info(struct trs_sq_map_addr *addr_info, struct trs_mem_map_para *map_para);
void trs_clear_map_addr_info(struct trs_sq_map_addr *addr_info);
void trs_shr_put_task_struct(ka_task_struct_t *tsk);
int trs_shr_unmap_sq(struct trs_proc_ctx *proc_ctx, struct trs_core_ts_inst *ts_inst,
    struct trs_mem_unmap_para *para);
int trs_shr_remap_sq(struct trs_proc_ctx *proc_ctx, struct trs_core_ts_inst *ts_inst,
    struct trs_mem_map_para *para);
ka_task_struct_t *trs_shr_get_task_struct(pid_t pid);
#endif
