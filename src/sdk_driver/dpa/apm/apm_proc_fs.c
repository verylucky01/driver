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
#include "ka_fs_pub.h"
#include "ka_kernel_def_pub.h"

#include "apm_auto_init.h"

#include "securec.h"
#include "apm_kern_log.h"
#include "apm_proc_fs.h"

#define APM_PROC_TOP_NAME    "apm"
#define APM_PROC_FS_MODE     0444
#define APM_PROC_NAME_LEN   64

static ka_proc_dir_entry_t *apm_top_entry;

static int apm_proc_show(ka_seq_file_t *seq, void *offset)
{
    int tgid = (int)(uintptr_t)(seq->private);

    module_feature_auto_show_task(0, tgid, 0, seq);
    return 0;
}

int apm_proc_open(ka_inode_t *inode, ka_file_t *file)
{
    return ka_fs_single_open(file, apm_proc_show, ka_base_pde_data(inode));
}

static const ka_procfs_ops_t apm_proc = {
    ka_fs_init_pf_owner(KA_THIS_MODULE) \
    ka_fs_init_pf_open(apm_proc_open) \
    ka_fs_init_pf_read(ka_fs_seq_read) \
    ka_fs_init_pf_lseek(ka_fs_seq_lseek) \
    ka_fs_init_pf_release(ka_fs_single_release) \
};

ka_proc_dir_entry_t *apm_proc_fs_add_task(const char *domain, int tgid)
{
    char name[APM_PROC_NAME_LEN];

    if (apm_top_entry == NULL) {
        return NULL;
    }

    (void)sprintf_s(name, APM_PROC_NAME_LEN, "%s-%d", domain, tgid);
    return ka_fs_proc_create_data((const char *)name, APM_PROC_FS_MODE, apm_top_entry, &apm_proc, (void *)(uintptr_t)tgid);
}

void apm_proc_fs_del_task(ka_proc_dir_entry_t *task_entry)
{
    if (task_entry != NULL) {
        ka_fs_proc_remove(task_entry);
    }
}

int apm_proc_fs_init(void)
{
    apm_top_entry = ka_fs_proc_mkdir(APM_PROC_TOP_NAME, NULL);
    if (apm_top_entry == NULL) {
        apm_err("Apm proc top dir create fail\n");
        return -ENODEV;
    }

    /* pid 0 means global show */
    (void)ka_fs_proc_create_data("summary", APM_PROC_FS_MODE, apm_top_entry, &apm_proc, (void *)(uintptr_t)0);

    return 0;
}

void apm_proc_fs_uninit(void)
{
    if (apm_top_entry != NULL) {
        ka_fs_remove_proc_subtree(APM_PROC_TOP_NAME, NULL);
        apm_top_entry = NULL;
    }
}

