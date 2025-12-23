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
#include <linux/rbtree.h>
#include <linux/spinlock.h>

#include "pbl/pbl_uda.h"
#include "devdrv_pasid_rbtree.h"
#include "devdrv_mem_alloc.h"
#include "devdrv_ctrl.h"
#include "devdrv_util.h"

#ifdef CFG_FEATURE_DMA_COPY_SVA
#define DEVDRV_NON_TRANS_MSG_SVA_DESC_SIZE 0x400
struct devdrv_dma_pasid_rbtree_ctrl g_pasid_rbtree[MAX_DEV_CNT];

STATIC int devdrv_dma_pasid_node_insert(spinlock_t *lock, struct rb_root *root,
    struct devdrv_dma_pasid_rbtree_node *dma_pasid_node)
{
    struct rb_node *parent = NULL;
    struct rb_node **new_node = NULL;
    u64 new_node_hash, tree_hash;

    spin_lock_bh(lock);
    new_node = &(root->rb_node);
    new_node_hash = dma_pasid_node->hash_va;

    /* Figure out where to put new node */
    while (*new_node) {
        struct devdrv_dma_pasid_rbtree_node *this = rb_entry(*new_node, struct devdrv_dma_pasid_rbtree_node, node);

        parent = *new_node;
        tree_hash = this->hash_va;
        if (new_node_hash < tree_hash) {
            new_node = &((*new_node)->rb_left);
        } else if (new_node_hash > tree_hash) {
            new_node = &((*new_node)->rb_right);
        } else {
            spin_unlock_bh(lock);
            return -EEXIST; // Node already exists
        }
    }

    /* Add new node and rebalance tree. */
    rb_link_node(&dma_pasid_node->node, parent, new_node);
    rb_insert_color(&dma_pasid_node->node, root);

    spin_unlock_bh(lock);

    return 0;
}

STATIC struct devdrv_dma_pasid_rbtree_node *devdrv_dma_pasid_node_search(spinlock_t *lock, struct rb_root *root,
    u64 hash_va)
{
    u64 tree_hash;
    struct rb_node *node = NULL;
    struct devdrv_dma_pasid_rbtree_node *dma_pasid_node = NULL;

    if (lock != NULL) {
        spin_lock_bh(lock);
    }
    node = root->rb_node;
    while (node != NULL) {
        dma_pasid_node = rb_entry(node, struct devdrv_dma_pasid_rbtree_node, node);
        tree_hash = dma_pasid_node->hash_va;

        if (hash_va < tree_hash) {
            node = node->rb_left;
        } else if (hash_va > tree_hash) {
            node = node->rb_right;
        } else {
            if (lock != NULL) {
                spin_unlock_bh(lock);
            }
            return dma_pasid_node;
        }
    }

    if (lock != NULL) {
        spin_unlock_bh(lock);
    }
    return NULL;
}

STATIC void devdrv_dma_pasid_node_erase(spinlock_t *lock, struct rb_root *root,
    struct devdrv_dma_pasid_rbtree_node *dma_pasid_node)
{
    // Erase may run with other processes under a lock or alone; thus, the lock can be null for easier combination.
    if (lock != NULL) {
        spin_lock_bh(lock);
    }
    rb_erase(&dma_pasid_node->node, root);
    if (lock != NULL) {
        spin_unlock_bh(lock);
    }
}

STATIC int devdrv_pasid_rbtree_add(u32 dev_id, u64 pasid)
{
    struct devdrv_dma_pasid_rbtree_node *node = NULL;
    int ret;

    if (dev_id >= MAX_DEV_CNT) {
        devdrv_err("Invalid dev_id.(dev_id=%u)\n", dev_id);
        return -EINVAL;
    }

    node = devdrv_kzalloc(sizeof(struct devdrv_dma_pasid_rbtree_node), GFP_KERNEL | __GFP_ACCOUNT);
    if (node == NULL) {
        devdrv_err("Alloc mem for rb_tree failed.(dev_id=%u).\n", dev_id);
        return -ENOMEM;
    }

    node->dev_id = dev_id;
    node->hash_va = pasid;

    ret = devdrv_dma_pasid_node_insert(&g_pasid_rbtree[dev_id].rb_lock, &g_pasid_rbtree[dev_id].rbtree, node);
    if (ret != 0) {
        devdrv_err("Insert rbtree failed.(dev_id=%u; pasid=%llu)\n", dev_id, pasid);
        devdrv_kfree(node);
        return ret;
    }

    return 0;
}

STATIC int devdrv_pasid_rbtree_del(u32 dev_id, u64 pasid)
{
    struct devdrv_dma_pasid_rbtree_node *dma_pasid_node = NULL;
    struct devdrv_dma_pasid_rbtree_ctrl *tree = NULL;

    if (dev_id >= MAX_DEV_CNT) {
        devdrv_err("Invalid dev_id.(dev_id=%u)\n", dev_id);
        return -EINVAL;
    }

    // Erase node from local tree
    tree = &g_pasid_rbtree[dev_id];
    spin_lock_bh(&tree->rb_lock);
    dma_pasid_node = devdrv_dma_pasid_node_search(NULL, &tree->rbtree, pasid);
    if (dma_pasid_node == NULL) {
        spin_unlock_bh(&tree->rb_lock);
        devdrv_err("Pasid is not in rbtree.(dev_id=%u; pasid=%llu)\n", dev_id, pasid);
        return -ENOENT; // Can not find node
    }

    devdrv_dma_pasid_node_erase(NULL, &tree->rbtree, dma_pasid_node);
    spin_unlock_bh(&tree->rb_lock);
    devdrv_kfree(dma_pasid_node);
    return 0;
}

STATIC int devdrv_pasid_msg_edge_check(u32 devid, void *data, u32 in_data_len, u32 out_data_len, u32 *p_real_out_len)
{
    u32 max_data_len = DEVDRV_NON_TRANS_MSG_SVA_DESC_SIZE - DEVDRV_NON_TRANS_MSG_HEAD_LEN;
    struct devdrv_pasid_msg *recv_msg = NULL;

    if ((devid >= MAX_DEV_CNT) || (data == NULL) || (p_real_out_len == NULL)) {
        devdrv_err("Input parameter is error.(devid=%u)\n", devid);
        return -EINVAL;
    }

    if ((in_data_len < sizeof(struct devdrv_pasid_msg)) || (in_data_len > max_data_len)) {
        devdrv_err("Input parameter in_data_len is error.(devid=%u; in_data_len=0x%x)\n", devid, in_data_len);
        return -EINVAL;
    }

    recv_msg = (struct devdrv_pasid_msg *)data;
    if ((recv_msg->op_code >= (u32)DEVDRV_PASID_MAX) || (recv_msg->dev_id >= MAX_DEV_CNT)) {
        devdrv_err("Pasid message channel recv message op_code or devid error.(dev_id=%u; msg_type=%u; msg_devid=%u;)"
            ")\n", devid, recv_msg->op_code, recv_msg->dev_id);
        return -EINVAL;
    }

    return 0;
}

STATIC int devdrv_pasid_non_trans_msg_recv(void *msg_chan, void *data, u32 in_data_len,
    u32 out_data_len, u32 *real_out_len)
{
    u32 devid = (u32)devdrv_get_msg_chan_devid_inner(msg_chan);
    struct devdrv_pasid_msg *recv_msg;
    u32 op_code;
    int ret;

    if (devdrv_pasid_msg_edge_check(devid, data, in_data_len, out_data_len, real_out_len) != 0) {
        devdrv_err("Input parameters check failed.(devid=%u)\n", devid);
        return -EINVAL;
    }
    *real_out_len = sizeof(struct devdrv_pasid_msg);
    recv_msg = (struct devdrv_pasid_msg *)data;
    op_code = recv_msg->op_code;
    if (op_code == DEVDRV_PASID_ADD) {
        ret = devdrv_pasid_rbtree_add(recv_msg->dev_id, recv_msg->hash_va);
        if (ret != 0) {
            devdrv_err("Add node to rbtree failed.(dev_id=%u; pasid=%llu)\n", devid, recv_msg->hash_va);
        }
    } else {
        ret = devdrv_pasid_rbtree_del(recv_msg->dev_id, recv_msg->hash_va);
        if (ret != 0) {
            devdrv_err("Del node from rbtree failed.(dev_id=%u; pasid=%llu)\n", devid, recv_msg->hash_va);
        }
    }

    recv_msg->error_code = ret;
    return 0;
}

STATIC void devdrv_pasid_rbtree_clear(u32 dev_id)
{
    struct devdrv_dma_pasid_rbtree_node *dma_pasid_node = NULL;
    struct devdrv_dma_pasid_rbtree_ctrl *tree = NULL;
    struct rb_node *node = NULL;

    tree = &g_pasid_rbtree[dev_id];
    spin_lock_bh(&tree->rb_lock);

    while((node = rb_first(&tree->rbtree)) != NULL) {
        dma_pasid_node = rb_entry(node, struct devdrv_dma_pasid_rbtree_node, node);
        devdrv_dma_pasid_node_erase(NULL, &tree->rbtree, dma_pasid_node);
        spin_unlock_bh(&tree->rb_lock);
        devdrv_kfree(dma_pasid_node);
        spin_lock_bh(&tree->rb_lock);
    }

    spin_unlock_bh(&tree->rb_lock);

    return;
}

STATIC struct devdrv_non_trans_msg_chan_info devdrv_pasid_non_trans_msg_chan_info = {
    .msg_type = devdrv_msg_client_pasid,
    .flag = 0,
    .level = DEVDRV_MSG_CHAN_LEVEL_HIGH,
    .s_desc_size = DEVDRV_NON_TRANS_MSG_SVA_DESC_SIZE,
    .c_desc_size = DEVDRV_NON_TRANS_MSG_SVA_DESC_SIZE,
    .rx_msg_process = devdrv_pasid_non_trans_msg_recv,
};
#endif

bool devdrv_dma_pasid_valid_check(u32 dev_id, u64 pasid, int env_boot_mode)
{
#ifdef CFG_FEATURE_DMA_COPY_SVA
    struct devdrv_dma_pasid_rbtree_node *dma_pasid_node = NULL;
    struct devdrv_dma_pasid_rbtree_ctrl *tree = NULL;

    if (dev_id >= MAX_DEV_CNT) {
        devdrv_err_spinlock("Invalid dev_id.(dev_id=%u)\n", dev_id);
        return false;
    }

    if ((env_boot_mode == DEVDRV_MDEV_FULL_SPEC_VF_VM_BOOT) || (env_boot_mode == DEVDRV_MDEV_VF_VM_BOOT)) {
        return true;
    }

    tree = &g_pasid_rbtree[dev_id];
    dma_pasid_node = devdrv_dma_pasid_node_search(&tree->rb_lock, &tree->rbtree, pasid);
    if (dma_pasid_node == NULL) {
        devdrv_err_spinlock("Pasid is not in rbtree.(dev_id=%u; pasid=%llu)\n", dev_id, pasid);
        return false;
    }
#endif

    return true;
}

void devdrv_pasid_rbtree_init(struct devdrv_pci_ctrl *pci_ctrl)
{
#ifdef CFG_FEATURE_DMA_COPY_SVA
    g_pasid_rbtree[pci_ctrl->dev_id].rbtree = RB_ROOT;
    spin_lock_init(&g_pasid_rbtree[pci_ctrl->dev_id].rb_lock);
#endif
    return;
}

void devdrv_pasid_rbtree_uninit(struct devdrv_pci_ctrl *pci_ctrl)
{
#ifdef CFG_FEATURE_DMA_COPY_SVA
    devdrv_pasid_rbtree_clear(pci_ctrl->dev_id);
#endif
    return;
}

int devdrv_pasid_non_trans_init(struct devdrv_pci_ctrl *pci_ctrl)
{
#ifdef CFG_FEATURE_DMA_COPY_SVA
    void *msg_chan = NULL;

    if (pci_ctrl->virtfn_flag == DEVDRV_SRIOV_TYPE_VF) {
        return 0;
    }

    msg_chan = devdrv_pcimsg_alloc_non_trans_queue_inner(pci_ctrl->dev_id, &devdrv_pasid_non_trans_msg_chan_info);
    if (msg_chan == NULL) {
        devdrv_err("Alloc common msg_queue failed, msg_chan is null. (dev_id=%u)\n", pci_ctrl->dev_id);
        return -EINVAL;
    }

    /* save common msg_chan to msg_dev */
    g_pasid_rbtree[pci_ctrl->dev_id].msg_chan = (struct devdrv_msg_chan *)msg_chan;
#endif
    return 0;
}

void devdrv_pasid_non_trans_uninit(struct devdrv_pci_ctrl *pci_ctrl)
{
#ifdef CFG_FEATURE_DMA_COPY_SVA
    int ret;

    if (pci_ctrl->virtfn_flag == DEVDRV_SRIOV_TYPE_VF) {
        return;
    }

    ret = devdrv_pcimsg_free_non_trans_queue_inner((void *)g_pasid_rbtree[pci_ctrl->dev_id].msg_chan);
    if (ret != 0) {
        devdrv_info("No need to free common msg_queue. (dev_id=%u; ret=%d)\n", pci_ctrl->dev_id, ret);
    }

    g_pasid_rbtree[pci_ctrl->dev_id].msg_chan = NULL;
#endif
    return;
}