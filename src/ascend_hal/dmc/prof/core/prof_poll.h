/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef PROF_POLL_H
#define PROF_POLL_H
#include "ascend_hal.h"

void prof_poll_chan_start(uint32_t dev_id, uint32_t chan_id);
void prof_poll_chan_stop(uint32_t dev_id, uint32_t chan_id);

void prof_poll_report(uint32_t dev_id, uint32_t chan_id);
int prof_poll_read(struct prof_poll_info *out_buf, uint32_t max_num, int timeout);

#endif
