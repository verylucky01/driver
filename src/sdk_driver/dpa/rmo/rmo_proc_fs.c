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
#include "rmo_auto_init.h"

#include "ka_common_pub.h"
#include "ka_fs_pub.h"
#include "ka_kernel_def_pub.h"
#include "securec.h"
#include "rmo_kern_log.h"
#include "rmo_proc_fs.h"

#define RMO_PROC_TOP_NAME    "rmo"
#define RMO_PROC_FS_MODE     0444
#define RMO_PROC_NAME_LEN   64

static ka_proc_dir_entry_t *rmo_top_entry;

static int rmo_proc_show(ka_seq_file_t *seq, void *offset)
{
    int tgid = (int)(uintptr_t)(ka_fs_get_seq_file_private(seq));

    module_feature_auto_show_task(0, tgid, 0, seq);
    return 0;
}

int rmo_proc_open(ka_inode_t *inode, ka_file_t *file)
{
    return ka_fs_single_open(file, rmo_proc_show, ka_base_pde_data(inode));
}

static const ka_procfs_ops_t rmo_proc = {
    ka_fs_init_pf_owner(KA_THIS_MODULE) \
    ka_fs_init_pf_open(rmo_proc_open) \
    ka_fs_init_pf_read(ka_fs_seq_read) \
    ka_fs_init_pf_lseek(ka_fs_seq_lseek) \
    ka_fs_init_pf_release(ka_fs_single_release) \
};

ka_proc_dir_entry_t *rmo_proc_fs_add_task(const char *domain, int tgid)
{
    char name[RMO_PROC_NAME_LEN];

    if (rmo_top_entry == NULL) {
        return NULL;
    }

    (void)sprintf_s(name, RMO_PROC_NAME_LEN, "%s-%d", domain, tgid);
    return ka_fs_proc_create_data((const char *)name, RMO_PROC_FS_MODE, rmo_top_entry, &rmo_proc, (void *)(uintptr_t)tgid);
}

void rmo_proc_fs_del_task(ka_proc_dir_entry_t *task_entry)
{
    if (task_entry != NULL) {
        ka_fs_proc_remove(task_entry);
    }
}

int rmo_proc_fs_init(void)
{
    rmo_top_entry = ka_fs_proc_mkdir(RMO_PROC_TOP_NAME, NULL);
    if (rmo_top_entry == NULL) {
        rmo_err("Rmo proc top dir create fail\n");
        return -ENODEV;
    }

    /* pid 0 means global show */
    (void)ka_fs_proc_create_data("summary", RMO_PROC_FS_MODE, rmo_top_entry, &rmo_proc, (void *)(uintptr_t)0);

    return 0;
}

void rmo_proc_fs_uninit(void)
{
    if (rmo_top_entry != NULL) {
        ka_fs_remove_proc_subtree(RMO_PROC_TOP_NAME, NULL);
        rmo_top_entry = NULL;
    }
}

ka_proc_dir_entry_t *rmo_get_top_entry(void)
{
    return rmo_top_entry;
}
