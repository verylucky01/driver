/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef PROF_COMMUNICATION_H
#define PROF_COMMUNICATION_H
#include "prof_common.h"

struct prof_comm_core_notifier {
    drvError_t (*chan_start)(uint32_t dev_id, uint32_t chan_id, struct prof_user_start_para *para, bool event_flag);
    drvError_t (*chan_stop)(uint32_t dev_id, uint32_t chan_id, struct prof_user_stop_para *para, bool event_flag);
    drvError_t (*chan_report)(uint32_t dev_id, uint32_t chan_id, void *data, uint32_t data_len, bool hal_flag);
};

void prof_comm_register_notifier(struct prof_comm_core_notifier *notifier);
struct prof_comm_core_notifier *prof_comm_get_notifier(void);

#endif
