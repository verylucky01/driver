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
#ifndef SVM_TASK_H
#define SVM_TASK_H

#include <linux/types.h>
#include "ka_common_pub.h"

#include "pbl_task_ctx.h"

/*
 * 2 is reserved for apm recycle scene, which process is released but hasn't delete task_ctx.
 * 3 is cp1, cp2, hccp.
 */
#define SVM_MAX_TASK_PER_DEV ((64U * 2U) * 3U)

int svm_add_task(u32 udevid, int tgid, struct task_start_time *start_time);
int svm_del_task(u32 udevid, int tgid, struct task_start_time *start_time);
void svm_task_ctx_recycle(struct task_ctx *ctx, void *priv);

void svm_show_task(u32 udevid, int tgid, int feature_id, ka_seq_file_t *seq);
int svm_task_init(void);
void svm_task_uninit(void);

#endif
