/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef PROF_ADAPT_HDC_H
#define PROF_ADAPT_HDC_H
#include "prof_adapt.h"
 
drvError_t prof_hdc_kernel_get_channels(uint32_t dev_id, struct prof_channel_list *channels);
drvError_t prof_hdc_get_chan_ops(struct prof_chan_ops **ops);
 
#endif
