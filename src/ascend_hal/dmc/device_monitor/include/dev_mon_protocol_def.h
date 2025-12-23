/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef DEV_MON_PROTOCOL_DEF
#define DEV_MON_PROTOCOL_DEF

#define MAX_RSP_DATA_LEN 20
#define REP_DATA_LEN(total_len) (((total_len) > MAX_RSP_DATA_LEN) ? MAX_RSP_DATA_LEN : (total_len))

#define DEV_MON_ERROR_CODE_LEN 2
#define DEV_MON_ERROR_MSG_DATA_LEN 4096
typedef struct op_msg_stru {
    unsigned short err_code;
    unsigned char op_cmd;
    unsigned char op_fun;
    unsigned int total_length;
    unsigned int data_length;
    unsigned char data[DEV_MON_ERROR_MSG_DATA_LEN];
} DM_REP_MSG;

#endif
