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

#include "ascend_hal_error.h"
#include "ascend_hal.h"

#include "svm_pub.h"
#include "svm_log.h"
#include "svm_init_pri.h"
#include "svm_sys_cmd.h"
#include "svm_dbi.h"
#include "va_allocator.h"

static u32 svm_get_default_connect_type(void)
{
    int64_t value;
    int ret;

    ret = halGetDeviceInfo(0, MODULE_TYPE_SYSTEM, INFO_TYPE_HD_CONNECT_TYPE, &value);
    return (ret == DRV_ERROR_NONE) ? (u32)value : HOST_DEVICE_CONNECT_PROTOCOL_UNKNOWN;
}

static int svm_pci_assign_init_dev(u32 devid)
{
    u32 hd_connect_type;

    if (devid == svm_get_host_devid()) {
        hd_connect_type = svm_get_default_connect_type();
    } else {
        hd_connect_type = svm_get_device_connect_type(devid);
    }

    if (hd_connect_type == HOST_DEVICE_CONNECT_TYPE_PCIE) {
        svm_enable_pcie_th();
    }

    return DRV_ERROR_NONE;
}

static void __attribute__ ((constructor(SVM_INIT_PRI_MEDIUM))) svm_pci_assign_init(void)
{
    int ret;

    ret = svm_register_ioctl_dev_init_post_handle(svm_pci_assign_init_dev);
    if (ret != DRV_ERROR_NONE) {
        svm_err("Register ioctl dev init post handle failed.\n");
    }
}