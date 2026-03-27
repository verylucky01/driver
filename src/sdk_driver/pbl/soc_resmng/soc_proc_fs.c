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
#include "ka_base_pub.h"
#include "ka_common_pub.h"
#include "ka_fs_pub.h"
#include "ka_kernel_def_pub.h"

#include "soc_resmng.h"
#include "soc_proc_fs.h"

#define PROC_FS_MODE 0400

void soc_res_show(u32 udevid, ka_seq_file_t *seq);
int soc_udev_open(ka_inode_t *inode, ka_file_t *file);

static int soc_udev_show(ka_seq_file_t *seq, void *offset)
{
    u32 i;

    for (i = 0; i < SOC_MAX_DAVINCI_NUM; i++) {
        soc_res_show(i, seq);
    }

    return 0;
}

int soc_udev_open(ka_inode_t *inode, ka_file_t *file)
{
    return ka_fs_single_open(file, soc_udev_show, ka_fs_get_inode_i_private(inode));
}

static const ka_procfs_ops_t udevice_ops = {
    ka_fs_init_pf_owner(KA_THIS_MODULE) \
    ka_fs_init_pf_open(soc_udev_open) \
    ka_fs_init_pf_read(ka_fs_seq_read) \
    ka_fs_init_pf_lseek(ka_fs_seq_lseek) \
    ka_fs_init_pf_release(ka_fs_single_release) \
};

void soc_proc_fs_init(void)
{
    ka_proc_dir_entry_t *top_entry = ka_fs_proc_mkdir("soc_res", NULL);
    if (top_entry != NULL) {
        (void)ka_fs_proc_create_data("udevice", PROC_FS_MODE, top_entry, &udevice_ops, NULL);
    }
}

void soc_proc_fs_uninit(void)
{
    (void)ka_fs_remove_proc_subtree("soc_res", NULL);
}
