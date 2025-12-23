/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef __DCMI_ENVIRONMENT_JUDGE_H__
#define __DCMI_ENVIRONMENT_JUDGE_H__
#include <stdbool.h>
#include "dcmi_common.h"

#define NPU_MAX_COUNT 64

#define DCMI_HOST_PHY_MACH_FLAG           0x5a6b7c8d    /* host physical machine flag */
#define IS_PLAIN_CONTAINER      1
#define IS_NOT_PLAIN_CONTAINER  0

#define DCMI_DOCKERENV_FILE    "/.dockerenv"

#define DCMI_BUFFER_SIZE            1024

struct dcmi_run_env {
    int init_flag;
    int is_not_root;
    int is_in_vm;
    int is_in_docker;
};


int dcmi_get_environment_flag(unsigned int *env_flag);

int dcmi_determine_environment_flag(unsigned int host_flag, unsigned int plain_container_flag,
    unsigned int container_flag, unsigned int *env_flag);

int dcmi_check_vnpu_in_docker_or_virtual(int card_id);

int dcmi_is_not_root_user();

int dcmi_is_in_virtual_machine(void);

bool dcmi_check_run_in_privileged_docker(void);

int dcmi_get_run_env_init_flag(void);

int dcmi_check_run_not_root();

int dcmi_check_run_in_vm(void);

int dcmi_check_run_in_docker(void);

bool dcmi_is_in_phy_machine_root(void);

bool dcmi_is_in_phy_machine(void);

bool dcmi_is_in_vm_root(void);

bool dcmi_is_in_docker_root(void);

bool dcmi_is_in_phy_privileged_docker_root(void);

bool dcmi_is_in_privileged_docker_root(void);

bool dcmi_is_arm(void);

bool dcmi_is_x86(void);

int dcmi_safe_exec_syscmd_without_output(char *cmdstring[]);

int dcmi_is_in_docker();

void dcmi_set_init_flag(int init_flag);

void dcmi_set_env_value(int is_not_root, int is_in_vm, int is_in_docker);

#ifndef _WIN32
void dcmi_strlwr(char *str, int len);
#endif
#endif