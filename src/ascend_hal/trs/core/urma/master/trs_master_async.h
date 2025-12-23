/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef TRS_MASTER_ASYNC_H
#define TRS_MASTER_ASYNC_H
#include "ascend_hal_define.h"
#include "trs_sqcq.h"

struct trs_d2d_async_devid {
    uint32_t src_devid;
    uint32_t dst_devid;
    uint32_t local_devid;
    uint32_t remote_devid;
    uint8_t local_eid[16];
    uint8_t remote_eid[16];
};

static inline void trs_pack_d2d_async_devid(uint32_t local_devid, uint32_t remote_devid, uint32_t src_devid,
    uint32_t dst_devid, struct trs_d2d_async_devid *d2d_async_devid)
{
    d2d_async_devid->local_devid = local_devid;
    d2d_async_devid->remote_devid = remote_devid;
    d2d_async_devid->src_devid = src_devid;
    d2d_async_devid->dst_devid = dst_devid;
}

drvError_t trs_async_dma_wqe_create(uint32_t dev_id, struct halAsyncDmaInputPara *in, struct halAsyncDmaOutputPara *out);
drvError_t trs_async_dma_wqe_destory(uint32_t dev_id, struct halAsyncDmaDestoryPara *para);
int trs_destroy_remote_d2d_jetty(uint32_t dev_id);
#endif