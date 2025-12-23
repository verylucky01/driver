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
#ifndef TRS_CHAN_NEAR_EVENT_UPDATE_H
#define TRS_CHAN_NEAR_EVENT_UPDATE_H
#include "trs_pub_def.h"
#include "trs_id.h"
#include "esched_kernel_interface.h"
int trs_stars_v2_chan_ops_sqcq_speicified_id_alloc(struct trs_id_inst *inst, int type, u32 *id);
int trs_stars_v2_chan_ops_sqcq_speicified_id_free(struct trs_id_inst *inst, int type, u32 id);

int _trs_sqcq_event_update(u32 devid, struct sched_published_event_info *event_info,
                           struct sched_published_event_func *event_func);
int trs_sqcq_event_update(u32 devid, struct sched_published_event_info *event_info,
                          struct sched_published_event_func *event_func);

int trs_sqcq_event_dev_init(u32 ts_inst_id);
void trs_sqcq_event_dev_uninit(u32 ts_inst_id);

int trs_sqcq_event_init(void);
void trs_sqcq_event_uninit(void);
#endif
