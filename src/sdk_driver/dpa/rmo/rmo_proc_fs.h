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
#ifndef RMO_PROC_FS_H
#define RMO_PROC_FS_H

#include <linux/proc_fs.h>
#include <linux/seq_file.h>

int rmo_proc_fs_init(void);
void rmo_proc_fs_uninit(void);

struct proc_dir_entry *rmo_proc_fs_add_task(const char *domain, int tgid);
void rmo_proc_fs_del_task(struct proc_dir_entry *task_entry);

int rmo_proc_open(struct inode *inode, struct file *file);

#endif /* RMO_PROC_FS_H__ */
