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

#ifndef DMS_CHIP_DEV_MAP_H
#define DMS_CHIP_DEV_MAP_H
#include <linux/list.h>
#include <linux/types.h>

#include "urd_define.h"
#include "devdrv_user_common.h"

#define RANDOM_SIZE 24
#define NOT_ASSIGN_TO_CHIP 0
#define ASSIGN_TO_CHIP 1

typedef struct {
    unsigned int dev_id;
    unsigned int assign_to_chip_flag;
    unsigned int belong_to_chip;
    char random_number[RANDOM_SIZE];
    struct list_head dev_node_list;
} dms_chip_dev_node_t;

typedef struct {
    unsigned int chip_id;
    unsigned int dev_num; /* device number belong to this chip */
    struct list_head dev_head;   /* link list head for devices of one chip */
    struct list_head chip_node_list;  /* link node for chips */
} dms_chip_node_t;

typedef struct {
    unsigned int chip_num; /* total chip number */
    struct list_head chip_head;
    unsigned int dev_num; /* total device num */
    dms_chip_dev_node_t *all_dev_info;
} dms_chip_dev_map_t;

int dms_get_chip_count(int *count);
int dms_get_chip_list(struct devdrv_chip_list *chip_info);
int dms_get_device_from_chip(struct devdrv_chip_dev_list *chip_dev_list);
int dms_get_chip_from_device(struct devdrv_get_dev_chip_id *chip_from_dev);

#endif