/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include <sys/ioctl.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include "dms/dms_misc_interface.h"
#include "dms_cmd_def.h"
#include "ascend_kernel_hal.h"
#include "dms_user_common.h"
#include "devdrv_ioctl.h"
#include "devmng_user_common.h"
#include "securec.h"
#include "ascend_dev_num.h"

#define DMS_LOG_BUF_SIZE     0x1400000
#define DMS_DIR_MAXLEN       256U
#define DMS_LOG_FILE_NAME    "host_kernel.log"
struct LogInfo {
    void *buffer;
    u32 length;
    u32 buf_len;
};

static int drvKlogSaveFs(const void *buffer, u32 len, const char *logPath)
{
#ifndef DMS_UT
    char fullPath[DMS_DIR_MAXLEN] = { 0 };
    ssize_t cnt = 0;
    int fd = -1;
    int ret;

    if (len == 0) {
        DMS_ERR("Invalid len to save file. (len=%u, path=%s)\n", len, logPath);
        return DRV_ERROR_PARA_ERROR;
    }

    ret = sprintf_s(fullPath, DMS_DIR_MAXLEN, "%s/%s", logPath, DMS_LOG_FILE_NAME);
    if (ret == -1) {
        DMS_ERR("Format full path failed. (dir=%s, fileName=%s)\n", logPath, DMS_LOG_FILE_NAME);
        return DRV_ERROR_INVALID_VALUE;
    }

    fd = open(fullPath, (O_CREAT | O_RDWR | O_TRUNC), S_IRUSR | S_IWUSR);
    if (fd < 0) {
        DMS_ERR("Open failed. (errno=%s, path=%s)\n", strerror(errno), fullPath);
        return DRV_ERROR_INVALID_VALUE;
    }

    cnt = write(fd, buffer, len);
    if (cnt < 0) {
        DMS_ERR("Write failed. (errno=%s, path=%s)\n", strerror(errno), fullPath);
        ret = DRV_ERROR_INVALID_VALUE;
        goto cleanup;
    }

    if (cnt != len) {
        DMS_WARN("Write file return bad write length. (path=%s, ret=%d, len=%u)", fullPath, ret, len);
    }

    if (fsync(fd) != 0) {
        DMS_ERR("Sync failed. (errno=%s, path=%s)\n", strerror(errno), fullPath);
        ret = DRV_ERROR_INVALID_VALUE;
        goto cleanup;
    }

    ret = DRV_ERROR_NONE;
cleanup:
    close(fd);
    fd = -1;
    return ret;
#else
    return DRV_ERROR_NONE;
#endif
}

static bool drvIsVritEnv(void)
{
#ifdef CFG_FEATURE_VDEVMNG_IN_VIRTUAL
    return DmsGetVirtFlag() != 0 ? true : false;
#else
    return false;
#endif
}

int drvGetKlogBuf(uint32_t devId, const char *path, unsigned int *pSize)
{
    unsigned int maxLen = DMS_DIR_MAXLEN - strlen(DMS_LOG_FILE_NAME);
    struct dms_ioctl_arg ioarg = { 0 };
    struct LogInfo log_info = { 0 };
    int ret;

    if ((devId >= ASCEND_PDEV_MAX_NUM) || (path == NULL) || (pSize == NULL)) {
        DMS_ERR("Invalid parameters. (devId=%u, buf%s, p_size%s)\n",
            devId, path == NULL ? "=NULL" : "!=NULL", pSize == NULL ? "=NULL" : "!=NULL");
        return DRV_ERROR_INVALID_VALUE;
    }

    if ((*pSize >= maxLen) || (strnlen(path, (size_t)maxLen) != *pSize)) {
        DMS_ERR("Invalid path or size. (devId=%u, *pSize=%u, pathLen=%d, maxLen=%d)\n", devId, *pSize,
            strnlen(path, (size_t)maxLen), maxLen);
        return DRV_ERROR_INVALID_VALUE;
    }

    log_info.buffer = (char *)malloc(DMS_LOG_BUF_SIZE);
    if (log_info.buffer == NULL) {
#ifndef DMS_UT
        DMS_ERR("Malloc failed. (devId=%u, size=%u)\n", devId, DMS_LOG_BUF_SIZE);
        return DRV_ERROR_OUT_OF_MEMORY;
#endif
    }
    log_info.length = DMS_LOG_BUF_SIZE;

#ifndef DMS_UT
    if (drvIsVritEnv()) {
        ret = dmanage_common_ioctl(DEVDRV_MANAGER_GET_HOST_KERN_LOG, &log_info);
        if (ret != 0) {
            DMS_EX_NOTSUPPORT_ERR(ret, "Ioctl failed. (dev_id=%u; ret=%d; errno=%d)\n", devId, ret, errno);
            free(log_info.buffer);
            return ret;
        }
    } else {
        ioarg.main_cmd = DMS_MAIN_CMD_LOG;
        ioarg.sub_cmd = DMS_SUBCMD_GET_LOG_INFO;
        ioarg.filter_len = 0;
        ioarg.input = &log_info;
        ioarg.input_len = sizeof(struct LogInfo);
        ioarg.output = &log_info.buf_len;
        ioarg.output_len = sizeof(u32);
        ret = errno_to_user_errno(DmsIoctl(DMS_IOCTL_CMD, &ioarg));
        if (ret != 0) {
            DMS_EX_NOTSUPPORT_ERR(ret, "drvGetKlogBuf failed. (devId=%u, ret=%d)", devId, ret);
            free(log_info.buffer);
            return ret;
        }
    }
#endif

    ret = drvKlogSaveFs((const char *)log_info.buffer, log_info.buf_len, path);
    if (ret != 0) {
#ifndef DMS_UT
        DMS_ERR("Save klog failed. (ret=%d)\n", ret);
        free(log_info.buffer);
        return ret;
#endif
    }

    free(log_info.buffer);
    return DRV_ERROR_NONE;
}

