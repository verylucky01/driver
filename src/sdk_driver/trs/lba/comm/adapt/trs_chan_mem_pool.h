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
#ifndef TRS_CHAN_MEM_POOL_H
#define TRS_CHAN_MEM_POOL_H

#include <linux/types.h>

#include "ka_common_pub.h"

#include "trs_pub_def.h"

struct trs_chan_mem_node_ops {
    void* (*alloc_ddr)(struct trs_id_inst *inst, size_t size, phys_addr_t *phy_addr);
    void (*free_ddr)(struct trs_id_inst *inst, void *vaddr, size_t size, phys_addr_t phy_addr);
};

struct trs_chan_mem_node_attr {
    struct trs_id_inst inst;
    struct trs_chan_mem_node_ops ops;
    phys_addr_t phy_addr;
    void *vaddr;
    size_t size;
    bool is_dma_addr;
};

void *trs_chan_mem_get_from_mem_list(struct trs_id_inst *inst, size_t size, phys_addr_t *phy_addr);
int trs_chan_mem_put_to_mem_list(struct trs_chan_mem_node_attr *attr);
void trs_chan_mem_node_recycle(void);
void trs_chan_mem_node_recycle_by_dev(u32 devid);
void trs_chan_mem_node_proc_fs_init(void);
void trs_chan_mem_node_proc_fs_uninit(void);

int chan_mem_node_ops_open(ka_inode_t *inode, ka_file_t *file);
ssize_t chan_mem_node_ops_write(ka_file_t *filp, const char __user *ubuf, size_t count, loff_t *ppos);
#endif