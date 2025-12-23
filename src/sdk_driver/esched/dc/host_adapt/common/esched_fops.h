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

#ifndef ESCHED_FOPS_H
#define ESCHED_FOPS_H

#include <linux/cdev.h>

#include "esched_kernel_interface.h"
#include "esched.h"

/* used for c-dev */
struct sched_char_dev {
    struct device *device;
    struct class *dev_class;
    struct cdev cdev;
    dev_t devno;
};

struct sched_task {
    struct hlist_node hnode; /* hash task link */
    pid_t             pid;
    unsigned int      devid;
    unsigned int      status;
};

#define MINOR_DEV_COUNT 1

#define TASK_RELEASE_INACTIVE 0
#define TASK_RELEASE_ACTIVE 1

#define ESCHED_MSLEEP_TIME 10
#define ESCHED_MAX_TIMEOUT_CNT 20 /* wait timeout 200ms. */

int32_t copy_from_user_safe(void *to, const void __user *from, unsigned long n);
int32_t copy_to_user_safe(void __user *to, const void *from, unsigned long n);

int32_t sched_publish_event_para_check(struct sched_published_event_info *event_info);
int sched_submit_event_pre_proc(unsigned int dev_id, SCHED_PROC_POS pos,
    struct sched_published_event_info *event_info, struct sched_published_event_func *event_func);
u32 sched_ioctl_devid(u32 open_devid, u32 cmd_devid);
void esched_register_ioctl_cmd_func(int nr, int32_t (*fn)(u32 devid, unsigned long arg));
int32_t sched_submit_event_to_thread(uint32_t chip_id, struct sched_published_event *event);
#endif
