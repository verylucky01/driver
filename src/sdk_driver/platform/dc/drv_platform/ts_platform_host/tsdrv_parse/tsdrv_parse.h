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
#ifndef TSDRV_PARSE_H
#define TSDRV_PARSE_H

#include <linux/types.h>
#include "devdrv_common.h"

int tsdrv_parse_init(u32 devid, struct devdrv_info *dev_info);
void tsdrv_parse_exit(u32 devid, struct devdrv_info *dev_info);

#endif /* __TSDRV_PARSE_H */
