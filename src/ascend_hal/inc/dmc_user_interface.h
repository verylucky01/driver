/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef __DMC_USER_INTERFACE_H__
#define __DMC_USER_INTERFACE_H__

#include "dmc/dmc_log_flow.h"
#include "dmc/dmc_log_user.h"
#include "dmc/dmc_share_log.h"

#include "dmc/dev_mon_cmd_def.h"

void mem_prof_register_get_module_stats_func(int (*func)(uint32_t devid, uint32_t module_id, uint64_t *alloced_size));

#endif
