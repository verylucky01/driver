/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef BBOX_DATA_MODEL_H
#define BBOX_DATA_MODEL_H

#include "bbox_ddr_data.h"
#include "bbox_data_parser.h"

struct bbox_plaintext_module_map {
    u8 coreid;
    enum plain_text_table_type normal_type;
    enum plain_text_table_type start_type;
    enum plain_text_table_type log_type;
};

typedef struct bbox_plaintext_table_map {
    enum plain_text_table_type type;
    u32 num;
    const struct model_element *table;
    const char *fname;
} plaintext_map_t;

typedef struct model_element model_elem_t;

const struct bbox_plaintext_module_map *bbox_get_module_type_map(void);
const struct bbox_plaintext_table_map *bbox_get_data_model_map(void);
const char *bbox_plaintext_get_fname(enum plain_text_table_type type);
const struct bbox_plaintext_table_map *bbox_plaintext_get_map(enum plain_text_table_type type);
bbox_status bbox_get_val_with_size(const char *data, u32 length, u64 *number);

#endif // BBOX_DATA_MODEL_H
