/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025. All rights reserved.
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

#ifndef MMS_CORE_H
#define MMS_CORE_H

#include "ka_memory_pub.h"
#include "ka_fs_pub.h"

#include "mms_def.h"
#include "mms_ctx.h"

#include "ascend_kernel_hal.h"

int mms_stats_mem_cfg(struct mms_ctx *mms_ctx, u64 va);
void mms_stats_mem_decfg(struct mms_ctx *mms_ctx);
void mms_mem_task_show(u32 udevid, struct mms_stats *mms_stats, ka_seq_file_t *seq);

#endif
/* MSS: module memory statistics. */