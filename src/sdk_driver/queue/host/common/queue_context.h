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
#ifndef QUEUE_CONTEXT_H
#define QUEUE_CONTEXT_H

#include <asm/atomic.h>
#include <linux/types.h>
#include <linux/spinlock_types.h>
#include <linux/proc_fs.h>

#include "queue_module.h"

struct queue_context {
    int pid;
    struct hlist_node link; /* hash task link */
    atomic_t refcnt;
    struct proc_dir_entry *entry;
    void *private_data;
    void *qid_status[MAX_SURPORT_QUEUE_NUM];
    spinlock_t qid_status_lock;
};

struct queue_context *queue_context_get(pid_t pid);
void queue_context_put(struct queue_context *ctx);
struct queue_context *queue_context_init(pid_t pid);
void queue_context_uninit(struct queue_context *ctx);
void *queue_context_private_data_create(void);
void queue_context_private_data_destroy(void *private_data);
#endif