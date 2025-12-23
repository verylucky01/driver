/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "bbox_data_model.h"
#include "bbox_rdr_pub.h"
#include "bbox_print.h"

/**
 * @brief       dump ddr data for different core id
 * @param [in]  coreid:    core id
 * @param [in]  type:      get parese type, COMMON_LOG_TYPE/NORMAL_DATA_TYPE/STARTUP_DATA_TYPE
 * @return      data type in plaintext_table_type_e
 */
enum plain_text_table_type bbox_plaintext_get_type(u8 coreid, enum data_parse_type type)
{
    BBOX_CHK_INVALID_PARAM(coreid >= (u8)BBOX_CORE_MAX, return PLAINTEXT_TABLE_NONE, "%u", coreid);

    s32 i;
    const struct bbox_plaintext_module_map *map = bbox_get_module_type_map();
    for (i = 0; map[i].coreid != (u8)BBOX_CORE_MAX; i++) {
        if (coreid == map[i].coreid) {
            return (type == COMMON_LOG_TYPE) ? map[i].log_type :
                ((type == NORMAL_DATA_TYPE) ? map[i].normal_type : map[i].start_type);
        }
    }
    return PLAINTEXT_TABLE_NONE;
}

/**
 * @brief       get filename of given parse type
 * @param [in]  type:    data type
 * @return      log file name
 */
const char *bbox_plaintext_get_fname(enum plain_text_table_type type)
{
    s32 i;
    const struct bbox_plaintext_table_map *map = bbox_get_data_model_map();
    for (i = 0; map[i].fname != NULL; i++) {
        if (type == map[i].type) {
            return map[i].fname;
        }
    }
    return NULL;
}

/**
 * @brief       get data model table map of given parse type
 * @param [in]  type:    data type
 * @return      map of model table
 */
const struct bbox_plaintext_table_map *bbox_plaintext_get_map(enum plain_text_table_type type)
{
    s32 i;
    const struct bbox_plaintext_table_map *map = bbox_get_data_model_map();
    for (i = 0; map[i].fname != NULL; i++) {
        if (type == map[i].type) {
            return &map[i];
        }
    }
    return NULL;
}

/**
 * @brief       read number with given length from buffer
 * @param [in]  data:    data buffer
 * @param [in]  length:  length of number data
 * @param [out] number:  parsed number in given length
 * @return      parsed number
 */
bbox_status bbox_get_val_with_size(const char *data, u32 length, u64 *number)
{
    BBOX_CHK_NULL_PTR(data, return BBOX_FAILURE);
    BBOX_CHK_NULL_PTR(number, return BBOX_FAILURE);
    if ((length != sizeof(u8)) && (length != sizeof(u16)) &&
        (length != sizeof(u32)) && (length != sizeof(u64))) {
        BBOX_ERR("invalid value length %u.", length);
        return BBOX_FAILURE;
    }
    s32 ret = memset_s(number, sizeof(u64), 0, sizeof(u64));
    if (ret != EOK) {
        BBOX_ERR("memset_s number failed.");
        return BBOX_FAILURE;
    }
    ret = memcpy_s(number, sizeof(u64), data, length);
    if (ret != EOK) {
        BBOX_ERR("memcpy_s number failed.");
        return BBOX_FAILURE;
    }
    return BBOX_SUCCESS;
}
