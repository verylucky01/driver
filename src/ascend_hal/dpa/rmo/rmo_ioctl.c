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
#include <unistd.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <sys/types.h>

#include "securec.h"
#include "davinci_interface.h"
#include "rmo.h"

#ifdef EMU_ST
int rmo_file_open(const char *pathname, int flags);
int rmo_file_close(int fd);
int rmo_file_ioctl(int fd, unsigned long cmd, void *para);
#else
#define rmo_file_open open
#define rmo_file_close close
#define rmo_file_ioctl(fd, cmd, ...) ioctl((fd), (cmd), ##__VA_ARGS__)
#endif

static THREAD__ int rmo_fd = -1;
static THREAD__ pid_t rmo_cur_pid = -1;
static pthread_mutex_t rmo_lock = PTHREAD_MUTEX_INITIALIZER;

int rmo_get_fd(void)
{
    return rmo_fd;
}

static void rmo_char_dev_set_close_exe(int fd)
{
    int flags;

    flags = fcntl(fd, F_GETFD);
    flags = (int)((unsigned int)flags | FD_CLOEXEC);
    (void)fcntl(fd, F_SETFD, flags);
}

static int rmo_char_dev_open(void)
{
    struct davinci_intf_open_arg arg;
    int fd = -1, ret;

    ret = strcpy_s(arg.module_name, DAVINIC_MODULE_NAME_MAX, RMO_CHAR_DEV_NAME);
    if (ret != EOK) {
        rmo_err("Strcpy fail. (ret=%d; name=%s)\n", ret, RMO_CHAR_DEV_NAME);
        return ret;
    }
    fd = rmo_file_open(davinci_intf_get_dev_path(), O_RDWR);
    if (fd < 0) {
        rmo_err("Open char dev fail. (errno=%d; name=%s)\n", errno, RMO_CHAR_DEV_NAME);
        return DRV_ERROR_INNER_ERR;
    }
    ret = rmo_file_ioctl(fd, DAVINCI_INTF_IOCTL_OPEN, &arg);
    if (ret != 0) {
        rmo_err("Open ioctl fail. (ret=%d; erron=%d)\n", ret, errno);
        rmo_file_close(fd);
        return ret;
    }

    rmo_char_dev_set_close_exe(fd);
    rmo_fd = fd;

    return 0;
}

static void rmo_char_dev_close(void)
{
    struct davinci_intf_open_arg arg;
    int ret;

    ret = strcpy_s(arg.module_name, DAVINIC_MODULE_NAME_MAX, RMO_CHAR_DEV_NAME);
    if (ret != EOK) {
        rmo_err("Strcpy fail. (ret=%d; name=%s)\n", ret, RMO_CHAR_DEV_NAME);
    } else {
        ret = rmo_file_ioctl(rmo_fd, DAVINCI_INTF_IOCTL_CLOSE, &arg);
        if (ret != 0) {
#ifndef EMU_ST
            rmo_err("Close ioctl fail. (fd=%d; ret=%d; erron=%d)\n", rmo_fd, ret, errno);
#endif
        }
    }

    rmo_file_close(rmo_fd);
    rmo_fd = -1;
}

int rmo_cmd_ioctl(unsigned long cmd, void *para)
{
    int ret;

    (void)pthread_mutex_lock(&rmo_lock);
    if (rmo_fd < 0 || rmo_cur_pid != getpid()) {
        ret = rmo_char_dev_open();
        if (ret != 0) {
            (void)pthread_mutex_unlock(&rmo_lock);
            return ret;
        }
        rmo_cur_pid = getpid();
    }
    (void)pthread_mutex_unlock(&rmo_lock);

    do {
        ret = rmo_file_ioctl(rmo_fd, cmd, para);
    } while ((ret == -1) && (errno == EINTR));

    if (ret < 0) {
        return errno_to_user_errno(ret);
    }

    return DRV_ERROR_NONE;
}

void rmo_user_uninit(void)
{
    if (rmo_fd != -1) {
        rmo_char_dev_close();
    }
}
