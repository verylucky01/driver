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
#ifndef TRS_HOST_TS_CQ_H
#define TRS_HOST_TS_CQ_H
#include "trs_pub_def.h"

typedef int (*ts_cq_process_func)(int irq_type, int irq_index, void *para, u32 cqid[], u32 cq_num);
struct trs_ts_cq_info {
    ts_cq_process_func host_ts_cq_process_func;
    void *para;
};

int trs_register_ts_cq_process_info(u32 udevid, ts_cq_process_func func, void *para);
void trs_unregister_ts_cq_process_info(u32 udevid);
struct trs_ts_cq_info *trs_get_ts_cq_info(u32 udevid);
int trs_ts_cq_cpy(struct trs_id_inst *inst, u32 cqid, u32 cq_type, u8 *cqe);
#endif
