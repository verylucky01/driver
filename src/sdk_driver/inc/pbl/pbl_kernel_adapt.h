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
#ifndef PBL_KERNEL_ADPAT_H
#define PBL_KERNEL_ADPAT_H
#ifndef EMU_ST
#include <trace/events/sched.h>
#include <linux/notifier.h>

int task_exit_notify_register(struct notifier_block *n);
int task_exit_notify_unregister(struct notifier_block *n);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 17, 0)
enum profile_type {
    PROFILE_TASK_EXIT,
    PROFILE_MUNMAP
};

int profile_event_register(enum profile_type type, struct notifier_block *n);
int profile_event_unregister(enum profile_type type, struct notifier_block *n);
#endif
#endif
#endif