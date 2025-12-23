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

#ifndef KA_MODULE_INIT_H
#define KA_MODULE_INIT_H
#include "dmc_kernel_interface.h"

#ifdef STATIC_SKIP
#define STATIC
#else
#define STATIC  static
#endif

#define module_ka "kernel_adapt"

#define ka_err(fmt, ...) drv_err(module_ka, \
    "<%s:%d:%d> " fmt, current->comm, current->tgid, current->pid, ##__VA_ARGS__)
#define ka_warn(fmt, ...) drv_warn(module_ka, \
    "<%s:%d:%d> " fmt, current->comm, current->tgid, current->pid, ##__VA_ARGS__)
#define ka_info(fmt, ...) drv_info(module_ka, \
    "<%s:%d:%d> " fmt, current->comm, current->tgid, current->pid, ##__VA_ARGS__)
#define ka_event(fmt, ...) drv_event(module_ka, \
    "<%s:%d:%d> " fmt, current->comm, current->tgid, current->pid, ##__VA_ARGS__)
#define ka_debug(fmt, ...) drv_pr_debug(module_ka, \
    "<%s:%d:%d> " fmt, current->comm, current->tgid, current->pid, ##__VA_ARGS__)

int ka_module_init(void);
void ka_module_exit(void);

#endif