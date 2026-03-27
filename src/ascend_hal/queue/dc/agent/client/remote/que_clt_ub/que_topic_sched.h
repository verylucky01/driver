/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef QUE_TOPIC_SCHED_H
#define QUE_TOPIC_SCHED_H

#include "ascend_hal_define.h"
#include "drv_type.h"
#include "que_event.h"

struct que_sqcq_info {
    drvSqCqType_t type; // normal : 0, callback : 1
    unsigned int ts_id;
    unsigned int sq_id;
    unsigned int cq_id;  // cq_id to be freed, if flag bit 0 is 0, don't care about it
    unsigned int flag;  // bit 0 : whether cq is to be freed  0 : free, 1 : no free
};

int que_topic_rtsq_init(unsigned int devid, struct que_sqcq_info *sqcq_info);
void que_topic_rtsq_uninit(unsigned int devid, struct que_sqcq_info *sqcq_info);
int que_enque_notify_proc(unsigned int devid, unsigned int qid, struct que_sqcq_info *sqcq_info);
#endif