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

#ifndef DMS_CHIP_INFO_H
#define DMS_CHIP_INFO_H

#include "devdrv_user_common.h"

typedef struct chip_value_name_map {
    unsigned int chip_value;
    char *chip_name;
} chip_value_name_map_t;

typedef struct dms_chip_aicore_info {
    unsigned int chip_ver;
    unsigned int chip_name;
    unsigned int num;
    unsigned int num_level;
    unsigned int freq;
    unsigned int freq_level;
} dms_chip_aicore_info_t;

#define MINIV2_PRO_CORE_NUM 10
#define MINIV2_CORE_NUM 8

int dms_get_chip_info(unsigned int virt_id, devdrv_query_chip_info_t *chip_info);
#endif