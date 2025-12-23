/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef QUEUE_CLIENT_COMM_H
#define QUEUE_CLIENT_COMM_H

#include "ascend_hal.h"
#include "esched_user_interface.h"

#define UNSUBSCRIBE_TRY_CNT 2

drvError_t queue_subscribe(unsigned int dev_id, unsigned int qid, unsigned int subevent_id, struct event_res *res);
drvError_t queue_unsubscribe(unsigned int dev_id, unsigned int qid, unsigned int subevent_id);
void queue_updata_timeout(struct timeval start, struct timeval end, int *timeout);
drvError_t queue_wait_event(unsigned int devid, unsigned int qid, int result, int timeout);
drvError_t queue_param_check(unsigned int dev_id, unsigned int qid, const struct buff_iovec *vector);
#endif
