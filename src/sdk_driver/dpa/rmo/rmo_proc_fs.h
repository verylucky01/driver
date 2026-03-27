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
#ifndef RMO_PROC_FS_H
#define RMO_PROC_FS_H

#include "ka_base_pub.h"
#include "ka_common_pub.h"

int rmo_proc_fs_init(void);
void rmo_proc_fs_uninit(void);

ka_proc_dir_entry_t *rmo_proc_fs_add_task(const char *domain, int tgid);
void rmo_proc_fs_del_task(ka_proc_dir_entry_t *task_entry);

int rmo_proc_open(ka_inode_t *inode, ka_file_t *file);
ka_proc_dir_entry_t *rmo_get_top_entry(void);

#endif /* RMO_PROC_FS_H__ */
