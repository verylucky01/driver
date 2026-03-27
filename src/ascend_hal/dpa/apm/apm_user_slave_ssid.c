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
#include <fcntl.h>

#include "ascend_hal.h"
#include "apm_ioctl.h"

#define SVM_DEV_NAME                "/dev/svm0"
#define SVM_IOCTL_PROCESS_BIND      0xffff

struct process_info {
    pid_t vpid;
    unsigned long long ttbr;
    unsigned long long tcr;
    int pasid;
    unsigned int flags;
};

int apm_svm_proc_bind(void)
{
    struct process_info info;
    int svmfd, ret;

    svmfd = open(SVM_DEV_NAME, O_RDWR);
    if (svmfd < 0) {
        apm_err("Open svm device failed.\n");
        return DRV_ERROR_FILE_OPS;
    }

    info.flags = 0;
    ret = ioctl(svmfd, SVM_IOCTL_PROCESS_BIND, &info);
    (void)close(svmfd);
    if (ret != 0) {
        apm_err("Ioctl svm device failed. (errno=%d)\n", errno);
        return DRV_ERROR_IOCRL_FAIL;
    }

    return DRV_ERROR_NONE;
}

/* This interface must be triggered by the RTS, because the host scenario depends on the CP process startup. */
drvError_t drvMemSmmuQuery(uint32_t device, uint32_t *SSID)
{
    struct apm_cmd_slave_ssid para;
    int ret = 0;

    if (SSID == NULL) {
        apm_err("Invalid para, SSID nullptr.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    para.devid = device;
    ret = apm_cmd_ioctl(APM_QUERY_SSID, &para);
    if (ret == DRV_ERROR_NONE) {
        *SSID = (uint32_t)para.ssid;
        return DRV_ERROR_NONE;
    }

    ret = apm_svm_proc_bind();
    if (ret != 0) {
        apm_err("Proc bind failed. (ret=%d)\n", ret);
        return DRV_ERROR_INVALID_VALUE;
    }

    ret = apm_cmd_ioctl(APM_QUERY_SSID, &para);
    if (ret == DRV_ERROR_NONE) {
        *SSID = (uint32_t)para.ssid;
        return DRV_ERROR_NONE;
    }

    return ret;
}

