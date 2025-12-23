/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef QUE_CLT_API_H
#define QUE_CLT_API_H

#include <stdint.h>

#include "ascend_hal_error.h"
#include "ascend_hal_define.h"
#include "esched_user_interface.h"

struct que_clt_api {
    drvError_t (* api_enque_buf)(unsigned int, unsigned int, struct buff_iovec *, int);
    drvError_t (* api_deque_buf)(unsigned int, unsigned int, struct buff_iovec *, int);
    drvError_t (* api_subscribe)(unsigned int, unsigned int, unsigned int, struct event_res *);
    drvError_t (* api_unsubscribe)(unsigned int, unsigned int, unsigned int);
    drvError_t (* api_peek)(unsigned int, unsigned int, uint64_t *, int);
};

#endif