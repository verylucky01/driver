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
#ifndef TRS_CORE_NEAR_OPS_H
#define TRS_CORE_NEAR_OPS_H

#include "ascend_hal_define.h"
#include "trs_pub_def.h"

int trs_core_ops_get_proc_num(struct trs_id_inst *inst, u32 *proc_num);
int trs_core_ops_get_ts_inst_status(struct trs_id_inst *inst, u32 *status);
void *trs_core_ops_cq_mem_alloc(struct trs_id_inst *inst, size_t size);
void trs_core_ops_cq_mem_free(struct trs_id_inst *inst, void *vaddr, size_t size);
bool trs_host_res_is_belong_to_proc(int master_tgid, int slave_tgid, u32 udevid, struct res_map_info_in *res_info);
#endif /* TRS_CORE_NEAR_OPS_H */
