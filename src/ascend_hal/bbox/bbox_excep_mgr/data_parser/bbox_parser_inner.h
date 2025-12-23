/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef BBOX_PARSER_INNER_H
#define BBOX_PARSER_INNER_H

#include "bbox_data_parser.h"
#include "bbox_ddr_int.h"
#include "os_data_parser/bbox_ddr_ap_adapter.h"

void bbox_plain_text_header(const char *log_path, enum plain_text_table_type type, const void *buffer, s32 block_id);

#endif
