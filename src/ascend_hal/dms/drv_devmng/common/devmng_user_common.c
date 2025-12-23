/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "devmng_common.h"
#include "dmc_user_interface.h"
#include "devmng_user_common.h"

int dmanage_common_ioctl(int ioctl_cmd, void *ioctl_arg)
{
    int ret;
    int fd = -1;
    int err_buf;

    fd = devdrv_open_device_manager();
    if ((int)fd == -DRV_ERROR_RESOURCE_OCCUPIED) {
        DEVDRV_DRV_ERR("Device is busy. (ret=%d)\n", (int)fd);
        return DRV_ERROR_RESOURCE_OCCUPIED;
    }
    if (fd < 0) {
        DEVDRV_DRV_ERR("open devdrv_device manager failed, fd(%d).\n", fd);
        return DRV_ERROR_INVALID_HANDLE;
    }

    ret = ioctl(fd, ioctl_cmd, ioctl_arg);
    /* Attention: dmanage_share_log_read or other function may change the errno. */
    err_buf = errno;
#ifdef __linux
    if (ret != 0) {
        dmanage_share_log_read();
    }
#endif
    return ret == -1 ? errno_to_user_errno(err_buf) : errno_to_user_errno(ret);
}

int dmanage_mmIoctl(mmProcess fd, signed int ioctl_code, mmIoctlBuf *buf_ptr)
{
    int ret;
    int err_buf;

    ret = mmIoctl(fd, ioctl_code, buf_ptr);
    /* Attention: dmanage_share_log_read or other function may change the errno. */
    err_buf = errno;
#ifdef __linux
    if (ret != 0) {
        dmanage_share_log_read();
    }
#endif
    errno = err_buf;
    return ret;
}

void dmanage_share_log_create(void)
{
#ifdef CFG_FEATURE_SHARE_LOG
    share_log_create(HAL_MODULE_TYPE_DEV_MANAGER, SHARE_LOG_MAX_SIZE);
#endif
}

void dmanage_share_log_destroy(void)
{
#ifdef CFG_FEATURE_SHARE_LOG
    dmanage_share_log_read();
    share_log_destroy(HAL_MODULE_TYPE_DEV_MANAGER);
#endif
}

void dmanage_share_log_read(void)
{
#ifdef CFG_FEATURE_SHARE_LOG
    share_log_read_err(HAL_MODULE_TYPE_DEV_MANAGER);
#endif
}
