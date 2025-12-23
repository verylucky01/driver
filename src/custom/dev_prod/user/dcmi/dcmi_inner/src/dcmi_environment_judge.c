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
#include <limits.h>
#include <errno.h>
#include <ctype.h>
#include <fcntl.h>
#include <stdbool.h>
#ifndef _WIN32
#include <sys/wait.h>
#endif
#include "securec.h"
#include "dcmi_interface_api.h"
#include "dcmi_os_adapter.h"
#include "dcmi_log.h"
#include "dcmi_product_judge.h"
#include "dcmi_inner_info_get.h"
#include "dcmi_environment_judge.h"

struct dcmi_run_env g_run_env = {0};

int devdrv_get_host_phy_mach_flag(unsigned int dev_id, unsigned int *host_flag);
int dmanage_get_container_flag(unsigned int *container_flag);

int dcmi_get_environment_flag(unsigned int *env_flag)
{
    unsigned int host_flag;
    unsigned int plain_container_flag;
    int i;
    int ret;
    int device_logic_id = 0;
    int card_count = 0;
    int card_id_list[NPU_MAX_COUNT] = {0};

    if (env_flag == NULL) {
        gplog(LOG_ERR, "env_flag is NULL.\n");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    ret = dcmi_get_card_num_list(&card_count, card_id_list, NPU_MAX_COUNT);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "get card num list fail. ret is %d\n", ret);
        return ret;
    }

    for (i = 0; i < card_count; i++) {
        ret = dcmi_get_device_logic_id(&device_logic_id, card_id_list[i], 0);
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "get device logic id fail. ret is %d\n", ret);
            continue;
        }

        ret = devdrv_get_host_phy_mach_flag(device_logic_id, &host_flag);
        if (ret == DCMI_OK) {
            break;
        }
        gplog(LOG_INFO, "get host phy mach flag fail. ret is %d\n", ret);
    }
    if (ret != DCMI_OK) {
        return DCMI_ERR_CODE_INNER_ERR;
    }

    ret = dmanage_get_container_flag(&plain_container_flag);
    if (ret != 0) {
        gplog(LOG_ERR, "dmanage_get_container_flag call error. err is %d\n", ret);
        return DCMI_ERR_CODE_INNER_ERR;
    }

    ret = dcmi_determine_environment_flag(host_flag, plain_container_flag, dcmi_check_run_in_docker(), env_flag);
    if (ret != 0) {
        gplog(LOG_ERR, "dcmi_determine_environment_flag call error. err is %d\n", ret);
        return ret;
    }

    return DCMI_OK;
}

int dcmi_determine_environment_flag(unsigned int host_flag, unsigned int plain_container_flag,
    unsigned int container_flag, unsigned int *env_flag)
{
    if (env_flag == NULL) {
        gplog(LOG_ERR, "env_flag is NULL.\n");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if (host_flag == DCMI_HOST_PHY_MACH_FLAG) {
        if (plain_container_flag == IS_PLAIN_CONTAINER) {
            *env_flag = ENV_PHYSICAL_PLAIN_CONTAINER;       // 物理机普通容器
        } else if (container_flag == FALSE) {
            *env_flag = ENV_PHYSICAL;                       // 物理机
        } else {
            *env_flag = ENV_PHYSICAL_PRIVILEGED_CONTAINER;  // 物理机特权容器
        }
    } else {
        if (plain_container_flag == IS_PLAIN_CONTAINER) {
            *env_flag = ENV_VIRTUAL_PLAIN_CONTAINER;        // 虚拟机普通容器
        } else if (container_flag == FALSE) {
            *env_flag = ENV_VIRTUAL;                        // 虚拟机
        } else {
            *env_flag = ENV_VIRTUAL_PRIVILEGED_CONTAINER;   // 虚拟机特权容器
        }
    }

    return DCMI_OK;
}

int dcmi_check_vnpu_in_docker_or_virtual(int card_id)
{
    int ret;
    unsigned int env_flag;
    struct dcmi_chip_info_v2 chip_info = { { 0 } };
    if (dcmi_board_chip_type_is_ascend_910b()) {
        ret = dcmi_get_npu_chip_info(card_id, 0, &chip_info);
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "call dcmi_get_npu_chip_info failed. ret is %d.", ret);
            return ret;
        }
        ret = dcmi_get_environment_flag(&env_flag);
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "get environment flag failed. ret is %d", ret);
            return ret;
        }
        // 算力切分虚拟机和容器下，获取不到mcu信息，返回DCMI_OK

        bool vnpu_container_orvirtual_env = (env_flag == ENV_VIRTUAL ||
            env_flag == ENV_PHYSICAL_PLAIN_CONTAINER || env_flag == ENV_VIRTUAL_PLAIN_CONTAINER);
        if (strstr((char*)chip_info.chip_name, "vir") && vnpu_container_orvirtual_env) {
            return TRUE;
        }
    }
    return FALSE;
}

int dcmi_is_not_root_user()
{
#ifndef _WIN32
    if (geteuid() != 0) {
        return TRUE;
    }
#endif
    return FALSE;
}

int dcmi_is_in_virtual_machine(void)
{
#ifndef _WIN32
    int ret;
    ret = system("/usr/sbin/dmidecode 2>&1 | grep -E \"xen|Xen|VMware|OpenStack|KVM Virtual Machine\" > /dev/null");
    if (ret != -1) {
        if (WIFEXITED(ret) && (WEXITSTATUS(ret) == 0)) {
            return TRUE;
        }
    }

    ret = system("/usr/sbin/dmidecode 2>&1 | grep -E \"Manufacturer: QEMU|Manufacturer: qemu\" > /dev/null");
    if (ret != -1) {
        if (WIFEXITED(ret) && (WEXITSTATUS(ret) == 0)) {
            return TRUE;
        }
    }

    ret = system("/usr/bin/systemd-detect-virt -v > /dev/null  2>&1");
    if (ret == 0) {
        return TRUE;
    }
#endif
    return FALSE;
}

#ifndef _WIN32
void dcmi_strlwr(char *str, int len)
{
    int i;
    char *ptr = str;

    if (ptr == NULL || len <= 0) {
        return;
    }

    for (i = 0; (*ptr != 0) && (i < len); ptr++, i++) {
        *ptr = tolower(*ptr);
    }
    return;
}
#endif

int dcmi_is_in_docker_by_cgroup_file()
{
    FILE *fp = NULL;
    char msg_info[DCMI_BUFFER_SIZE] = {0};
    char *find_flag_a = NULL;
    char *find_flag_b = NULL;
    int is_in_docker = FALSE;

    fp = fopen("/proc/self/cgroup", "r");
    if (fp == NULL) {
        gplog(LOG_ERR, "dcmi_is_in_docker call open failed.");
        return FALSE;
    }

    while (!feof(fp)) {
        if (fgets(msg_info, DCMI_BUFFER_SIZE, fp) == NULL) {
            break;
        }
        dcmi_strlwr(msg_info, strnlen(msg_info, DCMI_BUFFER_SIZE));
        // docker- docker/ 两种字符段匹配时说明在docker中
        find_flag_a = strstr(msg_info, "docker-");
        find_flag_b = strstr(msg_info, "docker/");
        if (find_flag_a != NULL || find_flag_b != NULL) {
            is_in_docker = TRUE;
            break;
        }
    }

    (void)fclose(fp);
    return is_in_docker;
}

int dcmi_is_in_docker_by_dockerenv_file()
{
    int is_in_docker = FALSE;

    if (access(DCMI_DOCKERENV_FILE, F_OK) == 0) {
        is_in_docker = TRUE;
    }
    return is_in_docker;
}

int dcmi_is_in_docker_by_file()
{
    return (dcmi_is_in_docker_by_cgroup_file() || dcmi_is_in_docker_by_dockerenv_file());
}

int dcmi_is_in_docker_by_cmd()
{
    int ret;
    char *cmd_str[] = {"/usr/bin/systemd-detect-virt", "-c", NULL};

    ret = dcmi_safe_exec_syscmd_without_output(cmd_str);
    if (ret == 0) {
        return TRUE;
    }

    ret = system("/bin/mount 2>&1 | grep -Ei '\\s+/\\s+' | \
        grep -E \"isulad|docker|containers|containerd\" > /dev/null");
    if (ret == 0) {
        return TRUE;
    }

    ret = system("/usr/bin/mount 2>&1 | grep -Ei '\\s+/\\s+' | \
        grep -E \"isulad|docker|containers|containerd\" > /dev/null");
    if (ret == 0) {
        return TRUE;
    }

    return FALSE;
}

int dcmi_is_in_docker()
{
#ifndef _WIN32
    if (dcmi_is_in_docker_by_file() == TRUE) {
        return TRUE;
    }

    if (dcmi_is_in_docker_by_cmd() == TRUE) {
        return TRUE;
    }

    return FALSE;
#else
    return FALSE;
#endif
}

bool dcmi_check_run_in_privileged_docker(void)
{
    unsigned int env_flag = 0;
    int ret;

    ret = dcmi_get_environment_flag(&env_flag);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "Failed to get environment flag. ret is %d.", ret);
        return FALSE;
    }

    if ((env_flag != ENV_PHYSICAL_PRIVILEGED_CONTAINER) && (env_flag != ENV_VIRTUAL_PRIVILEGED_CONTAINER)) {
        return FALSE;
    }

    return TRUE;
}

int dcmi_get_run_env_init_flag(void)
{
    return g_run_env.init_flag;
}

void dcmi_set_init_flag(int init_flag)
{
    g_run_env.init_flag = init_flag;
}

void dcmi_set_env_value(int is_not_root, int is_in_vm, int is_in_docker)
{
    g_run_env.is_not_root = is_not_root;
    g_run_env.is_in_vm = is_in_vm;
    g_run_env.is_in_docker = is_in_docker;
}

int dcmi_check_run_not_root()
{
    return g_run_env.is_not_root;
}

int dcmi_check_run_in_vm(void)
{
    return g_run_env.is_in_vm;
}

int dcmi_check_run_in_docker(void)
{
    return g_run_env.is_in_docker;
}

bool dcmi_is_in_phy_machine_root(void)
{
    return (!(dcmi_check_run_not_root() || dcmi_check_run_in_vm() || dcmi_check_run_in_docker()));
}

bool dcmi_is_in_vm_root(void)
{
    return (dcmi_is_in_virtual_machine() && (!dcmi_is_not_root_user()));
}

bool dcmi_is_in_phy_machine(void)
{
    return (!(dcmi_check_run_in_vm() || dcmi_check_run_in_docker()));
}

bool dcmi_is_in_docker_root(void)
{
    return (dcmi_check_run_in_docker() && (!dcmi_is_not_root_user()));
}

bool dcmi_is_in_phy_privileged_docker_root(void)
{
    return (dcmi_check_run_in_privileged_docker() && (!dcmi_is_not_root_user()) && (!dcmi_check_run_in_vm()));
}

bool dcmi_is_in_privileged_docker_root(void)
{
    return (dcmi_check_run_in_privileged_docker() && (!dcmi_is_not_root_user()));
}

bool dcmi_is_arm(void)
{
#ifdef ARM
    return true;
#endif
    return false;
}

bool dcmi_is_x86(void)
{
#ifdef X86
    return true;
#endif
    return false;
}