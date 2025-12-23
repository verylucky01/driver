/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <stdio.h>
#include <stdarg.h>

static int init_test(void)
{
    /* Place the initialization function here. */

    printf("[UT] Testcase initialize success.\r\n");

    return 0;
}

static int run_all_test(void)
{
    int ret = 0;

    printf("[UT] Testcase run start...\r\n");
    /* Place test cases here. */

    if (ret != 0) {
        printf("[UT] Testcase run failed. (ret=%d)\r\n", ret);
    } else {
        printf("[UT] Testcase run success.\r\n");
    }

    return ret;
}

int main(int arg_cnt, char* argv[])
{
    int ret = init_test();
    if (ret != 0) {
        printf("[UT] Failed to initialize testcase.\r\n");
    }

    return run_all_test();
}
