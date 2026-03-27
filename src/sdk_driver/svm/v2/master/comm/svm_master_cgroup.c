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

#include "svm_proc_mng.h"
#include "svm_cgroup_mng.h"
#include "kernel_cgroup_mem_adapt.h"
#include "ka_task_pub.h"
#include "ka_memory_pub.h"

ka_mem_cgroup_t *devmm_enable_cgroup(ka_mem_cgroup_t **memcg, ka_pid_t pid)
{
    return NULL;
}

void devmm_disable_cgroup(ka_mem_cgroup_t *memcg, ka_mem_cgroup_t *old_memcg)
{
}
