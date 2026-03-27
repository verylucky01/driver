/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include "urma_api.h"
#include "urma_types.h"

#include "ascend_urma_ctx.h"
#include "ascend_urma_log.h"

static void __attribute__((constructor)) ascend_urma_init(void)
{
    urma_init_attr_t conf = {0};
    urma_status_t urma_ret;

    urma_ret = urma_init(&conf);
    if (urma_ret != 0) {
        ascend_urma_info("Urma init check. (urma_ret=%d)\n", urma_ret);
    }
}

static void __attribute__((destructor)) ascend_urma_uninit(void)
{
    ascend_urma_ctxs_release();
}
