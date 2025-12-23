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
#ifndef _WIN32
#include <unistd.h>
#include <pwd.h>
#include <dirent.h>
#else
#include <io.h>
#include <process.h>
#endif
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <limits.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include<ctype.h>
#include<stdbool.h>

#include "securec.h"
#include "ioctl_comm_def.h"
#include "lingqu-dcmi-log.h"

#ifndef STATIC_SKIP
    #define STATIC static
#else
    #define STATIC
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#ifndef NULL
#define NULL ((void *)0)
#endif

#define TIMESTAMP_STR_SIZE 32
#define USER_NAME_MAX_LEN 256
#define USER_IP_MAX_LEN 20
#define DCMI_LOG_MAX_LEN 4096 /* 一条日志的最大长度 */
#define DCMI_LOGFILE_MAX_LEN (4 * 1024 * 1024)
#define DCMI_LOG_BASE_TIME 1900
#define DCMI_CFG_LINE_MAX_LEN 1024
#define DCMI_LOG_DIR_MAX_LEN 128

void lq_dcmi_log_record(const char *log_file, const char *log_file_bak, const char *log_buf);

int lq_dcmi_file_realpath_allow_nonexist(const char *file, char *path, int path_len)
{
    int check_result;
    (void)path_len;
#ifndef _WIN32
    check_result = ((file == NULL) || (strlen(file) > PATH_MAX) ||
                    ((realpath(file, path) == NULL) && (errno != ENOENT)));
#else
    check_result = ((file == NULL) || (strlen(file) > MAX_PATH) ||
                    ((PathCanonicalizeA(path, file) == FALSE) && (errno != ENOENT)));
#endif
    if (check_result) {
        return LQ_DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    return LQ_DCMI_OK;
}

int lq_dcmi_board_type_is_sei()
{
    char info_line[DCMI_CFG_LINE_MAX_LEN] = {0};
    char *tmp_str = NULL;
    size_t read_count;
    FILE *fp = NULL;

    fp = fopen("/run/board_cfg.ini", "r");
    if (fp == NULL) {
        return 0;
    }

    if (fseek(fp, 0, SEEK_SET) != 0) {
        printf("call fseek failed.\n");
        (void)fclose(fp);
        return 0;
    }

    read_count = fread(info_line, 1, sizeof(info_line) - 1, fp);
    if (read_count <= 0) {
        printf("call fread failed.read_count=%lu\n", (unsigned long)read_count);
        (void)fclose(fp);
        return 0;
    }

    info_line[DCMI_CFG_LINE_MAX_LEN - 1] = '\0';
    tmp_str = strstr(info_line, "board_type=");
    if (tmp_str == NULL) {
        (void)fclose(fp);
        return 0;
    }

    // key要全字匹配
    if (info_line != tmp_str && *(tmp_str - 1) != '\n') {
        (void)fclose(fp);
        return 0;
    }

    (void)fclose(fp);
    if ((strncmp(tmp_str + strlen("board_type="), "hilens", strlen("hilens")) == 0) ||
        (strncmp(tmp_str + strlen("board_type="), "sei", strlen("sei")) == 0)) {
        return 1;
    } else {
        return 0;
    }
}

int lq_dcmi_get_log_dir(char *log_dir, int log_dir_size)
{
    int ret;

#ifndef _WIN32
    struct passwd *user_info = NULL;

    user_info = getpwuid(getuid());
    if ((user_info == NULL) || (user_info->pw_name[0] == '\0')) {
        return strcpy_s(log_dir, log_dir_size, COMMON_LOG_DIR);
    }

    if (user_info->pw_uid == 0) {
        if (lq_dcmi_board_type_is_sei()) {
            ret = strcpy_s(log_dir, log_dir_size, DCMI_LOG_DIR);
        } else {
            ret = strcpy_s(log_dir, log_dir_size, COMMON_LOG_DIR);
        }
    } else {
        ret = sprintf_s(log_dir, log_dir_size, "%s/var/log", user_info->pw_dir);
        if (ret <= 0) {
            return -1;
        } else {
            return 0;
        }
    }

#else
        ret = strcpy_s(log_dir, log_dir_size, COMMON_LOG_DIR);
#endif
    if (ret != EOK) {
        return -1;
    }
    return 0;
}

#ifndef _WIN32
STATIC int lq_dcmi_get_user_name_linux(char *user_name, int name_len)
{
    int ret;
    struct passwd *user_info = NULL;

    if (user_name == NULL) {
        return -1;
    }
    /* 获取userName */
    user_info = getpwuid(getuid());
    if ((user_info == NULL) || (user_info->pw_name[0] == '\0')) {
        return -1;
    }

    ret = strncpy_s(user_name, name_len, user_info->pw_name, strlen(user_info->pw_name));
    return ret;
}

// 只检查ip的字符合法性
bool check_ip_valid(const char *ip, int ip_len)
{
    int  i;

    for (i = 0; i < ip_len; i++) {
        // ipv4点分加10进制数表示，ipv6点分或冒分加16进制表示。
        if (!isxdigit(ip[i]) && (ip[i] != '.') && (ip[i] != ':')) {
            return false;
        }
    }

    return true;
}

int lq_dcmi_get_ip_linux(char *user_ip, unsigned int ip_len)
{
    int ip_length = 0;
    char *usr_ip_tmp = NULL;
    char *usr_ssh_ip_tmp = NULL;
    char *spac_pos = NULL;

    if (user_ip == NULL) {
        return -1;
    }

    /* 获取userIP */
    usr_ip_tmp = getenv("REMOTEHOST");
    if ((usr_ip_tmp != NULL) && (strlen(usr_ip_tmp) <= ip_len)) {
        if (check_ip_valid(usr_ip_tmp, (int)strlen(usr_ip_tmp))) {
            return strncpy_s(user_ip, ip_len, usr_ip_tmp, strlen(usr_ip_tmp));
        } else {
            return strncpy_s(user_ip, ip_len, "Invalid IP", strlen("Invalid IP"));
        }
    }

    usr_ssh_ip_tmp = getenv("SSH_CLIENT");
    if (usr_ssh_ip_tmp == NULL) {
        return strncpy_s(user_ip, ip_len, "127.0.0.1", strlen("127.0.0.1"));
    }

    /* 去掉SSH 环境变量中的端口，只保留IP */
    spac_pos = strstr(usr_ssh_ip_tmp, " ");
    if (spac_pos != NULL) {
        ip_length = (int)(spac_pos - usr_ssh_ip_tmp);
    }

    if ((unsigned int)ip_length > ip_len) {
        return strncpy_s(user_ip, ip_len, "Error IP", strlen("Error IP"));
    } else {
        if (check_ip_valid(usr_ssh_ip_tmp, ip_length)) {
            return strncpy_s(user_ip, ip_len, usr_ssh_ip_tmp, ip_length);
        } else {
            return strncpy_s(user_ip, ip_len, "Invalid IP", strlen("Invalid IP"));
        }
    }

    return 0;
}

STATIC int lq_dcmi_get_user_name_and_ip_linux(char *user_name, int name_len, char *user_ip, unsigned int ip_len)
{
    int ret;

    /* 获取userName */
    ret = lq_dcmi_get_user_name_linux(user_name, name_len);
    if (ret != 0) {
        return ret;
    }

    /* 获取userIP */
    ret = lq_dcmi_get_ip_linux(user_ip, ip_len);
    return ret;
}

int creat_dir(const char *filename)
{
    char path[PATH_MAX + 1] = {0};
    size_t i, len;
    int ret;

    ret = strcpy_s(path, sizeof(path), filename);
    if (ret != EOK) {
        return -1;
    }

    len = strlen(path);
    if (path[len - 1] != '/') {
        if (strcat_s(path, sizeof(path), "/") != EOK) {
            return -1;
        }
        len++;
    }

    for (i = 1; i < len; i++) {
        if (path[i] == '/') {
            path[i] = '\0';
            if ((access(path, F_OK) != 0) && (mkdir(path, S_IRWXU | S_IRWXG | S_IRWXO) < 0)) {
                return -1;
            }
            path[i] = '/';
        }
    }
    return 0;
}

#else
int creat_dir(const char *filename)
{
    int ret;
    char path[MAX_PATH + 1] = {0};
    char *p = NULL;
    int pos;
    struct _stat st;

    ret = strcpy_s(path, sizeof(path), filename);
    if (ret != EOK) {
        return -1;
    }

    p = path;
    for (;;) {
        pos = (int)strcspn(p, "/\\");
        if (pos == (int)strlen(p)) {
            break;
        }
        if (pos != 0) {
            p[pos] = '\0';
            if ((p[pos - 1] != ':') && (_stat(path, &st) != 0) && (_mkdir(path) != 0)) {
                return -1;
            }
        }
        p[pos] = '/';
        p += pos + 1;
    }
    return 0;
}
STATIC int lq_dcmi_get_user_name_and_ip_win(char *user_name, int name_len, char *user_ip, int ip_len)
{
#define USER_IP_FIRST_INDEX 2
#define USER_IP_SECOND_INDEX 3
#define USER_IP_THIRD_INDEX 4
#define USER_IP_FORTH_INDEX 5

    char *wuser = NULL;
    char *buf = NULL;
    DWORD user_size;
    DWORD size = 0;
    DWORD last_err;
    int ret;

    if (!WTSQuerySessionInformation(WTS_CURRENT_SERVER_HANDLE, WTS_CURRENT_SESSION, WTSUserName, &wuser, &user_size)) {
        last_err = GetLastError();
        printf("call WTSQuerySessionInformation query WTSUserName failed, Err=%d\n", last_err);
    }
    ret = WideCharToMultiByte(CP_ACP, 0, wuser, -1, NULL, 0, NULL, FALSE);
    ret = ((ret < name_len) ? ret : name_len);
    ret = WideCharToMultiByte(CP_ACP, 0, wuser, -1, user_name, ret, NULL, FALSE);
    if (ret == FALSE) {
        last_err = GetLastError();
        printf("call MultiByteToWideChar Error: %d\n", last_err);
        WTSFreeMemory(wuser);
        return -1;
    }
    if (GetSystemMetrics(SM_REMOTESESSION)) {
        if (!WTSQuerySessionInformation(
            WTS_CURRENT_SERVER_HANDLE, WTS_CURRENT_SESSION, WTSClientAddress, &buf, &size)) {
            last_err = GetLastError();
            printf("call WTSQuerySessionInformation query WTSClientAddress failed, Err=%d\n", last_err);
            WTSFreeMemory(wuser);
            return -1;
        }
        WTS_CLIENT_ADDRESS *pAddr = (WTS_CLIENT_ADDRESS *)buf;
        sprintf_s(user_ip,
            ip_len,
            "%u.%u.%u.%u",
            pAddr->Address[USER_IP_FIRST_INDEX],
            pAddr->Address[USER_IP_SECOND_INDEX],
            pAddr->Address[USER_IP_THIRD_INDEX],
            pAddr->Address[USER_IP_FORTH_INDEX]);
        WTSFreeMemory(buf);
    } else {
        sprintf_s(user_ip, ip_len, "%s", "localhost");
    }
    WTSFreeMemory(wuser);
    return 0;
}
#endif

STATIC int lq_dcmi_get_username_and_ip(char *user_name, int name_len, char *user_ip, unsigned int ip_len)
{
#ifndef _WIN32
    return lq_dcmi_get_user_name_and_ip_linux(user_name, name_len, user_ip, ip_len);
#else
    return lq_dcmi_get_user_name_and_ip_win(user_name, name_len, user_ip, ip_len);
#endif
}

STATIC void lq_dcmi_get_curr_time_str(char *curr_time_str, int curr_time_str_size)
{
    struct tm *curr_time = NULL;
    time_t timep;
    int ret;

    (void)time(&timep);
    curr_time = localtime(&timep);
    if (curr_time == NULL) {
        return;
    }

    ret = snprintf_s(curr_time_str, curr_time_str_size, curr_time_str_size - 1, "[%04d/%02d/%02d %02d:%02d:%02d]",
        curr_time->tm_year + DCMI_LOG_BASE_TIME,
        curr_time->tm_mon + 1,
        curr_time->tm_mday,
        curr_time->tm_hour,
        curr_time->tm_min,
        curr_time->tm_sec);
    if (ret == -1) {
        return;
    }
}

STATIC void lq_dcmi_log_backup(const char *log_file, const char *log_file_bak)
{
    (void)unlink(log_file_bak);
    (void)rename(log_file, log_file_bak);
    (void)chmod(log_file_bak, S_IRUSR | S_IRGRP);
}

STATIC FILE *lq_dcmi_get_log_file_handle(const char *log_file)
{
    FILE *fp = NULL;
    int ret;

#ifndef _WIN32
    char path[PATH_MAX + 1] = {0x00};
#else
    char path[MAX_PATH + 1] = {0x00};
#endif
    ret = lq_dcmi_file_realpath_allow_nonexist(log_file, path, sizeof(path));
    if (ret != LQ_DCMI_OK) {
        gplog(LOG_ERR, "file path is illegal errno(%d).\n", errno);
        return NULL;
    }

    fp = fopen(path, "r");
    if (fp == NULL) {
        fp = fopen(path, "wb");
        if (fp == NULL) {
            return NULL;
        }
        (void)chmod(path, S_IRUSR | S_IWUSR | S_IRGRP);
    } else {
        (void)fclose(fp);
        (void)chmod(path, S_IRUSR | S_IWUSR | S_IRGRP);
        fp = fopen(path, "ab");
        if (fp == NULL) {
            return NULL;
        }
    }
    return fp;
}

STATIC int lq_dcmi_get_log_file(const char *log_dir, const char *mod, char *log_file, int log_file_len)
{
    int ret = sprintf_s(log_file, log_file_len, "%s/%s%s%s", log_dir, COMMON_LOG_PREFIX, mod, COMMON_LOG_SUFFIX);
    if (ret <= 0) {
        return -1;
    }
    return 0;
}

STATIC int lq_dcmi_get_log_file_bak(const char *log_dir, const char *mod, char *log_file_bak, int log_file_bak_len)
{
    int ret;

#ifndef _WIN32
    const char *log_prefix = (lq_dcmi_board_type_is_sei()) ? "ies_" : COMMON_LOG_PREFIX;
#else
    const char *log_prefix = COMMON_LOG_PREFIX;
#endif

    ret = sprintf_s(log_file_bak, log_file_bak_len, "%s/%s%s%s", log_dir, log_prefix, mod, COMMON_BAKLOG_SUFFIX);
    if (ret <= 0) {
        return -1;
    }
    return 0;
}

int lq_dcmi_chmod_log_file(const char *log_dir)
{
    struct passwd *user_info = NULL;
    user_info = getpwuid(getuid());
    if (user_info == NULL) {
        return -1;
    }
    if (user_info->pw_uid != 0) {
        (void)chmod(log_dir, S_IRWXU | S_IRGRP | S_IXGRP);
    }
    return 0;
}

void lq_dcmi_log_fun(const char *mod, const char *fmt, ...)
{
    char log_dir[DCMI_LOG_DIR_MAX_LEN] = {0};
#ifndef _WIN32
    char log_file[PATH_MAX + 1] = {0};
    char log_file_bak[PATH_MAX + 1] = {0};
#else
    char log_file[MAX_PATH + 1] = {0};
    char log_file_bak[MAX_PATH + 1] = {0};
#endif
    char log_buf[DCMI_LOG_MAX_LEN] = {0};
    va_list var_list;
    int log_len, ret = 0;

    if (fmt == NULL) {
        return;
    }

    ret = lq_dcmi_get_log_dir(log_dir, sizeof(log_dir));
    if (ret != 0) {
        return;
    }

#ifdef _WIN32
    struct _stat st;
    if (_stat(log_dir, &st) != 0) {
#else
    struct stat st;
    if (stat(log_dir, &st) != 0) {
#endif
        if (creat_dir(log_dir) != 0) {
            return;
        }
    }

    if (lq_dcmi_chmod_log_file((const char *)log_dir) != 0) {
        return;
    }

    if (lq_dcmi_get_log_file((const char *)log_dir, mod, log_file, sizeof(log_file)) != 0) {
        return;
    }
    if (lq_dcmi_get_log_file_bak((const char *)log_dir, mod, log_file_bak, sizeof(log_file_bak)) != 0) {
        return;
    }

    va_start(var_list, fmt);
    log_len = vsnprintf_s(log_buf, DCMI_LOG_MAX_LEN, DCMI_LOG_MAX_LEN - 1, fmt, var_list);
    va_end(var_list);
    if (log_len < 0) {
        return;
    }

    if (log_len >= DCMI_LOG_MAX_LEN) {
        log_buf[DCMI_LOG_MAX_LEN - 1] = '\0';
    }
    lq_dcmi_log_record(log_file, log_file_bak, (const char *)log_buf);
}

void lq_dcmi_log_record(const char *log_file, const char *log_file_bak, const char *log_buf)
{
    FILE *fp = NULL;
    size_t log_len;
    pid_t tid;
    char user_name[USER_NAME_MAX_LEN + 1] = {0};
    char user_ip[USER_IP_MAX_LEN + 1] = {0};
    char timestamp_str[TIMESTAMP_STR_SIZE] = {0};

    if ((log_file == NULL) || (log_file_bak == NULL)) {
        return;
    }

    fp = lq_dcmi_get_log_file_handle(log_file);
    if (fp == NULL) {
        return;
    }

    log_len = strlen(log_buf);
    if ((ftell(fp) + (long)log_len) > DCMI_LOGFILE_MAX_LEN) {
        (void)fclose(fp);
        lq_dcmi_log_backup(log_file, log_file_bak);
#ifndef _WIN32
        char path[PATH_MAX + 1] = {0x00};
#else
        char path[MAX_PATH + 1] = {0x00};
#endif
        int ret = lq_dcmi_file_realpath_allow_nonexist(log_file, path, sizeof(path));
        if (ret != LQ_DCMI_OK) {
            gplog(LOG_ERR, "file path is illegal errno(%d).\n", errno);
            return;
        }

        fp = fopen(path, "a");
        if (fp == NULL) {
            return;
        }
    }

    lq_dcmi_get_curr_time_str(timestamp_str, sizeof(timestamp_str));

#ifndef DT_FLAG
    tid = syscall(__NR_gettid);
    (void)lq_dcmi_get_username_and_ip(user_name, USER_NAME_MAX_LEN, user_ip, USER_IP_MAX_LEN);
    (void)fprintf(fp, "%s[%04d][%s][%s]%s", timestamp_str, tid, user_name, user_ip, log_buf);
#endif

    (void)fflush(fp);
    (void)fclose(fp);
    return;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
