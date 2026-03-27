/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include "ascend_hal.h"

#include "svm_pub.h"
#include "svm_log.h"
#include "svm_umc_client.h"
#include "svm_sub_event_type.h"
#include "svm_apbi.h"
#include "svm_dbi.h"
#include "madvise.h"
#include "madvise_msg.h"
#include "madvise_client.h"

static int svm_madvise_remote(u32 devid, u64 va, u64 size, u32 flag)
{
    struct svm_umc_msg_head head;
    struct svm_madvise_msg madvise_msg = {.size = size, .flag = flag, .va = va};
    struct svm_umc_msg msg = {
        .msg_in = (char *)(uintptr_t)&madvise_msg,
        .msg_in_len = sizeof(struct svm_madvise_msg),
        .msg_out = NULL,
        .msg_out_len = 0
    };
    struct svm_apbi apbi;
    u32 event_id = SVM_MADVISE_EVENT;
    u32 task_type = PROCESS_CP1;
    int ret;

    ret = svm_apbi_query(devid, (int)task_type, &apbi);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    svm_umc_msg_head_pack(devid, apbi.tgid, apbi.grp_id, event_id, &head);
    ret = svm_umc_h2d_send(&head, &msg);
    if (ret != DRV_ERROR_NONE) {
        if (ret == DRV_ERROR_NO_PROCESS) {
            (void)svm_apbi_clear(devid, (int)task_type);
        }
        svm_err("Mem advise msg failed. (devid=%u; devpid=%d; ret=%d; va=0x%llx; size=%llu; flag=0x%llx; "
            "task_type=%u)\n", devid, apbi.tgid, ret, va, size, flag, task_type);
        return ret;
    }

    return DRV_ERROR_NONE;
}

int svm_madvise_client(u32 devid, u64 va, u64 size, u32 flag)
{
    if (devid == svm_get_host_devid()) {
        return svm_madvise(devid, va, size, flag);
    } else {
        return svm_madvise_remote(devid, va, size, flag);
    }
}
