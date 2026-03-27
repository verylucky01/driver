/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025. All rights reserved.
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

#ifndef ASCEND_UB_HOTRESET_H
#define ASCEND_UB_HOTRESET_H

#include "ascend_ub_common.h"
#include "ascend_ub_dev.h"
#include "ascend_ub_unit_adapt.h"

#define AO_SUBSYS_SUBCTRL   0x200020E0000
#define HOTRESET_REG        0x7020
#define HOTRESET_REG_VALUE  0x5A5B5C5D

int ubdrv_hot_reset_process(u32 dev_id);
int ubdrv_proc_host_hot_reset(struct ascend_ub_msg_dev *msg_dev, void *data);
int ubdrv_set_hot_reset(void);
int ubdrv_set_hot_reset_msg_finish(struct ascend_ub_msg_dev *msg_dev, void *data);
extern void ubdrv_hot_reset_process_trigger(void);
int ubdrv_normal_preset(u32 dev_id);
#endif