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
#include "securec.h"
#include "ascend_hal_error.h"
#include "ascend_hal.h"
#include "dms_user_common.h"
#include "dmc_user_interface.h"
#include "dms_gpio.h"

drvError_t DmsGetGpioStatus(int device_id, unsigned int gpio_num, unsigned int *status)
{
    struct dms_ioctl_arg ioarg = {0};
    struct dms_get_gpio in = {0};
    int ret;
    if (status == NULL) {
        return DRV_ERROR_PARA_ERROR;
    }
    in.dev_id = (unsigned int)device_id;
    in.gpio_num = gpio_num;
    ioarg.main_cmd = DMS_GET_GET_GPIO_STATUS_CMD;
    ioarg.sub_cmd = ZERO_CMD;
    ioarg.filter_len = 0;
    ioarg.input = (void *)&in;
    ioarg.input_len = sizeof(struct dms_get_gpio);
    ioarg.output = (void *)status;
    ioarg.output_len = sizeof(unsigned int);

    ret = errno_to_user_errno(DmsIoctl(DMS_IOCTL_CMD, &ioarg));
    if (ret != 0) {
        DMS_ERR("Get GPIO status failed. (ret=%d)\n", ret);
        return ret;
    }
    DMS_DEBUG("Get GPIO status success.\n");
    return DRV_ERROR_NONE;
}

