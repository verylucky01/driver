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
#include <sys/types.h>
#include <pthread.h>

#include "securec.h"
#include "ascend_hal.h"
#include "davinci_interface.h"
#include "dp_proc_mng_ioctl.h"
#include "dp_adapt.h"
#include "dp_proc_mng.h"

static THREAD int g_dp_proc_mng_fd = -1;
static THREAD int g_dp_proc_mng_pid = -1;

#ifdef EMU_ST
int dp_proc_fie_open(const char *pathname, int flags);
int dp_proc_file_close(int fd);
int dp_proc_ioctl(int fd, unsigned long cmd, void *para);
int dp_proc_get_pid(void);
#else
#define dp_proc_fie_open open
#define dp_proc_file_close close
#define dp_proc_ioctl(fd, cmd, ...) ioctl((fd), (unsigned int)(cmd), ##__VA_ARGS__)
#define dp_proc_get_pid getpid
#endif

static int dp_proc_mng_open_dev(void)
{
    struct davinci_intf_open_arg arg = {0};
    int ret, fd, flags;

    ret = strcpy_s(arg.module_name, DAVINIC_MODULE_NAME_MAX, DAVINCI_DP_PROC_MNG_SUB_MODULE_NAME);
    if (ret != 0) {
        DP_PROC_MNG_ERR("strcpy_s failed. (ret=%d)\n", ret);
        return -1;
    }

    fd = dp_proc_fie_open(davinci_intf_get_dev_path(), O_RDWR | O_SYNC | O_CLOEXEC);
    if (fd < 0) {
        DP_PROC_MNG_ERR("open dev=%d.\n", fd);
        return fd;
    }

    flags = fcntl(fd, F_GETFD);
    flags = (int)((unsigned int)flags | FD_CLOEXEC);
    (void)fcntl(fd, F_SETFD, flags);

    ret = dp_proc_ioctl(fd, DAVINCI_INTF_IOCTL_OPEN, &arg);
    if (ret != 0) {
        DP_PROC_MNG_ERR("DAVINCI_INTF_IOCTL_OPEN failed. (ret=%d)\n", ret);
        (void)dp_proc_file_close(fd);
        return -1;
    }

    return fd;
}

STATIC void dp_proc_mng_close_dev(void)
{
#ifndef EMU_ST
    struct davinci_intf_close_arg arg = {0};
    int ret;

    ret = strcpy_s(arg.module_name, DAVINIC_MODULE_NAME_MAX, DAVINCI_DP_PROC_MNG_SUB_MODULE_NAME);
    if (ret != 0) {
        DP_PROC_MNG_ERR("strcpy_s failed. (ret=%d)\n", ret);
        (void)dp_proc_file_close(g_dp_proc_mng_fd);
        return;
    }

    ret = dp_proc_ioctl(g_dp_proc_mng_fd, DAVINCI_INTF_IOCTL_CLOSE, &arg);
    if (ret != 0) {
        DP_PROC_MNG_ERR("DAVINCI_INTF_IOCTL_CLOSE failed. (ret=%d)\n", ret);
    }

    (void)dp_proc_file_close(g_dp_proc_mng_fd);
    g_dp_proc_mng_fd = -1;
#endif
}

int dp_proc_mng_get_fd_inner(void)
{
    static pthread_mutex_t dp_fd_mutex = PTHREAD_MUTEX_INITIALIZER;

    (void)pthread_mutex_lock(&dp_fd_mutex);
    if ((g_dp_proc_mng_fd > 0) && (g_dp_proc_mng_pid == dp_proc_get_pid())) {
        (void)pthread_mutex_unlock(&dp_fd_mutex);
        return g_dp_proc_mng_fd;
    }

    if (g_dp_proc_mng_fd > 0) {
        dp_proc_mng_close_dev();
    }

    g_dp_proc_mng_fd = dp_proc_mng_open_dev();
    if (g_dp_proc_mng_fd < 0) {
        DP_PROC_MNG_ERR("Open dev file failed. (errno=%d)\n", errno);
    } else {
        g_dp_proc_mng_pid = dp_proc_get_pid();
    }
    (void)pthread_mutex_unlock(&dp_fd_mutex);

    return g_dp_proc_mng_fd;
}

drvError_t dp_proc_mng_ioctl(unsigned int cmd, void *para)
{
    int fd = dp_proc_mng_get_fd_inner();
    int ret;

    if (fd < 0) {
        DP_PROC_MNG_ERR("DpProcMng fd not init.\n");
        return DRV_ERROR_INVALID_DEVICE;
    }

    ret = dp_proc_ioctl(fd, cmd, para);
    if (ret != 0) {
        DP_PROC_MNG_ERR("ioctl failed. (cmd=%x; error=%d) \n", cmd, errno);
        return DRV_ERROR_IOCRL_FAIL;
    }

    return DRV_ERROR_NONE;
}

drvError_t __attribute__((weak)) halBindCgroup(BIND_CGROUP_TYPE bindType)
{
    struct dp_proc_mng_ioctl_arg dp_arg = {0};
    drvError_t ret_val;

    if (!dp_proc_support_bind_cgroup()) {
        DP_PROC_MNG_INFO("Not support yet.\n");
        return DRV_ERROR_NOT_SUPPORT;
    }

    if (bindType >= BIND_CGROUP_MAX_TYPE) {
        DP_PROC_MNG_INFO("Not support bind Cgroup this type. (type=%u)\n", bindType);
        return DRV_ERROR_NOT_SUPPORT;
    }

    dp_arg.data.bind_cgroup_para.bind_type = bindType;
    ret_val = dp_proc_mng_ioctl(DP_PROC_MNG_BIND_CGROUP, &dp_arg);
    if (ret_val != DRV_ERROR_NONE) {
        DP_PROC_MNG_ERR("Bind Cgroup failed. (ret=%d)\n", ret_val);
    }

    return ret_val;
}

