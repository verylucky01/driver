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
 
#ifndef _NDR_LOG_H_
#define _NDR_LOG_H_
 
 
#include "dmc/dmc_log.h"
 
#define MODULE_NDR "ascend_ndr"
 
#define ndr_err(fmt, ...) drv_err(MODULE_NDR, fmt, ##__VA_ARGS__)
#define ndr_warn(fmt, ...) drv_warn(MODULE_NDR, fmt, ##__VA_ARGS__)
#define ndr_info(fmt, ...) drv_info(MODULE_NDR, fmt, ##__VA_ARGS__)
#define ndr_event(fmt, ...) drv_event(MODULE_NDR, fmt, ##__VA_ARGS__)
#define ndr_debug(fmt, ...) drv_pr_debug(MODULE_NDR, fmt, ##__VA_ARGS__)
 
 
#endif