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

#ifndef RMO_KERN_LOG_H
#define RMO_KERN_LOG_H

#include <linux/types.h>
#include <linux/sched.h>
#include <linux/smp.h>

#ifndef EMU_ST
#include "dmc_kernel_interface.h"
#else
#include "ut_log.h"
#endif

#define MODULE_RMO "rmo"

#define rmo_err(fmt, ...) do { \
    drv_err(MODULE_RMO, "<%s:%d:%d:%d> " fmt, \
        current->comm, current->tgid, current->pid, smp_processor_id(), ##__VA_ARGS__); \
} while (0)

#define rmo_warn(fmt, ...) do { \
    drv_warn(MODULE_RMO, "<%s:%d:%d:%d> " fmt, \
        current->comm, current->tgid, current->pid, smp_processor_id(), ##__VA_ARGS__); \
} while (0)

#define rmo_info(fmt, ...) do { \
    drv_info(MODULE_RMO, "<%s:%d:%d:%d> " fmt, \
        current->comm, current->tgid, current->pid, smp_processor_id(), ##__VA_ARGS__); \
} while (0)

#define rmo_debug(fmt, ...) do { \
    drv_pr_debug(MODULE_RMO, "<%s:%d:%d:%d> " fmt, \
        current->comm, current->tgid, current->pid, smp_processor_id(), ##__VA_ARGS__); \
} while (0)

#endif

