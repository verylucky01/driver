/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef __DCMI_NPU_LINK_INTF_H__
#define __DCMI_NPU_LINK_INTF_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#define HAL_TOPOLOGY_HCCS       0
#define HAL_TOPOLOGY_PIX        1
#define HAL_TOPOLOGY_PIB        2
#define HAL_TOPOLOGY_PHB        3
#define HAL_TOPOLOGY_SYS        4
#define HAL_TOPOLOGY_SIO        5
#define HAL_TOPOLOGY_HCCS_SW    6

#define SYS_BUS_PCI_DEVICE_PATH     "/sys/bus/pci/devices"

#define DCMI_PCI_PROC_BUS_PCI_PATH "/proc/bus/pci"
#define DCMI_PCI_PCI_DOMAIN        0x0000
#define DCMI_PCI_INVALID_VALUE    (-1)

#define MAX_MACRO_ID 12
#define MAX_H60_ID   7
#define MAX_A_X_MACRO_ID   10
#define RES_MACRO_ID 8
#define MAX_310P_MACRO_ID 4
#define MIN_310P_MACRO_ID 0

#define HCCS_SINGLE_PACKETS_BYTES       20
#define PROFILING_BYTE_TRANS_TO_GMBYTE  (1024 * 1024 * 1024)
#define PROFILING_MS_TRANS_TO_S         1000

struct dcmi_hccs_link_bandwidth_info {
    int profiling_time;
    unsigned int tx_bandwidth[DCMI_HCCS_MAX_PCS_NUM];
    unsigned int rx_bandwidth[DCMI_HCCS_MAX_PCS_NUM];
};

int dcmi_pci_write_conf_byte(const char *param, unsigned int pos, unsigned char byte);

int dcmi_get_phyid_by_cardid_and_devid(int card_id, int device_id, unsigned int *phyid);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __DCMI_NPU_LINK_INTF_H__ */
