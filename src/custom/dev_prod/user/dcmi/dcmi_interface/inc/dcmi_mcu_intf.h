/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef __DCMI_MCU_INTF_H__
#define __DCMI_MCU_INTF_H__
 
#include "dcmi_interface_api.h"
 
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */
 
#define DCMI_MCU_MSG_LUN              0x80
#define DCMI_MCU_FILE_MAX_SIZE         (1024 * 728)
#define DCMI_MCU_MSG_MAX_TOTAL_LEN    256
 
#define DCMI_MCU_LOCK_FILE_NAME "/run/mcu_lock_flag"
#define DCMI_MCU_MUTEX_FIRST_TRY_TIMES 100 /* 获取超时锁，首先尝试调用trylock的次数 */
#define DCMI_MCU_MUTEX_SLEEP_TIMES_1MS 1000
#define DCMI_MCU_TASK_DELAY_10_MS     10000
 
#define DCMI_MCU_MSG_REPEATE_NUM      10 // 重复10次
#define DCMI_MCU_MSG_REPEATE_NUM_1MS  5
#define DCMI_MCU_TASK_DELAY_500_MS    500000
#define DCMI_MCU_TASK_DELAY_1_MS      1000
#define DCMI_MCU_DATA_EMPTY           5
#define DCMI_MCU_NO_SUPPORT           1
 
#define DCMI_MCU_SEND_MSG_DATA_LEN_FOR_DEFAULT         20
#define DCMI_MCU_RECV_MSG_DATA_LEN_FOR_DEFAULT         20
#define DCMI_MCU_SEND_MSG_DATA_LEN_FOR_310P_AND_910    20
#define DCMI_MCU_RECV_MSG_DATA_LEN_FOR_310P_AND_910    15
#define DCMI_MCU_GET_LOCK_TIMOUT                       50
 
#define DCMI_MCU_ERR_CODE_MAX         128
#define DCMI_MCU_GET_LOCK_MAX_TIME    50
#define DCMI_MCU_TASK_DELAY_100_MS    100000
 
#define DCMI_MCU_SET_DISK_POWER_STATE 11
#define DCMI_MCU_COLLECT_LOG_RETRY_TIMES 5
#define DCMI_MCU_COLLECT_LOG_WAIT_TIME 2
 
#define DCMI_MCU_HEALTH_OPCODE          0x1
#define DCMI_MCU_ERRCODE_OPCODE         0x2
#define DCMI_MCU_TEMPERATURE_OPCODE     0x3
#define DCMI_MCU_POWER_OPCODE           0x4
#define DCMI_MCU_VERSION_OPCODE         0x5
#define DCMI_MCU_SN_OPCODE              0X8
#define DCMI_MCU_VOLTAGE_OPCODE         0xB
#define DCMI_MCU_BOARD_ID_OPCODE        0xF
#define DCMI_MCU_PCB_ID_OPCODE          0x10
#define DCMI_MCU_SET_ELABEL_OPCODE      0x14
#define DCMI_MCU_ELABEL_OPCODE          0x15
#define DCMI_MCU_EQUIP_TEST_OPCODE      0x16
#define DCMI_MCU_EQUIP_RESULT_OPCODE    0x17
#define DCMI_MCU_UPGRADE_OPCODE         0x18
#define DCMI_MCU_UPGRADE_COMMAND_OPCODE 0x19
#define DCMI_MCU_UPGRADE_STATE_OPCODE   0x1A
#define DCMI_MCU_DEV_TEMPERATURE_OPCODE 0x1D
#define DCMI_MCU_BOM_ID_OPCODE          0x28
#define DCMI_MCU_DFLC_DATA_OPCODE       0x8C
#define DCMI_MCU_MCU_TYPE_ID_OPCODE     0x90
#define DCMI_MCU_SET_BOOT_AEREA_OPCODE  0x501
#define DCMI_MCU_GET_BOOT_AEREA_OPCODE  0x503
#define DCMI_MCU_GET_MONITOR_OPCODE     0x505
#define DCMI_MCU_SET_MONITOR_OPCODE     0x506
#define DCMI_MCU_UPGRADE_BOOT_OPCODE    0x508
#define DCMI_MCU_GET_BOOT_SEL_OPCODE    0x513
#define DCMI_MCU_SET_BOOT_SEL_OPCODE    0x514
#define DCMI_MCU_SWITCH_I2C_OPCODE      0x616
#define DCMI_MCU_GET_CUSTOMIZED_INFO    0x617
#define DCMI_MCU_SET_CUSTOMIZED_INFO    0x618
#define DCMI_MCU_CLEAR_CUSTOMIZED_INFO  0x619
#define DCMI_MCU_VRD_INFO_OPCODE        0x69C
 
#define DCMI_MCU_HEALTH_LEN             1
#define DCMI_MCU_ERRCODE_LEN            2
#define DCMI_MCU_TEMPERATURE_LEN        2
#define DCMI_MCU_POWER_LEN              2
#define DCMI_MCU_VERSION_LEN            3
#define DCMI_MCU_VOLTAGE_LEN            2
#define DCMI_MCU_BOARD_ID_LEN           2
#define DCMI_MCU_PCB_ID_LEN             1
#define DCMI_MCU_BOM_ID_LEN             1
#define DCMI_MCU_MCU_TYPE_ID_LEN        2
#define DCMI_MCU_UPGRADE_COMMAND_LEN    1
#define DCMI_MCU_UPGRADE_STATE_LEN      2
#define DCMI_MCU_EQUIP_TEST_LEN         2
#define DCMI_MCU_EQUIP_RESULT_LEN       2
#define DCMI_MCU_SET_ELABEL_LEN         2
#define DCMI_MCU_UPGRADE_BOOT_LEN       40
#define DCMI_MCU_GET_MONITOR_LEN        1
#define DCMI_MCU_SET_MONITOR_LEN        1
#define DCMI_MCU_SWITCH_I2C_LEN         1
#define DCMI_MCU_GET_BOOT_AEREA_LEN     1
#define DCMI_MCU_SET_BOOT_AEREA_LEN     1
#define DCMI_MCU_GET_BOOT_SEL_LEN       1
#define DCMI_MCU_SET_BOOT_SEL_LEN       1
#define DCMI_FIRST_POWER_ON_DATE_LEN    4
 
#define DCMI_MCU_CUSTOMIZED_INFO_HEAD_VERSION_LEN        4
#define DCMI_MCU_CUSTOMIZED_INFO_HEAD_SEND_LEN           2
#define DCMI_MCU_CUSTOMIZED_INFO_HEAD_CRC_LEN            2
#define DCMI_MCU_CUSTOMIZED_INFO_HEAD_LEN                8
#define DCMI_MCU_CUSTOMIZED_INFO_VERSION                 1
#define DCMI_MCU_CUSTOMIZED_INFO_MAX_LEN                 256
 
#define PRINT_PROCESS_CNT 1500
#define PERCENTAGE_FACTOR 100
 
typedef struct mcu_smbus_req_msg {
    unsigned char lun;
    unsigned char arg;
    unsigned short opcode;
    unsigned int offset;
    unsigned int lenth;
    int req_data_len;
    char *req_data;
} MCU_SMBUS_REQ_MSG;
 
typedef struct mcu_smbus_rsp_msg {
    int len;
    int total_len;
    char *data_info;
    unsigned short errorcode;
} MCU_SMBUS_RSP_MSG;

enum dcmi_mcu_log_type {
    MCU_LOG_ERR = 0,
    MCU_LOG_OPRATE,
    MCU_LOG_MAINT,
    MCU_LOG_MAX,
};

struct dcmi_mcu_log_info {
    int log_type;
    int opcode;
    int max_lenth;
    char file_name[MAX_LENTH];
};
 
int dcmi_mcu_set_lock_up(int *fd, unsigned int timeout);
 
void dcmi_mcu_set_unlock_up(int fd);
 
int dcmi_mcu_release_and_get_lock(int *fd);
 
int dcmi_mcu_get_send_data_max_len(void);
 
int dcmi_mcu_get_recv_data_max_len(void);
 
int dcmi_mcu_get_info_fix(int card_id, MCU_SMBUS_REQ_MSG *mcu_req, MCU_SMBUS_RSP_MSG *mcu_rsp);
 
int dcmi_mcu_get_fix_word(int card_id, unsigned char arg, unsigned short opcode,
    unsigned int get_lenth, int *data_info);
 
int dcmi_mcu_get_info_dynamic(int card_id, MCU_SMBUS_REQ_MSG *mcu_req, MCU_SMBUS_RSP_MSG *mcu_rsp);
 
int dcmi_mcu_set_info_dynamic(int card_id, unsigned short opcode, int len, char *data_info);
 
int dcmi_mcu_set_info_simple(int card_id, unsigned short opcode, unsigned int len, char *data_info);
 
int dcmi_get_mcu_connect_device_logic_id(int *device_logic_id, int *device_slot_id, int card_id);
 
int dcmi_mcu_get_info(int card_id, MCU_SMBUS_REQ_MSG *mcu_req, MCU_SMBUS_RSP_MSG *mcu_rsp);
 
int dcmi_mcu_set_info(int card_id, MCU_SMBUS_REQ_MSG *mcu_req);
 
int dcmi_mcu_version_parse(char *version, int len, MCU_SMBUS_RSP_MSG *mcu_rsp);
 
int dcmi_mcu_get_board_id(int card_id, unsigned int *board_id);
 
int dcmi_mcu_get_bom_id(int card_id, int *bom_id);
 
int dcmi_mcu_get_mcu_type_id(int card_id, unsigned int *mcu_type);
 
int dcmi_mcu_get_board_info(int card_id, struct dcmi_board_info *board_info);
 
int dcmi_mcu_get_chip_info(int card_id, struct dcmi_chip_info_v2 *chip_info);
 
int dcmi_mcu_get_power_info(int card_id, int *power);
 
int dcmi_mcu_set_customized_info(int card_id, char *license, int len);
 
int dcmi_mcu_get_customized_info(int card_id, char *data_info, int data_info_size, int *len);
 
int dcmi_mcu_get_health(int card_id, unsigned int *health);
 
int dcmi_mcu_get_device_errorcode(
    int card_id, int *error_count, unsigned int *errorcode_list, unsigned int list_len);
 
int dcmi_mcu_get_device_errorcode_string(int card_id, int error_code, unsigned char *error_info, int buff_size);
 
int dcmi_mcu_get_boot_area(int card_id, int *boot_status);
 
int dcmi_mcu_get_device_elabel_info(int card_id, struct dcmi_elabel_info *elabel_info);
 
int dcmi_mcu_set_boot_area(int card_id);
 
int dcmi_mcu_get_temperature(int card_id, int *temper);
 
int dcmi_mcu_get_voltage(int card_id, unsigned int *voltage);
 
int dcmi_mcu_get_first_power_on_date(int card_id, unsigned int *first_power_on_date);
 
int dcmi_mcu_set_info_simple(int card_id, unsigned short opcode, unsigned int len, char *data_info);
 
int dcmi_mcu_set_info_dynamic(int card_id, unsigned short opcode, int len, char *data_info);
 
int dcmi_set_check_customized_is_exist(int card_id);

int dcmi_mcu_set_monitor_enable(int card_id, int device_id, int enable_flag);

int dcmi_mcu_set_disk_power(int card_id, int device_id, int power_flag);

int dcmi_mcu_get_monitor_enable(int card_id, int device_id, int *enable_flag);

int dcmi_mcu_start_bootloader_upgrade(int card_id, char *data_info, int len);

int dcmi_get_vrd_info(int card_id, char *version, int len);

int dcmi_get_vrd_upgrade_status(int card_id, int *status, int *progress);

int dcmi_set_vrd_upgrade_stage(int card_id, enum dcmi_upgrade_type input_type);

int dcmi_mcu_get_boot_sel(int card_id, int device_id, int *boot_sel);

int dcmi_mcu_set_boot_sel(int card_id, int device_id, int boot_sel);
 
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
 
#endif /* __DCMI_MCU_INTF_H__ */