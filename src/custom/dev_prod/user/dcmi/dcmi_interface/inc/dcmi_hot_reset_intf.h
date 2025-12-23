/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef __DCMI_HOT_RESET_INTF_H__
#define __DCMI_HOT_RESET_INTF_H__
 
#include "dcmi_interface_api.h"
 
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#ifdef X86
#define NPU_910_PCIE_UPSTREAM_BUS_ADDR 0xa0
#define NPU_310P_PCIE_UPSTREAM_BUS_ADDR 0xa0
#define NPU_310_PCIE_UPSTREAM_BUS_ADDR 0x50
#else
#define PCIE_UPSTREAM_BUS_ADDR 0x50
#endif

#define BMC_RESET_CHIP_SUCCESS  0x0
#define BMC_RESET_CHIP_FAILED   0x01
#define BMC_RESET_CHIP_UNKNOWN  0x02

#define OPEN_PCIE_UPSTREAM         0x0
#define CLOSE_PCIE_UPSTREAM        0x10
#define ASM_PCIE_UPSTREAM_BUS_ADDR 0x90

#define DCMI_RESET_MIN_DELAY    3

/* D卡的miniD芯片槽位号与带外MCU使用的芯片ID的对应表，
   解决芯片故障时复位miniD芯片时芯片标识不准确的问题 */
#define MAX_DEVICE_IN_ALL_TYPE_CAR 16

#define MAX_RESET_CARD_NUM          8
#define MAX_RESET_DEVICE_NUM          2
/*
1. 910B、310B及后续的采用0xffffffff。
2. 310P、910、310不做修改，继续使用0xff
*/
#define ALL_DEVICE_RESET_FLAG_OLD   0xff
#define ALL_DEVICE_RESET_FLAG       0xffffffff
#define FIRST_DEVICE_RESET_FLAG     0xfffffffe
#define SECOND_DEVICE_RESET_FLAG    0xfffffffd
#define THIRD_DEVICE_RESET_FLAG     0xfffffffc
#define FORTH_DEVICE_RESET_FLAG     0xfffffffb
#define ALL_DEVICE_RESET_CARD_ID 0xff

#define DCMI_SMP_INVALID_ID (0xFFFFFFFF)
#define DCMI_SMP_DEVICE_NUMBER (4)
#define DCMI_SMP_MASTER_CARD_ID1 (0)
#define DCMI_SMP_MASTER_CARD_ID2 (4)
#define DCMI_SMP_SLAVER_NUM0 (0)
#define DCMI_SMP_SLAVER_NUM1 (1)
#define DCMI_SMP_SLAVER_NUM2 (2)
#define DCMI_SMP_MASTER_NUM (3)

int dcmi_check_device_reset_vnpu_mode(int card_id);
 
int dcmi_get_npu_outband_reset_state(int card_id, int device_id, unsigned char *reset_state);

int dcmi_check_device_reset_permission(const enum dcmi_reset_channel channel_type);

int dcmi_get_upstream_port_offset(int board_id, unsigned int *upstream_port_offset);

int dcmi_set_npu_device_pre_reset(int card_id, int device_id);

int dcmi_set_npu_device_rescan(int card_id, int device_id);

int dcmi_set_all_npu_hot_reset();

void dcmi_npu_msn_env_clean(int cardId);

int dcmi_reset_brother_card(int card_id, int device_id, int device_logic_id);

int dcmi_set_npu_device_reset_inband(int card_id, int device_id);

int dcmi_get_npu_device_outband_id(int card_id, int device_id, int *outband_id);

int dcmi_set_npu_device_reset_outband(int card_id, int device_id);

int dcmi_reset_smp_card(int card_id, int device_id);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
 
#endif /* __DCMI_HOT_RESET_INTF_H__ */