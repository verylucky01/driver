/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef PROF_HDC_MSG_H
#define PROF_HDC_MSG_H
#include "prof_comm.h"

enum prof_hdc_msg_type {
    PROF_HDC_CMD_GET_CHANNEL,
    PROF_HDC_CMD_START,
    PROF_HDC_CMD_STOP,
    PROF_HDC_DATA,
    PROF_HDC_CLOSE_SESSION,
    PROF_HDC_DATA_FLUSH,
    PROF_HDC_CMD_MAX
};

struct prof_hdc_start_para {
    uint32_t channel_type;          /* for ts and other device */
    uint32_t buf_len;               /* buffer size */
    uint32_t sample_period;
    char user_data[PROF_USER_DATA_LEN]; /* ts data */
    uint32_t user_data_size;        /* user data's size */
};

struct prof_hdc_msg {
    int msg_type;
    int ret_val;
    uint32_t cmd_verify;
    uint32_t channel_id;
    uint32_t data_len;
    uint32_t rsv;
    unsigned char data[];
};
#endif
