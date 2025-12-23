/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef _CFG_PARSE_H_
#define _CFG_PARSE_H_

#ifndef HDC_UT_TEST
#include "mmpa_api.h"
#endif

#define MAX_VALUE_NUM (32)
#define MAX_LINE_SIZE (512)
#define MAX_FILE_NAME_SIZE (1024)

typedef struct tg_CfgParseCB {
    FILE *fp;
    unsigned char opflag;
    char file_path[MAX_FILE_NAME_SIZE];
    char *value[MAX_VALUE_NUM];
} CFGPARSE_CB_T, *PCFGPARSE_CB_T;

extern signed int cfg_file_open(const char *filename, PCFGPARSE_CB_T *handle);
extern void cfg_file_close(PCFGPARSE_CB_T handle);
extern signed int str_search(PCFGPARSE_CB_T handle, const char *str, char **value, signed int *value_num);
extern signed int is_digit(const char str[], int len);

#endif
