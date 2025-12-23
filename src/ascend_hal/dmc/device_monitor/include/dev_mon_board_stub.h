/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef DEV_MON_BOARD_STUB_H
#define DEV_MON_BOARD_STUB_H

#include <stdint.h>
#include "dsmi_common_interface.h"
#include "dev_mon_board_interface.h"

int dm_get_peripheral_fw_version_info(int dev_id, unsigned char *version_info, unsigned int len);
int dm_get_device_sn(unsigned char *psn, unsigned int len);
int dm_get_device_name(unsigned char *pdevice_name, unsigned int len);
int dm_get_driver_version_info(unsigned char *version_info, unsigned int len);
int dm_get_device_flash_count(unsigned int *pflash_count);
int dm_get_fan_speed_info(unsigned char *fan_info, unsigned int *fan_info_len);
int set_dft_run_flag(unsigned char flag);
int get_dft_run_flag(unsigned char *flag);

#endif
