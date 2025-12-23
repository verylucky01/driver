/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef __DEV_MON_CMD_H__
#define __DEV_MON_CMD_H__

#include "hash_table.h"

#define DEV_MON_CMD_D_GET_CDR_INFO              0x30
#define DEV_MON_CMD_D_VRD_TEMP                  0x80
#define DEV_MON_CMD_GET_CHIP_PCIE_ERR_RATE      0x90
#define DEV_MON_CMD_CLEAR_CHIP_PCIE_ERR_RATE    0x91
#define DEV_MON_CMD_GET_ISOLATED_PAGES_INFO     0x81
#define DEV_MON_CMD_CLEAR_ISOLATED_INFO         0x82
#define DEV_MON_CMD_OUT_BAND_GET_UTILIZATION_INFO        0x83
#define DEV_MON_CMD_OUT_BAND_GET_FREQUENCY_INFO        0x84
#define DEV_MON_CMD_OUT_BAND_GET_HBM_INFO       0x85
#define DEV_MON_CMD_OUT_BAND_GET_MAC_INFO       0x86
#define DEV_MON_CMD_OUT_BAND_GET_GATEWAY_INFO   0x87
#define DEV_MON_CMD_OUT_BAND_GET_IP_INFO        0x88
#define DEV_MON_CMD_OUT_BAND_GET_PACKAGE_INFO   0x89
#define DEV_MON_CMD_GET_MULTI_ECC_TIME_INFO     0x8A
#define DEV_MON_CMD_GET_ECC_RECORD_INFO         0x8B
#define DEV_MON_CMD_OUT_BAND_GET_XSFP_BASE_INFO       0x8C
#define DEV_MON_CMD_OUT_BAND_GET_XSFP_THRESHOLD_INFO  0x8D
#define DEV_MON_CMD_OUT_BAND_GET_XSFP_POWER_INFO      0x8E
#define DEV_MON_CMD_OUT_BAND_GET_XSFP_MAC_PKT_INFO    0x8F
#define DEV_MON_CMD_SET_AICORE_COMPUTING_POWER  0x92
#define DEV_MON_CMD_SET_USER_CONFIG_PRODUCT     0x93
#define DEV_MON_CMD_GET_USER_CONFIG_PRODUCT     0x94
#define DEV_MON_CMD_CLEAR_USER_CONFIG_PRODUCT   0x95
#define DEV_MON_CMD_D_GET_MEM_INFO              0x96
#define DEV_MON_CMD_D_GET_DDR_TEMPR             0x97
#define DEV_MON_CMD_OUT_BAND_GET_MEM_INFO       0x98
#define DEV_MON_CMD_D_GET_DEVICE_FAULT_EVENT    0x99
#define DEV_MON_CMD_GET_SYS_INFO_PARA           0x9A
#define DEV_MON_CMD_D_GET_CPU_INFO              0x9B
#define DEV_MON_CMD_D_GET_VRD_VERSION           0x9C
#define DEV_MON_CMD_D_SET_CPU_FREQ_GEAR         0x9D
#define DEV_MON_CMD_D_GET_CHIP_TYPE             0x9E
#define DEV_MON_CMD_D_GET_SECURE_BOOT_INFO      0x9F
#define DEV_MON_CMD_OUT_BAND_GET_XSFP_LINKDOWN_INFO   0xA0
#define DEV_MON_CMD_D_GET_HBM_MANUFACTURER_ID         0xA1
#define DEV_MON_CMD_OUT_BAND_GET_AXTEND_POWER_INFO    0xA2
#define DEV_MON_CMD_OUT_BAND_GET_PORT_STATISTIC       0xA3
#define DEV_MON_CMD_OUT_BAND_OPTICAL_MODULE_PRBS      0xA4
#define DEV_MON_CMD_OUT_BAND_GET_PORT_LINKDOWN_INFO   0xA5
#define DEV_MON_CMD_OUT_BAND_GET_FMEA_INFO            0xA6
#define DEV_MON_CMD_OUT_BAND_GET_XSFP_POWER_INFO_EXTRA  0xA7
#define DEV_MON_CMD_OUT_BAND_GET_HUYANG_FMEA    0xA8
#define DEV_MON_CMD_D_GET_ROOTKEY_STATUS        0xB0
#define DEV_MON_CMD_D_SET_SERDES_INFO           0xB1
#define DEV_MON_CMD_D_GET_SERDES_INFO           0xB2
#define DEV_MOV_CMD_GET_MCU_BOARD_ID            0xB3
#define DEV_MOV_CMD_GET_DECRYPTION              0xB4
#define DEV_MON_CMD_OUT_BAND_GET_HCCS_INFO      0xB5

int dev_mon_cmd_register(hash_table *cmdhashtable);

#endif
