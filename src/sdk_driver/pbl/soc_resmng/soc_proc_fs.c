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

#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/version.h>
#include <linux/uaccess.h>

#include "soc_resmng.h"
#include "soc_proc_fs.h"

#define PROC_FS_MODE 0400

void soc_res_show(u32 udevid, struct seq_file *seq);
int soc_udev_open(struct inode *inode, struct file *file);

static int soc_udev_show(struct seq_file *seq, void *offset)
{
    u32 i;

    for (i = 0; i < SOC_MAX_DAVINCI_NUM; i++) {
        soc_res_show(i, seq);
    }

    return 0;
}

int soc_udev_open(struct inode *inode, struct file *file)
{
    return single_open(file, soc_udev_show, inode->i_private);
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 5, 0)
static const struct file_operations udevice_ops = {
    .owner = THIS_MODULE,
    .open    = soc_udev_open,
    .read    = seq_read,
    .llseek  = seq_lseek,
    .release = single_release,
};
#else

static const struct proc_ops udevice_ops = {
    .proc_open    = soc_udev_open,
    .proc_read    = seq_read,
    .proc_lseek   = seq_lseek,
    .proc_release = single_release,
};

#endif

void soc_proc_fs_init(void)
{
    struct proc_dir_entry *top_entry = proc_mkdir("soc_res", NULL);
    if (top_entry != NULL) {
        (void)proc_create_data("udevice", PROC_FS_MODE, top_entry, &udevice_ops, NULL);
    }
}

void soc_proc_fs_uninit(void)
{
    (void)remove_proc_subtree("soc_res", NULL);
}
