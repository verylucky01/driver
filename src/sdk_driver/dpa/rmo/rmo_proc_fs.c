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
#include <linux/version.h>
#include <linux/proc_fs.h>

#include "rmo_auto_init.h"

#include "securec.h"
#include "rmo_kern_log.h"
#include "rmo_proc_fs.h"

#define RMO_PROC_TOP_NAME    "rmo"
#define RMO_PROC_FS_MODE     0444
#define RMO_PROC_NAME_LEN   64

static struct proc_dir_entry *rmo_top_entry;

static int rmo_proc_show(struct seq_file *seq, void *offset)
{
    int tgid = (int)(uintptr_t)(seq->private);

    module_feature_auto_show_task(0, tgid, 0, seq);
    return 0;
}

int rmo_proc_open(struct inode *inode, struct file *file)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 17, 0)
    return single_open(file, rmo_proc_show, pde_data(inode));
#else
    return single_open(file, rmo_proc_show, PDE_DATA(inode));
#endif
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 5, 0)
static const struct file_operations rmo_proc = {
    .owner = THIS_MODULE,
    .open    = rmo_proc_open,
    .read    = seq_read,
    .llseek  = seq_lseek,
    .release = single_release,
};
#else
static const struct proc_ops rmo_proc = {
    .proc_open    = rmo_proc_open,
    .proc_read    = seq_read,
    .proc_lseek   = seq_lseek,
    .proc_release = single_release,
};
#endif

struct proc_dir_entry *rmo_proc_fs_add_task(const char *domain, int tgid)
{
    char name[RMO_PROC_NAME_LEN];

    if (rmo_top_entry == NULL) {
        return NULL;
    }

    (void)sprintf_s(name, RMO_PROC_NAME_LEN, "%s-%d", domain, tgid);
    return proc_create_data((const char *)name, RMO_PROC_FS_MODE, rmo_top_entry, &rmo_proc, (void *)(uintptr_t)tgid);
}

void rmo_proc_fs_del_task(struct proc_dir_entry *task_entry)
{
    if (task_entry != NULL) {
        proc_remove(task_entry);
    }
}

int rmo_proc_fs_init(void)
{
    rmo_top_entry = proc_mkdir(RMO_PROC_TOP_NAME, NULL);
    if (rmo_top_entry == NULL) {
        rmo_err("Rmo proc top dir create fail\n");
        return -ENODEV;
    }

    /* pid 0 means global show */
    (void)proc_create_data("summury", RMO_PROC_FS_MODE, rmo_top_entry, &rmo_proc, (void *)(uintptr_t)0);

    return 0;
}

void rmo_proc_fs_uninit(void)
{
    if (rmo_top_entry != NULL) {
        remove_proc_subtree(RMO_PROC_TOP_NAME, NULL);
        rmo_top_entry = NULL;
    }
}

