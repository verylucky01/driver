/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef PROF_CHAN_LIST_H
#define PROF_CHAN_LIST_H
#include "ascend_hal.h"

drvError_t prof_add_local_channel(uint32_t dev_id, uint32_t chan_id);
void prof_del_local_channel(uint32_t dev_id, uint32_t chan_id);

drvError_t prof_update_chan_list(uint32_t dev_id);
void prof_get_chan_list(uint32_t dev_id, struct channel_list *channels);

drvError_t prof_get_chan_attr(uint32_t dev_id, uint32_t chan_id, uint32_t *mode, uint32_t *remote_pid);

#endif
