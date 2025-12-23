/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef UT_TEST
#include "prof_communication.h"

static struct prof_comm_core_notifier g_comm_core_notifier = {NULL};

void prof_comm_register_notifier(struct prof_comm_core_notifier *notifier)
{
    g_comm_core_notifier.chan_start = notifier->chan_start;
    g_comm_core_notifier.chan_stop = notifier->chan_stop;
    g_comm_core_notifier.chan_report = notifier->chan_report;
}

struct prof_comm_core_notifier *prof_comm_get_notifier(void)
{
    return &g_comm_core_notifier;
}

#else
int prof_communication_ut_test(void)
{
    return 0;
}
#endif
