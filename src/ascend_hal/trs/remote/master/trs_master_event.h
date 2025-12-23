/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef TRS_MASTER_EVENT_H__
#define TRS_MASTER_EVENT_H__
#include "ascend_hal_define.h"
#include "trs_user_pub_def.h"

drvError_t trs_local_mem_event_sync(uint32_t dev_id, void *in, UINT64 size,
    uint32_t subevent_id, struct event_reply *reply);
drvError_t trs_svm_mem_event_sync(uint32_t dev_id, void *in, UINT64 size,
    uint32_t subevent_id, struct event_reply *reply);
drvError_t trs_sq_cq_query_sync(uint32_t dev_id, struct halSqCqQueryInfo *info);
drvError_t trs_sq_cq_config_sync(uint32_t dev_id, struct halSqCqConfigInfo *info);
#endif

