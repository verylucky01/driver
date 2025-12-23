/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef PROF_HDC_H
#define PROF_HDC_H

#include "prof_common.h"

drvError_t prof_hdc_start(uint32_t dev_id, uint32_t chan_id, struct prof_user_start_para *para);
drvError_t prof_hdc_stop(uint32_t dev_id, uint32_t chan_id);
drvError_t prof_hdc_flush(uint32_t dev_id, uint32_t chan_id);
drvError_t prof_hdc_get_channels(uint32_t dev_id, struct prof_channel_list *channels);

void prof_hdc_msg_proc(uint32_t dev_id, void *msg, uint32_t len);  // only be used by hdc_communication
#endif
