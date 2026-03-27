/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef QUE_JETTY_H
#define QUE_JETTY_H

#include "urma_api.h"
#include "queue_h2d_user_ub_msg.h"

#define QUE_DATA_RW_JETTY_POOL_DEPTH   8 /* Enque jetty number for each process */
#define QUE_JFS_HIGH_PRIORITY   10
#define QUE_JFS_MEDIUM_PRIORITY   3

struct que_jfr_pool_info {
    struct que_jfr *qjfr;
    struct que_recv_para *recv_para;
    bool jfr_busy_flag;
};

struct que_jfs *que_jfs_create(unsigned int devid, struct que_jfs_attr *attr, unsigned int d2d_flag);
void que_jfs_destroy(struct que_jfs *qjfs);
struct que_jfr *que_jfr_create(unsigned int devid, struct que_jfr_attr *attr, unsigned int d2d_flag);
void que_jfr_destroy(struct que_jfr *qjfr);

unsigned int que_idle_jetty_find(unsigned int devid, unsigned int d2d_flag);
unsigned int que_rw_jetty_alloc(unsigned int devid, unsigned int d2d_flag);
void que_rw_jetty_free(unsigned int devid, unsigned int jetty_idx, unsigned int d2d_flag);
struct que_jfs *que_qjfs_get(unsigned int devid, unsigned int jetty_idx, unsigned int d2d_flag);
struct que_jfs_rw_wr *que_send_wr_get(unsigned int devid, unsigned int jetty_idx, unsigned int d2d_flag);

int que_jfs_pool_init(unsigned int devid, struct que_jfs_attr *attr, unsigned int d2d_flag);
void que_jfs_pool_uninit(unsigned int devid, unsigned int d2d_flag);

#endif
