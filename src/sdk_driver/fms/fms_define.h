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
#ifndef FMS_DEFINE
#define FMS_DEFINE

#include "dmc_kernel_interface.h"

#ifdef STATIC_SKIP
#define STATIC
#else
#define STATIC                     static
#endif

#define MODULE_DMS "dms_module"
#ifdef UT_VCAST
#define dms_err(fmt, ...) drv_err(MODULE_DMS, fmt, ##__VA_ARGS__)
#define dms_warn(fmt, ...) drv_warn(MODULE_DMS, fmt, ##__VA_ARGS__)
#define dms_info(fmt, ...) drv_info(MODULE_DMS, fmt, ##__VA_ARGS__)
#define dms_event(fmt, ...) drv_event(MODULE_DMS, fmt, ##__VA_ARGS__)
#define dms_debug(fmt, ...) drv_pr_debug(MODULE_DMS, fmt, ##__VA_ARGS__)
#define dms_err_ratelimited(fmt, ...) drv_err_ratelimited(MODULE_DMS, fmt, ##__VA_ARGS__)
#else
#define dms_err(fmt, ...) do { \
    drv_err(MODULE_DMS, "<%s:%d:%d> " fmt, current->comm, current->tgid, current->pid, ##__VA_ARGS__); \
    share_log_err(DEVMNG_SHARE_LOG_START, fmt, ##__VA_ARGS__); \
} while (0)
#define dms_warn(fmt, ...) drv_warn(MODULE_DMS, \
    "<%s:%d:%d> " fmt, current->comm, current->tgid, current->pid, ##__VA_ARGS__)
#define dms_info(fmt, ...) drv_info(MODULE_DMS, \
    "<%s:%d:%d> " fmt, current->comm, current->tgid, current->pid, ##__VA_ARGS__)
#define dms_event(fmt, ...) drv_event(MODULE_DMS, \
    "<%s:%d:%d> " fmt, current->comm, current->tgid, current->pid, ##__VA_ARGS__)
#define dms_debug(fmt, ...) drv_pr_debug(MODULE_DMS, \
    "<%s:%d:%d> " fmt, current->comm, current->tgid, current->pid, ##__VA_ARGS__)
#define dms_err_ratelimited(fmt, ...) do { \
    drv_err_ratelimited(MODULE_DMS, "<%s:%d:%d> " fmt, \
        current->comm, current->tgid, current->pid, ##__VA_ARGS__); \
} while (0)
#endif
#ifdef CFG_FEATURE_HOST_LOG
#define ONE_TIME_EVENT 2
#define dms_fault_mng_event(assertion, fmt, ...) do {                       \
    if (((assertion) == ONE_TIME_EVENT)) {                                  \
        drv_event("fault_manager", fmt, ##__VA_ARGS__);                     \
    } else {                                                                \
        drv_err("fault_manager", fmt, ##__VA_ARGS__);                       \
    }                                                                       \
} while (0)
#else
#define dms_fault_mng_event(assertion, fmt, ...) do {                       \
    u64 ts = local_clock();                                                 \
    u64 rem_nsec = do_div(ts, 1000000000);                                  \
    drv_slog_event("fault_manager", "[%5lu.%06lu] " fmt, ts, rem_nsec / 1000, ##__VA_ARGS__);   \
} while (0)
#endif
#define dms_fmng_event(args...) (void)printk(KERN_NOTICE "[fault_manager] " args)
#define dms_fmng_err(args...) (void)printk(KERN_ERR "[fault_manager] " args)

#endif