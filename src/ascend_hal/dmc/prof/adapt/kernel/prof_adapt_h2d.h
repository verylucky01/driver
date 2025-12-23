/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef PROF_ADAPT_H2D_H
#define PROF_ADAPT_H2D_H
#include "prof_adapt.h"

struct prof_h2d_ops {
    drvError_t (*get_channels)(uint32_t dev_id, struct prof_channel_list *channels);
    drvError_t (*get_chan_ops)(struct prof_chan_ops **ops);
};

void prof_h2d_regiser_urma_ops(struct prof_h2d_ops *ops);

#endif

