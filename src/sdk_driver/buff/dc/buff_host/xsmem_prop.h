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
#ifndef XSMEM_PROP_H
#define XSMEM_PROP_H

#include "xsmem_framework.h"

void xsmem_proc_grp_prop_del(struct xsm_task_pool_node *node);
void xsmem_task_prop_del(int pid);
void xsmem_pool_prop_clear(struct xsm_pool *xp);
void xsmem_pool_task_prop_clear(struct xsm_pool *xp, const struct xsm_task *task);
int ioctl_xsmem_pool_prop_op(struct xsm_task *task, unsigned int cmd, unsigned long arg);

#endif
