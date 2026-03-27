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
#ifndef RMO_MEM_SHARING_H
#define RMO_MEM_SHARING_H
#include "pbl/pbl_task_ctx.h"
#include "dpa/dpa_rmo_kernel.h"
#include "rmo_mem_sharing_ctx.h"

int rmo_mem_addr_map(u32 devid, u64 paddr, u64 size, struct rmo_mem_map_addr *mapped_addr);
int rmo_mem_addr_unmap(u32 devid, struct rmo_mem_map_addr *mapped_addr, u64 size);
int rmo_mem_sharing_init(void);
void rmo_mem_sharing_uninit(void);
void rmo_mem_sharing_domain_task_exit(u32 udevid, int tgid, struct task_start_time *start_time);

#endif /* RMO_MEM_SHARING_H */