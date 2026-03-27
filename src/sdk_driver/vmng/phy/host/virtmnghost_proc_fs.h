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

#ifndef VIRTMNGHOST_PROC_FS_H
#define VIRTMNGHOST_PROC_FS_H
#include "ka_fs_pub.h"
#include "ka_memory_pub.h"
#include "ka_compiler_pub.h"
#include "ka_kernel_def_pub.h"

struct vmngh_user_input {
    u32 dev_id;
    u32 vfid;
};

struct vmngh_procfs_entry {
    ka_proc_dir_entry_t *dev_id;
    ka_proc_dir_entry_t *vf_id;
    ka_proc_dir_entry_t *each_resource_info;
};

int vmngh_proc_fs_init(void);
void vmngh_proc_fs_uninit(void);

#endif
