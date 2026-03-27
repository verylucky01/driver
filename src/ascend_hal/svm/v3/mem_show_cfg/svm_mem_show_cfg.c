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

#include "svm_log.h"
#include "svm_sys_cmd.h"
#include "svm_ioctl_ex.h"

int svm_show_mem_ack(u32 devid, char *buf)
{
    int ret = svm_cmd_ioctl(devid, SVM_MEM_SHOW_FEATURE_ACK, (void *)buf);
    if (ret != 0) {
        svm_err("Ack failed. (devid=%u)\n", devid);
    }

    return ret;
}
