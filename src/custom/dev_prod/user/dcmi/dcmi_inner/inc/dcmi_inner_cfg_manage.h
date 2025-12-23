/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef __DCMI_INNER_CFG_MANAGE_H__
#define __DCMI_INNER_CFG_MANAGE_H__

#include "dcmi_interface_api.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#define DCMI_CPU_NUM_MAX_AG         15
#define DCMI_CPU_NUM_MAX_PG         7
#define DCMI_CPU_NUM_MIN            1
#define DCMI_CPU_NUM_TOTAL_AG       16
#define DCMI_CPU_NUM_TOTAL_PG       8

#define DCMI_CPU_NUM_TOTAL_310B     4

#define CAPABILITY_GROUP_SHARE_RESOURCE     0xff

#define CARD_POS_NUMS_ON_ASCEND_BOARD 4 // 单个昇腾板上芯片卡位数量
#define FIRST_D_BOARD_WORK_MODE_REG_ADDR 0x34b
#define SECOND_D_BOARD_WORK_MODE_REG_ADDR 0x37b
#define D_BOARD_WORK_MODE_MASK 0x10

#define DCMI_NPU_WORK_MODE_INVALID  0xff
#define DCMI_NPU_WORK_MODE_MASK     0x01

#define DCMI_CAPABILITY_GROUP_GROUP_ID_ALL      (-1)
#define DCMI_CAPABILITY_GROUP_MAX_COUNT_NUM     4
#define DCMI_CAPABILITY_GROUP_MIN_COUNT_NUM     1

#define CPU_FREQ_UP_CONFIG_LEN          4

#define DCMI_CFG_PERSISTENCE_ENABLE     0x01
#define DCMI_CFG_PERSISTENCE_DISABLE    0x00

#define BRO_CARD_OFFSET             4

#define SYSLOG_PARA_NUM     11
#define SYSLOG_LEVEL_NUM    10
#define MAX_PARA_DESCRIP    20

#define SYS_LOG_MAX_CMD_LINE    512
#define MAX_PID_LENGTH          20
#define SYSLOG_LEVEL_MIN        1
#define SYSLOG_LEVEL_MAX        10
#define SYSLOG_FILE_MAX_NUM     100
#define MAX_INT_LEN             12
#define SYSLOG_MEMORY_FACTOR    204800 // 2 * 100 * 1024

#define SYSLOG_FILE_NAME_LEN    19
#define DCMI_DEC_TO_STR_BASE    10
#define FILE_NAME_MAX_LEN       256
#define MAX_SYSLOG_FILE_COUNT   2

#define DCMI_DEVICE_MAX_NUM     1

typedef enum {
    CTRL_CPU_CFG_INDEX,
    DATA_CPU_CFG_INDEX,
    AI_CPU_CFG_INDEX,
    CPU_CFG_INDEX_MAX,
} DCMI_CPU_CFG_INDEX;

int dcmi_set_npu_device_sec_revocation(
    int card_id, int device_id, enum dcmi_revo_type input_type, const unsigned char *file_data, unsigned int file_size);

int dcmi_set_npu_device_mac(int card_id, int device_id, int mac_id, const char *mac_addr, unsigned int len);

int dcmi_set_npu_device_gateway(
    int card_id, int device_id, enum dcmi_port_type input_type, int port_id, struct dcmi_ip_addr *gateway);

int dcmi_set_npu_device_ip(int card_id, int device_id, enum dcmi_port_type input_type, int port_id,
    struct dcmi_ip_addr *ip, struct dcmi_ip_addr *mask);

int dcmi_set_npu_device_ecc_enable(int card_id, int device_id, enum dcmi_device_type device_type, int enable_flag);

int dcmi_set_npu_device_user_config(
    int card_id, int device_id, const char *config_name, unsigned int buf_size, unsigned char *buf);

int dcmi_clear_npu_device_user_config(int card_id, int device_id, const char *config_name);

int dcmi_set_npu_device_share_enable(int card_id, int device_id, int enable_flag);

int dcmi_set_sleep_state(int card_id, int device_id, struct dcmi_power_state_info_stru power_info);

int dcmi_check_cpu_num_config(unsigned char *buf, unsigned int buf_size);

int dcmi_check_capability_group_para(int ts_id, struct dcmi_capability_group_info *group_info);

int dcmi_check_capability_group_support_type(void);

int dcmi_npu_create_capability_group(int card_id, int device_id, int ts_id,
    struct dcmi_capability_group_info *group_info);

int dcmi_npu_delete_capability_group(int card_id, int device_id, int ts_id, int group_id);

int dcmi_set_npu_syslog_dump(int card_id, int logic_id, int syslog_level, char *file_path);

int dcmi_set_syslog_persistence_cfg(char *cmdline, int cmdline_len);

int dcmi_clear_npu_syslog_cfg(void);

int dcmi_set_syslog_persistence_mode(int mode);

int dcmi_get_npu_custom_op_status(int card_id, int device_id, int *enable_flag);

int dcmi_clear_syslog_cfg();

int dcmi_switch_boot_area(int card_id, int device_id);

int dcmi_save_custom_op_cfg(int card_id, int device_id, int enable_value);

int dcmi_check_custom_op_config_recover_mode_is_permitted(const char *operate_mode);

int dcmi_set_npu_device_info(int card_id, int device_id, enum dcmi_main_cmd main_cmd, unsigned int sub_cmd,
    const void *buf, unsigned int buf_size);

int dcmi_set_custom_op_secverify_enable(int card_id, int device_id, const char *config_name, unsigned int buf_size,
    unsigned char *buf);

int dcmi_set_custom_op_secverify_mode(int card_id, int device_id, enum dcmi_main_cmd main_cmd, unsigned int sub_cmd,
    const void *buf, unsigned int buf_size);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __DCMI_INNER_CFG_MANAGE_H__ */