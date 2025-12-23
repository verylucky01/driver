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
#ifndef SVM_GFP_H
#define SVM_GFP_H

#include <linux/types.h>
#include <linux/mm.h>
#include "ka_common_pub.h"

struct devmm_phy_addr_attr {
    u32 side;
    u32 devid;
    u32 vfid;
    u32 module_id;

    u32 pg_type;
    u32 mem_type;
    u32 numa_id; // only host used
    bool is_continuous;
    bool is_compound_page;
    bool is_giant_page;
    bool is_already_clear;
};

int devmm_alloc_pages(struct devmm_phy_addr_attr *attr, ka_page_t **pages, u64 pg_num);
void devmm_free_pages(struct devmm_phy_addr_attr *attr, ka_page_t **pages, u64 pg_num);

void devmm_put_normal_page(ka_page_t *pg);
void devmm_put_huge_page(ka_page_t *hpage);

#endif
