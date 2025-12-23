/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef __DEVDRV_USER_CONFIG_COMMON_H
#define __DEVDRV_USER_CONFIG_COMMON_H

enum devdrv_flash_config_product_cmd {
    DEVDRV_FLASH_CONFIG_READ_PRODUCT_CMD = 0,
    DEVDRV_FLASH_CONFIG_WRITE_PRODUCT_CMD,
    DEVDRV_FLASH_CONFIG_CLEAR_PRODUCT_CMD,
    DEVDRV_FLASH_CONFIG_MAX_PRODUCT_CMD,
};

#define DSMI_USER_CONFIG_NAME_MAX           32
#define DSMI_USER_CONFIG_BUF_MAX_LEN        1024
#define MTD_NAME_LEN                        32

#define RECOVERY_PASSWD_CONFIG          ("recovery_mode_passwd")
#define RECOVERY_LOGIN_CONFIG           ("recovery_login_enable")
#define CERT_EXPIRED_THRESHOLD          ("cert_expired_threshold")

#define MB_CONFIG_EXPIRED_SIZE          1
#define EXPIERD_THRESHOLD_MAX           180
#define EXPIERD_THRESHOLD_MIN           7

#define SUPPORT_SET_USER_CFG             (1 << DEVDRV_FLASH_CONFIG_WRITE_PRODUCT_CMD)
#define SUPPORT_GET_USER_CFG             (1 << DEVDRV_FLASH_CONFIG_READ_PRODUCT_CMD)
#define SUPPORT_CLEAR_USER_CFG           (1 << DEVDRV_FLASH_CONFIG_CLEAR_PRODUCT_CMD)
#define SUPPORT_ALL_CFG_CFG              (SUPPORT_SET_USER_CFG | SUPPORT_GET_USER_CFG | SUPPORT_CLEAR_USER_CFG)

// NVE配置项
#define NVE_CONFIG_SIZE         1 /* 功耗等级: 0:low/1:middle/2:high/3:full */
#define RATE_LEVEL_FULL      0x03
#define GET_NVE_LEVEL_CONFIG "get_nve_level"
#define SET_NVE_LEVEL_CONFIG "set_nve_level"

/* Atlas 300V cpu提频相关配置 */
#define GET_CPU_FREQ_CONFIG             "get_cpu_freq"
#define SET_CPU_FREQ_CONFIG             "set_cpu_freq"
#define GET_CPU_FREQ_CONFIG_SIZE        4
#define SET_CPU_FREQ_CONFIG_SIZE        4
#define CPU_FREQ_UP                     0
#define CPU_FREQ_NORM                   1

typedef enum {
    FLASH_BLOCK_COMMON = 0,
    FLASH_BLOCK_MB,
    CONFIG_BLOCK_INDEX_MAX,
} CONFIG_BLOCK_TYPE;

struct user_config_item_product {
    const char *name;                          // cfg_name
    const char *mtd_Name;                      // flash mtd name
    unsigned int block_type;
    long block_offset;                   // block block offset from partition head address
    int offset;                          // the configuration item data is offset from the block
    int len;                             // the length of user config item
    unsigned int support_flag;           // support set/get/clear flag
    int authority_flag;                  // reserver field
    unsigned int mode_flag;              // EP/RC flag
    const char *default_data;
    int (*check_para)(unsigned int config_cmd, unsigned char *buf, unsigned int buf_size);
};

/* authority flag */
#define UC_PRO_AUTHORITY_ROOT_WR    0 /* device侧：root 用户可读可写 普通用户无法访问；host侧root无法访问 */
#define UC_PRO_AUTHORITY_USER_WR    1 /* device侧：root 用户可读可写 普通用户可读可写；host侧root可读可写 */
#define UC_PRO_AUTHORITY_USER_RO    2 /* device侧：root 用户可读可写 普通用户只读    ；host侧root只读 */

/* mode flag */
#define UC_SUPPORT_ALL     0
#define UC_ONLY_SUPPORT_EP 1
#define UC_ONLY_SUPPORT_RC 2

struct flash_config_ioctl_para {
    unsigned int dev_id;
    int cmd;
    unsigned int cfg_index;
    unsigned int buf_size;
    void* buf;
};

#endif
