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
#ifndef ASYNC_COPY_CORE_H
#define ASYNC_COPY_CORE_H

#include "copy_pub.h"
#include "async_copy_ctx.h"

int async_copy_task_submit(struct async_copy_ctx *ctx, struct copy_va_info *info, int *id);
int async_copy_task_submit_2d(struct async_copy_ctx *ctx, struct copy_2d_va_info *info, int *id);
int async_copy_task_submit_batch(struct async_copy_ctx *ctx, struct copy_batch_va_info *info, int *id);

int async_copy_task_wait(struct async_copy_ctx *ctx, int id);

int async_copy_ioctl_init(void);
int async_copy_2d_ioctl_init(void);
int async_copy_batch_ioctl_init(void);

#endif
