/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef QUE_URMA_H
#define QUE_URMA_H

#include "urma_api.h"
#include "queue_h2d_user_ub_msg.h"

#define DEQUE_INI_ACCESS (URMA_ACCESS_WRITE | URMA_ACCESS_READ)
#define ENQUE_INI_ACCESS (URMA_ACCESS_READ)
#define DEQUE_TGT_ACCESS (URMA_ACCESS_LOCAL_ONLY)
#define ENQUE_TGT_ACCESS (URMA_ACCESS_LOCAL_ONLY)

#define QUE_NON_PIN (0)
#define QUE_PIN (1)

#define QUE_BYTES_PER_KB 1024ULL
#define QUE_BYTES_PER_MB (1024 * QUE_BYTES_PER_KB)
#define QUE_BYTES_PER_GB (1024 * QUE_BYTES_PER_MB)
#define QUE_BYTES_PER_TB (1024 * QUE_BYTES_PER_GB)
#define QUE_SP_VA_START (224ULL * QUE_BYTES_PER_TB)
#define QUE_SP_VA_SIZE (16ULL * QUE_BYTES_PER_TB)

enum que_trans_type {
    TRANS_D2H_H2D,
    TRANS_HOST_MAX,
    TRANS_D2D = TRANS_HOST_MAX,
    TRANS_TYPE_MAX,
};

struct que_urma_addr {
    unsigned long long va;
    urma_target_seg_t *tseg;
};

struct que_jfs_rw_wr_data {
    struct que_urma_addr remote;
    struct que_urma_addr local;
    urma_opcode_t opcode;
    size_t size;
};

urma_jfce_t *que_urma_jfce_create(unsigned int devid, unsigned int d2d_flag);
void que_urma_jfce_destroy(urma_jfce_t *jfce);

urma_jfc_t *que_urma_jfc_create(unsigned int devid, urma_jfc_cfg_t *cfg, unsigned int d2d_flag);
void que_urma_jfc_destroy(urma_jfc_t *jfc);

urma_jfs_t *que_urma_jfs_create(unsigned int devid, urma_jfs_cfg_t *cfg, unsigned int d2d_flag);
void que_urma_jfs_destroy(urma_jfs_t *jfs);

urma_jfr_t *que_urma_jfr_create(unsigned int devid, urma_jfr_cfg_t *cfg, unsigned int d2d_flag);
void que_urma_jfr_destroy(urma_jfr_t *jfr);

urma_target_jetty_t *que_jfr_import(unsigned int devid, urma_jfr_id_t *jfr_id, urma_token_t *token, unsigned int d2d_flag);
void que_jfr_unimport(urma_target_jetty_t *tjetty);

urma_target_seg_t *que_pin_seg_create(unsigned int devid, unsigned long long va, unsigned long long size,
    unsigned int access, struct que_urma_token *token, unsigned int d2d_flag);
urma_target_seg_t *que_nonpin_seg_create(unsigned int devid, unsigned long long va, unsigned long long size,
    unsigned int access, struct que_urma_token *token, unsigned int d2d_flag);
void que_seg_destroy(urma_target_seg_t *tseg);

urma_target_seg_t *que_seg_import(unsigned int devid, unsigned int d2d_flag, struct urma_seg *seg, unsigned int access, urma_token_t *token);
void que_seg_unimport(urma_target_seg_t *tseg);

struct que_jfs_rw_wr *que_jfs_rw_wr_create(struct que_jfs_rw_wr_attr *attr);
void que_jfs_rw_wr_destroy(struct que_jfs_rw_wr *rw_wr);
struct que_urma_ctx *que_urma_ctx_create(unsigned int urma_devid);
void que_urma_ctx_destroy(struct que_urma_ctx *urma_ctx);
void que_urma_ctx_put_ex(unsigned int devid);
int que_fill_ctx_token(unsigned int devid, urma_token_t *token, unsigned int d2d_flag);
int que_urma_token_alloc(unsigned int devid, struct que_urma_token *token_info, unsigned int d2d_flag);
void que_urma_token_free(unsigned int devid, struct que_urma_token *token_info);
urma_target_seg_t *que_get_urma_ctx_tseg(unsigned int devid, unsigned int d2d_flag);
#endif
