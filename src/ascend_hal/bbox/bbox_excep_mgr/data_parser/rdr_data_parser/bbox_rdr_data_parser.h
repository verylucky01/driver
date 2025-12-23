/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef BBOX_RDR_DATA_PARSER_H
#define BBOX_RDR_DATA_PARSER_H

#include <stdbool.h>
#include "bbox_ddr_int.h"
#include "bbox_parser_inner.h"

struct ddr_dump_info {
    u32 excep_id;
    s32 is_start_up;
    u32 len;
    u32 offset;
};

s32 bbox_ddr_dump_module(u8 core_id, const u8 *data, u32 len, const char *path);
bool bbox_ddr_dump_check(const struct rdr_head *head);

#endif // BBOX_RDR_DATA_PARSER_H
