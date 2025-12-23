/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <time.h>
#include "dcmi_os_adapter.h"
#ifndef _WIN32
#include <sys/syscall.h>
#endif


#ifdef _WIN32

void sleep(int x)
{
    Sleep((x) * SLEEP_ADAPT);
}

void usleep(int x)
{
    Sleep((x) / SLEEP_ADAPT);
}

errno_t *localtime_r(const time_t *timep, struct tm *result)
{
    return localtime_s(result, timep);
}

int chown(const char *path, int owner, int group)
{
    return 0;
}

#else

pid_t GETTID(void)
{
    return syscall(__NR_gettid);
}

#endif