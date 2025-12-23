/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef LINGQU_DCMI_H
#define LINGQU_DCMI_H

#include "ioctl_comm_def.h"

typedef enum {
    EVENT_TYPE_ID = 1UL << 0,
    EVENT_ID = 1UL << 1,
    SEVERITY = 1UL << 2,
    CHIP_ID = 1UL << 3,
} LqDcmiEventFilterFlag;

typedef struct lq_dcmi_event_filter {
    LqDcmiEventFilterFlag filterFlag;
    // 接收指定类型的事件：参考《LingQu Computing Network健康管理故障定义》
    unsigned int eventTypeId;
    // 接收指定的事件：参考《LingQu Computing Network健康管理故障定义》
    unsigned int eventId;
    // 接收指定级别及以上的事件：故障等级：0：提示，1：次要，2：重要，3：紧急
    unsigned char severity;
    unsigned int chipId;
} LqDcmiEventFilter;

typedef void (*LqDcmiFaultEventCallback)(LqDcmiEvent* event);

int lq_dcmi_init();

int lq_dcmi_subscribe_fault_event(LqDcmiEventFilter filter, LqDcmiFaultEventCallback handler);

int lq_dcmi_get_fault_info(unsigned int listLen, unsigned int* eventListLen, LqDcmiEvent* eventList);

int lq_dcmi_get_version(unsigned int* lq_version, unsigned int* lqdcmi_version);

int lq_dcmi_get_fault_nums(unsigned int* fault_nums);

#endif // LINGQU_DCMI_H
