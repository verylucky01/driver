/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef BBOX_COMMON_H
#define BBOX_COMMON_H

#include <stdbool.h>

#include "bbox_print.h"
#include "bbox_int.h"
#include "bbox_adapt.h"

#define FD_INVALID_VAL       (-1)
#define CLOCK_VIRTUAL        100U
#define CMDLINE_BUFFER_SIZE  1024U
#define CMDLINE_FILE         "/proc/cmdline"
#define CMDLINE_DPCLK        "dpclk="
#define CMDLINE_DPCLK_LEN    6U
#define CLOCK_RELTIME        0U
#define KILO                 1000U
bbox_status bbox_dpclk_init(void);
u32 bbox_get_dpclk(void);

const char *bbox_get_core_name(u8 core_id);
const char *bbox_get_reboot_reason(u8 reason);

static inline bool bbox_check_feature(u32 feature)
{
    u32 product_feature = bbox_get_feature();
    return ((product_feature & feature) != 0) ? true : false;
}

#endif /* BBOX_COMMON_H */
