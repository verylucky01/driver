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
#ifndef XSMEM_FRAMEWORK_LOG_H
#define XSMEM_FRAMEWORK_LOG_H

#ifndef EMU_ST
#include "dmc_kernel_interface.h"
#else
#include "ut_log.h"
#endif

#define XSMEM_MODULE_NAME "xsmem"

#ifndef EMU_ST
#define xsmem_err(fmt, ...) do { \
    drv_err(XSMEM_MODULE_NAME, "<%s:%d> " fmt, current->comm, current->tgid, ##__VA_ARGS__); \
    share_log_err(XSMEM_SHARE_LOG_START, fmt, ##__VA_ARGS__); \
} while (0)
#define xsmem_warn(fmt, ...) do { \
    drv_warn(XSMEM_MODULE_NAME, "<%s:%d> " fmt, current->comm, current->tgid, ##__VA_ARGS__); \
} while (0)
#define xsmem_info(fmt, ...) do { \
    drv_info(XSMEM_MODULE_NAME, "<%s:%d> " fmt, current->comm, current->tgid, ##__VA_ARGS__); \
} while (0)
#define xsmem_debug(fmt, ...) do { \
    drv_pr_debug(XSMEM_MODULE_NAME, "<%s:%d> " fmt, current->comm, current->tgid, ##__VA_ARGS__); \
} while (0)
#define xsmem_event(fmt, ...) do { \
    drv_event(XSMEM_MODULE_NAME, "<%s:%d> " fmt, current->comm, current->tgid, ##__VA_ARGS__); \
} while (0)

#define xsmem_err_limited(fmt, ...) do { \
    if (printk_ratelimit() != 0) \
        xsmem_err(fmt, ##__VA_ARGS__); \
} while (0)

#define xsmem_run_info(fmt, ...) do { \
    share_log_run_info(XSMEM_SHARE_LOG_RUNINFO_START, fmt, ##__VA_ARGS__); \
} while (0)

#else
#define xsmem_err(fmt, ...)   printf("[err][%s %d] %d " fmt, __func__, __LINE__, current->tgid, ##__VA_ARGS__)
#define xsmem_warn(fmt, ...)  printf("[warn][%s %d] %d " fmt, __func__, __LINE__, current->tgid, ##__VA_ARGS__)
#define xsmem_info(fmt, ...)  printf("[info][%s %d] %d " fmt, __func__, __LINE__, current->tgid, ##__VA_ARGS__)
#define xsmem_debug(fmt, ...) printf("[debug][%s %d] %d " fmt, __func__, __LINE__, current->tgid, ##__VA_ARGS__)
#define xsmem_event(fmt, ...) printf("[event][%s %d] %d " fmt, __func__, __LINE__, current->tgid, ##__VA_ARGS__)
#define xsmem_err_limited(fmt, ...) printf("[event][%s %d] %d " fmt, __func__, __LINE__, current->tgid, ##__VA_ARGS__)
#define xsmem_run_info(fmt, ...) printf("[info][%s %d] %d " fmt, __func__, __LINE__, current->tgid, ##__VA_ARGS__)
#endif
#endif
