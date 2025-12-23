/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef DEVMM_PROC_EVENT_H_
#define DEVMM_PROC_EVENT_H_

struct devmm_add_group_proc {
    pid_t pid;
    bool alloc_attr;
};

struct devmm_process_cp_mmap {
    unsigned long long va;
    unsigned long long size;
};

struct devmm_process_cp_munmap {
    unsigned long long va;
    unsigned long long size;
};

struct devmm_event_msg_info {
    unsigned int subevent_id;
    int result_ret;
    struct event_proc_result result;
    unsigned int msg_len;
    const char *msg;
};

drvError_t devmm_process_task_mmap(uint32_t devid, enum devdrv_process_type proc_type,
    DVdeviceptr *va, size_t size, int fixed_va_flag);
drvError_t devmm_process_task_munmap(uint32_t devid, enum devdrv_process_type proc_type, DVdeviceptr va, size_t size);
drvError_t devmm_process_cp_mmap(uint32_t devid, DVdeviceptr *va, size_t size);
drvError_t devmm_process_cp_munmap(uint32_t devid, DVdeviceptr va);
void devmm_reset_event_devpid(uint32_t devid);

#endif

