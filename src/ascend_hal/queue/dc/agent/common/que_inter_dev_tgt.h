/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef QUE_INTER_DEV_TGT_H
#define QUE_INTER_DEV_TGT_H

#include <stdlib.h>

#include "securec.h"
#include "ascend_hal.h"
#include "ascend_hal_error.h"
#include "queue_interface.h"

struct drv_event_proc *que_get_comm_subevent_proc(enum drv_subevent_id subevent);
#endif