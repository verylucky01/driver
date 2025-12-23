/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <limits.h>
#include <sys/ioctl.h>
#include "securec.h"
#include "dms_user_common.h"
#include "dms/dms_misc_interface.h"
#include "ascend_hal_error.h"
#include "ascend_hal.h"
#include "dms/dms_devdrv_info_comm.h"
#include "devdrv_user_common.h"
#include "ascend_dev_num.h"

int dms_get_last_boot_state(unsigned int dev_id, BOOT_TYPE boot_type, unsigned int *state)
{
    struct dms_ioctl_arg ioarg = {0};
    int ret;
    last_bootstate_info_t bootstate_info = {0};

    if ((dev_id >= ASCEND_DEV_MAX_NUM) || (state == NULL)) {
        DMS_ERR("Invalid dev_id or state is NULL. (dev_id=%u)\n", dev_id);
        return DRV_ERROR_PARA_ERROR;
    }

    bootstate_info.dev_id = dev_id;
    bootstate_info.boot_type = boot_type;

    ioarg.main_cmd = DMS_MAIN_CMD_BBOX;
    ioarg.sub_cmd = DMS_SUBCMD_GET_LAST_BOOT_STATE;
    ioarg.filter_len = 0;
    ioarg.input = (void *)&bootstate_info;
    ioarg.input_len = sizeof(bootstate_info);
    ioarg.output = (void *)state;
    ioarg.output_len = sizeof(unsigned int);

    ret = errno_to_user_errno(DmsIoctl(DMS_IOCTL_CMD, &ioarg));
    if (ret != 0) {
        DMS_EX_NOTSUPPORT_ERR(ret, "Get last bootstate failed. (dev_id=%u; ret=%d)\n", dev_id, ret);
        return ret;
    }

    DMS_DEBUG("Get last bootstate success.\n");
    return DRV_ERROR_NONE;
}