/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef DEVMM_RECORD_H
#define DEVMM_RECORD_H
#include <sys/types.h>
#include <unistd.h>
#include "ascend_hal_error.h"

enum devmm_record_feature_type {
    DEVMM_FEATURE_IPC = 0,
    DEVMM_FEATURE_IMPORT,
    DEVMM_FEATURE_VMM_CREATE,
    DEVMM_FEATURE_VMM_MAP,
    DEVMM_FEATURE_TYPE_MAX
};

enum devmm_record_key_type {
    DEVMM_KEY_TYPE1 = 0,
    DEVMM_KEY_TYPE2,
    DEVMM_KEY_TYPE_MAX
};

enum devmm_record_node_status {
    DEVMM_NODE_INITING = 0,
    DEVMM_NODE_INITED,
    DEVMM_NODE_UNINITING,
    DEVMM_NODE_UNINITED,
    DEVMM_NODE_STATUS_MAX
};

struct devmm_record_data {
    uint64_t key1;  /* input: must set value */
    uint64_t key2;
    void *data;
    uint64_t data_len;
};

#define DEVMM_RECORD_KEY_INVALID UINT64_MAX
#define DEVMM_RECORD_INVALID_DEVID UINT32_MAX
#define DEVMM_RECORD_HOST_DEVID 65U

drvError_t devmm_record_create_and_get(enum devmm_record_feature_type type, uint32_t devid,
    enum devmm_record_key_type key_type, enum devmm_record_node_status status, struct devmm_record_data *data);
drvError_t devmm_record_get(enum devmm_record_feature_type type, uint32_t devid,
    enum devmm_record_key_type key_type, struct devmm_record_data *data);
drvError_t devmm_record_put(enum devmm_record_feature_type type, uint32_t devid, uint32_t key_type, uint64_t key,
    enum devmm_record_node_status status);
void devmm_record_recycle(uint32_t devid);
void devmm_record_restore_func_register(enum devmm_record_feature_type type,
    int (*restore)(uint64_t , uint64_t , uint32_t , void *));
int devmm_record_restore(void);

#endif