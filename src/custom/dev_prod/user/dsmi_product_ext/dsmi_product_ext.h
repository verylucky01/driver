/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef __DSMI_PRODUCT_EXT_H__
#define __DSMI_PRODUCT_EXT_H__

#include "dsmi_common.h"
#include "dev_mon_cmd_def.h"
#include "dsmi_dmp_command.h"
#include "dev_mon_iam_type.h"
#include "dev_mon_cmd.h"
#include "dsmi_inner_interface.h"
#include "dsmi_common_interface_custom.h"

#define __stringify_1(x...) #x
#define __stringify(x...)   __stringify_1(x)
#define CMD_LENGTH_INVALID      0XFFFF

#define COMMON_ERR      (-1)
#define NDIE_TYPE       4
#define NDIE_ID_OFFSET  16
#define COMPUTING_POWER_AICORE_INDEX    1
#undef DEVDRV_MAX_DAVINCI_NUM
#define DEVDRV_MAX_DAVINCI_NUM  64

#define BOARD_TYPE_EP            0
#define DEV_TYPE_MAX_LEN         8
#define DEV_PATH_MAX_LEN         64
#define SOFT_LINK_TYPE           10
#define PCI_DEVICE_BASE_PATH     "/sys/bus/pci/devices"
#define DEV_HISI_HDC_PATH        "/dev/hisi_hdc"
#define DEV_DAVINCI_MANAGER_PATH "/dev/davinci_manager"
#define DEV_DEVMMM_SVM_PATH      "/dev/devmm_svm"
#define DEV_SVM0_PATH            "/dev/svm0"

#define DSMI_MAX_VDEV_NUM 16 /**< max number a device can spilts */
#define DSMI_MAX_SPEC_RESERVE 8

#ifdef CFG_SOC_PLATFORM_CLOUD
#define HCCS_ON  1
#define HCCS_OFF 0
#endif

#ifdef CFG_FEATURE_HBM_MANUFACTURER_ID
#define CHECK_ONE_BYTE_BIT        8
#define CHECK_TWO_BYTE_BIT        (2 * CHECK_ONE_BYTE_BIT)
#define CHECK_THREE_BYTE_BIT      (3 * CHECK_ONE_BYTE_BIT)
#endif

struct dsmi_vdev_spec_info {
    unsigned char core_num; /**< aicore num for virtual device */
    unsigned char reservesd[DSMI_MAX_SPEC_RESERVE]; /**< reserved */
};

struct dsmi_sub_vdev_info {
    unsigned int status;       /**< whether the vdevice used by container */
    unsigned int vdevid;       /**< id number of vdevice */
    unsigned int vfid;
    unsigned long long cid;    /**< container id */
    struct dsmi_vdev_spec_info spec; /**< specification of vdevice */
};

struct dsmi_vdev_info {
    unsigned int vdev_num;         /**< number of vdevice the devid had created */
    struct dsmi_vdev_spec_info spec_unused;  /**< resource the devid unallocated */
    struct dsmi_sub_vdev_info vdev[DSMI_MAX_VDEV_NUM];
};

#ifdef CONFIG_LLT
static inline void printf_stub(char *format, ...) {
}
#define DEV_MON_ERR          printf_stub
#define DEV_MON_WARNING      printf_stub
#define DEV_MON_INFO         printf_stub
#define DEV_MON_DEBUG        printf_stub
#define DEV_MON_EVENT        printf_stub
#define DEV_MON_PRINT        printf_stub
#endif

#if !defined CFG_SOC_PLATFORM_CLOUD && !defined CFG_FEATURE_ECC_DDR
DSMI_CMD_DEF_DAVINCI_INSTANCE(DEV_MON_CMD_GET_CHIP_PCIE_ERR_RATE, 0, sizeof(PCIE_ERR_RATE_INFO_STU), STATE_MANAGE_TYPE)
DSMI_CMD_DEF_DAVINCI_INSTANCE(DEV_MON_CMD_CLEAR_CHIP_PCIE_ERR_RATE, 0, 0, STATE_MANAGE_TYPE)
#else
#if defined CFG_FEATURE_ECC_DDR
DSMI_CMD_DEF_DAVINCI_INSTANCE(DEV_MON_CMD_GET_CHIP_PCIE_ERR_RATE, 0, sizeof(PCIE_ERR_RATE_INFO_STU), STATE_MANAGE_TYPE)
DSMI_CMD_DEF_DAVINCI_INSTANCE(DEV_MON_CMD_CLEAR_CHIP_PCIE_ERR_RATE, 0, 0, STATE_MANAGE_TYPE)
#endif
DSMI_CMD_DEF_DAVINCI_INSTANCE(
    DEV_MON_CMD_GET_MULTI_ECC_TIME_INFO, 0, sizeof(struct dsmi_multi_ecc_time_data), STATE_MANAGE_TYPE)
DSMI_CMD_DEF_DAVINCI_INSTANCE(
    DEV_MON_CMD_GET_ECC_RECORD_INFO, sizeof(unsigned int) + sizeof(unsigned char) + sizeof(unsigned char),
    sizeof(struct dsmi_ecc_common_data), STATE_MANAGE_TYPE)
#endif

DSMI_CMD_DEF_DAVINCI_INSTANCE(DEV_MON_CMD_SET_USER_CONFIG_PRODUCT, CMD_LENGTH_INVALID, 0, STATE_MANAGE_TYPE)
DSMI_CMD_DEF_DAVINCI_INSTANCE(DEV_MON_CMD_GET_USER_CONFIG_PRODUCT, CMD_LENGTH_INVALID, 0, STATE_MANAGE_TYPE)
DSMI_CMD_DEF_DAVINCI_INSTANCE(DEV_MON_CMD_CLEAR_USER_CONFIG_PRODUCT, CMD_LENGTH_INVALID, 0, STATE_MANAGE_TYPE)
DSMI_CMD_DEF_DAVINCI_INSTANCE(DEV_MON_CMD_D_GET_MEM_INFO, 0, sizeof(struct dsmi_get_memory_info_stru),
    STATE_MANAGE_TYPE)
#ifdef CFG_FEATURE_HBM_MANUFACTURER_ID
DSMI_CMD_DEF_DAVINCI_INSTANCE(DEV_MON_CMD_D_GET_HBM_MANUFACTURER_ID, 0, sizeof(unsigned int), STATE_MANAGE_TYPE)
#endif
#ifdef CFG_FEATURE_ROOTKEY_STATUS
DSMI_CMD_DEF_DAVINCI_INSTANCE(DEV_MON_CMD_D_GET_ROOTKEY_STATUS, 0, sizeof(unsigned int), STATE_MANAGE_TYPE)
#endif
#ifdef CFG_FEATURE_SERDES_INFO
DSMI_CMD_DEF_DAVINCI_INSTANCE(DEV_MON_CMD_D_GET_SERDES_INFO, CMD_LENGTH_INVALID, CMD_LENGTH_INVALID, STATE_MANAGE_TYPE)
DSMI_CMD_DEF_DAVINCI_INSTANCE(DEV_MON_CMD_D_SET_SERDES_INFO, CMD_LENGTH_INVALID, CMD_LENGTH_INVALID, CONFIG_MANAGE_TYPE)
#endif
#if defined(CFG_FEATURE_INIT_MCU_BOARD_ID)
DSMI_CMD_DEF_DAVINCI_INSTANCE(DEV_MOV_CMD_GET_MCU_BOARD_ID, 0, sizeof(unsigned int), STATE_MANAGE_TYPE)
#endif
int dsmi_cmd_get_serdes_info(unsigned int dev_id, SERDES_MAIN_CMD main_cmd, unsigned int sub_cmd,
    void *buf, unsigned int *size);
int dsmi_cmd_set_serdes_info(unsigned int dev_id, SERDES_MAIN_CMD main_cmd, unsigned int sub_cmd,
    const void *buf, unsigned int buf_size);
#endif
