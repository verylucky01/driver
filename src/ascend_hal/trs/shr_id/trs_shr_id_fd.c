/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <fcntl.h>
#include <sys/ioctl.h>
#include <pthread.h>

#include "securec.h"
#include "davinci_interface.h"

#include "trs_shr_id_ioctl.h"
#include "trs_user_pub_def.h"

#ifdef EMU_ST
#define THREAD__  __thread
#else
#define THREAD__
#endif

static pthread_mutex_t shrid_device_mutex = PTHREAD_MUTEX_INITIALIZER;

static THREAD__ int shrid_fd = -1;
static THREAD__ pid_t shrid_pid = -1;

#ifndef EMU_ST
static int _shrid_open(const char *pathname, int flags)
{
    return open(pathname, flags);
}

static int _shrid_close(int fd)
{
    return close(fd);
}

static int _shrid_ioctl(int fd, unsigned int cmd, void *para)
{
    return ioctl(fd, cmd, para);
}
#else
int _shrid_open(const char *pathname, int flags);
int _shrid_close(int fd);
int _shrid_ioctl(int fd, unsigned int cmd, void *para);
#endif /* EMU_ST */

static void shrid_set_pid(pid_t pid)
{
    shrid_pid = pid;
}

static int shrid_get_pid(void)
{
    return shrid_pid;
}

void shrid_set_fd(int fd)
{
    shrid_fd = fd;
}

static int _shrid_get_fd(void)
{
    if ((shrid_fd >= 0) && (shrid_get_pid() == getpid())) {
        return shrid_fd;
    }

    if (shrid_fd >= 0) {
        (void)_shrid_close(shrid_fd);
    }

    return -1;
}

static int shrid_ioctl_open(void)
{
    struct davinci_intf_open_arg arg = {};
    int ret, fd;

    ret = strcpy_s(arg.module_name, DAVINIC_MODULE_NAME_MAX, DAVINCI_INTF_MODULE_TRS_SHR_ID);
    if (ret < 0) {
        trs_err("strcpy_s failed, ret(%d).\n", ret);
        return -1;
    }

    fd = _shrid_open(davinci_intf_get_dev_path(), O_RDWR | O_CLOEXEC);
    if (fd < 0) {
        trs_err("open failed, fd(%d), errno(%d), error(%s).\n", fd, errno, strerror(errno));
        return fd;
    }
    ret = _shrid_ioctl(fd, DAVINCI_INTF_IOCTL_OPEN, &arg);
    if (ret != 0) {
        trs_err("ioctl failed, fd(%d), errno(%d), error(%s).\n", fd, errno, strerror(errno));
        (void)_shrid_close(fd);
        fd = -1;
    }
    return fd;
}

static int shrid_open(void)
{
    int fd = -1;

    fd = _shrid_get_fd();
    if (fd < 0) {
        fd = shrid_ioctl_open();
        if (fd < 0) {
            trs_err("open failed, fd(%d), errno(%d), error(%s).\n", fd, errno, strerror(errno));
            return -1;
        }
        shrid_set_fd(fd);

        int flags = fcntl(fd, F_GETFD);
        flags = (int)((unsigned int)flags | FD_CLOEXEC);
        (void)fcntl(fd, F_SETFD, flags);
        shrid_set_pid(getpid());
    }

    return fd;
}

static int shrid_get_fd(void)
{
    int fd;

    (void)pthread_mutex_lock(&shrid_device_mutex);
    fd = _shrid_get_fd();
    if (fd <= 0) {
        fd = shrid_open();
    }
    (void)pthread_mutex_unlock(&shrid_device_mutex);

    return fd;
}

drvError_t shrid_ioctl(u32 cmd, void *para)
{
    int fd, ret;

    fd = shrid_get_fd();
    if (fd < 0) {
        trs_err("Open failed. (fd=%d; cmd=%u)\n", fd, _IOC_NR(cmd));
        return DRV_ERROR_OPEN_FAILED;
    }

    ret = _shrid_ioctl(fd, cmd, para);
    if (ret != 0) {
        if (errno == EOPNOTSUPP) {
            return DRV_ERROR_NOT_SUPPORT;
        } else if (errno == EPERM) {
            return DRV_ERROR_OPER_NOT_PERMITTED;
        } else {
            trs_err("Ioctl failed. (ret=%d; cmd=%u).\n", errno, _IOC_NR(cmd));
            return DRV_ERROR_IOCRL_FAIL;
        }
    }
    return DRV_ERROR_NONE;
}

void shrid_close(void)
{
    int fd = -1;

    fd = _shrid_get_fd();
    if (fd < 0) {
        return;
    }

    (void)_shrid_close(fd);
}
