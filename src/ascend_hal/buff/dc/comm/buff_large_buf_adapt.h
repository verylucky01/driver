/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef BUFF_LARGE_BUFF_ADAPT_H
#define BUFF_LARGE_BUFF_ADAPT_H

#include "drv_buff_common.h"

drvError_t buff_usr_alloc_large_buf(struct buff_req_mz_alloc_huge_buf *info);
drvError_t buff_usr_free_large_buf(void *huge_mng, struct buff_req_mz_free_huge_buf *info);
#endif

