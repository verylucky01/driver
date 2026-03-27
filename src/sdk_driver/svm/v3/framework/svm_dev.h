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

#ifndef SVM_DEV_H
#define SVM_DEV_H

#include "pbl_array_ctx.h"
#include "svm_proc_fs.h"

struct svm_dev_feature {
    const char *name;
    void *priv;
};

struct svm_dev_ctx {
    u32 udevid;
    u32 cur_task_id;
    struct array_ctx *ctx;
    struct task_ctx_domain *domain;
    ka_proc_dir_entry_t *entry;
    struct svm_dev_feature feature[];
};

int svm_dev_init(void);
void svm_dev_uninit(void);

#endif

