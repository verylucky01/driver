/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include "securec.h"

#include "davinci_interface.h"
#include "apm_ioctl.h"

#ifdef EMU_ST
int apm_file_open(const char *pathname, int flags);
int apm_file_close(int fd);
int apm_file_ioctl(int fd, unsigned long cmd, void *para);
#else
#define apm_file_open open
#define apm_file_close close
static inline int apm_file_ioctl(int fd, unsigned long cmd, void *arg) {
    int ret = ioctl(fd, cmd, arg);
    if (ret != 0) {
        share_log_read_err(HAL_MODULE_TYPE_APM);
    }
    return ret;
}
#endif

static THREAD__ int apm_fd = -1;
static THREAD__ pid_t apm_cur_pid = -1;
static pthread_mutex_t apm_lock = PTHREAD_MUTEX_INITIALIZER;

void apm_char_dev_set_close_exe(int fd)
{
    int flags;

    flags = fcntl(fd, F_GETFD);
    flags = (int)((unsigned int)flags | FD_CLOEXEC);
    (void)fcntl(fd, F_SETFD, flags);
}

int apm_char_dev_open(void)
{
    struct davinci_intf_open_arg arg;
    int fd = -1, ret;

    ret = strcpy_s(arg.module_name, DAVINIC_MODULE_NAME_MAX, APM_CHAR_DEV_NAME);
    if (ret != EOK) {
        apm_err("Strcpy fail. (ret=%d; name=%s)\n", ret, APM_CHAR_DEV_NAME);
        return ret;
    }
    fd = apm_file_open(davinci_intf_get_dev_path(), O_RDWR);
    share_log_read_run_info(HAL_MODULE_TYPE_APM);
    if (fd < 0) {
        apm_err("Open char dev fail. (errno=%d; name=%s)\n", errno, APM_CHAR_DEV_NAME);
        return DRV_ERROR_INNER_ERR;
    }
    ret = apm_file_ioctl(fd, DAVINCI_INTF_IOCTL_OPEN, &arg);
    if (ret != 0) {
        apm_err("Open ioctl fail. (ret=%d; erron=%d)\n", ret, errno);
        apm_file_close(fd);
        return ret;
    }

    apm_char_dev_set_close_exe(fd);
    apm_fd = fd;

    return 0;
}

void apm_char_dev_close(void)
{
    struct davinci_intf_open_arg arg;
    int ret;

    ret = strcpy_s(arg.module_name, DAVINIC_MODULE_NAME_MAX, APM_CHAR_DEV_NAME);
    if (ret != EOK) {
        apm_err("Strcpy fail. (ret=%d; name=%s)\n", ret, APM_CHAR_DEV_NAME);
    } else {
        ret = apm_file_ioctl(apm_fd, DAVINCI_INTF_IOCTL_CLOSE, &arg);
        if (ret != 0) {
#ifndef EMU_ST
            apm_err("Close ioctl fail. (fd=%d; ret=%d; erron=%d)\n", apm_fd, ret, errno);
#endif
        }
    }

    apm_file_close(apm_fd);
    apm_fd = -1;
}

int apm_cmd_ioctl(unsigned long cmd, void *para)
{
    int ret;

    (void)pthread_mutex_lock(&apm_lock);
    if (apm_fd < 0 || apm_cur_pid != getpid()) {
        share_log_create(HAL_MODULE_TYPE_APM, SHARE_LOG_MAX_SIZE);
        ret = apm_char_dev_open();
        if (ret != 0) {
            share_log_destroy(HAL_MODULE_TYPE_APM);
            (void)pthread_mutex_unlock(&apm_lock);
            return ret;
        }
        apm_cur_pid = getpid();
    }
    (void)pthread_mutex_unlock(&apm_lock);

    do {
        ret = apm_file_ioctl(apm_fd, cmd, para);
    } while ((ret == -1) && (errno == EINTR));

    if (ret < 0) {
        if (errno == ENODEV) {
            return DRV_ERROR_NO_DEVICE;
        } else if (errno == ENOMEM) {
            return DRV_ERROR_OUT_OF_MEMORY;
        } else if (errno == ESRCH) {
            return DRV_ERROR_NO_PROCESS;
        } else if (errno == EFAULT) {
            return DRV_ERROR_INNER_ERR;
        } else if (errno == EPERM) {
            return DRV_ERROR_OPER_NOT_PERMITTED;
        } else if (errno == ENXIO) {
            return DRV_ERROR_NOT_EXIST;
        } else if (errno == ERANGE) {
            return DRV_ERROR_PARA_ERROR;
        } else if (errno == EEXIST) {
            return DRV_ERROR_REPEATED_INIT;
        } else if (errno == EOPNOTSUPP) {
            return DRV_ERROR_NOT_SUPPORT;
        } else {
            return DRV_ERROR_IOCRL_FAIL;
        }
    }

    return DRV_ERROR_NONE;
}

void apm_user_uninit(void)
{
    if (apm_fd != -1) {
        apm_char_dev_close();
    }
}

