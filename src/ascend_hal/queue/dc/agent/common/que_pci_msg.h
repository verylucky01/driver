/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef QUE_PCI_MSG_H
#define QUE_PCI_MSG_H

#include "ascend_hal.h"
#include "queue_ioctl.h"
#include "queue_h2d_user_pci_msg.h"

#define QUEUE_EVENT_OUTSTANDING 128
#define QUEUE_EVENT_EXPIRED_THRES (QUEUE_EVENT_OUTSTANDING * 4)
#define QUEUE_PROC_EVENT_SN_NUM (QUEUE_EVENT_OUTSTANDING * 8)

struct queue_msg_info {
    unsigned int subevent_id;
    int result_ret;
    struct event_proc_result result;
    unsigned int msg_len;
    char *msg;
};

#endif
