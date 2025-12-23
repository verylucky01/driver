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
#include "ka_task_pub.h"
#include "ka_list_pub.h"
#include "ka_common_pub.h"
#include "ka_base_pub.h"
#include "ka_fs_pub.h"
#include "ka_kernel_def_pub.h"

#include "securec.h"
#include "trs_timestamp.h"
#include "trs_chan_mem_pool.h"

#define PROC_FS_MODE 0444

struct trs_chan_mem_node {
    ka_list_head_t list;
    struct trs_chan_mem_node_attr attr;
    u64 timestamp;
};

static KA_LIST_HEAD(os_mem);
static KA_TASK_DEFINE_MUTEX(os_mem_mutex);
static size_t os_mem_total_size = 0;
static bool magic_check_enable = false;
static bool free_to_mem_pool[TRS_DEV_MAX_NUM];

static bool trs_chan_mem_node_can_deleted(struct trs_chan_mem_node *node)
{
    return ((trs_get_us_timestamp() - node->timestamp) >= 300000);  /* 300000us = 300ms */
}

static void trs_chan_mem_node_ddr_magic_check(struct trs_chan_mem_node *node)
{
    int i, loop = node->attr.size / sizeof(u64);
    u64 *d_word = (u64*)(uintptr_t)node->attr.vaddr;
    u64 res = 0;

    if (!magic_check_enable) {
        return;
    }

    for (i = 0; i < loop; i++) {
        res |= (*d_word);
        d_word++;
    }

    if (res != 0) {
        trs_warn("Mem is written. (devid=%u)\n", node->attr.inst.devid);
    }
}

static int trs_chan_mem_node_alloc(struct trs_chan_mem_node_attr *attr)
{
    struct trs_chan_mem_node *node = NULL;

    if ((attr->ops.alloc_ddr == NULL) || (attr->ops.free_ddr == NULL)) {
        trs_err("Hook is null.\n");
        return -EINVAL;
    }

    node = trs_vzalloc(sizeof(struct trs_chan_mem_node));
    if (node == NULL) {
        return -ENOMEM;
    }

    node->attr = *attr;
    node->timestamp = trs_get_us_timestamp();
    ka_list_add_tail(&node->list, &os_mem);

    memset_s(attr->vaddr, attr->size, 0, attr->size);
    os_mem_total_size += attr->size;

    return 0;
}

static void trs_chan_mem_node_free(struct trs_chan_mem_node *node)
{
    os_mem_total_size -= node->attr.size;
    trs_chan_mem_node_ddr_magic_check(node);
    ka_list_del(&node->list);
    trs_vfree(node);
}

static void trs_chan_mem_node_over_total_size_handle(size_t size)
{
    struct trs_chan_mem_node *node = NULL;
    struct trs_chan_mem_node *next = NULL;
    size_t free_size = 0;

    if ((os_mem_total_size + size) <= 0x40000000) { /* os mem list total size is 1G. */
        return;
    }

    ka_list_for_each_entry_safe(node, next, &os_mem, list) {
        if (free_size >= size) {
            break;
        }
        node->attr.ops.free_ddr(&node->attr.inst, node->attr.vaddr, node->attr.size, node->attr.phy_addr);
        free_size += node->attr.size;
        trs_chan_mem_node_free(node);
    }
}

static void trs_chan_mem_node_unmatch_size_hanlde(struct trs_chan_mem_node *node)
{
    if (!trs_chan_mem_node_can_deleted(node)) {
        return;
    }

    node->attr.ops.free_ddr(&node->attr.inst, node->attr.vaddr, node->attr.size, node->attr.phy_addr);
    trs_chan_mem_node_free(node);
}

void *trs_chan_mem_get_from_mem_list(struct trs_id_inst *inst, size_t size, phys_addr_t *phy_addr)
{
    struct trs_chan_mem_node *node = NULL;
    struct trs_chan_mem_node *next = NULL;
    void *vaddr = NULL;

    ka_task_mutex_lock(&os_mem_mutex);
    free_to_mem_pool[inst->devid] = true;
    ka_list_for_each_entry_safe(node, next, &os_mem, list) {
        if (node->attr.size != size) {
            trs_chan_mem_node_unmatch_size_hanlde(node);
            continue;
        }

        if (node->attr.is_dma_addr && (node->attr.inst.devid != inst->devid)) {
            continue;
        } else {
            *phy_addr = node->attr.phy_addr;
            vaddr = node->attr.vaddr;
            trs_chan_mem_node_free(node);
            break;
        }
    }
    ka_task_mutex_unlock(&os_mem_mutex);
    return vaddr;
}

int trs_chan_mem_put_to_mem_list(struct trs_chan_mem_node_attr *attr)
{
    int ret;

    ka_task_mutex_lock(&os_mem_mutex);
    if (free_to_mem_pool[attr->inst.devid] == false) {
        attr->ops.free_ddr(&attr->inst, attr->vaddr, attr->size, attr->phy_addr);
        ka_task_mutex_unlock(&os_mem_mutex);
        return 0;
    }

    trs_chan_mem_node_over_total_size_handle(attr->size);
    ret = trs_chan_mem_node_alloc(attr);
    ka_task_mutex_unlock(&os_mem_mutex);
    return ret;
}

void trs_chan_mem_node_recycle(void)
{
    struct trs_chan_mem_node *node = NULL;
    struct trs_chan_mem_node *next = NULL;
    u32 devid;

    ka_task_mutex_lock(&os_mem_mutex);
    ka_list_for_each_entry_safe(node, next, &os_mem, list) {
        node->attr.ops.free_ddr(&node->attr.inst, node->attr.vaddr, node->attr.size, node->attr.phy_addr);
        trs_chan_mem_node_free(node);
    }

    if (os_mem_total_size != 0) {
        trs_err("Memory leak.\n");
    }
#ifndef EMU_ST
    for (devid = 0; devid < TRS_DEV_MAX_NUM; devid++) {
        free_to_mem_pool[devid] = false;
    }
#endif
    ka_task_mutex_unlock(&os_mem_mutex);
}

void trs_chan_mem_node_recycle_by_dev(u32 devid)
{
    struct trs_chan_mem_node *node = NULL;
    struct trs_chan_mem_node *next = NULL;

    ka_task_mutex_lock(&os_mem_mutex);
    free_to_mem_pool[devid] = false;
    ka_list_for_each_entry_safe(node, next, &os_mem, list) {
        if (devid == node->attr.inst.devid) {
            node->attr.ops.free_ddr(&node->attr.inst, node->attr.vaddr, node->attr.size, node->attr.phy_addr);
            trs_chan_mem_node_free(node);
        }
    }
    ka_task_mutex_unlock(&os_mem_mutex);
    trs_info("Chan mem pool recycle success. (devid=%u)\n", devid);
}

static int chan_mem_node_show(ka_seq_file_t *seq, void *offset)
{
    ka_fs_seq_printf(seq, "%d\n", magic_check_enable);
    return 0;
}

int chan_mem_node_ops_open(ka_inode_t *inode, ka_file_t *file)
{
    return ka_fs_single_open(file, chan_mem_node_show, ka_base_pde_data(inode));
}

ssize_t chan_mem_node_ops_write(ka_file_t *filp, const char __user *ubuf, size_t count, loff_t *ppos)
{
    char ch[2] = {0}; /* 2 bytes long */
    long val;

    if ((ppos == NULL) || (*ppos != 0) || (count != sizeof(ch)) || (ubuf == NULL)) {
        return -EINVAL;
    }

    if (ka_base_copy_from_user(ch, ubuf, count)) {
        return -ENOMEM;
    }

    ch[count - 1] = '\0';
    if (kstrtol(ch, 10, &val)) {
        return -EFAULT;
    }
    magic_check_enable= (val == 0) ? false : true;

    return (ssize_t)count;
}

static const ka_procfs_ops_t mem_node_ops = {               \
    ka_fs_init_pf_owner(KA_THIS_MODULE)                        \
    ka_fs_init_pf_open(chan_mem_node_ops_open)                \
    ka_fs_init_pf_read(ka_fs_seq_read)                        \
    ka_fs_init_pf_write(chan_mem_node_ops_write)              \
    ka_fs_init_pf_lseek(ka_fs_seq_lseek)                      \
    ka_fs_init_pf_release(ka_fs_single_release)               \
};

void trs_chan_mem_node_proc_fs_init(void)
{
    ka_proc_dir_entry_t *top_entry = ka_fs_proc_mkdir("trs_chan_mem_node", NULL);
    if (top_entry == NULL) {
        trs_err("create top entry dir failed\n");
        return;
    }

    (void)ka_fs_proc_create_data("magic_check_enable", PROC_FS_MODE, top_entry, &mem_node_ops, NULL);
}

void trs_chan_mem_node_proc_fs_uninit(void)
{
    (void)ka_fs_remove_proc_subtree("trs_chan_mem_node", NULL);
}