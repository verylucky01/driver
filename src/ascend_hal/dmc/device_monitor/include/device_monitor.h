/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef DEVICE_MONITOR_H
#define DEVICE_MONITOR_H

#ifdef IAM_CONFIG
int *dmp_get_proc_req_thread_cnt_ptr(int task_index);
#endif

signed int create_management_queue_task(void);
int dmp_start_time_sync(void);
#ifdef CFG_FEATURE_IAM_MONITOR
int dmp_iam_ready(void);
#endif

#endif
