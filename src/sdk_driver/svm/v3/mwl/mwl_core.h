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

#ifndef MWL_CORE_H
#define MWL_CORE_H

#include "ka_common_pub.h"
#include "mwl_ctx.h"

void mwl_mem_show(struct mwl_ctx *mwl_ctx, ka_seq_file_t *seq);
void mwl_mem_recycle(struct mwl_ctx *mwl_ctx);

#endif

