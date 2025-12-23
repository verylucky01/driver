/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef __TSDRV_USER_INTERFACE_H__
#define __TSDRV_USER_INTERFACE_H__

#include "tsdrv_user_common.h"
#include "ascend_hal.h"

#define TSDRV_SHR_ID_TYPE (1 << 20)

#ifdef __cplusplus
extern "C" {
#endif
drvError_t tsdrv_id_res_info_query(uint32_t devId, uint32_t tsId, struct tsdrv_id_query_para *para);
drvError_t tsDrvShrIdAllocIpcEventId(uint32_t devId, uint32_t tsId, uint32_t *id, uint32_t flag, u64 *val);
drvError_t drvDeviceOpenInner(uint32_t devId, halDevOpenIn *in, halDevOpenOut *out);
drvError_t drvDeviceCloseInner(uint32_t devId, halDevCloseIn *in);
drvError_t drvTrsDeviceCloseUserResInner(uint32_t dev_id, halDevCloseIn *in);
#ifdef __cplusplus
}
#endif

#endif /* __TSDRV_USER_INTERFACE_H__ */
