/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef TRS_MASTER_URMA_H
#define TRS_MASTER_URMA_H
#include "ascend_hal_define.h"
#include "trs_sqcq.h"

drvError_t trs_register_reg(uint32_t dev_id, uint64_t va, uint32_t size);
void trs_unregister_reg(uint32_t dev_id, uint64_t va, uint32_t size);
drvError_t trs_sqcq_urma_alloc(uint32_t dev_id, struct halSqCqInputInfo *in,
    struct halSqCqOutputInfo *out);
drvError_t trs_sqcq_urma_free(uint32_t dev_id, struct halSqCqFreeInfo *info, bool remote_free_flag);
drvError_t trs_sq_task_send_urma(uint32_t dev_id, struct halTaskSendInfo *info, struct sqcq_usr_info *sq_info);
drvError_t trs_sq_task_srgs_async_copy(uint32_t dev_id, struct halSqTaskArgsInfo *info,
    struct sqcq_usr_info *sq_info);
drvError_t trs_get_urma_tseg_info_by_va(uint32_t devid, uint64_t va, uint64_t size, uint32_t flag,
    struct halTsegInfo *tsegInfo);
drvError_t trs_put_urma_tseg_info(uint32_t devid, struct halTsegInfo *tsegInfo);
drvError_t trs_urma_sq_switch_stream_batch(uint32_t dev_id, struct sq_switch_stream_info *info, uint32_t num);
#endif
