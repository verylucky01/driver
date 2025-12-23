/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef __DEVDRV_USER_CONFIG_H__
#define __DEVDRV_USER_CONFIG_H__

#define CTRL_CPU_NUM_MIN 1
#define DATA_CPU_NUM_MIN 0
#define AI_CPU_NUM_MIN 1
#define COM_CPU_NUM_MAX 2
#define TOTAL_CPU_NUM_MAX 16

int devdrv_get_user_config_ex(unsigned int dev_id, const char *name, unsigned char *buf, unsigned int *buf_size);
int devdrv_set_user_config_ex(unsigned int dev_id, const char *name, unsigned char *buf, unsigned int buf_size);
int devdrv_clear_user_config_ex(unsigned int dev_id, const char *name);
int devdrv_get_boot_cfg(unsigned char *chip_info);
int devdrv_get_chip_type_from_user_cfg(unsigned int *chip_type);
int devdrv_user_config_common_check(unsigned int dev_id, const char *name);
#endif
