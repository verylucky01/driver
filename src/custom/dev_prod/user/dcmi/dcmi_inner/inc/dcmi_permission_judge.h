/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef __DCMI_PERMISSION_JUDGE_H__
#define __DCMI_PERMISSION_JUDGE_H__

#include <stdbool.h>
#include "dcmi_interface_api.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#ifdef DT_FLAG
#define STATIC
#else
#define STATIC static
#endif

#define DCMI_GET_DEVICE_INFO_CMD_SUPPORT_NO_LIMIT    0
#define DCMI_GET_DEVICE_INFO_CMD_SUPPORT_ONLY_ROOT   1
#define DCMI_GET_DEVICE_INFO_CMD_SUPPORT_ROOT_DOCKER_VM 2
#define DCMI_GET_DEVICE_INFO_CMD_SUPPORT_PHY_MACHINE 3

#define COMPAT_ITEM_SIZE_MAX        1024

/* user acc ctrl */
#define DCMI_USER_ACC_MASK 0X000FU
#define DCMI_ACC_ROOT 0X0001U

/* env acc ctrl */
#define DCMI_RUN_ENV_MASK 0X00F0U
#define DCMI_ENV_PHYSICAL 0X0010U
/* Support virtual */
#define DCMI_ENV_VIRTUAL 0X0020U
/* Support noraml docker */
#define DCMI_ENV_DOCKER 0X0040U
/* Support admin docker */
#define DCMI_ENV_ADMIN_DOCKER 0X0080U
#define DCMI_ENV_ALL (DCMI_ENV_PHYSICAL | DCMI_ENV_VIRTUAL | DCMI_ENV_DOCKER | DCMI_ENV_ADMIN_DOCKER)
#define DCMI_ENV_NOT_VIRTUAL (DCMI_ENV_PHYSICAL | DCMI_ENV_DOCKER | DCMI_ENV_ADMIN_DOCKER)
#define DCMI_ENV_NOT_DOCKER (DCMI_ENV_PHYSICAL | DCMI_ENV_VIRTUAL)
#define DCMI_ENV_NOT_NORMAL_DOCKER (DCMI_ENV_PHYSICAL | DCMI_ENV_VIRTUAL | DCMI_ENV_ADMIN_DOCKER)
#define DCMI_ENV_PHY_ADMIN_DOCKER (DCMI_ENV_PHYSICAL | DCMI_ENV_ADMIN_DOCKER)

struct dcmi_get_device_main_cmd_table {
    int main_cmd;
    int cmd_permission;
};

struct dcmi_set_device_main_cmd_table {
    unsigned int main_cmd;
    unsigned int sub_cmd;
    unsigned int acc_ctrl;
};

struct dcmi_device_info_main_cmd_product_table {
    unsigned int main_cmd;
    int (*cmd_produc_check_func)(unsigned int main_cmd, unsigned int sub_cmd);
};

/* DCMI sub commond not support for Low power */
typedef enum {
    DCMI_LP_SUB_CMD_SET_STRESS_TEST = 12,    // 2024-12-30之后废弃
    DCMI_LP_SUB_CMD_GET_AIC_CPM,        // 2024-12-30之后废弃
    DCMI_LP_SUB_CMD_GET_BUS_CPM,        // 2024-12-30之后废弃
    DCMI_LP_SUB_CMD_INNER_MAX = 0x100,
    DCMI_LP_SUB_CMD_INNER_SET_STRESS_TEST,
    DCMI_LP_SUB_CMD_INNER_GET_AIC_CPM,
    DCMI_LP_SUB_CMD_INNER_GET_BUS_CPM,
    DCMI_LP_SUB_CMD_NOT_SUPPROT_MAX,
} DCMI_LP_SUB_CMD_NOT_SUPPORT;

int dcmi_get_device_info_maincmd_permission(int main_cmd, int *cmd_permission);

int dcmi_set_device_info_permission_check(unsigned int main_cmd, unsigned int sub_cmd);

int dcmi_device_info_is_support(int cmd_permission);

int dcmi_judge_compatibility(unsigned char* version, int ver_len, unsigned char* compat_list, int list_len,
    enum dcmi_device_compat *compatibility);

int dcmi_check_user_config_parameter(const char *config_name, unsigned int buf_size, unsigned char *buf);

int dcmi_check_a2_a3_device_reset_docker_permission();

int dcmi_acc_ctrl_check(unsigned int acc_ctrl);

int dcmi_cmd_product_support_check(unsigned int main_cmd, unsigned int sub_cmd);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __DCMI_PERMISSION_JUDGE_H__ */