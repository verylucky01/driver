/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2026. All rights reserved.
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
#ifndef RMO_MEM_SHARING_CTX_H
#define RMO_MEM_SHARING_CTX_H
#include "pbl/pbl_task_ctx.h"
#include "ka_list_pub.h"
#include "rmo_ioctl.h"

struct rmo_mem_map_addr {
    void *addr_ptr;
    struct rmo_mem_raw_addr raw_addr;
};

struct rmo_mem_sharing_info {
    u64 sharing_pa;
    struct rmo_mem_map_addr convert_addr;
    struct rmo_cmd_mem_sharing mem_shr;
};

struct rmo_mem_sharing_node {
    ka_list_head_t node;
    struct rmo_mem_sharing_info ctx;
};

struct rmo_mem_sharing_ctx {
    ka_list_head_t head;
};

int rmo_mem_sharing_add_node(struct task_ctx_domain *domain, int tgid, struct rmo_mem_sharing_info *para);
int rmo_mem_sharing_del_node(struct task_ctx_domain *domain, int tgid, struct rmo_mem_sharing_info *para);
int rmo_mem_sharing_query_node(struct task_ctx_domain *domain, int tgid, struct rmo_mem_sharing_info *para);
void rmo_mem_sharing_ctx_destroy(struct task_ctx_domain *domain, int tgid);

#endif
