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

#ifndef MWL_H
#define MWL_H

#include <linux/types.h>
#include <stdbool.h>

/*
    MWL: memory white list
    add/del: add or del a mem in wml inst. The mem can partially overlap, but not completely overlap.
    add_trusted_task: add trusted_tgid to the mem
    usage:
        mem owner task: add or del mem, add trusted task
        mem shared task: use task_is_trusted to check
*/

int svm_mwl_add_mem(u32 udevid, int tgid, u64 id, u64 va, u64 size);
int svm_mwl_del_mem(u32 udevid, int tgid, u64 id);
int svm_mwl_add_trusted_task(u32 udevid, int tgid, u64 id, u32 trusted_server_id, int trusted_tgid);
int svm_mwl_del_trusted_task(u32 udevid, int tgid, u64 id, u32 trusted_server_id, int trusted_tgid);
bool svm_mwl_task_is_trusted(u32 udevid, int tgid, u64 id, u32 checked_server_id, int checked_tgid);

#endif

