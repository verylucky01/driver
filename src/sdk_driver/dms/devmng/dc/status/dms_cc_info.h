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
#ifndef DMS_CC_INFO_H
#define DMS_CC_INFO_H

#include "pbl/pbl_urd.h"
#include "pbl/pbl_urd_main_cmd_def.h"
#include "pbl/pbl_urd_sub_cmd_def.h"
#include "urd_define.h"
#include "urd_feature.h"
#include "urd_acc_ctrl.h"

#define DMS_CC_INFO_CMD_NAME "DMS_CC_INFO"
#define CC_CMDLINE_VALUE_SIZE 1

#ifdef CFG_FEATURE_CC_INFO
int hal_kernel_get_cc_info(unsigned int dev_id, struct dms_cc_info *cc_info);
int dms_ioctl_get_cc_info(const struct urd_cmd *cmd, struct urd_cmd_kernel_para *kernel_para,
    struct urd_cmd_para *para);
int dms_ioctl_set_cc_info(const struct urd_cmd *cmd, struct urd_cmd_kernel_para *kernel_para,
    struct urd_cmd_para *para);
#endif
/* [Optimization] This file has a soft link under the status directory.
   It will be optimized after extracting the external directory */
int dms_cc_info_init(void);
void dms_cc_info_uninit(void);

#endif