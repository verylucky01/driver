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
#include <stddef.h>
#include <time.h>
#ifndef _WIN32
#include <unistd.h>
#include <sys/mman.h>
#endif
#include "securec.h"
#include "dsmi_common_interface_custom.h"

#ifndef _WIN32
#include "ascend_hal.h"
#endif
#include "dcmi_fault_manage_intf.h"
#include "dcmi_log.h"
#include "dcmi_product_judge.h"
#include "dcmi_environment_judge.h"
#include "dcmi_inner_info_get.h"
#include "dcmi_permission_judge.h"

int dcmi_get_device_info_maincmd_permission(int main_cmd, int *cmd_permission)
{
    size_t index, table_size;

    struct dcmi_get_device_main_cmd_table cmd_permission_table[] = {
        {DCMI_MAIN_CMD_DVPP, DCMI_GET_DEVICE_INFO_CMD_SUPPORT_NO_LIMIT},
        {DCMI_MAIN_CMD_ISP, DCMI_GET_DEVICE_INFO_CMD_SUPPORT_NO_LIMIT},
        {DCMI_MAIN_CMD_TS_GROUP_NUM, DCMI_GET_DEVICE_INFO_CMD_SUPPORT_NO_LIMIT},
        {DCMI_MAIN_CMD_CAN, DCMI_GET_DEVICE_INFO_CMD_SUPPORT_NO_LIMIT},
        {DCMI_MAIN_CMD_UART, DCMI_GET_DEVICE_INFO_CMD_SUPPORT_NO_LIMIT},
        {DCMI_MAIN_CMD_UPGRADE, DCMI_GET_DEVICE_INFO_CMD_SUPPORT_NO_LIMIT},
        {DCMI_MAIN_CMD_UFS, DCMI_GET_DEVICE_INFO_CMD_SUPPORT_NO_LIMIT},
        {DCMI_MAIN_CMD_OS_POWER, DCMI_GET_DEVICE_INFO_CMD_SUPPORT_NO_LIMIT},
        {DCMI_MAIN_CMD_LP, DCMI_GET_DEVICE_INFO_CMD_SUPPORT_NO_LIMIT},
        {DCMI_MAIN_CMD_MEMORY, DCMI_GET_DEVICE_INFO_CMD_SUPPORT_NO_LIMIT},
        {DCMI_MAIN_CMD_RECOVERY, DCMI_GET_DEVICE_INFO_CMD_SUPPORT_NO_LIMIT},
        {DCMI_MAIN_CMD_TS, DCMI_GET_DEVICE_INFO_CMD_SUPPORT_NO_LIMIT},
        {DCMI_MAIN_CMD_CHIP_INF, DCMI_GET_DEVICE_INFO_CMD_SUPPORT_NO_LIMIT},
        {DCMI_MAIN_CMD_QOS, DCMI_GET_DEVICE_INFO_CMD_SUPPORT_PHY_MACHINE},
        {DCMI_MAIN_CMD_SOC_INFO, DCMI_GET_DEVICE_INFO_CMD_SUPPORT_NO_LIMIT},
        {DCMI_MAIN_CMD_SILS, DCMI_GET_DEVICE_INFO_CMD_SUPPORT_NO_LIMIT},
        {DCMI_MAIN_CMD_HCCS, DCMI_GET_DEVICE_INFO_CMD_SUPPORT_NO_LIMIT},
        {DCMI_MAIN_CMD_HOST_AICPU, DCMI_GET_DEVICE_INFO_CMD_SUPPORT_NO_LIMIT},
        {DCMI_MAIN_CMD_TEMP, DCMI_GET_DEVICE_INFO_CMD_SUPPORT_NO_LIMIT},
        {DCMI_MAIN_CMD_SVM, DCMI_GET_DEVICE_INFO_CMD_SUPPORT_ONLY_ROOT},
        {DCMI_MAIN_CMD_VDEV_MNG, DCMI_GET_DEVICE_INFO_CMD_SUPPORT_ROOT_DOCKER_VM},
        {DCMI_MAIN_CMD_SEC, DCMI_GET_DEVICE_INFO_CMD_SUPPORT_ONLY_ROOT},
        {DCMI_MAIN_CMD_EX_COMPUTING, DCMI_GET_DEVICE_INFO_CMD_SUPPORT_NO_LIMIT},
        {DCMI_MAIN_CMD_DEVICE_SHARE, DCMI_GET_DEVICE_INFO_CMD_SUPPORT_NO_LIMIT},
        {DCMI_MAIN_CMD_SIO, DCMI_GET_DEVICE_INFO_CMD_SUPPORT_NO_LIMIT},
        {DCMI_MAIN_CMD_EX_CERT, DCMI_GET_DEVICE_INFO_CMD_SUPPORT_NO_LIMIT},
        {DCMI_MAIN_CMD_PCIE, DCMI_GET_DEVICE_INFO_CMD_SUPPORT_NO_LIMIT},
    };

    table_size = sizeof(cmd_permission_table) / sizeof(cmd_permission_table[0]);
    for (index = 0; index < table_size; index++) {
        if (main_cmd == cmd_permission_table[index].main_cmd) {
            *cmd_permission = cmd_permission_table[index].cmd_permission;
            return DCMI_OK;
        }
    }

    return DCMI_ERR_CODE_INVALID_PARAMETER;
}

int dcmi_device_info_is_support(int cmd_permission)
{
    if (cmd_permission == DCMI_GET_DEVICE_INFO_CMD_SUPPORT_ONLY_ROOT) {
        if (!dcmi_is_in_phy_machine_root()) {
            gplog(LOG_OP, "Operation not permitted, only root user on physical machine can call this api.");
            return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
        }
    }
    if (cmd_permission == DCMI_GET_DEVICE_INFO_CMD_SUPPORT_ROOT_DOCKER_VM) {
        if (dcmi_check_run_not_root() && !dcmi_check_run_in_vm() &&
            !dcmi_check_run_in_docker()) {
            gplog(LOG_OP, "Operation not permitted, only root user can call this api.");
            return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
            }
    }
    if (cmd_permission == DCMI_GET_DEVICE_INFO_CMD_SUPPORT_PHY_MACHINE) {
        if (!dcmi_is_in_phy_machine()) {
            gplog(LOG_OP, "Operation not permitted, only on physical machine can call this api.");
            return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
        }
    }

    return DCMI_OK;
}

STATIC int dcmi_check_hccs_cmd_permission(unsigned int main_cmd, unsigned int sub_cmd)
{
    if ((main_cmd == DCMI_MAIN_CMD_HCCS) && (dcmi_board_chip_type_is_ascend_310p()) &&
        (!dcmi_board_type_is_a300i_duo())) {
        gplog(LOG_ERR, "This device does not support getting hccs lane info.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
    return DCMI_OK;
}

STATIC int dcmi_check_custom_op_cmd_permission(unsigned int main_cmd, unsigned int sub_cmd)
{
    if ((main_cmd == DCMI_MAIN_CMD_SOC_INFO && sub_cmd == DCMI_SOC_INFO_SUB_CMD_CUSTOM_OP) &&
        dcmi_board_chip_type_is_ascend_910_93() != TRUE) {
        // AI CPU 算子免权签功能暂时仅支持A3
        gplog(LOG_OP, "This device does not support setting custom-op status.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
    return DCMI_OK;
}

STATIC int dcmi_check_lp_sub_cmd_permission(unsigned int main_cmd, unsigned int sub_cmd)
{
    if ((dcmi_board_chip_type_is_ascend_910b() || dcmi_board_chip_type_is_ascend_310p()) &&
        (main_cmd == DCMI_MAIN_CMD_LP)) {
        if ((sub_cmd == DCMI_LP_SUB_CMD_SET_STRESS_TEST) || (sub_cmd == DCMI_LP_SUB_CMD_GET_AIC_CPM) ||
            (sub_cmd == DCMI_LP_SUB_CMD_GET_BUS_CPM) || (sub_cmd >= DCMI_LP_SUB_CMD_INNER_MAX)) {
            gplog(LOG_ERR, "sub_cmd is not support. main_cmd=%u, sub_cmd=%u", main_cmd, sub_cmd);
            return DCMI_ERR_CODE_NOT_SUPPORT;
        }
    }
    return DCMI_OK;
}

static int dcmi_verison_compat_cmp(const char *start_ver, const char *end_ver, const char *cmp_ver,
    enum dcmi_device_compat *compatibility)
{
    int ret = 0;
    char cmp[MAX_VER_LEN] = {0};
    unsigned int len;

    if ((start_ver == NULL) || (end_ver == NULL) || (cmp_ver == NULL) || (compatibility == NULL)) {
        gplog(LOG_ERR, "input para is NULL.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    len = (strlen(start_ver) > strlen(end_ver)) ? strlen(start_ver) : strlen(end_ver);

    ret = strncpy_s(cmp, MAX_VER_LEN, cmp_ver, len);
    if (ret != EOK) {
        gplog(LOG_ERR, "strncpy_s failed. err is %d", ret);
        return DCMI_ERR_CODE_SECURE_FUN_FAIL;
    }

    if (strcmp(cmp, start_ver) >= 0 &&  strcmp(cmp, end_ver) <= 0) {
        *compatibility = DCMI_COMPAT_OK;
        return DCMI_OK;
    }
    *compatibility = DCMI_COMPAT_NOK;
    return DCMI_OK;
}

int dcmi_judge_compatibility(unsigned char* version, int ver_len, unsigned char* compat_list, int list_len,
    enum dcmi_device_compat *compatibility)
{
    int ret = 0;
    unsigned char start_ver[MAX_VER_LEN] = {0};
    unsigned char end_ver[MAX_VER_LEN] = {0};
    unsigned char tmp_compat_list[COMPAT_ITEM_SIZE_MAX] = {0};
    char seps[]   = "],[";
    char *context = NULL;
    char *token = NULL;

    if ((version == NULL) || (compat_list == NULL) || (compatibility == NULL)) {
        gplog(LOG_ERR, "The input parameters of the function[dcmi_judge_compatibility] are incorrect.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    *compatibility = DCMI_COMPAT_NOK;  // 初始化为不兼容

    // parse version list
    ret = strcpy_s((char*)tmp_compat_list, COMPAT_ITEM_SIZE_MAX, (const char*)compat_list);
    if (ret != EOK) {
        gplog(LOG_ERR, "strcpy_s failed. err is %d", ret);
        return DCMI_ERR_CODE_SECURE_FUN_FAIL;
    }
    token = strtok_s((char*)tmp_compat_list, seps, &context);
    while (token != NULL) {
        ret = strcpy_s((char*)start_ver, MAX_VER_LEN, token);
        if (ret != EOK) {
            gplog(LOG_ERR, "strcpy_s failed. err is %d", ret);
            return DCMI_ERR_CODE_SECURE_FUN_FAIL;
        }
        token = strtok_s(NULL, seps, &context);
        if (token != NULL) {
            ret = strcpy_s((char*)end_ver, MAX_VER_LEN, token);
            if (ret != EOK) {
                gplog(LOG_ERR, "strcpy_s failed. err is %d", ret);
                return DCMI_ERR_CODE_SECURE_FUN_FAIL;
            }
            token = strtok_s(NULL, seps, &context);
            // 兼容性判断
            ret = dcmi_verison_compat_cmp((const char*)start_ver, (const char*)end_ver,
                                          (const char*)version, compatibility);
            if (ret != DCMI_OK) {
                gplog(LOG_ERR, "dcmi_verison_compat_cmp failed. err is %d", ret);
                return ret;
            }
            if (*compatibility == DCMI_COMPAT_OK) {
                return DCMI_OK;
            }
        }
    }
    return DCMI_OK;
}

int dcmi_check_user_config_parameter(const char *config_name, unsigned int buf_size, unsigned char *buf)
{
    if (config_name == NULL) {
        gplog(LOG_ERR, "input para config_name is NULL.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }
    if (buf_size == 0) {
        gplog(LOG_ERR, "input para buf_size is invalid. buf_size=%u.", buf_size);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }
    if (buf == NULL) {
        gplog(LOG_ERR, "input para buf is NULL.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }
    return DCMI_OK;
}

// A2/A3热复位容器权限检查
int dcmi_check_a2_a3_device_reset_docker_permission()
{
    int ret;
    unsigned int env_flag = ENV_PHYSICAL;

    ret = dcmi_get_environment_flag(&env_flag);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_environment_flag failed. err is %d.", ret);
        return ret;
    }

    // 910A3，仅支持物理机特权容器，虚机不支持
    if (dcmi_board_chip_type_is_ascend_910_93()) {
        if (env_flag != ENV_PHYSICAL_PRIVILEGED_CONTAINER) {
            gplog(LOG_OP, "Operation not permitted, only physical privileged containers are supported.");
            return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
        } else {
            return DCMI_OK;
        }
    }

    // 910A2，支持物理机特权容器与虚机特权容器
    if (env_flag != ENV_PHYSICAL_PRIVILEGED_CONTAINER && env_flag != ENV_VIRTUAL_PRIVILEGED_CONTAINER) {
        gplog(LOG_OP, "Operation not permitted, only privileged containers are supported.");
        return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
    }

    return DCMI_OK;
}

STATIC int dcmi_check_user(unsigned int acc_ctrl)
{
    unsigned int user_acc_ctrl = acc_ctrl & DCMI_USER_ACC_MASK;

    if (((user_acc_ctrl & DCMI_ACC_ROOT) != 0) && dcmi_check_run_not_root()) {
        gplog(LOG_ERR, "Operation not permitted, only root user can call this api.");
        return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
    }

    return DCMI_OK;
}

STATIC int dcmi_check_env(unsigned int acc_ctrl)
{
    unsigned int env_acc_ctrl = acc_ctrl & DCMI_RUN_ENV_MASK;

    if (((env_acc_ctrl & DCMI_ENV_PHYSICAL) == 0) &&
        (!dcmi_check_run_in_vm() && !dcmi_check_run_in_docker())) {
        goto env_not_permitted;
    }

    if (((env_acc_ctrl & DCMI_ENV_VIRTUAL) == 0) && dcmi_check_run_in_vm()) {
        goto env_not_permitted;
    }

    if (((env_acc_ctrl & DCMI_ENV_DOCKER) == 0) &&
        (dcmi_check_run_in_docker() && !dcmi_check_run_in_privileged_docker())) {
        goto env_not_permitted;
    }

    if (((env_acc_ctrl & DCMI_ENV_ADMIN_DOCKER) == 0) && (dcmi_check_run_in_privileged_docker())) {
        goto env_not_permitted;
    }

    return DCMI_OK;

env_not_permitted:
    gplog(LOG_ERR, "Operation not permitted, current environment can't call this api.");
    return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
}

int dcmi_acc_ctrl_check(unsigned int acc_ctrl)
{
    int ret;

    ret= dcmi_check_env(acc_ctrl);
    if (ret != DCMI_OK) {
        return ret;
    }

    ret = dcmi_check_user(acc_ctrl);
    if (ret != DCMI_OK) {
        return ret;
    }

    return DCMI_OK;
}

STATIC int dcmi_ckeck_chip_inf_sub_set_cmd_product(unsigned int main_cmd, unsigned int sub_cmd)
{
    switch (sub_cmd) {
        case DSMI_CHIP_INF_SUB_CMD_SPOD_NODE_STATUS:
            if (!dcmi_board_chip_type_is_ascend_910_93()) {
                return DCMI_ERR_CODE_NOT_SUPPORT;
            }
            break;
        default:
            break;
    }

    return DCMI_OK;
}

int dcmi_set_device_info_permission_check(unsigned int main_cmd, unsigned int sub_cmd)
{
    size_t index, table_size;

    static struct dcmi_set_device_main_cmd_table cmd_permission_table[] = {
        {DCMI_MAIN_CMD_TS, DCMI_TS_SUB_CMD_SET_FAULT_MASK, DCMI_ENV_ALL},
        {DCMI_MAIN_CMD_CHIP_INF, DCMI_CHIP_INF_SUB_CMD_SPOD_NODE_STATUS, DCMI_ACC_ROOT | DCMI_ENV_NOT_NORMAL_DOCKER},
    };

    table_size = sizeof(cmd_permission_table) / sizeof(cmd_permission_table[0]);
    for (index = 0; index < table_size; ++index) {
        if ((main_cmd == cmd_permission_table[index].main_cmd) &&
            (sub_cmd == cmd_permission_table[index].sub_cmd)) {
            return dcmi_acc_ctrl_check(cmd_permission_table[index].acc_ctrl);
        }
    }

    /* default: only support root on phy machine */
    if (!dcmi_is_in_phy_machine_root()) {
        gplog(LOG_ERR, "Operation not permitted, only root user on physical machine can call this api.");
        return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
    }

    return DCMI_OK;
}

int dcmi_cmd_product_support_check(unsigned int main_cmd, unsigned int sub_cmd)
{
    size_t index, table_size;

    static struct dcmi_device_info_main_cmd_product_table cmd_product_table[] = {
        {DCMI_MAIN_CMD_HCCS, dcmi_check_hccs_cmd_permission},
        {DCMI_MAIN_CMD_SOC_INFO, dcmi_check_custom_op_cmd_permission},
        {DCMI_MAIN_CMD_LP, dcmi_check_lp_sub_cmd_permission},
        {DCMI_MAIN_CMD_CHIP_INF, dcmi_ckeck_chip_inf_sub_set_cmd_product},
    };

    table_size = sizeof(cmd_product_table) / sizeof(cmd_product_table[0]);
    for (index = 0; index < table_size; ++index) {
        if ((main_cmd == cmd_product_table[index].main_cmd) &&
            (cmd_product_table[index].cmd_produc_check_func != NULL)) {
            return cmd_product_table[index].cmd_produc_check_func(main_cmd, sub_cmd);
        }
    }

    return DCMI_OK;
}