/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "hccn_comm.h"
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pwd.h>
#include <unistd.h>
#include "securec.h"
#include "user_log.h"

STATIC char g_hccn_usr_name[HCCN_USER_NAME_LEN] = {0};
STATIC char g_hccn_usr_ip[HCCN_USER_IP_LEN] = {0};

char *hccn_get_g_usr_name(void)
{
    return g_hccn_usr_name;
}

char *hccn_get_g_usr_ip(void)
{
    return g_hccn_usr_ip;
}

int hccn_check_usr_identify()
{
    int ret, ret_val;
    struct passwd *pwd = getpwuid(getuid());

    if (pwd == NULL) {
        roce_err("pwd is NULL! getpwuid fail.");
        return -EINVAL;
    }
#ifndef CONFIG_LLT
    if (pwd->pw_name == NULL) {
        roce_err("pwd->pw_name is NULL, errno:%d", errno);
        ret = -EINVAL;
        goto out;
    }

    if (strncmp(pwd->pw_name, HCCN_CHECK_USER_IS_ROOT, strlen(HCCN_CHECK_USER_IS_ROOT) + 1)) {
        roce_err("only root user is legal, current user is:%s ", pwd->pw_name);
        ret = -EACCES;
        goto out;
    }
#endif
    ret = 0;
out:
    ret_val = memset_s(pwd, sizeof(struct passwd), 0, sizeof(struct passwd));
    if (ret_val) {
        roce_err("memset error, ret_val[%d]", ret_val);
        ret = -ENOMEM;
    }

    return ret;
}

int hccn_get_usr_name()
{
    int ret, ret_val;
    struct passwd *pwd = getpwuid(getuid());

    if (pwd == NULL) {
        roce_err("pwd is NULL! getpwuid fail.");
        return -EINVAL;
    }

    if (pwd->pw_name == NULL) {
        roce_err("pwd->pw_name is NULL, errno:%d", errno);
        ret = -EINVAL;
        goto out;
    }

    ret = strncpy_s(g_hccn_usr_name, HCCN_USER_NAME_LEN, pwd->pw_name, strlen(pwd->pw_name));
    if (ret) {
        roce_err("strncpy_s user name failed, ret[%d]", ret);
        goto out;
    }

    ret = 0;
out:
    ret_val = memset_s(pwd, sizeof(struct passwd), 0, sizeof(struct passwd));
    if (ret_val) {
        roce_err("memset error, ret_val[%d]", ret_val);
        ret = ret_val;
    }

    return ret;
}

int hccn_get_usr_ip(void)
{
    int ret;
    struct in_addr addr;
    char *spac_pos = NULL;
    char *usr_ssh_ip = NULL;
    int ip_len = HCCN_USER_IP_LEN + 1;

    usr_ssh_ip = getenv("SSH_CLIENT");
    if (usr_ssh_ip == NULL) {
        return strncpy_s(g_hccn_usr_ip, HCCN_USER_IP_LEN, "127.0.0.1", strlen("127.0.0.1"));
    }

    spac_pos = strchr(usr_ssh_ip, ' ');
    if (spac_pos != NULL) {
        ip_len = (int)(spac_pos - usr_ssh_ip);
    }

    if (ip_len > HCCN_USER_IP_LEN) {
        return strncpy_s(g_hccn_usr_ip, HCCN_USER_IP_LEN, "Error IP", strlen("Error IP"));
    }

    ret = strncpy_s(g_hccn_usr_ip, HCCN_USER_IP_LEN, usr_ssh_ip, ip_len);
    if (ret) {
        roce_err("strncpy_s g_hccn_usr_ip name failed, ret[%d]", ret);
        goto out;
    }
    ret = inet_pton(AF_INET, g_hccn_usr_ip, &addr);
    if (ret <= 0) {
        roce_err("inet_pton g_hccn_usr_ip failed, ret[%d]", ret);
        goto out;
    }
    return 0;

out:
    memset_s(g_hccn_usr_ip, HCCN_USER_IP_LEN, 0, HCCN_USER_IP_LEN);
    return strncpy_s(g_hccn_usr_ip, HCCN_USER_IP_LEN, "Invalid IP", strlen("Invalid IP"));
}