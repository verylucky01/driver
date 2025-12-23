/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "mmpa_api.h"
#include "devmng_common.h"
#include "devdrv_pcie.h"
#include "devdrv_user_common.h"
#include "devdrv_ioctl.h"
#include "devmng_user_common.h"
#include "ascend_dev_num.h"

#ifdef __linux
#define fd_is_invalid(fd) ((fd) < 0)
#else
#define fd_is_invalid(fd) (fd == (mmProcess)DEVDRV_INVALID_FD_OR_INDEX)
#endif

drvError_t drvPcieIMUDDRRead(uint32_t devId, uint32_t offset, uint8_t *value, uint32_t len)
{
    struct devdrv_pcie_imu_ddr_read_para imu_ddr_read_para = {0};
    uint32_t i = 0;
    int ret;
    mmProcess fd = -1;
    mmIoctlBuf buf = {0};

    if (devId >= ASCEND_DEV_MAX_NUM) {
        DEVDRV_DRV_ERR("invalid devid(%u).\n", devId);
        return DRV_ERROR_INVALID_DEVICE;
    }
    if (value == NULL) {
        DEVDRV_DRV_ERR("value is NULL. devid(%u).\n", devId);
        return DRV_ERROR_INVALID_HANDLE;
    }
    if ((len == 0) || (len > sizeof(imu_ddr_read_para.value))) {
        DEVDRV_DRV_ERR("invalid len(%u). devid(%u)\n", len, devId);
        return DRV_ERROR_INVALID_VALUE;
    }

    fd = devdrv_open_device_manager();
    if (fd_is_invalid(fd)) {
        DEVDRV_DRV_ERR("open device manager failed, fd(%d). devid(%u)\n", fd, devId);
        return DRV_ERROR_INVALID_HANDLE;
    }
    imu_ddr_read_para.devId = devId;
    imu_ddr_read_para.offset = offset;
    imu_ddr_read_para.len = len;
    buf.inbuf = (void *)&imu_ddr_read_para;
    buf.inbufLen = sizeof(struct devdrv_pcie_imu_ddr_read_para);
    buf.outbuf = buf.inbuf;
    buf.outbufLen = buf.inbufLen;
    buf.oa = NULL;
    ret = dmanage_mmIoctl(fd, DEVDRV_MANAGER_PCIE_IMU_DDR_READ, &buf);
    if (ret) {
        DEVDRV_DRV_ERR("ioctl failed, devid(%u), ret(%d).\n", devId, ret);
        return DRV_ERROR_IOCRL_FAIL;
    }
    for (i = 0; i < len; i++) {
        value[i] = imu_ddr_read_para.value[i];
    }

    return DRV_ERROR_NONE;
}
