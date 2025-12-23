/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef BBOX_DUMP_LIB_H
#define BBOX_DUMP_LIB_H

#include <stdbool.h>
#include "bbox_system_api.h"

#define ROOT_DIR                "hisi_logs"

#define BBOX_THREAD_HOST_DUMP   "bbox_dump"

struct BboxDumpOpt {
    bool all;
    bool force;
    bool vmcore;
    bool heart_beat_lost;
    u32 print_mode;
    u32 log_level;
};

bbox_status BboxStartDump(s32 dev_id, const char *path, s32 p_size, const struct BboxDumpOpt *opt);
void BboxStopDump(void);

#endif // BBOX_DUMP_LIB_H

