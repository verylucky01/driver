/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include "dms_flash.h"

drvError_t DmsGetFlashContent(int dev_id, DSMI_FLASH_CONTENT content_info)
{
    int ret;
    struct dms_ioctl_arg ioarg = {0};
    struct dms_flash_content_in flash_content;
    flash_content.dev_id = dev_id;
    flash_content.type = content_info.type;
    flash_content.buf = content_info.buf;
    flash_content.size = content_info.size;
    flash_content.offset = content_info.offset;

    ioarg.main_cmd = DMS_GET_FLASH_CONTENT;
    ioarg.sub_cmd = ZERO_CMD;
    ioarg.filter_len = 0;
    ioarg.input = (void *)(&flash_content);
    ioarg.input_len = sizeof(struct dms_flash_content_in);
    ioarg.output = NULL;
    ioarg.output_len = 0;

    ret = DmsIoctl(DMS_IOCTL_CMD, &ioarg);
    if (ret != 0) {
        DMS_EX_NOTSUPPORT_ERR(ret, "DmsGetFlashContent failed. (dev_id=%d; ret=%d)\n", dev_id, ret);
        ret = errno_to_user_errno(ret);
        return ret;
    }

    return DRV_ERROR_NONE;
}

drvError_t DmsSetFlashContent(int dev_id, DSMI_FLASH_CONTENT content_info)
{
    int ret;
    struct dms_ioctl_arg ioarg = {0};
    struct dms_flash_content_in flash_content;
    flash_content.dev_id = dev_id;
    flash_content.type = content_info.type;
    flash_content.buf = content_info.buf;
    flash_content.size = content_info.size;
    flash_content.offset = content_info.offset;

    ioarg.main_cmd = DMS_SET_FLASH_CONTENT;
    ioarg.sub_cmd = ZERO_CMD;
    ioarg.filter_len = 0;
    ioarg.input = (void *)(&flash_content);
    ioarg.input_len = sizeof(struct dms_flash_content_in);
    ioarg.output = NULL;
    ioarg.output_len = 0;

    ret = DmsIoctl(DMS_IOCTL_CMD, &ioarg);
    if (ret != 0) {
        DMS_EX_NOTSUPPORT_ERR(ret, "DmsSetFlashContent failed. (dev_id=%d; ret=%d)\n", dev_id, ret);
        ret = errno_to_user_errno(ret);
        return ret;
    }

    return DRV_ERROR_NONE;
}