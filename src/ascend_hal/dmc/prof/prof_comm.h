/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef PROF_COMM_H
#define PROF_COMM_H
#include "pbl/pbl_prof_interface.h"

#ifndef PROF_UNIT_TEST
#define PROF_EVENT_REPLY_BUFFER_RET_OFFSET       (sizeof(int))
#else
#define PROF_EVENT_REPLY_BUFFER_RET_OFFSET       (sizeof(unsigned long long))
#endif
#define PROF_EVENT_REPLY_BUFFER_RET(ptr)         (*((int *)ptr))
#define PROF_EVENT_REPLY_BUFFER_DATA_PTR(ptr)    (((char *)ptr) + PROF_EVENT_REPLY_BUFFER_RET_OFFSET)

#define PROF_MIN(a, b)          (((a) < (b)) ? (a) : (b))
#define PROF_MAX(a, b)          (((a) > (b)) ? (a) : (b))

#define PROF_CHAN_NAME_LEN 32
#define PROF_POLL_DEPTH 512U
enum channel_poll_flag {
    POLL_INVALID,
    POLL_VALID
};

/* sample period time / ms */
#define PROF_PERIOD_MIN 10U    /* 10ms */
#define PROF_PERIOD_MAX 10000U /* 10s */

/* prof wait time(us) */
#define PROF_WAIT_US_MIN 1000U
#define PROF_WAIT_US_MAX 1010U
#define PROF_WAIT_CLOSE_MIN 10000U
#define PROF_WAIT_CLOSE_MAX 10010U
#endif
