/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef TRS_CB_EVNET_H
#define TRS_CB_EVNET_H

#include "ascend_hal_define.h"
#include "trs_uk_msg.h"

#ifdef CFG_FEATURE_CALLBACK_EVENT
int trs_cb_event_init(uint32_t dev_id);
void trs_cb_event_uninit(uint32_t dev_id);

int trs_cb_event_submit(uint32_t dev_id, char *sqe, uint32_t e_size);
int trs_cb_event_wait(uint32_t dev_id, uint32_t tid, int32_t timeout, uint8_t *buff);
#else
static inline int trs_cb_event_init(uint32_t dev_id)
{
    return 0;
}

static inline void trs_cb_event_uninit(uint32_t dev_id)
{
}

static inline int trs_cb_event_submit(uint32_t dev_id, char *sqe, uint32_t e_size)
{
    return 0;
}

static inline int trs_cb_event_wait(uint32_t dev_id, uint32_t tid, int32_t timeout, uint8_t *buff)
{
    return 0;
}
#endif

#endif

