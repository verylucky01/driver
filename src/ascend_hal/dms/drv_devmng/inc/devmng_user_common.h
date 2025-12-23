/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef __DEVMNG_USER_COMMON_H__
#define __DEVMNG_USER_COMMON_H__

#include "mmpa_api.h"

int dmanage_common_ioctl(int ioctl_cmd, void *ioctl_arg);
int dmanage_mmIoctl(mmProcess fd, signed int ioctl_code, mmIoctlBuf *buf_ptr);

void dmanage_share_log_create(void);
void dmanage_share_log_destroy(void);
void dmanage_share_log_read(void);

#endif /* DEVMNG_USER_COMMON_H */