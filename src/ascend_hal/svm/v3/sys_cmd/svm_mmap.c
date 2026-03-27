/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <errno.h>

#include "securec.h"
#include "davinci_interface.h"
#include "pbl_uda_user.h"

#include "svm_user_adapt.h"
#include "svm_log.h"
#include "svm_pub.h"
#include "svm_ioctl_ex.h"

static int svm_mmap_fd = -1;
static pthread_mutex_t svm_mmap_fd_mutex = PTHREAD_MUTEX_INITIALIZER;

static int _svm_mmap_char_dev_open(int fd, struct davinci_intf_open_arg *arg)
{
    int ret;

    /* May be interrupted by signals, should retry. */
    do {
        ret = svm_user_ioctl(fd, DAVINCI_INTF_IOCTL_OPEN, arg);
    } while ((ret != 0) && (errno == EINTR));

    return (ret == 0) ? DRV_ERROR_NONE : errno_to_user_errno(errno);
}

static int svm_mmap_char_dev_open(int *fd)
{
    struct davinci_intf_open_arg arg;
    int tmp_errno, ret;

    arg.device_id = 0;
    ret = strcpy_s(arg.module_name, DAVINIC_MODULE_NAME_MAX, SVM_MMAP_CHAR_DEV_NAME);
    if (ret < 0) {
        svm_err("Strcpy_s failed. (ret=%d)\n", ret);
        return DRV_ERROR_INNER_ERR;
    }

    *fd = svm_file_open(davinci_intf_get_dev_path(), O_RDWR | O_CLOEXEC);
    if (*fd < 0) {
        tmp_errno = errno;
        svm_err("Open device failed. (fd=%d; errno=%d, error=%s)\n", *fd, tmp_errno, strerror(tmp_errno));
        return errno_to_user_errno(tmp_errno);
    }

    ret = _svm_mmap_char_dev_open(*fd, &arg);
    if (ret != 0) {
        svm_err("Ioctl failed. (fd=%d; errno=%d, error=%s)\n", *fd, errno, strerror(errno));
        svm_file_close(*fd);
        return ret;
    }

    return 0;
}

static int svm_mmap_char_dev_close(int fd)
{
    struct davinci_intf_close_arg arg;
    int tmp_errno;
    int ret;

    arg.device_id = 0;
    ret = strcpy_s(arg.module_name, DAVINIC_MODULE_NAME_MAX, SVM_MMAP_CHAR_DEV_NAME);
    if (ret < 0) {
        svm_err("Strcpy_s failed. (ret=%d)\n", ret);
        return DRV_ERROR_INNER_ERR;
    }

    ret = svm_user_ioctl(fd, DAVINCI_INTF_IOCTL_CLOSE, &arg);
    if (ret != 0) {
        tmp_errno = errno;
        svm_warn("Ioctl warn. (fd=%d; errno=%d, error=%s)\n", fd, tmp_errno, strerror(tmp_errno));
    }

    svm_file_close(fd);

    return (ret == 0) ? DRV_ERROR_NONE : errno_to_user_errno(tmp_errno);
}

static int svm_mmap_init(void)
{
    int ret;

    if (svm_mmap_fd >= 0) {
        return 0;
    }

    (void)pthread_mutex_lock(&svm_mmap_fd_mutex);
    if (svm_mmap_fd >= 0) {
        (void)pthread_mutex_unlock(&svm_mmap_fd_mutex);
        return 0;
    }

    ret = svm_mmap_char_dev_open(&svm_mmap_fd);
    (void)pthread_mutex_unlock(&svm_mmap_fd_mutex);

    return ret;
}

void *svm_cmd_mmap(void *addr, size_t length, int prot, int flags, off_t offset)
{
    int ret = svm_mmap_init();
    if (ret != 0) {
        return NULL;
    }

    return svm_user_mmap(addr, length, prot, flags, svm_mmap_fd, offset);
}

int svm_cmd_munmap(void *addr, size_t length)
{
    int ret = svm_mmap_init();
    if (ret != 0) {
        return ret;
    }

    return svm_user_munmap(addr, length);
}

void __attribute__((destructor)) svm_mmap_uninit(void)
{
    if (svm_mmap_fd >= 0) {
        (void)svm_mmap_char_dev_close(svm_mmap_fd);
        svm_mmap_fd = -1;
    }
}

