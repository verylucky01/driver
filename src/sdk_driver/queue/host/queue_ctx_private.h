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
#ifndef QUEUE_CTX_PRIVATE_H
#define QUEUE_CTX_PRIVATE_H

#include <linux/spinlock.h>
#include <linux/list.h>

#include "queue_module.h"
#include "queue_fops.h"

struct context_private_data {
    struct list_head node_list_head;
    spinlock_t lock;
    int hdc_session[MAX_DEVICE];
};

#endif

