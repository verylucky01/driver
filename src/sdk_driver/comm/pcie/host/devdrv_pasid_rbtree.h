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
#ifndef _DEVDRV_RBTREE_H_
#define _DEVDRV_RBTREE_H_

#include <linux/rbtree.h>

#include "comm_cmd_msg.h"
#include "devdrv_pci.h"

struct devdrv_dma_pasid_rbtree_node {
    struct rb_node node;
    u32 dev_id;
    u64 hash_va;  // process pasid
};

struct devdrv_dma_pasid_rbtree_ctrl {
    spinlock_t rb_lock;
    struct rb_root rbtree;
    struct devdrv_msg_chan *msg_chan;
};

enum devdrv_pasid_op_code {
    DEVDRV_PASID_ADD = 0,
    DEVDRV_PASID_DEL,
    DEVDRV_PASID_MAX,
};

bool devdrv_dma_pasid_valid_check(u32 dev_id, u64 pasid, int env_boot_mode);
void devdrv_pasid_rbtree_init(struct devdrv_pci_ctrl *pci_ctrl);
void devdrv_pasid_rbtree_uninit(struct devdrv_pci_ctrl *pci_ctrl);
int devdrv_pasid_non_trans_init(struct devdrv_pci_ctrl *pci_ctrl);
void devdrv_pasid_non_trans_uninit(struct devdrv_pci_ctrl *pci_ctrl);
#endif