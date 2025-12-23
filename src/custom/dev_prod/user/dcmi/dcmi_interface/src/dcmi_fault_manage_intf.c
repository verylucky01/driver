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
#include <time.h>
#include <limits.h>
#include <errno.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
 
#include "securec.h"
#include "dsmi_common_interface_custom.h"
#include "dcmi_interface_api.h"
#include "dcmi_common.h"
#include "dcmi_init_basic.h"
#include "dcmi_log.h"
#include "dcmi_mcu_intf.h"
#include "dcmi_os_adapter.h"
#include "dcmi_inner_info_get.h"
#include "dcmi_product_judge.h"
#include "dcmi_environment_judge.h"
#include "dcmi_fault_manage_intf.h"

struct err_code_map_node g_err_code_map[DCMI_ERR_CODE_MAX_NUM] = {
    {DSMI_OK, DCMI_OK},
    {DSMI_ERR_NO_DEVICE,           DCMI_ERR_CODE_DEVICE_NOT_EXIST},
    {DSMI_ERR_INVALID_DEVICE,      DCMI_ERR_CODE_INVALID_DEVICE_ID},
    {DSMI_ERR_INVALID_VALUE,       DCMI_ERR_CODE_INVALID_PARAMETER},
    {DSMI_ERR_INVALID_HANDLE,      DCMI_ERR_CODE_INVALID_PARAMETER},
    {DSMI_ERR_INNER_ERR,           DCMI_ERR_CODE_INNER_ERR},
    {DSMI_ERR_PARA_ERROR,          DCMI_ERR_CODE_INVALID_PARAMETER},
    {DSMI_ERR_NOT_EXIST,           DCMI_ERR_CODE_DEVICE_NOT_EXIST},
    {DSMI_ERR_DEVICE_BUSY,         DCMI_ERR_CODE_NOT_SUPPORT_IN_CONTAINER},
    {DSMI_ERR_WAIT_TIMEOUT,        DCMI_ERR_CODE_TIME_OUT},
    {DSMI_ERR_PARA_INVALID,        DCMI_ERR_CODE_INVALID_PARAMETER},
    {DSMI_ERR_IOCRL_FAIL,          DCMI_ERR_CODE_IOCTL_FAIL},
    {DSMI_ERR_SEND_MESG,           DCMI_ERR_CODE_SEND_MSG_FAIL},
    {DSMI_ERR_OPER_NOT_PERMITTED,  DCMI_ERR_CODE_OPER_NOT_PERMITTED},
    {DSMI_ERR_TRY_AGAIN,           DCMI_ERR_CODE_NOT_REDAY},
    {DSMI_ERR_FILE_OPS,            DCMI_ERR_CODE_FILE_OPERATE_FAIL},
    {DSMI_ERR_MEMORY_OPT_FAIL,     DCMI_ERR_CODE_MEM_OPERATE_FAIL},
    {DSMI_ERR_PARTITION_NOT_RIGHT, DCMI_ERR_CODE_PARTITION_NOT_RIGHT},
    {DSMI_ERR_RESOURCE_OCCUPIED,   DCMI_ERR_CODE_RESOURCE_OCCUPIED},
    {DSMI_ERR_NOT_SUPPORT,         DCMI_ERR_CODE_NOT_SUPPORT},
};

int dcmi_convert_error_code(int dsmi_err_code)
{
    size_t index;
    size_t err_code_map_size = sizeof(g_err_code_map) / sizeof(struct err_code_map_node);

    for (index = 0; index < err_code_map_size; index++) {
        if (dsmi_err_code == g_err_code_map[index].dsmi_err_code) {
            return g_err_code_map[index].dcmi_err_code;
        }
    }
    return DCMI_ERR_CODE_INNER_ERR;
}

#if defined DCMI_VERSION_2
int dcmi_get_device_errorcode_v2(int card_id, int device_id, int *error_count, unsigned int *error_code_list,
    unsigned int list_len)
{
    int err;
    enum dcmi_unit_type device_type = NPU_TYPE;
    /* 错误码的最大个数为128，修改此大小，需要评估D芯片和MCU的错误码个数是否满足要求 */
    unsigned int err_code_list[DCMI_ERROR_CODE_MAX_COUNT] = {0}, err_code_count = 0;

    bool check_result = ((error_count == NULL) || (error_code_list == NULL) || list_len == 0);
    if (check_result) {
        gplog(LOG_ERR, "error_count or error_code_list is invalid.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    err = dcmi_get_device_type(card_id, device_id, &device_type);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d.", err);
        return err;
    }

    if (device_type == NPU_TYPE) {
        err = dcmi_get_npu_device_errorcode(card_id, device_id, (int *)(void *)&err_code_count, err_code_list,
            DCMI_ERROR_CODE_MAX_COUNT);
    } else if (device_type == MCU_TYPE) {
        err = dcmi_mcu_get_device_errorcode(card_id, (int *)(void *)&err_code_count,
                                            err_code_list, DCMI_ERROR_CODE_MAX_COUNT);
#ifndef _WIN32
    } else if (device_type == CPU_TYPE) {
        err = dcmi_cpu_get_device_errorcode(card_id, (int *)(void *)&err_code_count,
                                            err_code_list, DCMI_ERROR_CODE_MAX_COUNT);
#endif
    } else {
        gplog(LOG_ERR, "device_type %d is error.", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    if (err != DCMI_OK) {
        gplog(LOG_ERR, "get device error code failed. err is %d.", err);
        return DCMI_ERR_CODE_INNER_ERR;
    }

    if (err_code_count > list_len) {
        gplog(LOG_ERR, "list_len %u is smaller than err_code_count %u.", list_len, err_code_count);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    err = memcpy_s(error_code_list, list_len * sizeof(unsigned int), err_code_list,
        err_code_count * sizeof(unsigned int));
    if (err != EOK) {
        gplog(LOG_ERR, "memcpy_s failed. err is  %d.", err);
        return DCMI_ERR_CODE_SECURE_FUN_FAIL;
    }
    *error_count = err_code_count;
    return DCMI_OK;
}

int dcmi_get_device_errorcode_string(int card_id, int device_id, unsigned int error_code, unsigned char *error_info,
    int buf_size)
{
    int err;
    enum dcmi_unit_type device_type = NPU_TYPE;

    if (error_info == NULL || buf_size < DCMI_MIN_ERR_INFO_LEN) {
        gplog(LOG_ERR, "error_info or buff_size %d is invalid.", buf_size);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    err = dcmi_get_device_type(card_id, device_id, &device_type);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d.", err);
        return err;
    }

    if (device_type == NPU_TYPE) {
        return dcmi_get_npu_device_errorcode_string(card_id, device_id, error_code, error_info, buf_size);
    } else if (device_type == MCU_TYPE) {
        return dcmi_mcu_get_device_errorcode_string(card_id, error_code, error_info, buf_size);
    } else {
        gplog(LOG_ERR, "device_type %d is error.", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
}

int dcmi_get_driver_errorcode(int *error_count, unsigned int *error_code_list, unsigned int list_len)
{
#ifndef _WIN32
    int err;
    unsigned int err_code_list[DCMI_ERROR_CODE_MAX_COUNT] = {0}, err_count = 0;

    bool check_result = (error_count == NULL || error_code_list == NULL || list_len == 0);
    if (check_result) {
        gplog(LOG_ERR, "error_count or error_code_list or list_len is invalid.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    err = dsmi_get_driver_errorcode((int *)(void *)&err_count, err_code_list);
    if (err != DSMI_OK) {
        gplog(LOG_ERR, "dsmi_get_driver_errorcode failed. err is %d.", err);
        return dcmi_convert_error_code(err);
    }

    if (err_count > list_len) {
        gplog(LOG_ERR, "list_len %u is smaller than error_count %u.", list_len, err_count);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    err = memcpy_s(error_code_list, list_len * sizeof(unsigned int), err_code_list, err_count * sizeof(unsigned int));
    if (err != EOK) {
        gplog(LOG_ERR, "memcpy_s failed. err is %d.", err);
        return DCMI_ERR_CODE_SECURE_FUN_FAIL;
    }
    *error_count = err_count;
    return DCMI_OK;
#else
    return DCMI_ERR_CODE_NOT_SUPPORT;
#endif
}

int dcmi_get_fault_event(int card_id, int device_id, int timeout, struct dcmi_event_filter filter,
    struct dcmi_event *event)
{
    int err;
    enum dcmi_unit_type device_type = NPU_TYPE;

    if (dcmi_check_run_in_docker() && dcmi_check_run_not_root()) {
        gplog(LOG_OP, "Operation not permitted, only root user on physical or virtual machine"
            " or privileged docker can call this api.");
        return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
    }

    if (event == NULL) {
        gplog(LOG_ERR, "input para event is NULL.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }
    if ((timeout < 0 && timeout != -1) || (timeout > DCMI_GET_EVENT_WAIT_TIME_MAX)) {
        gplog(LOG_ERR, "input para timeout is invalid. timeout=%d.", timeout);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if (dcmi_board_chip_type_is_ascend_310()) {
        gplog(LOG_OP, "Operation not support.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    err = dcmi_get_device_type(card_id, device_id, &device_type);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d.\n", err);
        return err;
    }

    if (device_type == NPU_TYPE) {
        return dcmi_get_npu_fault_event(card_id, device_id, timeout, filter, event);
    } else {
        gplog(LOG_ERR, "device_type %d is not support.", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
}

int dcmi_subscribe_fault_event(int card_id, int device_id, struct dcmi_event_filter filter,
    dcmi_fault_event_callback handler)
{
    int err;
    enum dcmi_unit_type device_type = NPU_TYPE;

    if (dcmi_check_run_in_docker() && dcmi_check_run_not_root()) {
        gplog(LOG_OP, "Operation not permitted, only root user on physical or virtual machine"
            " or privileged docker can call this api.");
        return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
    }

    if (handler == NULL) {
        gplog(LOG_ERR, "handler is invalid.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if (dcmi_board_chip_type_is_ascend_310()) {
        gplog(LOG_OP, "Operation not support.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    if (card_id == -1 && device_id == -1) {
        device_type = NPU_TYPE;
    } else {
        err = dcmi_get_device_type(card_id, device_id, &device_type);
        if (err != DCMI_OK) {
            gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d.\n", err);
            return err;
        }
    }

    if (device_type == NPU_TYPE) {
        return dcmi_subscribe_npu_fault_event(card_id, device_id, filter, handler);
    } else {
        gplog(LOG_ERR, "device_type %d is not support.", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
}

int dcmi_get_device_pcie_error_cnt(int card_id, int device_id, struct dcmi_chip_pcie_err_rate *pcie_err_code_info)
{
    int err;
    enum dcmi_unit_type device_type = NPU_TYPE;

    if (pcie_err_code_info == NULL) {
        gplog(LOG_ERR, "pcie_err_code_info is invalid.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    err = dcmi_get_device_type(card_id, device_id, &device_type);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d.", err);
        return err;
    }

    if (dcmi_board_chip_type_is_ascend_910() == TRUE || dcmi_board_chip_type_is_ascend_910b() == TRUE) {
        gplog(LOG_ERR, "This device does not support.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    if (device_type == NPU_TYPE) {
        return dcmi_get_npu_device_pcie_error(card_id, device_id, pcie_err_code_info);
    } else {
        gplog(LOG_ERR, "device_type %d is error.", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
}
#endif

#if defined DCMI_VERSION_1
int dcmi_clear_pcie_error_cnt(int card_id, int device_id)
{
    return dcmi_set_device_clear_pcie_error(card_id, device_id);
}

int dcmi_get_pcie_error_cnt(int card_id, int device_id, struct dcmi_chip_pcie_err_rate_stru *pcie_err_code_info)
{
    return dcmi_get_device_pcie_error_cnt(card_id, device_id, (struct dcmi_chip_pcie_err_rate *)pcie_err_code_info);
}

int dcmi_get_device_errorinfo(int card_id, int device_id, int error_code, unsigned char *error_info, int buf_size)
{
    return dcmi_get_device_errorcode_string(card_id, device_id, error_code, error_info, buf_size);
}

int dcmi_get_device_errorcode(
    int card_id, int device_id, int *error_count, unsigned int *error_code_list, int *error_width)
{
    int err;
    unsigned int err_code_list[DCMI_ERROR_CODE_MAX_COUNT] = {0};

    bool check_result = (error_count == NULL || error_code_list == NULL || error_width == NULL);
    if (check_result) {
        gplog(LOG_ERR, "error_count or error_code_list or error_width is invalid.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if (dcmi_board_chip_type_is_ascend_910b() || dcmi_board_chip_type_is_ascend_910_93()) {
        gplog(LOG_ERR, "This device does not support.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    err = dcmi_get_device_errorcode_v2(card_id, device_id, error_count, err_code_list, DCMI_ERROR_CODE_MAX_COUNT);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_errorcode_v2 failed. err is %d.", err);
        return err;
    }

    err = memcpy_s(error_code_list, DCMI_ERROR_CODE_MAX_COUNT * sizeof(unsigned int), err_code_list,
        (*error_count) * (sizeof(unsigned int)));
    if (err != EOK) {
        gplog(LOG_ERR, "memcpy_s failed. err is %d.", err);
        return DCMI_ERR_CODE_SECURE_FUN_FAIL;
    }
    *error_width = sizeof(unsigned int);
    return DCMI_OK;
}
#endif

// 解析告警信息内容
int dcmi_get_alarm_info(char *alarm_line, size_t str_len, char section_info[][CPU_ALARM_SECTION_LEN],
    int section_array_len, int *sec_count)
{
    char *tmp_str = NULL;
    int section_count = 0;
    int ret;
    long long last_pos = -1;
    long long section_len;

    while ((tmp_str = strchr(alarm_line + last_pos + 1, '@')) != NULL) {
        if ((tmp_str - alarm_line >= str_len - 1) && (section_count >= section_array_len)) {
            break;
        }

        section_len = tmp_str - alarm_line - last_pos - 1;
        ret = strncpy_s(section_info[section_count], CPU_ALARM_SECTION_LEN, alarm_line + last_pos + 1, section_len);
        if (ret != EOK) {
            gplog(LOG_ERR, "call strncpy_s failed. err is %d.", ret);
            return DCMI_ERR_CODE_SECURE_FUN_FAIL;
        }

        ++section_count;
        last_pos = tmp_str - alarm_line;
    }

    // @aabb为结束符
    if (((unsigned long long)last_pos == str_len - strlen("@aabb")) && section_count < section_array_len) {
        section_len = strlen("aabb");
        ret = strncpy_s(section_info[section_count], CPU_ALARM_SECTION_LEN, alarm_line + last_pos + 1, section_len);
        if (ret != EOK) {
            gplog(LOG_ERR, "call strncpy_s failed. err is %d.", ret);
            return DCMI_ERR_CODE_SECURE_FUN_FAIL;
        }
        ++section_count;
    } else {
        gplog(LOG_ERR, "last_pos(%lld, %lu) section_count(%d, %d)!",
              last_pos, str_len, section_count, section_array_len);
        gplog(LOG_ERR, "alarm_line(%s) format invalid!", alarm_line);
        return DCMI_ERR_CODE_INNER_ERR;
    }

    *sec_count = section_count;
    return DCMI_OK;
}

int dcmi_hilens_cpu_get_device_errorcode(int *error_count, unsigned int *error_code_list, int list_size)
{
    char info_line[DCMI_CFG_LINE_MAX_LEN] = {0};
    size_t str_len;
    int alarm_count = 0;
    // 单个告警信息最大为6个字段，每个字段不超过256字符
    char alarm_section_info[CPU_ALARM_KEY_SECTION_NUM][BOARD_INFO_LINE_LEN] = { { 0 } };
    int ret = 0;
    int section_count = 0;

    // 文件不存在，则认为没有告警
    if (access(CPU_ALARM_FILE, F_OK) != DCMI_OK) {
        gplog(LOG_INFO, "can't open alarm file, maybe not exist.");
        *error_count = 0;
        return DCMI_OK;
    }

    FILE *fp = fopen(CPU_ALARM_FILE, "r");
    if (fp == NULL) {
        gplog(LOG_ERR, "open alarm file failed!\n");
        return DCMI_ERR_CODE_FILE_OPERATE_FAIL;
    }

    unsigned int *pcode = error_code_list;

    while (fgets(info_line, sizeof(info_line) - 1, fp) != NULL) {
        str_len = strlen(info_line) - 1;
        if (str_len <= 0 || str_len >= (sizeof(info_line) - 1)) {
            gplog(LOG_ERR, "str_len (%lu)  is invalid.", str_len);
            continue;
        }
        if (info_line[str_len] == '\n') {
            info_line[str_len] = '\0';
        }

        ret = dcmi_get_alarm_info(info_line, str_len, alarm_section_info, CPU_ALARM_KEY_SECTION_NUM, &section_count);
        if (ret != DCMI_OK) {
            break;
        }

        // 只包含3个字段，表示是告警头
        if (section_count == CPU_NO_ALARM_SECTION_NUM) {
            *error_count = strtol(alarm_section_info[CPU_ALARM_COUNT_INDEX], NULL, DCMI_NUMBER_BASE);
        } else if (section_count == CPU_ALARM_EXIST_SECTION_NUM) {
            // 包含6个字段为告警本身信息
            pcode[alarm_count++] = (unsigned int)strtol(alarm_section_info[CPU_ALARM_ID_INDEX], NULL, DCMI_NUMBER_BASE);
        } else {
            gplog(LOG_ERR, "alarm_line(%s) format invalid!", info_line);
            ret = DCMI_ERR_CODE_INNER_ERR;
            break;
        }

        // error_code存储的告警数量有限制,达到就不读取了
        if (alarm_count >= list_size) {
            gplog(LOG_ERR, "alarm_count(%d) reach max length!", alarm_count);
            *error_count = alarm_count;
            ret = DCMI_ERR_CODE_INVALID_PARAMETER;
            break;
        }
    }

    (void)fclose(fp);
    return ret;
}

int dcmi_cpu_get_device_errorcode(int card_id, int* error_count, unsigned int *error_code_list, unsigned int list_len)
{
    if (dcmi_board_type_is_hilens()) {
        return dcmi_hilens_cpu_get_device_errorcode(error_count, error_code_list, list_len);
    }

    return dcmi_mcu_get_device_errorcode(card_id, error_count, error_code_list, list_len);
}

int dcmi_get_npu_device_errorcode(
    int card_id, int device_id, int *error_count, unsigned int *error_code_list, unsigned int list_len)
{
    int ret;
    int device_logic_id = 0;
    /* 调用点使用的list_len为 dsmi_get_device_errorcode的最大错误码个数128，此处不做有效性判断 */
    (void)list_len;

    ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", ret);
        return ret;
    }

    ret = dsmi_get_device_errorcode(device_logic_id, error_count, error_code_list);
    if ((ret != DSMI_OK) && (ret != DSMI_ERR_NOT_SUPPORT)) {
        gplog(LOG_ERR, "call dsmi_get_device_errorcode failed. err is %d.", ret);
    }

    return dcmi_convert_error_code(ret);
}

int dcmi_get_npu_device_errorcode_string(
    int card_id, int device_id, int error_code, unsigned char *error_info, int buff_size)
{
    int ret;
    int device_logic_id = 0;

    ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", ret);
        return ret;
    }

    ret = dsmi_query_errorstring(device_logic_id, error_code, error_info, buff_size);
    if ((ret != DSMI_OK) && (ret != DSMI_ERR_NOT_SUPPORT)) {
        gplog(LOG_ERR, "call dsmi_query_errorstring failed. err is %d.", ret);
    }

    return dcmi_convert_error_code(ret);
}

int dcmi_subscribe_npu_fault_event(int card_id, int device_id, struct dcmi_event_filter filter,
    dcmi_fault_event_callback handler)
{
    int ret;
    int device_logic_id = 0;

    if (card_id == -1 && device_id == -1) {
        device_logic_id = -1;
    } else {
        ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", ret);
            return ret;
        }
    }

    ret = dsmi_subscribe_fault_event(device_logic_id, *(struct dsmi_event_filter *)&filter,
    (fault_event_callback)handler);
    if ((ret != DSMI_OK) && (ret != DSMI_ERR_NOT_SUPPORT)) {
        gplog(LOG_ERR, "call dcmi_subscribe_npu_fault_event failed. err is %d.", ret);
    }

    return dcmi_convert_error_code(ret);
}