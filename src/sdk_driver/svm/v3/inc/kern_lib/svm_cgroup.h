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

#ifndef SVM_CGROUP_H
#define SVM_CGROUP_H

#include "ka_common_pub.h"

int svm_cur_memcg_change(ka_pid_t pid, ka_mem_cgroup_t **old_cg, ka_mem_cgroup_t **new_cg);
void svm_cur_memcg_reset(ka_mem_cgroup_t *old_cg, ka_mem_cgroup_t *new_cg);

#endif
