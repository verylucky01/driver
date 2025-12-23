/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef _WIN32
#include <sys/stat.h>
#include <limits.h>
#include <unistd.h>
#include <fcntl.h>
#endif
#include <errno.h>
#include "dirent.h"
#include "ctype.h"
#include "securec.h"
#include "dcmi_log.h"
#include "dsmi_common_interface_custom.h"
#include "dcmi_interface_api.h"
#include "dcmi_common.h"
#include "dcmi_fault_manage_intf.h"
#include "dcmi_product_judge.h"
#include "dcmi_inner_cfg_persist.h"
#include "dcmi_inner_info_get.h"
#include "dcmi_mcu_intf.h"
#include "dcmi_environment_judge.h"
#include "dcmi_inner_cfg_manage.h"

int dcmi_set_npu_device_sec_revocation(
    int card_id, int device_id, enum dcmi_revo_type input_type, const unsigned char *file_data, unsigned int file_size)
{
    int ret;
    int device_logic_id = 0;

    ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", ret);
        return ret;
    }

    ret = dsmi_set_sec_revocation(device_logic_id, (DSMI_REVOCATION_TYPE)input_type, file_data, file_size);
    if ((ret != DSMI_OK) && (ret != DSMI_ERR_NOT_SUPPORT)) {
        gplog(LOG_ERR, "call dsmi_set_sec_revocation failed. err is %d.", ret);
    }

    return dcmi_convert_error_code(ret);
}

int dcmi_set_npu_device_mac(int card_id, int device_id, int mac_id, const char *mac_addr, unsigned int len)
{
    int ret;
    int device_logic_id = 0;

    ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", ret);
        return ret;
    }

    ret = dsmi_set_mac_addr(device_logic_id, mac_id, mac_addr, len);
    if ((ret != DSMI_OK) && (ret != DSMI_ERR_NOT_SUPPORT)) {
        gplog(LOG_ERR, "call dsmi_set_mac_addr failed. err is %d.", ret);
    }

    return dcmi_convert_error_code(ret);
}

int dcmi_set_npu_device_gateway(
    int card_id, int device_id, enum dcmi_port_type input_type, int port_id, struct dcmi_ip_addr *gateway)
{
    int ret;
    int device_logic_id = 0;

    ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", ret);
        return ret;
    }

    ret = dsmi_set_gateway_addr(device_logic_id, input_type, port_id, *(ip_addr_t *)gateway);
    if ((ret != DSMI_OK) && (ret != DSMI_ERR_NOT_SUPPORT)) {
        gplog(LOG_ERR, "call dsmi_set_gateway_addr failed. err is %d.", ret);
    }

    return dcmi_convert_error_code(ret);
}

int dcmi_set_npu_device_ip(int card_id, int device_id, enum dcmi_port_type input_type, int port_id,
    struct dcmi_ip_addr *ip, struct dcmi_ip_addr *mask)
{
    int ret;
    int device_logic_id = 0;
    ip_addr_t ip_addr = { { { 0 } } };
    ip_addr_t mask_addr = { { { 0 } } };

    ret = memcpy_s(&ip_addr, sizeof(ip_addr_t), ip, sizeof(struct dcmi_ip_addr));
    if (ret != EOK) {
        gplog(LOG_ERR, "memcpy_s ip_addr failed. err is %d.", ret);
        return DCMI_ERR_CODE_SECURE_FUN_FAIL;
    }

    ret = memcpy_s(&mask_addr, sizeof(ip_addr_t), mask, sizeof(struct dcmi_ip_addr));
    if (ret != EOK) {
        gplog(LOG_ERR, "memcpy_s mask_addr failed. err is %d.", ret);
        return DCMI_ERR_CODE_SECURE_FUN_FAIL;
    }

    ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", ret);
        return ret;
    }

    ret = dsmi_set_device_ip_address(device_logic_id, input_type, port_id, ip_addr, mask_addr);
    if ((ret != DSMI_OK) && (ret != DSMI_ERR_NOT_SUPPORT)) {
        gplog(LOG_ERR, "call dsmi_set_device_ip_address failed. err is %d.", ret);
    }

    return dcmi_convert_error_code(ret);
}

int dcmi_set_npu_device_ecc_enable(int card_id, int device_id, enum dcmi_device_type device_type, int enable_flag)
{
    int ret;
    int device_logic_id = 0;

    ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", ret);
        return DCMI_ERR_CODE_INNER_ERR;
    }

    ret = dsmi_config_ecc_enable(device_logic_id, (DSMI_DEVICE_TYPE)device_type, enable_flag);
    if ((ret != DSMI_OK) && (ret != DSMI_ERR_NOT_SUPPORT)) {
        gplog(LOG_ERR, "call dsmi_config_ecc_enable failed. err is %d.", ret);
    }

    return dcmi_convert_error_code(ret);
}

int dcmi_set_npu_device_user_config(
    int card_id, int device_id, const char *config_name, unsigned int buf_size, unsigned char *buf)
{
    int ret;
    int device_logic_id = 0;
    if (strcmp(config_name, "cpu_num_cfg") == 0) {
        ret = dcmi_check_cpu_num_config(buf, buf_size);
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "dcmi_check_cpu_num_config failed.");
            return DCMI_ERR_CODE_INVALID_PARAMETER;
        }
    }

    ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", ret);
        return ret;
    }

    ret = dsmi_set_user_config(device_logic_id, config_name, buf_size, buf);
    if ((ret != DSMI_OK) && (ret != DSMI_ERR_NOT_SUPPORT)) {
        gplog(LOG_ERR, "call dsmi_set_user_config failed. err is %d.", ret);
    }

    return dcmi_convert_error_code(ret);
}

int dcmi_clear_npu_device_user_config(int card_id, int device_id, const char *config_name)
{
    int ret;
    int device_logic_id = 0;

    ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", ret);
        return ret;
    }

    ret = dsmi_clear_user_config(device_logic_id, config_name);
    if ((ret != DSMI_OK) && (ret != DSMI_ERR_NOT_SUPPORT)) {
        gplog(LOG_ERR, "call dsmi_clear_user_config failed. err is %d.", ret);
    }

    return dcmi_convert_error_code(ret);
}

int dcmi_set_npu_device_share_enable(int card_id, int device_id, int enable_flag)
{
    int err;
    int device_logic_id = 0;
    const int device_share_main_cmd = 0x8001;
    const int device_share_sub_cmd = 0;

    err = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", err);
        return err;
    }

    err = dsmi_set_device_info(device_logic_id, device_share_main_cmd, device_share_sub_cmd,
        (void *)&enable_flag, (unsigned int)sizeof(int));
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "call dsmi_set_device_info failed.%d.", err);
    }

    return dcmi_convert_error_code(err);
}

int dcmi_set_sleep_state(int card_id, int device_id, struct dcmi_power_state_info_stru power_info)
{
    int ret;
    struct dsmi_power_state_info_stru dsmi_power_info = {0};

    ret = memcpy_s(&dsmi_power_info, sizeof(dsmi_power_info), &power_info, sizeof(power_info));
    if (ret != EOK) {
        gplog(LOG_ERR, "call memcpy_s failed. err is %d.", ret);
        return DCMI_ERR_CODE_SECURE_FUN_FAIL;
    }
    ret = dsmi_set_power_state_v2(device_id, dsmi_power_info);
    if ((ret != DSMI_OK) && (ret != DSMI_ERR_NOT_SUPPORT)) {
        gplog(LOG_ERR, "call dsmi_set_power_state_v2 failed. err is %d.", ret);
    }

    return dcmi_convert_error_code(ret);
}

STATIC int check_cpu_num_cfg_310p_soc(unsigned char *buf, int cpu_num_total)
{
    int sum = 0;

    if (buf[CTRL_CPU_CFG_INDEX] > cpu_num_total || buf[CTRL_CPU_CFG_INDEX] < DCMI_CPU_NUM_MIN) {
        gplog(LOG_ERR, "ctrl cpu value[%u] invalid!", buf[CTRL_CPU_CFG_INDEX]);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if (buf[AI_CPU_CFG_INDEX] > cpu_num_total || buf[AI_CPU_CFG_INDEX] < 0) {
        gplog(LOG_ERR, "ai cpu value[%u] invalid!", buf[AI_CPU_CFG_INDEX]);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    sum = buf[CTRL_CPU_CFG_INDEX] + buf[AI_CPU_CFG_INDEX];

    if (sum != cpu_num_total) {
        gplog(LOG_ERR, "The sum of ctrl_cpu_num[%u], data_cpu_num[%u] and ai_cpu_num[%u] is not %d!",
              buf[CTRL_CPU_CFG_INDEX], buf[DATA_CPU_CFG_INDEX], buf[AI_CPU_CFG_INDEX], cpu_num_total);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }
    return DCMI_OK;
}

STATIC int dcmi_check_cpu_num_config_310p(unsigned char *buf)
{
    int sum = 0;
    int i;
    int cpu_num_max = DCMI_CPU_NUM_MAX_PG;
    int cpu_num_total = DCMI_CPU_NUM_TOTAL_PG;

    if (dcmi_310p_chip_is_ag() == TRUE) {
        cpu_num_max = DCMI_CPU_NUM_MAX_AG;
        cpu_num_total = DCMI_CPU_NUM_TOTAL_AG;
    }

    if (dcmi_get_product_type_inner() == DCMI_A200I_SOC_A1) {
        int ret = check_cpu_num_cfg_310p_soc(buf, cpu_num_total);
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "check cpu num cfg failed, ret = %d", ret);
        }
        return ret;
    }

    for (i = 0; i < CPU_CFG_INDEX_MAX; i++) {
        if (i == DATA_CPU_CFG_INDEX) {
            continue;
        }
        if (buf[i] > cpu_num_max || buf[i] < DCMI_CPU_NUM_MIN) {
            gplog(LOG_ERR, "cpu_param[%u] invalid!", buf[i]);
            return DCMI_ERR_CODE_INVALID_PARAMETER;
        }
        sum += (int)buf[i];
    }

    if (sum != cpu_num_total) {
        gplog(LOG_ERR, "The sum of ctrl_cpu_num[%u], data_cpu_num[%u] and ai_cpu_num[%u] is not %d!",
              buf[CTRL_CPU_CFG_INDEX], buf[DATA_CPU_CFG_INDEX], buf[AI_CPU_CFG_INDEX], cpu_num_total);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }
    return DCMI_OK;
}

STATIC int dcmi_check_cpu_num_config_310b(unsigned char *buf)
{
    int sum = 0;
    int cpu_num_total = DCMI_CPU_NUM_TOTAL_310B;

    if (buf[CTRL_CPU_CFG_INDEX] > cpu_num_total || buf[CTRL_CPU_CFG_INDEX] < DCMI_CPU_NUM_MIN) {
        gplog(LOG_ERR, "ctrl cpu value[%u] invalid!", buf[CTRL_CPU_CFG_INDEX]);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if (buf[AI_CPU_CFG_INDEX] > cpu_num_total || buf[AI_CPU_CFG_INDEX] < 0) {
        gplog(LOG_ERR, "ai cpu value[%u] invalid!", buf[AI_CPU_CFG_INDEX]);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    sum = buf[CTRL_CPU_CFG_INDEX] + buf[AI_CPU_CFG_INDEX];

    if (sum != cpu_num_total) {
        gplog(LOG_ERR, "The sum of ctrl_cpu_num[%u], data_cpu_num[%u] and ai_cpu_num[%u] is not %d!",
              buf[CTRL_CPU_CFG_INDEX], buf[DATA_CPU_CFG_INDEX], buf[AI_CPU_CFG_INDEX], cpu_num_total);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }
    return DCMI_OK;
}

int dcmi_check_cpu_num_config(unsigned char *buf, unsigned int buf_size)
{
    int ret = DCMI_OK;

    if (buf_size != DCMI_CPU_NUM_CFG_LEN) {
        gplog(LOG_ERR, "buf_size invalid, the value should be 16!");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if (buf[DATA_CPU_CFG_INDEX] != 0) {
        gplog(LOG_ERR, "data_cpu_num[%u] invalid, the value should be 0!", buf[DATA_CPU_CFG_INDEX]);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    int chip_type = dcmi_get_board_chip_type();
    switch (chip_type) {
        case DCMI_CHIP_TYPE_D310B:
            ret = dcmi_check_cpu_num_config_310b(buf);
            if (ret != DCMI_OK) {
                gplog(LOG_ERR, "dcmi_check_cpu_num_config_310b failed, ret = %d.", ret);
                return ret;
            }
            break;
        case DCMI_CHIP_TYPE_D310P:
            ret = dcmi_check_cpu_num_config_310p(buf);
            if (ret != DCMI_OK) {
                gplog(LOG_ERR, "dcmi_check_cpu_num_config_310p failed, ret = %d.", ret);
                return ret;
            }
            break;
        default:
            ret = DCMI_ERR_CODE_NOT_SUPPORT;
            gplog(LOG_ERR, "This device does not support, ret = %d.", ret);
            return ret;
    }
    return ret;
}

int dcmi_check_capability_group_para(int ts_id, struct dcmi_capability_group_info *group_info)
{
    if (ts_id != DCMI_TS_AICORE) {
        gplog(LOG_ERR, "ts_id invalid. ts_id=%d", ts_id);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if (group_info == NULL) {
        gplog(LOG_ERR, "group_info invalid. group_info is NULL");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if (group_info->aivector_number != CAPABILITY_GROUP_SHARE_RESOURCE ||
        group_info->sdma_number != CAPABILITY_GROUP_SHARE_RESOURCE ||
        group_info->aicpu_number != CAPABILITY_GROUP_SHARE_RESOURCE ||
        group_info->active_sq_number != CAPABILITY_GROUP_SHARE_RESOURCE) {
        gplog(LOG_ERR, "para of group_info invalid. aivector_number=%u,"
            "sdma_number=%u, aicpu_number=%u, active_sq_number=%u", group_info->aivector_number,
            group_info->sdma_number, group_info->aicpu_number, group_info->active_sq_number);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    return DCMI_OK;
}

int dcmi_check_capability_group_support_type()
{
    int board_id = dcmi_get_board_id_inner();
    if (board_id == DCMI_310P_1P_CARD_BOARD_ID || board_id == DCMI_310P_1P_CARD_BOARD_ID_V2 ||
        board_id == DCMI_310P_1P_CARD_BOARD_ID_V3 || board_id == DCMI_310P_1P_XP_BOARD_ID) {
        return DCMI_OK;
    }
    return DCMI_ERR_CODE_NOT_SUPPORT;
}

int dcmi_npu_create_capability_group(int card_id, int device_id, int ts_id,
    struct dcmi_capability_group_info *group_info)
{
    int ret;
    int device_logic_id = 0;
    struct dsmi_capability_group_info create_group_info = {0};

    ret = memcpy_s(&create_group_info, sizeof(struct dsmi_capability_group_info),
        group_info, sizeof(struct dcmi_capability_group_info));
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "memcpy_s dcmi_capability_group_info failed. err is %d.", ret);
        return DCMI_ERR_CODE_SECURE_FUN_FAIL;
    }

    ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", ret);
        return ret;
    }

    ret = dsmi_create_capability_group(device_logic_id, ts_id, &create_group_info);
    if (ret != DSMI_OK) {
        gplog(LOG_ERR, "dsmi_create_capability_group failed. err is %d", ret);
        return dcmi_convert_error_code(ret);
    }

    return DCMI_OK;
}

int dcmi_npu_delete_capability_group(int card_id, int device_id, int ts_id, int group_id)
{
    int ret;
    int device_logic_id = 0;

    ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", ret);
        return ret;
    }

    ret = dsmi_delete_capability_group(device_logic_id, ts_id, group_id);
    if (ret != DSMI_OK) {
        gplog(LOG_ERR, "dsmi_create_capability_group failed. err is %d", ret);
        return dcmi_convert_error_code(ret);
    }

    return DCMI_OK;
}

void comp_and_switch_file_name(char file_a[SYSLOG_FILE_NAME_LEN + 1], char file_b[SYSLOG_FILE_NAME_LEN + 1])
{
    int i;
    char tmp;

    if (strcmp(file_a, file_b) > 0) {
        for (i = 0; i < SYSLOG_FILE_NAME_LEN + 1; i++) {
            tmp =  file_a[i];
            file_a[i] = file_b[i];
            file_b[i] = tmp;
        }
    }
}

int dcmi_str2int(int *ptmp_num, const char *str)
{
    int num;
    char *end_ptr = NULL;
    size_t i;
    const char *tmp_str = str;

    for (i = 0; i < strlen(tmp_str); i++) {
        if (!isdigit(tmp_str[i])) {
            return DCMI_ERR_CODE_INVALID_PARAMETER;
        }
    }

    num = strtol(tmp_str, &end_ptr, DCMI_DEC_TO_STR_BASE);
    /* 转换后num为0，但实际传入tmp_str不为'0...0'时，说明传入参数有误 */
    if (num == 0) {
        do {
            if (*tmp_str != '0') {
                return DCMI_ERR_CODE_INVALID_PARAMETER;
            }
        } while (*++tmp_str != '\0');
    }

    *ptmp_num = num;
    return DCMI_OK;
}

int dcmi_get_logfile_size(char *file_path, int *file_size)
{
    int ret, i;
    FILE *fp;
    char msn_cmd[SYS_LOG_MAX_CMD_LINE] = {0};
    char str_file_size[MAX_INT_LEN] = {0};

    ret = sprintf_s(msn_cmd, sizeof(msn_cmd), "du -sk %s | awk '{print $1}'", file_path);
    if (ret < EOK) {
        gplog(LOG_ERR, "Call sprintf_s failed. err is %d", ret);
        return DCMI_ERR_CODE_SECURE_FUN_FAIL;
    }

    fp = popen(msn_cmd, "r");
    if (fp == NULL) {
        gplog(LOG_ERR, "Failed to run the command on the popen, errno is %d\n", errno);
        return DCMI_ERR_CODE_INNER_ERR;
    }

    char *str = fgets(str_file_size, sizeof(str_file_size), fp);
    if (str == NULL) {
        (void)pclose(fp);
        gplog(LOG_ERR, "Failed to get file size by fgets, errno is %d\n", errno);
        return DCMI_ERR_CODE_INNER_ERR;
    }

    for (i = 0; i < strlen(str_file_size); i++) {
        if (str_file_size[i] < '0' || str_file_size[i] > '9') {
            str_file_size[i] = '\0';
        }
    }

    ret = dcmi_str2int(file_size, str_file_size);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "Str2int failed, ret = %d.\n", ret);
        (void)pclose(fp);
        return DCMI_ERR_CODE_INNER_ERR;
    }

    (void)pclose(fp);
    return DCMI_OK;
}

int dcmi_delete_dev_syslog_by_time(char *dir_path, char file_names[SYSLOG_FILE_MAX_NUM][SYSLOG_FILE_NAME_LEN + 1],
                                   int *file_size, int file_count, int max_size)
{
    int ret = 0, i, j;
    int end_index = file_count;
    int rest_memory = max_size;
    char clear_dir_cmd[SYS_LOG_MAX_CMD_LINE] = {0};
    char tmp_path[SYS_LOG_MAX_CMD_LINE] = {0};

    for (i = 0; i < file_count - 1; i++) {       // 把日志文件按照时间进行排序
        for (j = 0; j < file_count - i - 1; j++) {
            comp_and_switch_file_name(file_names[j], file_names[j + 1]);
        }
    }

    for (i = 0; i < file_count; i++) {
        (void)memset_s(tmp_path, SYS_LOG_MAX_CMD_LINE, 0, SYS_LOG_MAX_CMD_LINE);
        ret = sprintf_s(tmp_path, SYS_LOG_MAX_CMD_LINE, "%s%s", dir_path, file_names[i]);
        if (ret <= 0) {
            gplog(LOG_ERR, "call sprintf_s fail.ret is %d.", ret);
            return DCMI_ERR_CODE_SECURE_FUN_FAIL;
        }
        ret = dcmi_get_logfile_size(tmp_path, &file_size[i]);
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "get log file size failed.ret is %d.", ret);
            return ret;
        }
    }

    while (end_index >= 0 && rest_memory >= 0) {
        end_index--;
        if (end_index >= 0) {
            rest_memory = rest_memory - file_size[end_index];
        }
    }

    if (file_count - end_index > SYSLOG_FILE_MAX_NUM) {
        end_index = file_count - SYSLOG_FILE_MAX_NUM;
    }

    for (i = 0; i <= end_index && i < SYSLOG_FILE_MAX_NUM; i++) {
        (void)memset_s(clear_dir_cmd, SYS_LOG_MAX_CMD_LINE, 0, SYS_LOG_MAX_CMD_LINE);
        ret = sprintf_s(clear_dir_cmd, SYS_LOG_MAX_CMD_LINE, "rm -rf %s%s", dir_path, file_names[i]);
        if (ret <= 0) {
            gplog(LOG_ERR, "call sprintf_s fail.ret is %d.", ret);
            return DCMI_ERR_CODE_SECURE_FUN_FAIL;
        }

        ret = system(clear_dir_cmd);
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "excute cmd failed. err is %d.", ret);
            return DCMI_ERR_CODE_INNER_ERR;
        }
    }

    return ret;
}

int check_file_name(const char *entry_name)
{
    bool all_digits_except_dash = true;
    for (int i = 0; i < strlen(entry_name); i++) {
        if (entry_name[i] != '-' && !isdigit(entry_name[i])) {
            all_digits_except_dash = false;
            break;
        }
    }
    return all_digits_except_dash;
}

int dcmi_delete_extra_dev_syslog(char *dir_path, int max_file_size)
{
    int ret = 0, file_count = 0;
    struct dirent *entry;
    char file_names[SYSLOG_FILE_MAX_NUM][SYSLOG_FILE_NAME_LEN + 1] = {0};
    int file_size[SYSLOG_FILE_MAX_NUM] = {0};

    DIR *dir = opendir(dir_path);
    if (dir == NULL) {
        if (errno == ENOENT) {
            return DCMI_OK;
        }
        return DCMI_ERR_CODE_FILE_OPERATE_FAIL;
    }

    while ((entry = readdir(dir)) != NULL && file_count < SYSLOG_FILE_MAX_NUM) {
        if (entry->d_type == DT_DIR && strlen(entry->d_name) == SYSLOG_FILE_NAME_LEN && // 是文件且文件名长度19
            entry->d_name[4] == '-' && entry->d_name[7] == '-' &&   // 文件名第 4 个字符为 - 文件名第 7 个字符为 -
            entry->d_name[10] == '-' && entry->d_name[13] == '-' && // 文件名第 10 个字符为 - 文件名第 13 个字符为 -
            entry->d_name[16] == '-' && check_file_name(entry->d_name) == true) {  // 日志文件名第 16 个字符为 -
            if (strcpy_s(file_names[file_count], (SYSLOG_FILE_NAME_LEN + 1), entry->d_name) < 0) {
                gplog(LOG_ERR, "call strcpy_s failed.");
                (void)closedir(dir);
                return DCMI_ERR_CODE_SECURE_FUN_FAIL;
            }
            file_count++;
        }
    }
    (void)closedir(dir);

    ret = dcmi_delete_dev_syslog_by_time(dir_path, file_names, file_size, file_count, max_file_size);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "delete extra syslog by time failed.ret is %d.", ret);
        return ret;
    }

    return DCMI_OK;
}

/* *******************************************************************************************************************
    npu-smi set -t sys-log-dump -s gear -f path命令底层封装了msnpureport report --permanent [options]命令；
    npu-smi命令的入参gear取值范围为[1,10]，不同gear映射为不同的msnpureport命令入参[options]，对不同日志的容量和数量进行限制；
    npu-smi命令入参gear与msnpureport命令入参[options]映射关系表如下；
    例：npu-smi set -t sys-log-dump -s 1 -f path对应msnpureport report --permanent -d 0 --sys_log_num 10
    --sys_log_size 3 --event_log_num 10 --event_log_size 1 --sec_log_num 5 --sec_log_size 1 --fw_log_num 10
    --fw_log_size 3 --stackcore_num 50 --bbox_num 100 --fault_event_num 10
******************************************************************************************************************* */
int g_syslog_para[SYSLOG_LEVEL_NUM][SYSLOG_PARA_NUM] = {
    {10, 3, 10, 1, 5, 1, 10, 3, 50, 100, 10},
    {10, 6, 10, 2, 5, 2, 10, 6, 50, 100, 10},
    {10, 9, 10, 3, 5, 3, 10, 9, 50, 100, 10},
    {10, 12, 10, 4, 5, 4, 10, 12, 50, 100, 10},
    {10, 15, 10, 5, 5, 5, 10, 15, 50, 100, 10},
    {10, 18, 10, 6, 5, 6, 10, 18, 50, 100, 10},
    {10, 21, 10, 7, 5, 7, 10, 21, 50, 100, 10},
    {10, 24, 10, 8, 5, 8, 10, 24, 50, 100, 10},
    {10, 27, 10, 9, 5, 9, 10, 27, 50, 100, 10},
    {10, 30, 10, 10, 5, 10, 10, 30, 50, 100, 10},
};

char g_syslog_para_descrip[SYSLOG_PARA_NUM][MAX_PARA_DESCRIP] = {
    "--sys_log_num", "--sys_log_size", "--event_log_num",
    "--event_log_size", "--sec_log_num", "--sec_log_size", "--fw_log_num",
    "--fw_log_size", "--stackcore_num", "--bbox_num", "--fault_event_num"
};

int dcmi_set_npu_syslog_dump(int card_id, int logic_id, int syslog_level, char *file_path)
{
    int i, ret;
    char cmdline[SYS_LOG_MAX_CMD_LINE] = {0}, dir_path[SYS_LOG_MAX_CMD_LINE] = {0};
    int syslog_parameter[SYSLOG_PARA_NUM] = {0};

    ret = dcmi_check_file_path(file_path);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "filepath[%s] is invalid.", file_path ? file_path : "NULL");
        return ret;
    }

    ret = sprintf_s(dir_path, sizeof(dir_path), "%s/NPU-%d/", file_path, card_id);
    if (ret < EOK) {
        gplog(LOG_ERR, "call sprintf_s fail.ret is %d.", ret);
        return DCMI_ERR_CODE_SECURE_FUN_FAIL;
    }
    ret = dcmi_delete_extra_dev_syslog(dir_path, syslog_level * SYSLOG_MEMORY_FACTOR);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "delete extra syslog failed.ret is %d.", ret);
        return ret;
    }

    for (i = 0; i < SYSLOG_PARA_NUM; i ++) {
        syslog_parameter[i] = g_syslog_para[syslog_level - 1][i];
    }

    ret = sprintf_s(cmdline, sizeof(cmdline),
            "cd %s && mkdir -p NPU-%d && cd NPU-%d\nmsnpureport report --permanent -d "
            "%d %s %d %s %d %s %d %s %d %s %d %s %d %s %d %s %d %s %d %s %d %s %d &",
            file_path, card_id, card_id, logic_id,
            g_syslog_para_descrip[0], syslog_parameter[0],            // 0 记录参数 sys_log_num
            g_syslog_para_descrip[1], syslog_parameter[1],            // 1 记录参数 sys_log_size
            g_syslog_para_descrip[2], syslog_parameter[2],            // 2 记录参数 event_log_num
            g_syslog_para_descrip[3], syslog_parameter[3],            // 3 记录参数 event_log_size
            g_syslog_para_descrip[4], syslog_parameter[4],            // 4 记录参数 sec_log_num
            g_syslog_para_descrip[5], syslog_parameter[5],            // 5 记录参数 sec_log_size
            g_syslog_para_descrip[6], syslog_parameter[6],            // 6 记录参数 fw_log_num
            g_syslog_para_descrip[7], syslog_parameter[7],            // 7 记录参数 fw_log_size
            g_syslog_para_descrip[8], syslog_parameter[8],            // 8 记录参数 stackcore_num
            g_syslog_para_descrip[9], syslog_parameter[9],            // 9 记录参数 bbox_num
            g_syslog_para_descrip[10], syslog_parameter[10]           // 10 记录参数 fault_event_num
        );
    if (ret < EOK) {
        gplog(LOG_ERR, "sprintf_s failed. err is %d.", ret);
        return DCMI_ERR_CODE_SECURE_FUN_FAIL;
    }

    ret = system(cmdline);
    if (ret != 0) {
        gplog(LOG_ERR, "Excute cmd failed. err is %d.", ret);
        return DCMI_ERR_CODE_INNER_ERR;
    }
    gplog(LOG_OP, "Start exporting logs and files to path: %s/NPU-%d/", file_path, card_id);
    return DCMI_OK;
}

STATIC int dcmi_clear_npu_syslog_cfg_inner(int device_count, int *device_id_list, FILE *fp)
{
    int ret, num_id;
    char msn_cmd[SYS_LOG_MAX_CMD_LINE] = {0};
    char msn_pid[MAX_PID_LENGTH] = {0};
    char clear_cmd[SYS_LOG_MAX_CMD_LINE] = {0};

    for (num_id = 0; num_id < device_count; num_id++) {
        (void)memset_s(msn_cmd, sizeof(msn_cmd), 0, sizeof(msn_cmd));
        ret = sprintf_s(msn_cmd, sizeof(msn_cmd),
            "ps -ef | grep \"msnpureport report --permanent -d %d \" | grep -v grep |awk '{print $2}'",
            device_id_list[num_id]);
        if (ret < EOK) {
            gplog(LOG_ERR, "Call sprintf_s failed. err is %d", ret);
            return DCMI_ERR_CODE_SECURE_FUN_FAIL;
        }

        fp = popen(msn_cmd, "r");
        if (fp == NULL) {
            gplog(LOG_ERR, "Failed to run the command on the popen, errno is %d\n", errno);
            return DCMI_ERR_CODE_INNER_ERR;
        }

        (void)memset_s(msn_pid, sizeof(msn_pid), 0, sizeof(msn_pid));
        char *str = fgets(msn_pid, sizeof(msn_pid), fp);
        if (str == NULL) {
            (void)pclose(fp);
            continue;
        }

        (void)memset_s(clear_cmd, sizeof(clear_cmd), 0, sizeof(clear_cmd));
        ret = sprintf_s(clear_cmd, sizeof(clear_cmd), "kill -15 %s", msn_pid);
        if (ret < EOK) {
            gplog(LOG_ERR, "Call sprintf_s failed. err is %d", ret);
            (void)pclose(fp);
            return DCMI_ERR_CODE_SECURE_FUN_FAIL;
        }
        
        ret = system(clear_cmd);
        if (ret != 0) {
            gplog(LOG_ERR, "Failed to run the clear command on system. card_id:%d", device_id_list[num_id]);
            (void)pclose(fp);
            return DCMI_ERR_CODE_INNER_ERR;
        }
        (void)pclose(fp);
    }

    return DCMI_OK;
}

int dcmi_clear_npu_syslog_cfg()
{
    int ret, device_count = 0;
    FILE *fp = NULL;
    int device_id_list[MAX_DEVICE_NUM] = {0};

    ret = dcmi_get_npu_device_list((int *)&device_id_list[0], MAX_DEVICE_NUM, &device_count);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_npu_device_list failed. err is %d", ret);
        return DCMI_ERR_CODE_INNER_ERR;
    }

    ret = dcmi_clear_npu_syslog_cfg_inner(device_count, device_id_list, fp);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_clear_npu_syslog_cfg_inner failed. err is %d", ret);
        return ret;
    }

    ret = dcmi_clear_syslog_cfg();
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_clear_syslog_cfg failed. err is %d", ret);
        return ret;
    }
    return DCMI_OK;
}

int dcmi_set_syslog_persistence_cfg(char *cmdline, int cmdline_len)
{
    int err;

    if (cmdline_len <= 0) {
        gplog(LOG_ERR, "cmdline len err, cmdline_len is %d.", cmdline_len);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    err = dcmi_cfg_insert_syslog_persistence_cmdline(cmdline);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_cfg_insert_syslog_persistence_cmdline failed. err is %d", err);
        return err;
    }

    return DCMI_OK;
}

int dcmi_clear_syslog_cfg()
{
    int ret, err;
    int persistence_enable;
    char cfg[SYS_LOG_MAX_CMD_LINE] = {0};
    
    ret = access(DCMI_SYSLOG_CONF, F_OK);
    if (ret != 0) {
        goto CREAT_CFG_DEFAULT;
    }
 
    ret = dcmi_check_syslog_cfg_legal(cfg, sizeof(cfg) / sizeof(char));
    if (ret != DCMI_OK) {
        goto CREAT_CFG_DEFAULT;
    }
 
    err = dcmi_get_syslog_cfg_recover_mode(&persistence_enable);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_cfg_get_config_recover_mode failed. err is %d", err);
        return err;
    }

    err = dcmi_cfg_syslog_clear_file(persistence_enable);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_cfg_syslog_clear_file failed. err is %d", err);
        return DCMI_ERR_CODE_INNER_ERR;
    }

    return DCMI_OK;

CREAT_CFG_DEFAULT:
    ret = dcmi_cfg_create_default_syslog_file();
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_cfg_create_default_syslog_file failed. err is %d", ret);
        return ret;
    }
    return DCMI_OK;
}

int dcmi_set_syslog_persistence_mode(int mode)
{
    int ret;
    char cfg[SYS_LOG_MAX_CMD_LINE] = {0};
    
    ret = access(DCMI_SYSLOG_CONF, F_OK);
    if (ret != 0) {
        ret = dcmi_cfg_create_default_syslog_file();
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "dcmi_cfg_create_default_syslog_file failed. ret is %d", ret);
            return ret;
        }
    }
    ret = dcmi_check_syslog_cfg_legal(cfg, sizeof(cfg) / sizeof(char));
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_check_syslog_cfg_legal failed. ret is %d", ret);
        return ret;
    }

    ret = dcmi_set_syslog_cfg_recover_mode(mode);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_set_syslog_cfg_recover_mode failed. ret is %d", ret);
        return ret;
    }
    return DCMI_OK;
}

int dcmi_get_npu_custom_op_status(int card_id, int device_id, int *enable_flag)
{
    int err;
    int device_logic_id = 0;
    unsigned int size = (unsigned int)sizeof(int);

    err = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", err);
        return DCMI_ERR_CODE_INNER_ERR;
    }

    err = dsmi_get_device_info(device_logic_id, DCMI_MAIN_CMD_SOC_INFO, DCMI_SOC_INFO_SUB_CMD_CUSTOM_OP,
        (void *)enable_flag, &size);
    if ((err != DSMI_OK) && (err != DSMI_ERR_NOT_SUPPORT)) {
        gplog(LOG_ERR, "call dsmi_get_device_info failed. err is %d.", err);
    }

    return dcmi_convert_error_code(err);
}

int dcmi_save_custom_op_cfg(int card_id, int device_id, int enable_value)
{
    int err;
    unsigned int recover_enable;
    if ((dcmi_is_in_phy_machine_root() == TRUE) && (dcmi_board_chip_type_is_ascend_910_93() == TRUE)) {
        err = dcmi_cfg_get_custom_op_config_recover_mode(&recover_enable);
        if (err != DCMI_OK) {
            gplog(LOG_ERR, "dcmi_cfg_get_custom_op_config_recover_mode failed. err is %d", err);
            return err;
        }

        if (recover_enable == DCMI_CFG_RECOVER_ENABLE) {
            err = dcmi_cfg_insert_set_custom_op_cmdline(card_id, device_id, enable_value);
            if (err != DCMI_OK) {
                gplog(LOG_ERR, "dcmi_cfg_insert_set_custom_op_cmdline failed. err is %d", err);
                return err;
            }
        }
    }
    return DCMI_OK;
}

int dcmi_set_npu_device_info(
    int card_id, int device_id, enum dcmi_main_cmd main_cmd, unsigned int sub_cmd, const void *buf, unsigned int size)
{
    int ret;
    int device_logic_id = 0;

    if (main_cmd == DCMI_MAIN_CMD_SEC && sub_cmd == DCMI_SEC_SUB_CMD_PSS) {
        // pkcs使能整机生效，与device_id, card_id无关，参数不会使用，此处强制写0
        ret = dsmi_set_device_info(0, (DSMI_MAIN_CMD)main_cmd, sub_cmd, buf, size);
        if (ret != DSMI_OK) {
            gplog(LOG_ERR, "call dsmi_set_device_info failed. err is %d.", ret);
            return dcmi_convert_error_code(ret);
        }
        gplog(LOG_OP, "set pkcs-enable success.");
        return dcmi_convert_error_code(ret);
    }

    ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", ret);
        return ret;
    }

    ret = dsmi_set_device_info(device_logic_id, (DSMI_MAIN_CMD)main_cmd, sub_cmd, buf, size);
    if ((ret != DSMI_OK) && (ret != DSMI_ERR_NOT_SUPPORT)) {
        gplog(LOG_ERR, "call dsmi_set_device_info failed. err is %d.", ret);
    }

    if (main_cmd == DCMI_MAIN_CMD_SOC_INFO && sub_cmd == DCMI_SOC_INFO_SUB_CMD_CUSTOM_OP) {
        ret = dcmi_save_custom_op_cfg(card_id, device_id, *(int *)(buf));
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "dcmi_save_custom_op_cfg failed. err is %d", ret);
        }
        ret = DCMI_OK;
    }

    return dcmi_convert_error_code(ret);
}

#if defined DCMI_VERSION_2
int dcmi_switch_boot_area(int card_id, int device_id)
{
    int ret;
    enum dcmi_unit_type device_type = NPU_TYPE;

    if (dcmi_board_type_is_station()) {
        ret = dcmi_get_device_type(card_id, device_id, &device_type);
        if (ret != 0) {
            gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d.", ret);
            return ret;
        }

        if (device_type == MCU_TYPE) {
            ret = dcmi_mcu_set_boot_area(card_id);
        } else {
            gplog(LOG_ERR, "device_type %d is not support.", device_type);
            return DCMI_ERR_CODE_NOT_SUPPORT;
        }
    } else {
        gplog(LOG_ERR, "This device does not support setting boot area.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    if (ret != DCMI_OK) {
        gplog(LOG_OP, "set boot area failed. card_id=%d, device_id=%d, ret=%d", card_id, device_id, ret);
        return ret;
    }

    gplog(LOG_OP, "set boot area success. card_id=%d device_id=%d", card_id, device_id);
    return DCMI_OK;
}
#endif