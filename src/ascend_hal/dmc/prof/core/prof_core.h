/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef PROF_CORE_H
#define PROF_CORE_H
#include "ascend_inpackage_hal.h"

drvError_t prof_core_register_channel(uint32_t dev_id, uint32_t chan_id, struct prof_sample_register_para *para);
drvError_t prof_core_get_channels(uint32_t dev_id, struct channel_list *channels);
int prof_core_poll_channels(struct prof_poll_info *out_buf, uint32_t num, int timeout);

drvError_t prof_core_chan_start(uint32_t dev_id, uint32_t chan_id, struct prof_start_para *start_para);
drvError_t prof_core_chan_stop(uint32_t dev_id, uint32_t chan_id);
drvError_t prof_core_chan_flush(uint32_t dev_id, uint32_t chan_id, uint32_t *data_len);
int prof_core_chan_read(uint32_t dev_id, uint32_t chan_id, char *out_buf, uint32_t buf_size);
drvError_t prof_core_chan_query(uint32_t dev_id, uint32_t chan_id, uint32_t *avail_len);
drvError_t prof_core_chan_report(uint32_t dev_id, uint32_t chan_id, void *data, uint32_t data_len);

#endif
