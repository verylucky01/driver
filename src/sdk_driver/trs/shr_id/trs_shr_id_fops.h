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

#ifndef TRS_SHR_ID_FOPS_H
#define TRS_SHR_ID_FOPS_H

#include "ka_base_pub.h"
#include "ka_list_pub.h"
#include "ka_task_pub.h"
#include "ka_hashtable_pub.h"
#include "ka_common_pub.h"

#include "pbl_kref_safe.h"

#include "trs_shr_id_ioctl.h"

#define SHR_ID_HASH_TABLE_BIT           4

struct shr_id_node_htable {
    KA_DECLARE_HASHTABLE(htable, SHR_ID_HASH_TABLE_BIT);
    ka_rwlock_t lock;
};

struct shr_id_hash_node {
    u32 key;
    ka_hlist_node_t link;
};

struct shr_id_proc_ctx {
    int pid;
    u64 start_time;
    u32 open_node_num;
    u32 create_node_num;
    ka_list_head_t create_list_head;
    ka_list_head_t open_list_head;
    ka_rwlock_t lock;

    ka_hlist_node_t link;
    struct kref_safe ref;
    ka_mutex_t mutex;
    ka_atomic_t proc_in_release;
    struct shr_id_node_htable abnormal_ht;
};

void shr_id_register_ioctl_cmd_func(int nr, int (*fn)(struct shr_id_proc_ctx *proc_ctx, unsigned long arg));
int shr_id_init_module(void);
void shr_id_exit_module(void);
#endif

