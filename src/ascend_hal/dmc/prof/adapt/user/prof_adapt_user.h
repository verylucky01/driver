/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef PROF_ADAPT_USER_H
#define PROF_ADAPT_USER_H
#include "ascend_inpackage_hal.h"
#include "prof_adapt.h"

drvError_t prof_user_register_channel(uint32_t dev_id, uint32_t chan_id, struct prof_sample_register_para *para);
drvError_t prof_user_get_chan_ops(struct prof_chan_ops **ops);

struct prof_user_kernel_ops {
    drvError_t (*chan_register)(uint32_t dev_id, uint32_t chan_id);
    drvError_t (*chan_query)(uint32_t dev_id, uint32_t chan_id, uint32_t *avail_len);
    drvError_t (*chan_writer)(uint32_t dev_id, uint32_t chan_id, void *data, uint32_t data_len);
};

void prof_user_regiser_kernel_ops(struct prof_user_kernel_ops *ops);

#endif