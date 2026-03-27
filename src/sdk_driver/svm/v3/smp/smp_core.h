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

#ifndef SMP_CORE_H
#define SMP_CORE_H

#include "ka_common_pub.h"
#include "smp_ctx.h"

void smp_mem_show(struct smp_ctx *smp_ctx, ka_seq_file_t *seq);
void smp_mem_recycle(struct smp_ctx *smp_ctx);

#endif

