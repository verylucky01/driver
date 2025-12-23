/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef DRV_DEVMNG_ADAPT_H__
#define DRV_DEVMNG_ADAPT_H__

#include "drv_type.h"
#include "ascend_hal_error.h"
#include <sys/types.h>
#include "mmpa_api.h"

drvError_t drv_get_info_type_version_adapt(uint32_t devId, int32_t info_type, int64_t *value);
drvError_t drvGetPlatformInfo(uint32_t *info);
void drv_ioctl_param_init(mmIoctlBuf *icotl_buf, void *inbuf, int inbufLen);
int drv_common_ioctl(mmIoctlBuf *icotl_buf, int cmd);

#endif