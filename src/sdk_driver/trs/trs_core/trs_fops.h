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

#ifndef TRS_FOPS_H
#define TRS_FOPS_H

#include "trs_proc.h"

int trs_core_init_module(void);
void trs_core_exit_module(void);

int ioctl_trs_stl_bind(struct trs_proc_ctx *proc_ctx, unsigned int cmd, unsigned long arg);
int ioctl_trs_stl_launch(struct trs_proc_ctx *proc_ctx, unsigned int cmd, unsigned long arg);
int ioctl_trs_stl_query(struct trs_proc_ctx *proc_ctx, unsigned int cmd, unsigned long arg);
int ioctl_trs_ub_info_query(struct trs_proc_ctx *proc_ctx, unsigned int cmd, unsigned long arg);
int ioctl_trs_msg_ctrl(struct trs_proc_ctx *proc_ctx, unsigned int cmd, unsigned long arg);
int ioctl_trs_sqcq_get(struct trs_proc_ctx *proc_ctx, unsigned int cmd, unsigned long arg);
int ioctl_trs_sqcq_restore(struct trs_proc_ctx *proc_ctx, unsigned int cmd,
                           unsigned long arg);
void trs_handle_proc_release_result(struct trs_core_ts_inst *ts_inst, struct trs_proc_ctx *proc_ctx,
                                    int exit_stage, bool is_success);
#endif

