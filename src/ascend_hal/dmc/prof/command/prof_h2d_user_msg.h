/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef PROF_H2D_USER_MSG_H
#define PROF_H2D_USER_MSG_H
#include <stdint.h>

#define PROF_EVENT_DATA_SIZE_MAX 8
struct prof_start_event_msg {
    uint32_t chan_id;
    uint32_t sample_period;
    uint32_t data_len;
    char data[PROF_EVENT_DATA_SIZE_MAX];
};

struct prof_stop_event_msg {
    uint32_t chan_id;
};
#endif
