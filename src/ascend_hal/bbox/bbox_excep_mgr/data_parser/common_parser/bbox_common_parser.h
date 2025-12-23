/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef BBOX_COMMON_PARSER_H
#define BBOX_COMMON_PARSER_H

#include "bbox_int.h"
#include "bbox_data_model.h"
#include "bbox_fs_api.h"

s32 bbox_plaintext_out_data(const file_hdl_t *file_hdl, const model_elem_t *table, u32 num, const char *data, u32 length);

#endif
