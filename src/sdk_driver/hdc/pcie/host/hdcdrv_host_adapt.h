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

#ifndef _HDCDRV_HOST_ADAPT_H_
#define _HDCDRV_HOST_ADAPT_H_

bool hdcdrv_is_phy_dev(u32 devid);
int hdcdrv_check_in_container(void);
int hdcdrv_host_init_module(void);
void hdcdrv_host_exit_module(void);
#endif
