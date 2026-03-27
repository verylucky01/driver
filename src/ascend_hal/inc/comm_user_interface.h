/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef COMM_USER_INTERFACE_H
#define COMM_USER_INTERFACE_H

#include <stdint.h>
#include <stdbool.h>
#include "urma_types.h"

#define ASCEND_URMA_MAX_DEV_NUM 65U

/* ================================ Ascend Urma Raw ==============================*/
int ascend_urma_get_token_val(uint32_t devid, uint32_t *token_val);
urma_target_jetty_t *ascend_urma_import_jfr(urma_context_t *urma_ctx,
    urma_rjfr_t *rjfr, urma_token_t *token_value);

/* ================================ Ascend Urma Dev ==============================*/
bool ascend_urma_dev_is_exist(uint32_t devid);

/* ================================ Ascend Urma Ctx ==============================*/
void *ascend_urma_ctx_get(uint32_t devid);
void ascend_urma_ctx_put(void *ctx);

urma_context_t *ascend_to_urma_ctx(void *ctx);

/* ================================ Ascend Urma Seg ==============================*/
#define ASCEND_URMA_SEG_FLAG_ACCESS_WRITE          (1U << 0U)
#define ASCEND_URMA_SEG_FLAG_PIN                   (1U << 1U)
#define ASCEND_URMA_SEG_FLAG_WITHOUT_TOKEN_VAL     (1U << 2U)

struct ascend_urma_seg_mng_attr {
    uint32_t token_num_default;
    uint32_t token_num_cache_up_thres;
    uint32_t max_seg_num_per_token;
};

struct ascend_urma_seg_info {
    uint64_t start;
    uint64_t size;
    uint32_t token_id;
    uint32_t token_val;
    uint32_t seg_flag;
    urma_target_seg_t *tseg;
};

void *ascend_urma_seg_mng_create(uint32_t devid, struct ascend_urma_seg_mng_attr *attr);
void ascend_urma_seg_mng_destroy(void *seg_mng);

/* If already register, will return DRV_ERROR_BUSY */
int ascend_urma_register_seg(void *seg_mng, uint64_t start, uint64_t size, uint32_t seg_flag);
int ascend_urma_unregister_seg(void *seg_mng, uint64_t start, uint64_t size);

int ascend_urma_get_seg_info(void *seg_mng, uint64_t va, struct ascend_urma_seg_info *info);

#endif