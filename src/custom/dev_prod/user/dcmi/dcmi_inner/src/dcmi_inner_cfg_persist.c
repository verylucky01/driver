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
#include "securec.h"
#include "dcmi_log.h"
#include "dcmi_common.h"
#include "dcmi_interface_api.h"
#include "dcmi_environment_judge.h"
#include "dcmi_product_judge.h"
#include "dcmi_inner_cfg_persist.h"


int dcmi_cfg_create_lock_dir(char* path)
{
#ifndef _WIN32
    char tmp_path[PATH_MAX + 1] = {0x00};

    if (realpath(path, tmp_path) == NULL && errno != ENOENT) {
        gplog(LOG_ERR, "realpath error. errno is %d", errno);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if (access(tmp_path, F_OK) == DCMI_OK) {
        return DCMI_OK;
    }

    if (mkdir(tmp_path, S_IRWXU) != 0) {
        gplog(LOG_ERR, "mkdir error. errno is %d", errno);
        return DCMI_ERR_CODE_FILE_OPERATE_FAIL;
    }
#endif
    return DCMI_OK;
}

int dcmi_cfg_get_file_lock(int fd, unsigned int timeout)
{
#ifndef _WIN32
    int ret;
    unsigned int index;
    unsigned int time_curr = 0;
    struct flock f_lock = {0};

    f_lock.l_type = F_WRLCK;
    f_lock.l_whence = 0;
    f_lock.l_len = 0;

    while (time_curr < timeout) {
        /* 首先循环一定次数尝试不阻塞方式获取锁 */
        for (index = 0; index < DCMI_CFG_MUTEX_FIRST_TRY_TIMES; index++) {
            ret = fcntl(fd, F_SETLK, &f_lock);
            if (ret == DCMI_OK) {
                return DCMI_OK;
            }
        }

        /* 未获取到锁，等待1ms，再次尝试获取 */
        (void)usleep(DCMI_CFG_MUTEX_SLEEP_TIMES_1MS);
        time_curr++;
    }
    return DCMI_ERR_CODE_INNER_ERR;
#else
    return DCMI_OK;
#endif
}

/* 进程间文件锁，当做进程间锁 */
int dcmi_cfg_set_lock(int *fd, unsigned int timeout, const char *file_path)
{
#ifndef _WIN32
    int fd_curr = -1;

    if (file_path == NULL) {
        gplog(LOG_ERR, "dcmi_cfg_set_lock:path is NULL!");
        return DCMI_ERR_CODE_INNER_ERR;
    }

    fd_curr = open(file_path, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd_curr < 0) {
        gplog(LOG_ERR, "dcmi_cfg_set_lock:open file %s failed! ", file_path);
        return DCMI_ERR_CODE_FILE_OPERATE_FAIL;
    }

    *fd = fd_curr;

    if (dcmi_cfg_get_file_lock(fd_curr, timeout) != DCMI_OK) {
        /* 失败需要关闭文件，成功需要调用dcmi_cfg_set_unlock来关闭 */
        close(fd_curr);
        *fd = -1;
        return DCMI_ERR_CODE_INNER_ERR;
    }
#endif
    return DCMI_OK;
}

/* 进程间文件锁，当做进程间锁 */
void dcmi_cfg_set_unlock(int fd)
{
#ifndef _WIN32
    struct flock f_lock = {0};
    if (fd < 0) {
        gplog(LOG_ERR, "dcmi_cfg_set_unlock:fd invalid. fd = %d", fd);
        return;
    }

    f_lock.l_type = F_UNLCK;
    f_lock.l_whence = 0;
    f_lock.l_len = 0;

    fcntl(fd, F_SETLK, &f_lock);
    close(fd);
#endif
    return;
}

STATIC void dcmi_cfg_write_default_context(char *path, char *default_comment)
{
#ifndef _WIN32
    FILE *fp_tmp = NULL;
    int fd_num;
    char path_tmp[PATH_MAX + 1] = {0x00};
    int ret;

    if (strlen(path) > PATH_MAX + 1 || strlen(default_comment) > PATH_MAX + 1) {
        return;
    }

    if (realpath(path, path_tmp) == NULL && errno != ENOENT) {
        gplog(LOG_ERR, "realpath error. errno is %d", errno);
        return;
    }

    fp_tmp = fopen(path_tmp, "w");
    if (fp_tmp == NULL) {
        return;
    }
    fd_num = fileno(fp_tmp);
    if (fd_num >= 0) {
        (void)fchmod(fd_num, (S_IRUSR | S_IWUSR));
    }
    ret = (int)fwrite(default_comment, 1, strlen(default_comment), fp_tmp);
    if (ret != (int)strlen(default_comment)) {
        gplog(LOG_ERR, "fwrite error.errno is %d", errno);
        fclose(fp_tmp);
        return;
    }
    (void)fclose(fp_tmp);
    gplog(LOG_OP, "dcmi_cfg_write_default_context success");
    return;
#else
    return;
#endif
}

STATIC int dcmi_cfg_get_cfg_path(char *path, unsigned int path_size, char *path_bak, unsigned int path_bak_size)
{
#ifndef _WIN32

    if (path_size > PATH_MAX + 1 || path_bak_size > PATH_MAX + 1) {
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }
    if (realpath(DCMI_VNPU_CONF, path) == NULL && errno != ENOENT) {
        gplog(LOG_ERR, "realpath error. errno is %d", errno);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if (realpath(DCMI_VNPU_CONF_BAK, path_bak) == NULL && errno != ENOENT) {
        gplog(LOG_ERR, "realpath error. errno is %d", errno);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }
#endif
    return DCMI_OK;
}

STATIC void dcmi_cfg_fix_0_size_file()
{
#ifndef _WIN32
    char path[PATH_MAX + 1] = {0x00};
    char path_bak[PATH_MAX + 1] = {0x00};
    struct stat buf;
    int ret;
    uid_t uid;

    ret = dcmi_cfg_get_cfg_path(path, sizeof(path), path_bak, sizeof(path_bak));
    if (ret != DCMI_OK) {
        return;
    }

    ret = stat(path_bak, &buf);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "stat failed. ret is %d. errno is %d.", ret, errno);
        dcmi_cfg_write_default_context(DCMI_VNPU_CONF, DCMI_VNPU_CONF_DEFAULT_COMMENT);
        return;
    }
    uid = getuid();
    if (uid != buf.st_uid) {
        gplog(LOG_ERR, "file uid invalid.uid %u st_pid %u.", uid, buf.st_uid);
        return;
    }
    
    ret = rename(path_bak, path);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "rename error. errno is %d", errno);
        return;
    }
    gplog(LOG_INFO, "rename cfg ok.");
    return;
#else
    return;
#endif
}

int dcmi_cfg_open_file(FILE **fp)
{
#ifdef _WIN32
    return DCMI_ERR_CODE_NOT_SUPPORT;
#else
    FILE *fp_tmp = NULL;
    unsigned int file_len;

    char path[PATH_MAX + 1] = {0x00};

    if (fp == NULL) {
        gplog(LOG_ERR, "fp is NULL.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if (realpath(DCMI_VNPU_CONF, path) == NULL && errno != ENOENT) {
        gplog(LOG_ERR, "realpath error. errno is %d", errno);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    fp_tmp = fopen(path, "r");
    if ((fp_tmp == NULL) && (errno == ENOENT)) {
        dcmi_cfg_write_default_context(DCMI_VNPU_CONF, DCMI_VNPU_CONF_DEFAULT_COMMENT);
    } else if (fp_tmp == NULL) {
        gplog(LOG_ERR, "fopen %s error. errno is %d", path, errno);
        return DCMI_ERR_CODE_FILE_OPERATE_FAIL;
    } else {
        (void)fseek(fp_tmp, 0, SEEK_END);
        file_len = (unsigned int)ftell(fp_tmp);
        rewind(fp_tmp);
        fclose(fp_tmp);
        if (file_len == 0) {
            dcmi_cfg_fix_0_size_file();
        }
    }
    fp_tmp = fopen(path, "r");
    if (fp_tmp == NULL) {
        return DCMI_ERR_CODE_FILE_OPERATE_FAIL;
    }
    *fp = fp_tmp;
    return DCMI_OK;
#endif
}

STATIC int dcmi_cfg_syslog_get_cfg_path(char *path, unsigned int path_size, char *path_bak, unsigned int path_bak_size)
{
#ifndef _WIN32

    if (path_size > PATH_MAX + 1 || path_bak_size > PATH_MAX + 1) {
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }
    if (realpath(DCMI_SYSLOG_CONF, path) == NULL && errno != ENOENT) {
        gplog(LOG_ERR, "realpath error. errno is %d", errno);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if (realpath(DCMI_SYSLOG_CONF_BAK, path_bak) == NULL && errno != ENOENT) {
        gplog(LOG_ERR, "realpath error. errno is %d", errno);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }
#endif
    return DCMI_OK;
}

int dcmi_cfg_syslog_open_file(FILE **fp)
{
#ifdef _WIN32
    return DCMI_ERR_CODE_NOT_SUPPORT;
#else
    FILE *fp_tmp = NULL;

    char path[PATH_MAX + 1] = {0x00};

    if (fp == NULL) {
        gplog(LOG_ERR, "fp is NULL.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if (realpath(DCMI_SYSLOG_CONF, path) == NULL && errno != ENOENT) {
        gplog(LOG_ERR, "realpath error. errno is %d", errno);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    fp_tmp = fopen(path, "r");
    if ((fp_tmp == NULL) && (errno == ENOENT)) {
        // 如果文件不存在，则创建一个默认的配置文件
        dcmi_cfg_write_default_context(DCMI_SYSLOG_CONF, DCMI_SYSLOG_CONF_DISABLE_COMMENT);
        fp_tmp = fopen(path, "r");
    }

    if (fp_tmp == NULL) {
        gplog(LOG_ERR, "fopen %s error. errno is %d", path, errno);
        return DCMI_ERR_CODE_FILE_OPERATE_FAIL;
    }
    *fp = fp_tmp;
    return DCMI_OK;
#endif
}

STATIC int dcmi_cfg_custom_op_get_cfg_path(char *path, unsigned int path_size, char *path_bak,
    unsigned int path_bak_size)
{
#ifndef _WIN32

    if (path_size > PATH_MAX + 1 || path_bak_size > PATH_MAX + 1) {
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }
    if (realpath(DCMI_CUSTOM_OP_CONF, path) == NULL && errno != ENOENT) {
        gplog(LOG_ERR, "realpath check path error. errno is %d", errno);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if (realpath(DCMI_CUSTOM_OP_CONF_BAK, path_bak) == NULL && errno != ENOENT) {
        gplog(LOG_ERR, "realpath check path_bak error. errno is %d", errno);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }
#endif
    return DCMI_OK;
}

STATIC void dcmi_cfg_custom_op_fix_empty_file()
{
#ifndef _WIN32
    char path[PATH_MAX + 1] = {0x00};
    char path_bak[PATH_MAX + 1] = {0x00};
    struct stat buf;
    int ret;
    uid_t uid;

    ret = dcmi_cfg_custom_op_get_cfg_path(path, sizeof(path), path_bak, sizeof(path_bak));
    if (ret != DCMI_OK) {
        return;
    }

    ret = stat(path_bak, &buf);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "stat failed. ret is %d. errno is %d.", ret, errno);
        dcmi_cfg_write_default_context(DCMI_CUSTOM_OP_CONF, DCMI_CUSTOM_OP_CONF_DEFAULT_COMMENT);
        return;
    }
    uid = getuid();
    if (uid != buf.st_uid) {
        gplog(LOG_ERR, "file uid invalid.uid %u st_pid %u.", uid, buf.st_uid);
        return;
    }
    
    ret = rename(path_bak, path);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "rename error. errno is %d", errno);
        return;
    }
    gplog(LOG_INFO, "custom op rename cfg ok.");
    return;
#else
    return;
#endif
}

int dcmi_cfg_custom_op_open_file(FILE **fp)
{
#ifdef _WIN32
    return DCMI_ERR_CODE_NOT_SUPPORT;
#else
    FILE *fp_tmp = NULL;
    unsigned int file_len;

    char path[PATH_MAX + 1] = {0x00};

    if (fp == NULL) {
        gplog(LOG_ERR, "fp is NULL.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if (realpath(DCMI_CUSTOM_OP_CONF, path) == NULL && errno != ENOENT) {
        gplog(LOG_ERR, "realpath error. errno is %d", errno);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    fp_tmp = fopen(path, "r");
    if ((fp_tmp == NULL) && (errno == ENOENT)) {
        dcmi_cfg_write_default_context(DCMI_CUSTOM_OP_CONF, DCMI_CUSTOM_OP_CONF_DEFAULT_COMMENT);
        // 如果文件不存在，则创建一个默认的配置文件
    } else if (fp_tmp == NULL) {
        gplog(LOG_ERR, "fopen %s error. errno is %d", path, errno);
        return DCMI_ERR_CODE_FILE_OPERATE_FAIL;
    } else {
        (void)fseek(fp_tmp, 0, SEEK_END);
        file_len = (unsigned int)ftell(fp_tmp);
        rewind(fp_tmp);
        fclose(fp_tmp);
        if (file_len == 0) {
            dcmi_cfg_custom_op_fix_empty_file();
        }
    }

    fp_tmp = fopen(path, "r");
    if (fp_tmp == NULL) {
        return DCMI_ERR_CODE_FILE_OPERATE_FAIL;
    }

    *fp = fp_tmp;
    return DCMI_OK;
#endif
}

int dcmi_cfg_create_default_syslog_file()
{
    int ret;
    int lock_fd;
 
    ret = dcmi_cfg_set_lock(&lock_fd, DCMI_CFG_GET_LOCK_TIMEOUT, DCMI_CFG_SYSLOG_LOCK_FILE_NAME);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_cfg_set_lock failed. ret is %d", ret);
        return DCMI_ERR_CODE_RESOURCE_OCCUPIED;
    }
    dcmi_cfg_write_default_context(DCMI_SYSLOG_CONF, DCMI_SYSLOG_CONF_DISABLE_COMMENT);
    ret = system("sync");
    if ((ret == -1) || (WIFEXITED(ret) == 0) || (WEXITSTATUS(ret) != 0)) {
        gplog(LOG_ERR, "%s sync failed. err is %d", DCMI_SYSLOG_CONF, ret);
        dcmi_cfg_set_unlock(lock_fd);
        return DCMI_ERR_CODE_INNER_ERR;
    }
    dcmi_cfg_set_unlock(lock_fd);
    return DCMI_OK;
}

int dcmi_cfg_syslog_check_cfg_path(char *path, unsigned int path_size, char *path_bak, unsigned int path_bak_size)
{
    int ret;

    ret = dcmi_cfg_syslog_get_cfg_path(path, path_size, path_bak, path_bak_size);
    if (ret != DCMI_OK) {
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    ret = rename(path, path_bak);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "rename error. errno is %d", errno);
        return DCMI_ERR_CODE_FILE_OPERATE_FAIL;
    }

    return DCMI_OK;
}

int dcmi_cfg_syslog_write_to_file(const char *buf, unsigned int buf_len)
{
#ifdef _WIN32
    return DCMI_ERR_CODE_NOT_SUPPORT;
#else
    char path[PATH_MAX + 1] = {0x00};
    char path_bak[PATH_MAX + 1] = {0x00};
    unsigned int write_len;
    int fd_num, ret;
    FILE *fp = NULL;

    if (buf == NULL) {
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    ret = dcmi_cfg_syslog_check_cfg_path(path, sizeof(path), path_bak, sizeof(path_bak));
    if (ret != DCMI_OK) {
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    fp = fopen(path, "w");
    if (fp == NULL) {
        (void)rename(path_bak, path);
        return DCMI_ERR_CODE_FILE_OPERATE_FAIL;
    }
    fd_num = fileno(fp);
    if (fd_num >= 0) {
        (void)fchmod(fd_num, (S_IRUSR | S_IWUSR));
    }

    write_len = fwrite(buf, 1, buf_len, fp);
    if (write_len != buf_len) {
        gplog(LOG_ERR, "fwrite error. write_len is %u, buf_len is %u", write_len, buf_len);
        goto write_fail;
    }
    ret = fsync(fd_num);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "fsync error. errno is %d", errno);
        goto write_fail;
    }
    (void)unlink(path_bak);
    (void)fclose(fp);
    return DCMI_OK;

write_fail:
    (void)fclose(fp);
    (void)rename(path_bak, path);
    return DCMI_ERR_CODE_FILE_OPERATE_FAIL;
#endif
}

int dcmi_cfg_check_cfg_path(char *path, unsigned int path_size, char *path_bak, unsigned int path_bak_size)
{
    int ret;

    ret = dcmi_cfg_get_cfg_path(path, path_size, path_bak, path_bak_size);
    if (ret != DCMI_OK) {
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if (!dcmi_check_run_in_docker()) {
        ret = rename(path, path_bak);
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "rename error. errno is %d", errno);
            return DCMI_ERR_CODE_FILE_OPERATE_FAIL;
        }
    }

    return DCMI_OK;
}

int dcmi_cfg_write_to_file(const char *buf, unsigned int buf_len)
{
#ifdef _WIN32
    return DCMI_ERR_CODE_NOT_SUPPORT;
#else
    FILE *fp = NULL;
    char path[PATH_MAX + 1] = {0x00};
    char path_bak[PATH_MAX + 1] = {0x00};
    unsigned int write_len;
    int fd_num, ret;

    if (buf == NULL) {
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    ret = dcmi_cfg_check_cfg_path(path, sizeof(path), path_bak, sizeof(path_bak));
    if (ret != DCMI_OK) {
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    fp = fopen(path, "w");
    if (fp == NULL) {
        (void)rename(path_bak, path);
        return DCMI_ERR_CODE_FILE_OPERATE_FAIL;
    }
    fd_num = fileno(fp);
    if (fd_num >= 0) {
        (void)fchmod(fd_num, (S_IRUSR | S_IWUSR));
    }

    write_len = fwrite(buf, 1, buf_len, fp);
    if (write_len != buf_len) {
        gplog(LOG_ERR, "fwrite error. write_len is %u, buf_len is %u", write_len, buf_len);
        goto write_fail;
    }
    ret = fsync(fd_num);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "fsync error. errno is %d", errno);
        goto write_fail;
    }
    (void)fclose(fp);
    (void)unlink(path_bak);
    return DCMI_OK;

write_fail:
    (void)fclose(fp);
    (void)rename(path_bak, path);
    return DCMI_ERR_CODE_FILE_OPERATE_FAIL;
#endif
}

static int dcmi_cfg_check_is_cover(const char *cmdline, char *buf_tmp)
{
    int ret = DCMI_CFG_DIFF_LINE;
    if (strstr(cmdline, buf_tmp) != NULL) {
        return DCMI_CFG_HAS_SAME_LINE;
    }

    // Compatible with earlier versions. must be overwritten.
    if (strncmp(cmdline, buf_tmp, strlen(buf_tmp) - 1) == 0) {
        ret = DCMI_CFG_COVER_LINE;
    }

    /* Adapting Persistence Configuration of Privilege Containers */
    if (strlen(cmdline) < DCMI_CFG_CMD_LINE_LEN) {
        return DCMI_CFG_DIFF_LINE;
    }
    if (strlen(buf_tmp) < DCMI_CFG_CMD_LINE_LEN) {
        return DCMI_CFG_DIFF_LINE;
    }
    if (strncmp(cmdline, buf_tmp, DCMI_CFG_CMD_LINE_LEN) == 0) {
        ret = DCMI_CFG_COVER_LINE;
    }
    return ret;
}

static int dcmi_cfg_get_action(unsigned int start, unsigned int end, const char *cmdline,
    int phy_id, char *buf_tmp)
{
    unsigned int phy_id_tmp, vnpu_id_tmp;
    unsigned int phy_id_cmd, vnpu_id_cmd;
    int ret;

    ret = sscanf_s(cmdline, "%u:%u:npu-smi ", &phy_id_cmd, &vnpu_id_cmd);
    if (ret < 1) {
        gplog(LOG_ERR, "sscanf_s failed. ret is %d", ret);
        return DCMI_CFG_NOT_NEED_INSERT;
    }

    if (start != DCMI_VNPU_FLAG_NOT_FIND) {
        if (end == DCMI_VNPU_FLAG_NOT_FIND) {
            ret = dcmi_cfg_check_is_cover(cmdline, buf_tmp);
            if (ret != DCMI_CFG_DIFF_LINE) {
                return ret;
            }

            ret = sscanf_s((const char *)buf_tmp, "%u:%u:npu-smi ", &phy_id_tmp, &vnpu_id_tmp);
            if (ret < 1) {
                gplog(LOG_ERR, "sscanf_s failed. ret is %d", ret);
                return DCMI_ERR_CODE_SECURE_FUN_FAIL;
            }

            if (phy_id_tmp > phy_id_cmd) {
                return DCMI_CFG_NEED_INSERT;
            } else if ((phy_id_tmp == phy_id_cmd) && (vnpu_id_tmp > vnpu_id_cmd)) {
                return DCMI_CFG_NEED_INSERT;
            } else {
                return DCMI_CFG_NOT_NEED_INSERT;
            }
        } else {
            if (end - start >= 1) {
                return DCMI_CFG_NEED_INSERT;
            }
        }
    }
    return DCMI_CFG_NOT_NEED_INSERT;
}

int dcmi_cfg_malloc_buffer_and_init(char **buf_out, unsigned int buf_size)
{
    if (buf_size == 0 || buf_size >= DCMI_VNPU_CONF_MAX_SIZE) {
        gplog(LOG_ERR, "file_len is invalid. buf_size is %u", buf_size);
        return DCMI_ERR_CODE_FILE_OPERATE_FAIL;
    }

    *buf_out = malloc(buf_size);
    if (*buf_out == NULL) {
        gplog(LOG_ERR, "malloc failed. errno is %d", errno);
        return DCMI_ERR_CODE_MEM_OPERATE_FAIL;
    }
    (void)memset_s(*buf_out, buf_size, 0, buf_size);
    return DCMI_OK;
}

static int dcmi_cfg_process_action(int action, struct cfg_buf_info *buf_info,  unsigned int *len,
    const char *cmdline, const char *buf_tmp)
{
    int ret, str_ret;

    if (buf_info == NULL) {
        return DCMI_CFG_NOT_INSERT;
    }

    switch (action) {
        case DCMI_CFG_NEED_INSERT:
        case DCMI_CFG_COVER_LINE:
            ret = strncat_s(buf_info->buf, buf_info->buf_size, cmdline, strlen(cmdline));
            if (ret != 0) {
                return DCMI_ERR_CODE_SECURE_FUN_FAIL;
            }
            *len += strlen(cmdline);
            ret = DCMI_CFG_INSERT_OK;
            break;
        case DCMI_CFG_HAS_SAME_LINE:
            ret = DCMI_CFG_INSERT_OK;
            break;
        default:
            ret = DCMI_CFG_NOT_INSERT;
            break;
    }

    if (action != DCMI_CFG_COVER_LINE) {
        str_ret = strncat_s(buf_info->buf, buf_info->buf_size, buf_tmp, strlen(buf_tmp));
        if (str_ret != 0) {
            return DCMI_ERR_CODE_SECURE_FUN_FAIL;
        }
        *len += strlen(buf_tmp);
    }

    return ret;
}

int dcmi_cfg_insert_cmdline_to_buffer(const char *cmdline, int phy_id, FILE *fp, char **buf_out, unsigned int *len)
{
    int ret, action;
    char buf_tmp[DCMI_VNPU_CONF_ONE_LINE_MAX_LEN] = {0};
    unsigned int insert_flag = 0;
    unsigned int line = 0;
    unsigned int start_flag = 0;
    unsigned int end_flag = 0;
    struct cfg_buf_info buf_info;

    (void)fseek(fp, 0, SEEK_END);
    buf_info.buf_size = (unsigned int)ftell(fp) + DCMI_VNPU_CONF_ONE_LINE_MAX_LEN + 1;
    rewind(fp);
    if (dcmi_cfg_malloc_buffer_and_init(buf_out, buf_info.buf_size) != DCMI_OK) {
        return DCMI_ERR_CODE_INNER_ERR;
    }
    buf_info.buf = *buf_out;

    while (!feof(fp)) {
        (void)memset_s(buf_tmp, sizeof(buf_tmp), 0, sizeof(buf_tmp));
        char *str = fgets(buf_tmp, sizeof(buf_tmp), fp);
        if (str == NULL) {
            break;
        }
        line++;
        
        if (insert_flag != DCMI_CFG_INSERT_COMPLETE) {
            if (end_flag == DCMI_VNPU_FLAG_NOT_FIND) {
                end_flag = (strcmp(buf_tmp, "[vnpu-config end]\n") == 0) ? (line - 1) : DCMI_VNPU_FLAG_NOT_FIND;
            }
            action = dcmi_cfg_get_action(start_flag, end_flag, cmdline, phy_id, buf_tmp);
            ret = dcmi_cfg_process_action(action, &buf_info, len, cmdline, buf_tmp);
            insert_flag = ((ret == DCMI_CFG_INSERT_OK) ? DCMI_CFG_INSERT_COMPLETE : insert_flag);
            if (ret == DCMI_ERR_CODE_SECURE_FUN_FAIL) {
                goto SECURE_FUN_FAIL;
            }
            if (start_flag == DCMI_VNPU_FLAG_NOT_FIND) {
                start_flag = (strcmp(buf_tmp, "[vnpu-config start]\n") == 0) ? (line - 1) : DCMI_VNPU_FLAG_NOT_FIND;
            }
        } else {
            ret = strncat_s(*buf_out, buf_info.buf_size, buf_tmp, strlen(buf_tmp));
            if (ret != 0) {
                goto SECURE_FUN_FAIL;
            }
            *len += strlen(buf_tmp);
        }
    }

    return DCMI_OK;
SECURE_FUN_FAIL:
    gplog(LOG_ERR, "strncat_s failed. ret is %d", ret);
    return DCMI_ERR_CODE_SECURE_FUN_FAIL;
}

int dcmi_cfg_check_is_delete(unsigned int start, unsigned int end, const char *cmdline, const char *buf_tmp)
{
    if (start != DCMI_VNPU_FLAG_NOT_FIND && end == DCMI_VNPU_FLAG_NOT_FIND) {
        if (strncmp(buf_tmp, cmdline, strlen(cmdline) - 1) == 0) {
            return DCMI_CFG_NEED_DELETE;
        }
    }

    /* Adapting Persistence Configuration of Privilege Containers */
    if (strlen(cmdline) < DCMI_CFG_CMD_LINE_LEN) {
        return DCMI_CFG_DIFF_LINE;
    }
    if (strlen(buf_tmp) < DCMI_CFG_CMD_LINE_LEN) {
        return DCMI_CFG_DIFF_LINE;
    }
    if (strncmp(cmdline, buf_tmp, DCMI_CFG_CMD_LINE_LEN) == 0) {
        return DCMI_CFG_NEED_DELETE;
    }
    return DCMI_CFG_NOT_NEED_DELETE;
}

int dcmi_cfg_delete_cmdline_form_buffer(const char *cmdline, FILE *fp, char **buf_out, unsigned int *buf_len)
{
    unsigned int start_flag = 0;
    unsigned int end_flag = 0;
    unsigned int buf_size;
    char buf_tmp[DCMI_VNPU_CONF_ONE_LINE_MAX_LEN] = {0};
    int ret, delete_action;
    unsigned int out_file_len = 0;
    unsigned int str_len;
    unsigned int line = 0;
    char *str = NULL;

    (void)fseek(fp, 0, SEEK_END);
    buf_size = (unsigned int)ftell(fp) + 1;
    rewind(fp);

    if (dcmi_cfg_malloc_buffer_and_init(buf_out, buf_size) != DCMI_OK) {
        return DCMI_ERR_CODE_INNER_ERR;
    }

    while (!feof(fp)) {
        (void)memset_s(buf_tmp, sizeof(buf_tmp), 0, sizeof(buf_tmp));
        str = fgets(buf_tmp, sizeof(buf_tmp), fp);
        if (str == NULL) {
            break;
        }
    
        line++;
        str_len = strlen(buf_tmp);
        out_file_len += str_len;
        if (end_flag == DCMI_VNPU_FLAG_NOT_FIND) {
            end_flag = (strcmp(buf_tmp, "[vnpu-config end]\n") == 0) ? (line - 1) : DCMI_VNPU_FLAG_NOT_FIND;
        }
        delete_action = dcmi_cfg_check_is_delete(start_flag, end_flag, cmdline, buf_tmp);
        if (delete_action == DCMI_CFG_NEED_DELETE) {
            out_file_len -= str_len;
            continue;
        } else {
            ret = strncat_s(*buf_out, buf_size, buf_tmp, str_len);
            if (ret != 0) {
                gplog(LOG_ERR, "strncat_s failed, ret is %d", ret);
                free(*buf_out);
                return DCMI_ERR_CODE_SECURE_FUN_FAIL;
            }
        }

        if (start_flag == DCMI_VNPU_FLAG_NOT_FIND) {
            start_flag = (strcmp(buf_tmp, "[vnpu-config start]\n") == 0) ? (line - 1) : DCMI_VNPU_FLAG_NOT_FIND;
        }
    }
    *buf_len = out_file_len;
    return DCMI_OK;
}

int dcmi_cfg_insert_creat_vnpu_cmdline(unsigned int phy_id, struct dcmi_create_vdev_out *vdev,
    int card_id, int chip_id, const char *vnpu_conf_name)
{
    // Open the file and read the configuration file.
    // Find the configuration position of the last phy_id.
    // Add a configuration command and save it to another buffer.
    // Write configuration to file
    char *buf = NULL;
    unsigned int buf_len = 0;
    int ret;
    char cmdline_buf[DCMI_VNPU_CONF_ONE_LINE_MAX_LEN] = {0};
    FILE *fp = NULL;
    int lock_fd;

    if (vnpu_conf_name == NULL) {
        gplog(LOG_ERR, "vnpu_conf_name is NULL");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if (dcmi_board_chip_type_is_ascend_310p()) {
        ret = snprintf_s(cmdline_buf, sizeof(cmdline_buf), sizeof(cmdline_buf) - 1,
            "%u:%u:npu-smi set -t create-vnpu -i %d -c %d -f %s -v %u -g %u\n",
            phy_id, vdev->vdev_id, card_id, chip_id, vnpu_conf_name, vdev->vdev_id, vdev->vfg_id);
    } else {
        ret = snprintf_s(cmdline_buf, sizeof(cmdline_buf), sizeof(cmdline_buf) - 1,
            "%u:%u:npu-smi set -t create-vnpu -i %d -c %d -f %s -v %u\n",
            phy_id, vdev->vdev_id, card_id, chip_id, vnpu_conf_name, vdev->vdev_id);
    }
    if (ret <= 0) {
        gplog(LOG_ERR, "snprintf_s failed, ret is %d", ret);
        return DCMI_ERR_CODE_SECURE_FUN_FAIL;
    }

    ret = dcmi_cfg_set_lock(&lock_fd, DCMI_CFG_GET_LOCK_TIMEOUT, DCMI_CFG_VNPU_LOCK_FILE_NAME);
    if (ret != DCMI_OK) {
        return DCMI_ERR_CODE_RESOURCE_OCCUPIED;
    }

    ret = dcmi_cfg_open_file(&fp);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_cfg_open_file failed. ret is %d", ret);
        dcmi_cfg_set_unlock(lock_fd);
        return DCMI_ERR_CODE_FILE_OPERATE_FAIL;
    }

    ret = dcmi_cfg_insert_cmdline_to_buffer(cmdline_buf, phy_id, fp, &buf, &buf_len);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_cfg_insert_cmdline_to_buffer cmdline_buf %s failed. ret is %d", cmdline_buf, ret);
        fclose(fp);
        dcmi_cfg_set_unlock(lock_fd);
        return ret;
    }
    (void)fclose(fp);
    ret = dcmi_cfg_write_to_file(buf, buf_len);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_cfg_write_to_file failed, ret is %d", ret);
    }
    free(buf);
    dcmi_cfg_set_unlock(lock_fd);
    return ret;
}

static int dcmi_cfg_clear_cmdline_from_buffer(FILE *fp, char **buf_out, unsigned int *buf_len)
{
    unsigned int start_flag = DCMI_VNPU_FLAG_NOT_FIND;
    unsigned int end_flag = DCMI_VNPU_FLAG_NOT_FIND;
    char buf_tmp[DCMI_VNPU_CONF_ONE_LINE_MAX_LEN] = {0};
    unsigned int out_file_len = 0;
    unsigned int line = 0;
    int ret;

    (void)fseek(fp, 0, SEEK_END);
    unsigned int buf_size = (unsigned int)ftell(fp) + DCMI_VNPU_CONF_ONE_LINE_MAX_LEN + 1;
    rewind(fp);

    if (dcmi_cfg_malloc_buffer_and_init(buf_out, buf_size) != DCMI_OK) {
        return DCMI_ERR_CODE_INNER_ERR;
    }

    while (!feof(fp)) {
        (void)memset_s(buf_tmp, sizeof(buf_tmp), 0, sizeof(buf_tmp));
        char *str = fgets(buf_tmp, sizeof(buf_tmp), fp);
        if (str == NULL) {
            break;
        }

        line++;
        if (end_flag == DCMI_VNPU_FLAG_NOT_FIND) {
            end_flag = (strcmp(buf_tmp, "[vnpu-config end]\n") == 0) ? (line - 1) : DCMI_VNPU_FLAG_NOT_FIND;
        }

        if ((start_flag == DCMI_VNPU_FLAG_NOT_FIND) || (end_flag != DCMI_VNPU_FLAG_NOT_FIND)) {
            ret = strncat_s(*buf_out, buf_size, buf_tmp, strlen(buf_tmp));
            if (ret != 0) {
                free(*buf_out);
                return DCMI_ERR_CODE_SECURE_FUN_FAIL;
            }
            out_file_len += strlen(buf_tmp);
        }

        if (start_flag == DCMI_VNPU_FLAG_NOT_FIND) {
            start_flag = (strcmp(buf_tmp, "[vnpu-config start]\n") == 0) ? (line - 1) : DCMI_VNPU_FLAG_NOT_FIND;
        }
    }

    *buf_len = out_file_len;
    return DCMI_OK;
}

static int dcmi_cfg_insert_destroy_all_npu_cmdline(void)
{
    char *buf = NULL;
    int ret;
    unsigned int buf_len = 0;
    FILE *fp = NULL;
    int lock_fd;

    ret = dcmi_cfg_set_lock(&lock_fd, DCMI_CFG_GET_LOCK_TIMEOUT, DCMI_CFG_VNPU_LOCK_FILE_NAME);
    if (ret != DCMI_OK) {
        return DCMI_ERR_CODE_RESOURCE_OCCUPIED;
    }

    ret = dcmi_cfg_open_file(&fp);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_cfg_open_file failed. ret is %d", ret);
        dcmi_cfg_set_unlock(lock_fd);
        return DCMI_ERR_CODE_FILE_OPERATE_FAIL;
    }

    ret = dcmi_cfg_clear_cmdline_from_buffer(fp, &buf, &buf_len);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_cfg_clear_cmdline_from_buffer failed, ret is %d", ret);
        fclose(fp);
        dcmi_cfg_set_unlock(lock_fd);
        return DCMI_ERR_CODE_INNER_ERR;
    }
    (void)fclose(fp);
    ret = dcmi_cfg_write_to_file(buf, buf_len);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_cfg_write_to_file failed, ret is %d", ret);
    }
    free(buf);
    dcmi_cfg_set_unlock(lock_fd);
    return ret;
}

int dcmi_cfg_insert_destroy_vnpu_cmdline(unsigned int phy_id, unsigned int vnpu_id, int card_id, int chip_id)
{
    // 打开文件读取配置文件
    // 找到最后一个phy_id的配置位置
    // 添加配置命令后保存到另外一个buf
    // 写入配置到文件
    char *buf = NULL;
    unsigned int buf_len = 0;
    int ret;
    char cmdline_buf[DCMI_VNPU_CONF_ONE_LINE_MAX_LEN] = {0};
    FILE *fp = NULL;
    int lock_fd;

    if (vnpu_id == 0xffff) { // vnpu_id 为65535即0xffff时，删除所有虚拟NPU
        return dcmi_cfg_insert_destroy_all_npu_cmdline();
    }

    ret = snprintf_s(cmdline_buf, sizeof(cmdline_buf), sizeof(cmdline_buf) - 1,
        "%u:%u:npu-smi set -t create-vnpu -i %d -c %d\n", phy_id, vnpu_id, card_id, chip_id);
    if (ret <= 0) {
        gplog(LOG_ERR, "snprintf_s failed, ret is %d", ret);
        return DCMI_ERR_CODE_SECURE_FUN_FAIL;
    }

    ret = dcmi_cfg_set_lock(&lock_fd, DCMI_CFG_GET_LOCK_TIMEOUT, DCMI_CFG_VNPU_LOCK_FILE_NAME);
    if (ret != DCMI_OK) {
        return DCMI_ERR_CODE_RESOURCE_OCCUPIED;
    }

    ret = dcmi_cfg_open_file(&fp);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_cfg_open_file failed. ret is %d", ret);
        dcmi_cfg_set_unlock(lock_fd);
        return DCMI_ERR_CODE_FILE_OPERATE_FAIL;
    }

    ret = dcmi_cfg_delete_cmdline_form_buffer(cmdline_buf, fp, &buf, &buf_len);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_cfg_delete_cmdline_form_buffer cmdline_buf %s failed. ret is %d", cmdline_buf, ret);
        fclose(fp);
        dcmi_cfg_set_unlock(lock_fd);
        return DCMI_ERR_CODE_INNER_ERR;
    }
    (void)fclose(fp);
    ret = dcmi_cfg_write_to_file(buf, buf_len);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_cfg_write_to_file failed, ret is %d", ret);
    }
    free(buf);
    dcmi_cfg_set_unlock(lock_fd);
    return ret;
}

int dcmi_cfg_check_is_found(unsigned int start, unsigned int end, const char *cmdline, const char *buf_tmp)
{
    if (start != DCMI_VNPU_FLAG_NOT_FIND && end == DCMI_VNPU_FLAG_NOT_FIND) {
        if (strncmp(buf_tmp, cmdline, strlen(cmdline) - 1) == 0) {
            return DCMI_CFG_CMDLINE_FOUND;
        }
    }
    return DCMI_CFG_CMDLINE_NOT_FOUND;
}

int dcmi_cfg_find_cmdline_from_file(const char *cmdline, FILE *fp, char *buf_out, unsigned int buf_size)
{
    unsigned int start_flag = 0;
    unsigned int end_flag = 0;
    char buf_tmp[DCMI_VNPU_CONF_ONE_LINE_MAX_LEN] = {0};
    int ret, found_flag;
    unsigned int line = 0;
    char *str = NULL;

    while (!feof(fp)) {
        (void)memset_s(buf_tmp, sizeof(buf_tmp), 0, sizeof(buf_tmp));
        str = fgets(buf_tmp, sizeof(buf_tmp), fp);
        if (str == NULL) {
            break;
        }
    
        line++;
        if (end_flag == DCMI_VNPU_FLAG_NOT_FIND) {
            end_flag = (strcmp(buf_tmp, "[vnpu-config end]\n") == 0) ? (line - 1) : DCMI_VNPU_FLAG_NOT_FIND;
        }
        found_flag = dcmi_cfg_check_is_found(start_flag, end_flag, cmdline, buf_tmp);
        if (found_flag == DCMI_CFG_CMDLINE_FOUND) {
            ret = strcpy_s(buf_out, buf_size, buf_tmp);
            if (ret == EOK) {
                return DCMI_OK;
            } else {
                gplog(LOG_ERR, "strcpy_s failed. ret is %d", ret);
                return DCMI_ERR_CODE_SECURE_FUN_FAIL;
            }
        }

        if (start_flag == DCMI_VNPU_FLAG_NOT_FIND) {
            start_flag = (strcmp(buf_tmp, "[vnpu-config start]\n") == 0) ? (line - 1) : DCMI_VNPU_FLAG_NOT_FIND;
        }
    }
    return DCMI_ERR_CODE_DEVICE_NOT_EXIST;
}

int dcmi_cfg_get_create_vnpu_template(unsigned int phy_id, unsigned int vdev_id,
    struct dcmi_create_vdev_res_stru *vdev)
{
    char buf[DCMI_VNPU_CONF_ONE_LINE_MAX_LEN] = {0};
    int ret;
    char cmdline_buf[DCMI_VNPU_CONF_ONE_LINE_MAX_LEN] = {0};
    FILE *fp = NULL;
    int lock_fd;
    const char *name;

    if (vdev == NULL) {
        gplog(LOG_ERR, "vdev is NULL");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    name = vdev->template_name;

    ret = snprintf_s(cmdline_buf, sizeof(cmdline_buf), sizeof(cmdline_buf) - 1,
        "%u:%u:npu-smi set -t create-vnpu ", phy_id, vdev_id);
    if (ret <= 0) {
        gplog(LOG_ERR, "snprintf_s failed, ret is %d", ret);
        return DCMI_ERR_CODE_SECURE_FUN_FAIL;
    }

    ret = dcmi_cfg_set_lock(&lock_fd, DCMI_CFG_GET_LOCK_TIMEOUT, DCMI_CFG_VNPU_LOCK_FILE_NAME);
    if (ret != DCMI_OK) {
        return DCMI_ERR_CODE_RESOURCE_OCCUPIED;
    }

    ret = dcmi_cfg_open_file(&fp);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_cfg_open_file failed. ret is %d", ret);
        dcmi_cfg_set_unlock(lock_fd);
        return DCMI_ERR_CODE_FILE_OPERATE_FAIL;
    }

    ret = dcmi_cfg_find_cmdline_from_file(cmdline_buf, fp, buf, sizeof(buf));
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_cfg_find_cmdline_from_file cmdline_buf %s failed. ret is %d", cmdline_buf, ret);
        fclose(fp);
        dcmi_cfg_set_unlock(lock_fd);
        return ret;
    }
    dcmi_cfg_set_unlock(lock_fd);
    (void)fclose(fp);
    ret = sscanf_s(buf, "%*s%*s%*s%*s%*s%*s%*s%*s -f %s -v", name, sizeof(vdev->template_name));
    if (ret < 0) {
        gplog(LOG_ERR, "sscanf_s failed. ret is %d", ret);
        return DCMI_ERR_CODE_SECURE_FUN_FAIL;
    }
    return DCMI_OK;
}

int dcmi_cfg_check_vnpu_config_context_is_delete(unsigned int start, unsigned int end, unsigned int mode,
    unsigned set_flag, const char *buf)
{
    if ((!set_flag) && (strncmp(buf, "vnpu_config_recover:", strlen("vnpu_config_recover:")) == 0)) {
        return DCMI_CFG_NEED_INSERT;
    } else {
        if (mode == DCMI_CFG_RECOVER_ENABLE) {
            return DCMI_CFG_NOT_NEED_DELETE;
        } else {
            if (end == DCMI_VNPU_FLAG_NOT_FIND && start != DCMI_VNPU_FLAG_NOT_FIND) {
                return DCMI_CFG_NEED_DELETE;
            } else {
                return DCMI_CFG_NOT_NEED_DELETE;
            }
        }
    }
}

int dcmi_cfg_set_recover_to_buffer(const char *cmdline, unsigned int mode, FILE *fp,
    char **buf_out, unsigned int *buf_len)
{
    unsigned int start_flag = DCMI_VNPU_FLAG_NOT_FIND;
    unsigned int end_flag = DCMI_VNPU_FLAG_NOT_FIND;
    char buf_tmp[DCMI_VNPU_CONF_ONE_LINE_MAX_LEN] = {0};
    unsigned int out_file_len = 0;
    unsigned int line = 0;
    unsigned int set_complete = FALSE;
    int ret, action;

    (void)fseek(fp, 0, SEEK_END);
    unsigned int buf_size = (unsigned int)ftell(fp) + DCMI_VNPU_CONF_ONE_LINE_MAX_LEN + 1;
    rewind(fp);

    if (dcmi_cfg_malloc_buffer_and_init(buf_out, buf_size) != DCMI_OK) {
        return DCMI_ERR_CODE_INNER_ERR;
    }

    while (!feof(fp)) {
        (void)memset_s(buf_tmp, sizeof(buf_tmp), 0, sizeof(buf_tmp));
        char *str = fgets(buf_tmp, sizeof(buf_tmp), fp);
        if (str == NULL) {
            break;
        }

        line++;
        if (end_flag == DCMI_VNPU_FLAG_NOT_FIND) {
            end_flag = (strcmp(buf_tmp, "[vnpu-config end]\n") == 0) ? (line - 1) : DCMI_VNPU_FLAG_NOT_FIND;
        }

        action = dcmi_cfg_check_vnpu_config_context_is_delete(start_flag, end_flag, mode, set_complete, buf_tmp);
        if (action == DCMI_CFG_NEED_DELETE) {
            continue;
        } else if (action == DCMI_CFG_NEED_INSERT) {
            ret = strncat_s(*buf_out, buf_size, cmdline, strlen(cmdline));
            out_file_len += strlen(cmdline);
            set_complete = TRUE;
        } else {
            ret = strncat_s(*buf_out, buf_size, buf_tmp, strlen(buf_tmp));
            out_file_len += strlen(buf_tmp);
        }

        if (start_flag == DCMI_VNPU_FLAG_NOT_FIND) {
            start_flag = (strcmp(buf_tmp, "[vnpu-config start]\n") == 0) ? (line - 1) : DCMI_VNPU_FLAG_NOT_FIND;
        }

        if (ret != 0) {
            free(*buf_out);
            return DCMI_ERR_CODE_SECURE_FUN_FAIL;
        }
    }
    if (set_complete == FALSE) {
        free(*buf_out);
        return DCMI_ERR_CODE_INNER_ERR;
    }
    *buf_len = out_file_len;
    return DCMI_OK;
}

int dcmi_cfg_set_config_recover_mode(unsigned int mode)
{
    char *buf = NULL;
    int ret;
    char cmdline_buf[DCMI_VNPU_CONF_ONE_LINE_MAX_LEN] = {0};
    unsigned int buf_len = 0;
    FILE *fp = NULL;
    int lock_fd;

    ret = snprintf_s(cmdline_buf, sizeof(cmdline_buf), sizeof(cmdline_buf) - 1,
        "vnpu_config_recover:%s\n", (mode == DCMI_CFG_RECOVER_ENABLE) ? "enable" : "disable");
    if (ret <= 0) {
        gplog(LOG_ERR, "snprintf_s failed, ret is %d, mode is %u", ret, mode);
        return DCMI_ERR_CODE_SECURE_FUN_FAIL;
    }

    ret = dcmi_cfg_set_lock(&lock_fd, DCMI_CFG_GET_LOCK_TIMEOUT, DCMI_CFG_VNPU_LOCK_FILE_NAME);
    if (ret != DCMI_OK) {
        return DCMI_ERR_CODE_RESOURCE_OCCUPIED;
    }

    ret = dcmi_cfg_open_file(&fp);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_cfg_open_file failed. ret is %d", ret);
        dcmi_cfg_set_unlock(lock_fd);
        return DCMI_ERR_CODE_FILE_OPERATE_FAIL;
    }

    ret = dcmi_cfg_set_recover_to_buffer(cmdline_buf, mode, fp, &buf, &buf_len);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_cfg_set_recover_to_buffer cmdline_buf %s failed, ret is %d", cmdline_buf, ret);
        fclose(fp);
        dcmi_cfg_set_unlock(lock_fd);
        return DCMI_ERR_CODE_INNER_ERR;
    }
    (void)fclose(fp);
    ret = dcmi_cfg_write_to_file(buf, buf_len);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_cfg_write_to_file failed, ret is %d", ret);
    }
    free(buf);
    dcmi_cfg_set_unlock(lock_fd);
    return ret;
}

int dcmi_cfg_get_config_recover_mode(unsigned int *mode)
{
    int ret;
    char buf_tmp[DCMI_VNPU_CONF_ONE_LINE_MAX_LEN] = {0};
    FILE *fp = NULL;
    char *str = NULL;
    int lock_fd;

    ret = dcmi_cfg_set_lock(&lock_fd, DCMI_CFG_GET_LOCK_TIMEOUT, DCMI_CFG_VNPU_LOCK_FILE_NAME);
    if (ret != DCMI_OK) {
        return DCMI_ERR_CODE_RESOURCE_OCCUPIED;
    }

    ret = dcmi_cfg_open_file(&fp);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_cfg_open_file failed. ret is %d", ret);
        dcmi_cfg_set_unlock(lock_fd);
        return DCMI_ERR_CODE_FILE_OPERATE_FAIL;
    }

    while (!feof(fp)) {
        (void)memset_s(buf_tmp, sizeof(buf_tmp), 0, sizeof(buf_tmp));
        str = fgets(buf_tmp, sizeof(buf_tmp), fp);
        if (str == NULL) {
            break;
        }

        if (strncmp(buf_tmp, "vnpu_config_recover:", strlen("vnpu_config_recover:")) == 0) {
            if (strncmp(buf_tmp, "vnpu_config_recover:enable", strlen("vnpu_config_recover:enable")) == 0) {
                *mode = DCMI_CFG_RECOVER_ENABLE;
            } else {
                *mode = DCMI_CFG_RECOVER_DISABLE;
            }
            fclose(fp);
            dcmi_cfg_set_unlock(lock_fd);
            return DCMI_OK;
        }
    }
    (void)fclose(fp);
    dcmi_cfg_set_unlock(lock_fd);
    return DCMI_ERR_CODE_INNER_ERR;
}

int dcmi_cfg_syslog_malloc_buffer_and_init(char **buf_out, unsigned int buf_size)
{
    if (buf_size == 0 || buf_size >= DCMI_SYSLOG_CONF_MAX_SIZE) {
        gplog(LOG_ERR, "file_len is invalid. buf_size is %u", buf_size);
        return DCMI_ERR_CODE_FILE_OPERATE_FAIL;
    }

    *buf_out = malloc(buf_size);
    if (*buf_out == NULL) {
        gplog(LOG_ERR, "malloc failed. errno is %d", errno);
        return DCMI_ERR_CODE_MEM_OPERATE_FAIL;
    }
    (void)memset_s(*buf_out, buf_size, 0, buf_size);
    return DCMI_OK;
}

int dcmi_cfg_insert_syslog_cmdline_to_buffer(char *cmdline, FILE *fp, char **buf_out, unsigned int *len)
{
    int ret;
    char buf_tmp[DCMI_SYSLOG_CONF_ONE_LINE_MAX_LEN] = {0};
    struct cfg_buf_info buf_info;

    (void)fseek(fp, 0, SEEK_END);
    buf_info.buf_size = (unsigned int)ftell(fp) + DCMI_SYSLOG_CONF_ONE_LINE_MAX_LEN + 1;
    rewind(fp);
    if (dcmi_cfg_syslog_malloc_buffer_and_init(buf_out, buf_info.buf_size) != DCMI_OK) {
        return DCMI_ERR_CODE_INNER_ERR;
    }
    buf_info.buf = *buf_out;

    (void)memset_s(buf_tmp, sizeof(buf_tmp), 0, sizeof(buf_tmp));
    char *str = fgets(buf_tmp, sizeof(buf_tmp), fp); /* buf_tmp 存储每一行的配置信息 */
    if (str == NULL) {
        free(*buf_out);
        return DCMI_ERR_CODE_FILE_OPERATE_FAIL;
    }
 
    ret = strncat_s(*buf_out, buf_info.buf_size, buf_tmp, strlen(buf_tmp));
    if (ret != 0) {
        goto SECURE_FUN_FAIL;
    }
    *len += strlen(buf_tmp);
 
    ret = strncat_s(*buf_out, buf_info.buf_size, cmdline, strlen(cmdline));
    *len += strlen(cmdline);
    if (ret != 0) {
        goto SECURE_FUN_FAIL;
    }
    return DCMI_OK;

SECURE_FUN_FAIL:
    gplog(LOG_ERR, "strncat_s failed. ret is %d", ret);
    free(*buf_out);
    return DCMI_ERR_CODE_SECURE_FUN_FAIL;
}

int dcmi_get_syslog_cfg_recover_mode(int *mode)
{
    int ret;
    char buf_tmp[DCMI_SYSLOG_CONF_ONE_LINE_MAX_LEN] = {0};
    FILE *fp = NULL;
    char *str = NULL;
    int lock_fd;

    ret = dcmi_cfg_set_lock(&lock_fd, DCMI_CFG_GET_LOCK_TIMEOUT, DCMI_CFG_SYSLOG_LOCK_FILE_NAME);
    if (ret != DCMI_OK) {
        return DCMI_ERR_CODE_RESOURCE_OCCUPIED;
    }

    ret = dcmi_cfg_syslog_open_file(&fp);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_cfg_syslog_open_file failed. ret is %d", ret);
        dcmi_cfg_set_unlock(lock_fd);
        return DCMI_ERR_CODE_FILE_OPERATE_FAIL;
    }

    while (!feof(fp)) {
        (void)memset_s(buf_tmp, sizeof(buf_tmp), 0, sizeof(buf_tmp));
        str = fgets(buf_tmp, sizeof(buf_tmp), fp);
        if (str == NULL) {
            break;
        }

        if (strncmp(buf_tmp, "syslog_persistence_config_mode:", strlen("syslog_persistence_config_mode:")) == 0) {
            if (strncmp(buf_tmp, "syslog_persistence_config_mode:enable",
                        strlen("syslog_persistence_config_mode:enable")) == 0) {
                *mode = DCMI_CFG_PERSISTENCE_ENABLE;
            } else {
                *mode = DCMI_CFG_PERSISTENCE_DISABLE;
            }
            fclose(fp);
            ret = system("sync");
            if ((ret == -1) || (WIFEXITED(ret) == 0) || (WEXITSTATUS(ret) != 0)) {
                gplog(LOG_ERR, "%s sync failed. err is %d", DCMI_SYSLOG_CONF, ret);
                dcmi_cfg_set_unlock(lock_fd);
                return DCMI_ERR_CODE_INNER_ERR;
            }
            dcmi_cfg_set_unlock(lock_fd);
            return DCMI_OK;
        }
    }
    (void)fclose(fp);
    dcmi_cfg_set_unlock(lock_fd);
    return DCMI_ERR_CODE_INNER_ERR;
}

int dcmi_clear_cfg_file(char *path, int mode)
{
    int ret;
    FILE *fp_tmp = NULL;
    int fd_num;
    fp_tmp = fopen(path, "w"); /* 如果文件不存在，会创建一个新的文件；如果文件存在，文件会被清空 */
    if (fp_tmp == NULL) {
        gplog(LOG_ERR, "fopen %s error. errno is %d", path, errno);
        return DCMI_ERR_CODE_FILE_OPERATE_FAIL;
    }
    fd_num = fileno(fp_tmp);
    if (fd_num >= 0) {
        (void)fchmod(fd_num, (S_IRUSR | S_IWUSR));
    }
    if (mode == DCMI_CFG_PERSISTENCE_ENABLE) {
        ret = (int)fwrite(DCMI_SYSLOG_CONF_ENABLE_COMMENT, 1, strlen(DCMI_SYSLOG_CONF_ENABLE_COMMENT), fp_tmp);
        if (ret != (int)strlen(DCMI_SYSLOG_CONF_ENABLE_COMMENT)) {
            gplog(LOG_ERR, "fwrite error.errno is %d", errno);
            fclose(fp_tmp);
            return DCMI_OK;
        }
    } else {
        ret = (int)fwrite(DCMI_SYSLOG_CONF_DISABLE_COMMENT, 1, strlen(DCMI_SYSLOG_CONF_DISABLE_COMMENT), fp_tmp);
        if (ret != (int)strlen(DCMI_SYSLOG_CONF_DISABLE_COMMENT)) {
            gplog(LOG_ERR, "fwrite error.errno is %d", errno);
            fclose(fp_tmp);
            return DCMI_OK;
        }
    }
    (void)fclose(fp_tmp);
    gplog(LOG_OP, "dcmi_cfg_syslog_clear_file success");
    return DCMI_OK;
}

int dcmi_cfg_syslog_clear_file(int mode)
{
    int ret;
    int lock_fd;

    char path[PATH_MAX + 1] = {0x00};

    ret = dcmi_cfg_set_lock(&lock_fd, DCMI_CFG_GET_LOCK_TIMEOUT, DCMI_CFG_SYSLOG_LOCK_FILE_NAME);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_cfg_set_lock failed. ret is %d", ret);
        return DCMI_ERR_CODE_RESOURCE_OCCUPIED;
    }

    if (sizeof(path) > PATH_MAX + 1) {
        goto SECURE_FUN_FAIL;
    }

    if (realpath(DCMI_SYSLOG_CONF, path) == NULL && errno != ENOENT) {
        gplog(LOG_ERR, "realpath error. errno is %d", errno);
        goto SECURE_FUN_FAIL;
    }

    ret = dcmi_clear_cfg_file(path, mode); /* 清空配置文件 */
    if (ret != DCMI_OK) {
        goto FILE_OPERATE_FAIL;
    }

    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_cfg_open_file failed. ret is %d", ret);
        goto FILE_OPERATE_FAIL;
    }
    ret = system("sync");
    if ((ret == -1) || (WIFEXITED(ret) == 0) || (WEXITSTATUS(ret) != 0)) {
        gplog(LOG_ERR, "%s sync failed. err is %d", DCMI_SYSLOG_CONF, ret);
        dcmi_cfg_set_unlock(lock_fd);
        return DCMI_ERR_CODE_INNER_ERR;
    }
    dcmi_cfg_set_unlock(lock_fd);
    return DCMI_OK;

SECURE_FUN_FAIL:
    dcmi_cfg_set_unlock(lock_fd);
    return DCMI_ERR_CODE_INVALID_PARAMETER;
FILE_OPERATE_FAIL:
    dcmi_cfg_set_unlock(lock_fd);
    return DCMI_ERR_CODE_FILE_OPERATE_FAIL;
}

int dcmi_write_config_file_to_buffer(const char *buf_tmp, int mode, char **buf_out,
                                     unsigned int *len, unsigned int buf_size)
{
    int ret;
 
    if (strncmp(buf_tmp, "syslog_persistence_config_mode:", strlen("syslog_persistence_config_mode:")) == 0) {
        if (mode == DCMI_CFG_PERSISTENCE_ENABLE) {
            ret = strncat_s(*buf_out, buf_size, DCMI_SYSLOG_CONF_ENABLE_COMMENT,
                            strlen(DCMI_SYSLOG_CONF_ENABLE_COMMENT));
            if (ret != 0) {
                goto SECURE_FUN_FAIL;
            }
            *len += strlen(DCMI_SYSLOG_CONF_ENABLE_COMMENT);
        } else {
            ret = strncat_s(*buf_out, buf_size, DCMI_SYSLOG_CONF_DISABLE_COMMENT,
                            strlen(DCMI_SYSLOG_CONF_DISABLE_COMMENT));
            if (ret != 0) {
                goto SECURE_FUN_FAIL;
            }
            *len += strlen(DCMI_SYSLOG_CONF_DISABLE_COMMENT);
        }
    } else {
        ret = strncat_s(*buf_out, buf_size, buf_tmp, strlen(buf_tmp));
        if (ret != 0) {
            goto SECURE_FUN_FAIL;
        }
        *len += strlen(buf_tmp);
    }
    return DCMI_OK;
 
SECURE_FUN_FAIL:
    gplog(LOG_ERR, "strncat_s failed. ret is %d", ret);
    return DCMI_ERR_CODE_SECURE_FUN_FAIL;
}

int dcmi_cfg_set_syslog_mode(int mode, FILE *fp, char **buf_out, unsigned int *len)
{
    int ret;
    char buf_tmp[DCMI_SYSLOG_CONF_ONE_LINE_MAX_LEN] = {0};
    struct cfg_buf_info buf_info; /* 存储配置信息 */
    char *str = NULL;

    (void)fseek(fp, 0, SEEK_END);
    buf_info.buf_size = (unsigned int)ftell(fp) + DCMI_SYSLOG_CONF_ONE_LINE_MAX_LEN + 1;
    rewind(fp);

    /* 申请指定大小的内存空间，并初始化 */
    if (dcmi_cfg_syslog_malloc_buffer_and_init(buf_out, buf_info.buf_size) != DCMI_OK) {
        return DCMI_ERR_CODE_INNER_ERR;
    }
    buf_info.buf = *buf_out;

    while (!feof(fp)) {
        (void)memset_s(buf_tmp, sizeof(buf_tmp), 0, sizeof(buf_tmp));
        str = fgets(buf_tmp, sizeof(buf_tmp), fp); /* buf_tmp 存储每一行的配置信息 */
        if (str == NULL) {
            break;
        }
        
        ret = dcmi_write_config_file_to_buffer(buf_tmp, mode, buf_out, len, buf_info.buf_size);
        if (ret != DCMI_OK) {
            free(*buf_out); /* 释放内存 */
            return DCMI_ERR_CODE_INNER_ERR;
        }
    }
    return DCMI_OK;
}

int dcmi_set_syslog_cfg_recover_mode(int mode)
{
    int ret;
    int lock_fd;
    FILE *fp = NULL;
    char *buf = NULL;
    unsigned int buf_len = 0;

    ret = dcmi_cfg_set_lock(&lock_fd, DCMI_CFG_GET_LOCK_TIMEOUT, DCMI_CFG_SYSLOG_LOCK_FILE_NAME);  /* 获取锁 */
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_cfg_set_lock failed. ret is %d", ret);
        return DCMI_ERR_CODE_RESOURCE_OCCUPIED;
    }

    ret = dcmi_cfg_syslog_open_file(&fp);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_cfg_syslog_open_file. ret is %d", ret);
        goto FILE_OPERATE_FAIL;
    }

    ret = dcmi_cfg_set_syslog_mode(mode, fp, &buf, &buf_len);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_cfg_set_syslog_mode failed. ret is %d", ret);
        (void)fclose(fp);
        goto FILE_OPERATE_FAIL;
    }

    (void)fclose(fp);
    ret = dcmi_cfg_syslog_write_to_file(buf, buf_len);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_cfg_write_to_file failed, ret is %d", ret);
    }
 
    free(buf);
FILE_OPERATE_FAIL:
    ret = system("sync");
    if ((ret == -1) || (WIFEXITED(ret) == 0) || (WEXITSTATUS(ret) != 0)) {
        gplog(LOG_ERR, "%s sync failed. err is %d", DCMI_SYSLOG_CONF, ret);
        dcmi_cfg_set_unlock(lock_fd);
        return DCMI_ERR_CODE_INNER_ERR;
    }
    dcmi_cfg_set_unlock(lock_fd);
    return ret == DCMI_OK ? DCMI_OK : DCMI_ERR_CODE_FILE_OPERATE_FAIL;
}

int dcmi_cfg_insert_syslog_persistence_cmdline(char *cmdline)
{
    // Open the file and read the configuration file.
    // Find the configuration position of the last phy_id.
    // Add a configuration command and save it to another buffer.
    // Write configuration to file
    char *buf = NULL;
    unsigned int buf_len = 0;
    int ret;
    FILE *fp = NULL;
    int lock_fd;

    ret = dcmi_cfg_set_lock(&lock_fd, DCMI_CFG_GET_LOCK_TIMEOUT, DCMI_CFG_SYSLOG_LOCK_FILE_NAME);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_cfg_set_lock failed. ret is %d", ret);
        return DCMI_ERR_CODE_RESOURCE_OCCUPIED;
    }

    ret = dcmi_cfg_syslog_open_file(&fp);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_cfg_syslog_open_file failed. ret is %d", ret);
        goto FILE_OPERATE_FAIL;
    }

    ret = dcmi_cfg_insert_syslog_cmdline_to_buffer(cmdline, fp, &buf, &buf_len);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_cfg_insert_syslog_cmdline_to_buffer cmdline %s failed. ret is %d", cmdline, ret);
        (void)fclose(fp);
        goto FILE_OPERATE_FAIL;
    }
    (void)fclose(fp);
    ret = dcmi_cfg_syslog_write_to_file(buf, buf_len);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_cfg_write_to_file failed, ret is %d", ret);
    }
 
    free(buf);
FILE_OPERATE_FAIL:
    ret = system("sync");
    if ((ret == -1) || (WIFEXITED(ret) == 0) || (WEXITSTATUS(ret) != 0)) {
        gplog(LOG_ERR, "%s sync failed. err is %d", DCMI_SYSLOG_CONF, ret);
        dcmi_cfg_set_unlock(lock_fd);
        return DCMI_ERR_CODE_INNER_ERR;
    }
    dcmi_cfg_set_unlock(lock_fd);
    return ret;
}

int dcmi_check_line_by_line_is_legal(int line, const char *buf_tmp, char *cmdline, int cmd_len)
{
    int ret, gear;
    char save_path[PATH_MAX + 1] = {0x00};
    if (line == 1) {
        ret = sscanf_s(buf_tmp, "/usr/local/bin/npu-smi set -t sys-log-dump -s %d -f %s",
                       &gear, save_path, sizeof(save_path));
        if (ret < 1) {  /* 解析失败 */
            gplog(LOG_ERR, "sscanf_s failed. ret is %d", ret);
            return DCMI_ERR_CODE_SYSLOG_CONFIG_ILLEGAL;
        }
        if (gear > DCMI_SYSLOG_DUMP_MAX_GEAR || gear < DCMI_SYSLOG_DUMP_MIN_GEAR ||
            access(save_path, F_OK) != DCMI_OK) {  /* 判断挡位合法性 判断路径是否存在 */
            return DCMI_ERR_CODE_SYSLOG_CONFIG_ILLEGAL;
        }
        ret = strcpy_s(cmdline, cmd_len, buf_tmp);
        if (ret != 0) {
            gplog(LOG_ERR, "strcpy_s failed. ret is %d", ret);
            return DCMI_ERR_CODE_INNER_ERR;
        }
    }
    if ((line == DCMI_SYSLOG_CONF_MAX_LINE) && (strcmp(buf_tmp, "y\n") != 0)) {
        return DCMI_ERR_CODE_SYSLOG_CONFIG_ILLEGAL;
    }
    return DCMI_OK;
}
 
int dcmi_cfg_syslog_check_cmdline_legal(FILE *fp, char *cmdline, int cmd_len)
{
    int ret, line = 0;
    char buf_line[DCMI_SYSLOG_CONF_ONE_LINE_MAX_LEN] = {0};
    char *str = NULL;
    
    (void)memset_s(cmdline, cmd_len * sizeof(char), 0, cmd_len * sizeof(char));
    str = fgets(buf_line, sizeof(buf_line), fp);
    if (str == NULL) {
        return DCMI_ERR_CODE_SYSLOG_CONFIG_ILLEGAL;
    }
    
    if ((strcmp(buf_line, DCMI_SYSLOG_CONF_DISABLE_COMMENT) != 0) &&
        (strcmp(buf_line, DCMI_SYSLOG_CONF_ENABLE_COMMENT) != 0)) {
        return DCMI_ERR_CODE_SYSLOG_CONFIG_ILLEGAL;
    }
    
    while (!feof(fp)) {
        (void)memset_s(buf_line, sizeof(buf_line), 0, sizeof(buf_line));
        str = fgets(buf_line, sizeof(buf_line), fp);
        if (str == NULL) {
            break;
        }
        line++;
        ret = dcmi_check_line_by_line_is_legal(line, buf_line, cmdline, cmd_len);
        if (ret != DCMI_OK) {
            return ret;
        }
    }
    if ((line != DCMI_SYSLOG_CONF_MAX_LINE) && (line != DCMI_SYSLOG_CONF_MIN_LINE)) {
        return DCMI_ERR_CODE_SYSLOG_CONFIG_ILLEGAL;
    }
    return DCMI_OK;
}
 
int dcmi_cfg_check_syslog_cfg_legal(char *cmdline, int cmd_len)
{
    int ret;
    FILE *fp = NULL;
    char path[PATH_MAX + 1] = {0x00};
 
    if (realpath(DCMI_SYSLOG_CONF, path) == NULL && errno != ENOENT) {
        gplog(LOG_ERR, "realpath error. errno is %d", errno);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    fp = fopen(path, "r");
    if (fp == NULL) {
        return DCMI_ERR_CODE_FILE_OPERATE_FAIL;
    }

    ret = dcmi_cfg_syslog_check_cmdline_legal(fp, cmdline, cmd_len);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_cfg_syslog_check_cmdline_legal failed. ret is %d", ret);
    }
    (void)fclose(fp);
    return ret;
}
 
int dcmi_check_syslog_cfg_legal(char *cfg, int cfg_len)
{
    int ret;
    int lock_fd;
 
    ret = dcmi_cfg_set_lock(&lock_fd, DCMI_CFG_GET_LOCK_TIMEOUT, DCMI_CFG_SYSLOG_LOCK_FILE_NAME);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_cfg_set_lock failed. ret is %d", ret);
        return DCMI_ERR_CODE_RESOURCE_OCCUPIED;
    }
    ret = dcmi_cfg_check_syslog_cfg_legal(cfg, cfg_len);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_cfg_check_syslog_cfg_legal failed. ret is %d", ret);
        dcmi_cfg_set_unlock(lock_fd);
        return ret;
    }
    dcmi_cfg_set_unlock(lock_fd);
    return DCMI_OK;
}

int dcmi_get_syslog_persistence_info(int *mode, char *cfg, int cfg_len)
{
    int ret;
 
    if (access(DCMI_SYSLOG_CONF, F_OK) != DCMI_OK) { // 说明配置文件不存在
        gplog(LOG_ERR, "%s does not exist.", DCMI_SYSLOG_CONF);
        return DCMI_ERR_CODE_CONFIG_INFO_NOT_EXIST;
    }

    ret = dcmi_check_syslog_cfg_legal(cfg, cfg_len);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_check_syslog_cfg_legal failed. ret is %d", ret);
        return ret;
    }

    ret = dcmi_get_syslog_cfg_recover_mode(mode);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_cfg_get_config_recover_mode failed. err is %d", ret);
        return ret;
    }
    return DCMI_OK;
}

int dcmi_cfg_exist_vnpu_config(const char *target_buf, int *flag)
{
    int lock_fd;
    FILE *fp = NULL;
    unsigned int start_flag = 0;
    unsigned int end_flag = 0;
    char path[PATH_MAX + 1] = {0x00};
    char buf[DCMI_VNPU_CONF_ONE_LINE_MAX_LEN] = {0};

    if (dcmi_cfg_set_lock(&lock_fd, DCMI_CFG_GET_LOCK_TIMEOUT, DCMI_CFG_VNPU_LOCK_FILE_NAME) != DCMI_OK) {
        return DCMI_ERR_CODE_RESOURCE_OCCUPIED;
    }

    if (realpath(DCMI_VNPU_CONF, path) == NULL) {
        gplog(LOG_ERR, "realpath error. errno is %d", errno);
        goto NO_EXIST;
    }

    fp = fopen(path, "r");
    if (fp == NULL) {
        gplog(LOG_INFO, "cannot open %s. ret is %d", path, errno);
        goto NO_EXIST;
    }

    while (!feof(fp)) {
        (void)memset_s(buf, sizeof(buf), 0, sizeof(buf));
        if (fgets(buf, sizeof(buf), fp) == NULL) {
            break;
        }

        if ((start_flag != DCMI_VNPU_FLAG_NOT_FIND) && (end_flag == DCMI_VNPU_FLAG_NOT_FIND)) {
            if (strstr(buf, target_buf) != NULL) {
                gplog(LOG_INFO, "find %s in conf %s", target_buf, buf);
                dcmi_cfg_set_unlock(lock_fd);
                (void)fclose(fp);
                *flag = DCMI_EXIST_VNPU_CFG;
                return DCMI_OK;
            }
        }

        if (start_flag == DCMI_VNPU_FLAG_NOT_FIND) {
            start_flag = (strcmp(buf, "[vnpu-config start]\n") == 0) ? 1 : DCMI_VNPU_FLAG_NOT_FIND;
        }
        if (end_flag == DCMI_VNPU_FLAG_NOT_FIND) {
            end_flag = (strcmp(buf, "[vnpu-config end]\n") == 0) ? 1 : DCMI_VNPU_FLAG_NOT_FIND;
        }
    }

NO_EXIST:
    dcmi_cfg_set_unlock(lock_fd);
    if (fp != NULL) {
        (void)fclose(fp);
    }
    *flag = DCMI_NO_EXIST_VNPU_CFG;
    return DCMI_OK;
}

int dcmi_cfg_card_exist_vnpu_config(int card_id, int *flag)
{
    int ret;
    char target_buf[DCMI_VNPU_CONF_ONE_LINE_MAX_LEN] = {0};

    ret = snprintf_s(target_buf, sizeof(target_buf), sizeof(target_buf) - 1,
        "npu-smi set -t create-vnpu -i %d -c", card_id);
    if (ret <= 0) {
        gplog(LOG_ERR, "dcmi_cfg_card_exist_vnpu_config snprintf_s failed, ret is %d", ret);
        return DCMI_ERR_CODE_SECURE_FUN_FAIL;
    }
    ret = dcmi_cfg_exist_vnpu_config(target_buf, flag);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_cfg_card_exist_vnpu_config dcmi_cfg_card_exist_vnpu_config failed, ret %d", ret);
        return DCMI_ERR_CODE_INNER_ERR;
    }
    return DCMI_OK;
}

int dcmi_cfg_chip_exist_vnpu_config(int card_id, int chip_id, int *flag)
{
    int ret;
    char target_buf[DCMI_VNPU_CONF_ONE_LINE_MAX_LEN] = {0};

    ret = snprintf_s(target_buf, sizeof(target_buf), sizeof(target_buf) - 1,
        "npu-smi set -t create-vnpu -i %d -c %d", card_id, chip_id);
    if (ret <= 0) {
        gplog(LOG_ERR, "dcmi_cfg_chip_exist_vnpu_config snprintf_s failed, ret is %d", ret);
        return DCMI_ERR_CODE_SECURE_FUN_FAIL;
    }
    ret = dcmi_cfg_exist_vnpu_config(target_buf, flag);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_cfg_chip_exist_vnpu_config dcmi_cfg_exist_vnpu_config failed, ret %d", ret);
        return DCMI_ERR_CODE_INNER_ERR;
    }
    return DCMI_OK;
}

int dcmi_cfg_custom_op_check_cfg_path(char *path, unsigned int path_size, char *path_bak, unsigned int path_bak_size)
{
    int ret;

    ret = dcmi_cfg_custom_op_get_cfg_path(path, path_size, path_bak, path_bak_size);
    if (ret != DCMI_OK) {
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if (!dcmi_check_run_in_docker()) {
        ret = rename(path, path_bak);
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "rename error. errno is %d", errno);
            return DCMI_ERR_CODE_FILE_OPERATE_FAIL;
        }
    }

    return DCMI_OK;
}

int dcmi_cfg_custom_op_write_to_file(const char *buf, unsigned int buf_len)
{
#ifdef _WIN32
    return DCMI_ERR_CODE_NOT_SUPPORT;
#else
    FILE *fp = NULL;
    char path[PATH_MAX + 1] = {0x00};
    char path_bak[PATH_MAX + 1] = {0x00};
    unsigned int write_len;
    int fd_num, ret;

    if (buf == NULL) {
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    ret = dcmi_cfg_custom_op_check_cfg_path(path, sizeof(path), path_bak, sizeof(path_bak));
    if (ret != DCMI_OK) {
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    fp = fopen(path, "w");
    if (fp == NULL) {
        (void)rename(path_bak, path);
        return DCMI_ERR_CODE_FILE_OPERATE_FAIL;
    }
    fd_num = fileno(fp);
    if (fd_num >= 0) {
        (void)fchmod(fd_num, (S_IRUSR | S_IWUSR));
    }

    write_len = fwrite(buf, 1, buf_len, fp);
    if (write_len != buf_len) {
        gplog(LOG_ERR, "fwrite error. write_len is %u, buf_len is %u", write_len, buf_len);
        goto write_fail;
    }
    ret = fsync(fd_num);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "fsync error. errno is %d", errno);
        goto write_fail;
    }
    (void)fclose(fp);
    (void)unlink(path_bak);
    return DCMI_OK;

write_fail:
    (void)fclose(fp);
    (void)rename(path_bak, path);
    return DCMI_ERR_CODE_FILE_OPERATE_FAIL;
#endif
}

static int dcmi_cfg_custom_op_check_is_cover(const char *cmdline, char *buf_tmp)
{
    int ret = DCMI_CFG_DIFF_LINE;
    if (strstr(cmdline, buf_tmp) != NULL) {
        return DCMI_CFG_HAS_SAME_LINE;
    }

    // Compatible with earlier versions. must be overwritten.
    if (strncmp(cmdline, buf_tmp, strlen(buf_tmp) - DCMI_CFG_CUSTOM_OP_DLINE_LEN) == 0) {
        ret = DCMI_CFG_COVER_LINE;
    }

    /* Adapting Persistence Configuration of Privilege Containers */
    if (strlen(cmdline) < DCMI_CFG_CUSTOM_OP_CMD_LINE_LEN) {
        return DCMI_CFG_DIFF_LINE;
    }
    if (strlen(buf_tmp) < DCMI_CFG_CUSTOM_OP_CMD_LINE_LEN) {
        return DCMI_CFG_DIFF_LINE;
    }
    if (strncmp(cmdline, buf_tmp, DCMI_CFG_CUSTOM_OP_CMD_LINE_LEN) == 0) {
        ret = DCMI_CFG_COVER_LINE;
    }
    return ret;
}

static int dcmi_cfg_custom_op_get_action(unsigned int start, unsigned int end, const char *cmdline, char *buf_tmp)
{
    unsigned int card_id_tmp, chip_id_tmp, enable_type_tmp;
    unsigned int card_id_cmd, chip_id_cmd, enable_type_cmd;
    int ret;

    ret = sscanf_s(cmdline, "npu-smi set -t custom-op -i %u -c %u -d %u ", &card_id_cmd, &chip_id_cmd,
                   &enable_type_cmd);
    if (ret < 1) {
        gplog(LOG_ERR, "sscanf_s failed. ret is %d", ret);
        return DCMI_CFG_NOT_NEED_INSERT;
    }

    if (start != DCMI_CUSTOM_OP_FLAG_NOT_FIND) {
        if (end == DCMI_CUSTOM_OP_FLAG_NOT_FIND) {
            ret = dcmi_cfg_custom_op_check_is_cover(cmdline, buf_tmp);
            if (ret != DCMI_CFG_DIFF_LINE) {
                return ret;
            }

            ret = sscanf_s((const char *)buf_tmp, "npu-smi set -t custom-op -i %u -c %u -d %u ",
                           &card_id_tmp, &chip_id_tmp, &enable_type_tmp);
            if (ret < 1) {
                gplog(LOG_ERR, "sscanf_s failed. ret is %d", ret);
                return DCMI_ERR_CODE_CUSTOM_OP_CONFIG_ILLEGAL;
            }

            if (card_id_tmp > card_id_cmd) {
                return DCMI_CFG_NEED_INSERT;
            } else if ((card_id_tmp == card_id_cmd) && (chip_id_tmp > chip_id_cmd)) {
                return DCMI_CFG_NEED_INSERT;
            } else {
                return DCMI_CFG_NOT_NEED_INSERT;
            }
        } else {
            if (end - start >= 1) {
                return DCMI_CFG_NEED_INSERT;
            }
        }
    }
    return DCMI_CFG_NOT_NEED_INSERT;
}

int dcmi_cfg_check_custom_op_config_context_is_delete(unsigned int start, unsigned int end, unsigned int mode,
    unsigned set_flag, const char *buf)
{
    if ((!set_flag) && (strncmp(buf, "custom-op-recover:", strlen("custom-op-recover:")) == 0)) {
        return DCMI_CFG_NEED_INSERT;
    } else {
        if (mode == DCMI_CFG_RECOVER_ENABLE) {
            return DCMI_CFG_NOT_NEED_DELETE;
        } else {
            if (end == DCMI_VNPU_FLAG_NOT_FIND && start != DCMI_VNPU_FLAG_NOT_FIND) {
                return DCMI_CFG_NEED_DELETE;
            } else {
                return DCMI_CFG_NOT_NEED_DELETE;
            }
        }
    }
}

int dcmi_cfg_set_custom_op_recover_to_buffer(const char *cmdline, unsigned int mode, FILE *fp,
    char **buf_out, unsigned int *buf_len)
{
    unsigned int start_flag = DCMI_CUSTOM_OP_FLAG_NOT_FIND;
    unsigned int end_flag = DCMI_CUSTOM_OP_FLAG_NOT_FIND;
    char buf_tmp[DCMI_CUSTOM_OP_CONF_ONE_LINE_MAX_LEN] = {0};
    unsigned int out_file_len = 0, line = 0;
    unsigned int set_complete = FALSE;
    int ret, action;

    (void)fseek(fp, 0, SEEK_END);
    unsigned int buf_size = (unsigned int)ftell(fp) + DCMI_CUSTOM_OP_CONF_ONE_LINE_MAX_LEN + 1;
    rewind(fp);

    if (dcmi_cfg_malloc_buffer_and_init(buf_out, buf_size) != DCMI_OK) {
        return DCMI_ERR_CODE_INNER_ERR;
    }

    while (!feof(fp)) {
        (void)memset_s(buf_tmp, sizeof(buf_tmp), 0, sizeof(buf_tmp));
        char *str = fgets(buf_tmp, sizeof(buf_tmp), fp);
        if (str == NULL) {
            break;
        }

        line++;
        if (end_flag == DCMI_CUSTOM_OP_FLAG_NOT_FIND) {
            end_flag = (strcmp(buf_tmp, "[custom-op-config end]\n") == 0) ? (line - 1) : end_flag;
        }

        action = dcmi_cfg_check_custom_op_config_context_is_delete(start_flag, end_flag, mode, set_complete, buf_tmp);
        if (action == DCMI_CFG_NEED_DELETE) {
            continue;
        } else if (action == DCMI_CFG_NEED_INSERT) {
            ret = strncat_s(*buf_out, buf_size, cmdline, strlen(cmdline));
            out_file_len += strlen(cmdline);
            set_complete = TRUE;
        } else {
            ret = strncat_s(*buf_out, buf_size, buf_tmp, strlen(buf_tmp));
            out_file_len += strlen(buf_tmp);
        }

        if (start_flag == DCMI_CUSTOM_OP_FLAG_NOT_FIND) {
            start_flag = (strcmp(buf_tmp, "[custom-op-config start]\n") == 0) ? (line - 1) : start_flag;
        }

        if (ret != 0) {
            free(*buf_out);
            return DCMI_ERR_CODE_SECURE_FUN_FAIL;
        }
    }

    if (set_complete == FALSE) {
        free(*buf_out);
        gplog(LOG_OP, "The configuraion file has been modified unexpectedly.");
        return DCMI_ERR_CODE_CUSTOM_OP_CONFIG_ILLEGAL;
    }
    *buf_len = out_file_len;
    return DCMI_OK;
}
int dcmi_cfg_set_custom_op_config_recover_mode(unsigned int mode)
{
    int ret;
    char cmdline_buf[DCMI_CUSTOM_OP_CONF_ONE_LINE_MAX_LEN] = {0};
    FILE *fp = NULL;
    char *buf = NULL;
    int lock_fd;
    unsigned int buf_len = 0;
    ret = snprintf_s(cmdline_buf, sizeof(cmdline_buf), sizeof(cmdline_buf) - 1,
        "custom-op-recover:%s\n", (mode == DCMI_CFG_RECOVER_ENABLE) ? "enable" : "disable");
    if (ret <= 0) {
        gplog(LOG_ERR, "snprintf_s failed, ret is %d, mode is %u", ret, mode);
        return DCMI_ERR_CODE_SECURE_FUN_FAIL;
    }

    ret = dcmi_cfg_set_lock(&lock_fd, DCMI_CFG_GET_LOCK_TIMEOUT, DCMI_CFG_CUSTOM_OP_LOCK_FILE_NAME);
    if (ret != DCMI_OK) {
        return DCMI_ERR_CODE_RESOURCE_OCCUPIED;
    }

    ret = dcmi_cfg_custom_op_open_file(&fp);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_cfg_custom_op_open_file failed. ret is %d", ret);
        dcmi_cfg_set_unlock(lock_fd);
        return DCMI_ERR_CODE_FILE_OPERATE_FAIL;
    }

    ret = dcmi_cfg_set_custom_op_recover_to_buffer(cmdline_buf, mode, fp, &buf, &buf_len);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_cfg_set_recover_to_buffer cmdline_buf %s failed, ret is %d", cmdline_buf, ret);
        fclose(fp);
        dcmi_cfg_set_unlock(lock_fd);
        return ret;
    }

    (void)fclose(fp);
    ret = dcmi_cfg_custom_op_write_to_file(buf, buf_len);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_cfg_custom_op_write_to_file failed, ret is %d", ret);
    }

    free(buf);
    dcmi_cfg_set_unlock(lock_fd);
    return ret;
}

int dcmi_cfg_get_custom_op_config_recover_mode(unsigned int *mode)
{
    int ret;
    char buf_tmp[DCMI_CUSTOM_OP_CONF_ONE_LINE_MAX_LEN] = {0};
    FILE *fp = NULL;
    char *str = NULL;
    int lock_fd;

    ret = dcmi_cfg_set_lock(&lock_fd, DCMI_CFG_GET_LOCK_TIMEOUT, DCMI_CFG_CUSTOM_OP_LOCK_FILE_NAME);
    if (ret != DCMI_OK) {
        return DCMI_ERR_CODE_RESOURCE_OCCUPIED;
    }

    ret = dcmi_cfg_custom_op_open_file(&fp);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_cfg_custom_op_open_file failed. ret is %d", ret);
        dcmi_cfg_set_unlock(lock_fd);
        return DCMI_ERR_CODE_FILE_OPERATE_FAIL;
    }

    ret = DCMI_ERR_CODE_CUSTOM_OP_CONFIG_ILLEGAL;
    while (!feof(fp)) {
        (void)memset_s(buf_tmp, sizeof(buf_tmp), 0, sizeof(buf_tmp));
        str = fgets(buf_tmp, sizeof(buf_tmp), fp);
        if (str == NULL) {
            break;
        }

        if (strncmp(buf_tmp, "custom-op-recover:enable", strlen("custom-op-recover:enable")) == 0) {
            *mode = DCMI_CFG_RECOVER_ENABLE;
            ret = DCMI_OK;
        } else if (strncmp(buf_tmp, "custom-op-recover:disable", strlen("custom-op-recover:disable")) == 0) {
            *mode = DCMI_CFG_RECOVER_DISABLE;
            ret = DCMI_OK;
        }
        if (ret == DCMI_OK) {
            fclose(fp);
            dcmi_cfg_set_unlock(lock_fd);
            return DCMI_OK;
        }
    }
    (void)fclose(fp);
    dcmi_cfg_set_unlock(lock_fd);
    gplog(LOG_OP, "The configuration file was modified unexpectedly.");
    return ret;
}

int dcmi_cfg_insert_custom_op_cmdline_to_buffer(const char *cmdline, FILE *fp, char **buf_out, unsigned int *len)
{
    int ret, action;
    char buf_tmp[DCMI_CUSTOM_OP_CONF_ONE_LINE_MAX_LEN] = {0};
    unsigned int insert_flag = 0, line = 0, start_flag = 0, end_flag = 0;
    struct cfg_buf_info buf_info;

    (void)fseek(fp, 0, SEEK_END);
    buf_info.buf_size = (unsigned int)ftell(fp) + DCMI_CUSTOM_OP_CONF_ONE_LINE_MAX_LEN + 1;
    rewind(fp);
    if (dcmi_cfg_malloc_buffer_and_init(buf_out, buf_info.buf_size) != DCMI_OK) {
        return DCMI_ERR_CODE_INNER_ERR;
    }
    buf_info.buf = *buf_out;

    while (!feof(fp)) {
        (void)memset_s(buf_tmp, sizeof(buf_tmp), 0, sizeof(buf_tmp));
        char *str = fgets(buf_tmp, sizeof(buf_tmp), fp);
        if (str == NULL) {
            break;
        }
        line++;
        
        if (insert_flag != DCMI_CFG_INSERT_COMPLETE) {
            if (end_flag == DCMI_CUSTOM_OP_FLAG_NOT_FIND) {
                end_flag = (strcmp(buf_tmp, "[custom-op-config end]\n") == 0) ? (line - 1) : end_flag;
            }

            action = dcmi_cfg_custom_op_get_action(start_flag, end_flag, cmdline, buf_tmp);
            if (action == DCMI_ERR_CODE_CUSTOM_OP_CONFIG_ILLEGAL) {
                return DCMI_ERR_CODE_CUSTOM_OP_CONFIG_ILLEGAL;
            }
            ret = dcmi_cfg_process_action(action, &buf_info, len, cmdline, buf_tmp);
            insert_flag = ((ret == DCMI_CFG_INSERT_OK) ? DCMI_CFG_INSERT_COMPLETE : insert_flag);
            if (ret == DCMI_ERR_CODE_SECURE_FUN_FAIL) {
                goto SECURE_FUN_FAIL;
            }
            if (start_flag == DCMI_CUSTOM_OP_FLAG_NOT_FIND) {
                start_flag = (strcmp(buf_tmp, "[custom-op-config start]\n") == 0) ? (line - 1) : start_flag;
            }
        } else {
            ret = strncat_s(*buf_out, buf_info.buf_size, buf_tmp, strlen(buf_tmp));
            if (ret != 0) {
                goto SECURE_FUN_FAIL;
            }
            *len += strlen(buf_tmp);
        }
    }

    ret = (start_flag == 0) ? DCMI_ERR_CODE_CUSTOM_OP_CONFIG_ILLEGAL : DCMI_OK;
    return ret;
SECURE_FUN_FAIL:
    gplog(LOG_ERR, "strncat_s failed. ret is %d", ret);
    return DCMI_ERR_CODE_SECURE_FUN_FAIL;
}

int dcmi_cfg_insert_set_custom_op_cmdline(int card_id, int chip_id, int enable_value)
{
    char *buf = NULL;
    unsigned int buf_len = 0;
    int ret, lock_fd;
    char cmdline_buf[DCMI_CUSTOM_OP_CONF_ONE_LINE_MAX_LEN] = {0};
    FILE *fp = NULL;

    ret = snprintf_s(cmdline_buf, sizeof(cmdline_buf), sizeof(cmdline_buf) - 1,
        "npu-smi set -t custom-op -i %d -c %d -d %d\n",
         card_id, chip_id, enable_value);
    if (ret <= 0) {
        gplog(LOG_ERR, "snprintf_s failed, ret is %d", ret);
        return DCMI_ERR_CODE_SECURE_FUN_FAIL;
    }

    ret = dcmi_cfg_set_lock(&lock_fd, DCMI_CFG_GET_LOCK_TIMEOUT, DCMI_CFG_CUSTOM_OP_LOCK_FILE_NAME);
    if (ret != DCMI_OK) {
        return DCMI_ERR_CODE_RESOURCE_OCCUPIED;
    }

    ret = dcmi_cfg_custom_op_open_file(&fp);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_cfg_custom_op_open_file failed. ret is %d", ret);
        dcmi_cfg_set_unlock(lock_fd);
        return DCMI_ERR_CODE_FILE_OPERATE_FAIL;
    }

    ret = dcmi_cfg_insert_custom_op_cmdline_to_buffer(cmdline_buf, fp, &buf, &buf_len);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_cfg_insert_custom_op_cmdline_to_buffer cmdline_buf %s failed. ret is %d",
              cmdline_buf, ret);
        if (ret == DCMI_ERR_CODE_CUSTOM_OP_CONFIG_ILLEGAL) {
            gplog(LOG_OP, "The configuraion file has been modified unexpectedly.");
        }
        fclose(fp);
        dcmi_cfg_set_unlock(lock_fd);
        return ret;
    }

    (void)fclose(fp);
    ret = dcmi_cfg_custom_op_write_to_file(buf, buf_len);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_cfg_write_to_file failed, ret is %d", ret);
    }

    free(buf);
    dcmi_cfg_set_unlock(lock_fd);
    return ret;
}