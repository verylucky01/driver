/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef DPA_DP_PROC_MNG_H
#define DPA_DP_PROC_MNG_H

#include <stdint.h>

#include "ascend_inpackage_hal.h"
#ifndef EMU_ST
#include "slog.h"
#else
#include "ut_log.h"
#endif

#define MEM_STATS_MAX_MODULE_ID       MAX_MODULE_ID

struct module_mem_info {
    uint32_t module_id;
    uint32_t memory_type;
    uint64_t timestamp;
    uint64_t total_size;
};

int dp_proc_mng_mem_stats_sample(struct module_mem_info *mem_info, uint32_t num, uint32_t devid);
void dp_proc_mng_module_used_size_update(uint32_t devid, uint32_t module_id, uint64_t size);
bool dp_proc_mng_get_prof_start_sample_flag(void);

#endif

