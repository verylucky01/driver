/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef GRP_MNG_H
#define GRP_MNG_H

#include "ascend_hal_define.h"

static inline bool is_authed_read(GroupShareAttr attr)
{
    return (attr.read != 0);
}

static inline bool is_authed_write(GroupShareAttr attr)
{
    return (attr.write != 0);
}

static inline bool is_authed_alloc(GroupShareAttr attr)
{
    return (attr.alloc != 0);
}

bool is_cache_size_valid(unsigned long long cache_size);
drvError_t buff_group_addr_query(GrpQueryGroupAddrInfo *addr_buff, unsigned int *query_cnt);
bool buff_is_enable_cache(void);
drvError_t buff_is_support(void);

#endif

