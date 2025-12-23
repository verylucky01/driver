/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef QUERY_FEATURE_H__
#define QUERY_FEATURE_H__

#if defined(CFG_SOC_PLATFORM_CLOUD_V2) && defined(DRV_HOST)
#define  TRSDRV_SQ_DEVICE_MEM_PRIORITY_SUPPORT true
#else
#define  TRSDRV_SQ_DEVICE_MEM_PRIORITY_SUPPORT false
#endif

#endif