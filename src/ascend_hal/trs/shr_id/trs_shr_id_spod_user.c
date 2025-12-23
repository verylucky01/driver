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
#include "ascend_hal.h"
#include "trs_user_pub_def.h"
#include "trs_shr_id_fd.h"
#include "trs_shr_id_ioctl.h"

drvError_t halShrIdSetPodPid(const char *name, uint32_t sdid, pid_t pid)
{
    struct shr_id_pod_pid_ioctl_info ioctl_info = { 0 };
    int ret;
    size_t len;

    /* pass in only one parameter at a time in the current scene */
    if (name == NULL) {
        trs_err("invlaid parameter.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    len = strnlen(name, SHR_ID_NSM_NAME_SIZE);
    if ((len == 0) || (len == SHR_ID_NSM_NAME_SIZE)) {
        trs_err("Invalid Name. (len=%zu).\n", len);
        return DRV_ERROR_INVALID_VALUE;
    }

    ret = memcpy_s(ioctl_info.name, SHR_ID_NSM_NAME_SIZE, name, len + 1);
    if (ret != EOK) {
        trs_err("memcpy failed, (len=%zu; ret=%d).\n", len, ret);
        return DRV_ERROR_INNER_ERR;
    }

    ioctl_info.pid = pid;
    ioctl_info.sdid = sdid;
    if (shrid_ioctl(SHR_ID_SET_POD_PID, &ioctl_info) != DRV_ERROR_NONE) {
        return DRV_ERROR_IOCRL_FAIL;
    }
    trs_debug("Pid info. (pid=%d)\n", pid);

    return DRV_ERROR_NONE;
}