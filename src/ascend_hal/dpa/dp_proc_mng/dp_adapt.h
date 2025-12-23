/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef DP_ADAPT_H
#define DP_ADAPT_H

#include "dpa/dpa_dp_proc_mng.h"

#define MEM_STATS_DEVICE_CNT                        64

drvError_t dp_proc_mng_update_mbuff_and_process_mem_stats(struct module_mem_info *mem_info, uint32_t devid);
drvError_t dp_proc_mng_get_dev_info(uint32_t *dev_num, uint32_t *ids);
int dp_proc_mng_get_fd(uint32_t dev_id);
void dp_proc_mng_prof_stop(struct prof_sample_stop_para *para);
bool dp_proc_support_bind_cgroup(void);

#endif

