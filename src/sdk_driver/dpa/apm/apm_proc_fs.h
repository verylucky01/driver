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
#ifndef APM_PROC_FS_H
#define APM_PROC_FS_H

#include "ka_base_pub.h"
#include "ka_common_pub.h"

int apm_proc_fs_init(void);
void apm_proc_fs_uninit(void);
int apm_proc_open(ka_inode_t *inode, ka_file_t *file);

ka_proc_dir_entry_t *apm_proc_fs_add_task(const char *domain, int tgid);
void apm_proc_fs_del_task(ka_proc_dir_entry_t *task_entry);

#endif /* APM_PROC_FS_H__ */
