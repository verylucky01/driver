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

#include "ascend_hal.h"

#include "svm_pub.h"
#include "svm_log.h"
#include "svm_pci_get_mem_token_info.h"
#include "svm_init_pri.h"
#include "svm_sys_cmd.h"
#include "svm_dbi.h"

static int svm_pci_query_init(u32 devid)
{
    u32 hd_connect_type = svm_get_device_connect_type(devid);
    if ((hd_connect_type == HOST_DEVICE_CONNECT_TYPE_PCIE) ||
        (hd_connect_type == HOST_DEVICE_CONNECT_TYPE_HCCS)) {
        return svm_pci_get_mem_token_info_init(devid);
    }

    return DRV_ERROR_NONE;
}

static void __attribute__ ((constructor(SVM_INIT_PRI_MEDIUM))) svm_pci_query_set_up(void)
{
    int ret;

    ret = svm_register_ioctl_dev_init_post_handle(svm_pci_query_init);
    if (ret != DRV_ERROR_NONE) {
        svm_warn("Register ioctl dev init post handle failed.\n");
    }
}
