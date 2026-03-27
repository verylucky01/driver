/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef ESCHED_TOPIC_SQE_H
#define ESCHED_TOPIC_SQE_H

#include "ascend_hal_define.h"
#include "drv_user_common.h"
#include "esched_user_interface.h"

void *esched_alloc_ext_msg();
void esched_free_ext_msg(void *ext_msg);
void esched_fill_sqcq_alloc_info(unsigned int sqe_depth, unsigned int cqe_depth, struct halSqCqInputInfo *in, void *ext_msg_t);

void *esched_alloc_topic_sqe();
void esched_free_topic_sqe(void *sqe);
int esched_fill_topic_sqe(unsigned int devid, struct event_summary *event, void *sqe_t);
#endif