/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef PROF_URMA_H
#define PROF_URMA_H

#include "prof_common.h"
#include "prof_urma_comm.h"

drvError_t prof_urma_get_channels(uint32_t dev_id, struct prof_channel_list *channels);
drvError_t prof_urma_start(uint32_t dev_id, uint32_t chan_id, struct prof_user_start_para *para,
    struct prof_urma_start_para *urma_start_para, struct prof_urma_chan_info *urma_chan_info);
drvError_t prof_urma_stop(uint32_t dev_id, uint32_t chan_id, struct prof_user_stop_para *para,
    struct prof_urma_chan_info *urma_chan_info);
drvError_t prof_urma_flush(uint32_t dev_id, uint32_t chan_id);
drvError_t prof_urma_write_remote_r_ptr(uint32_t dev_id, uint32_t chan_id, struct prof_urma_chan_info *urma_chan_info);
#endif
