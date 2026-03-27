/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef MMS_H
#define MMS_H

#include "mms_def.h"

/* MSS: module memory statistics. */

void svm_mms_add(u32 devid, u32 module_id, u32 type, u64 size);
void svm_mms_sub(u32 devid, u32 module_id, u32 type, u64 size);
int svm_mms_get(u32 devid, u32 module_id, u32 type, struct mms_type_stats *stats);
void svm_mms_uninit(void);

#endif
