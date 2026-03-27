/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include <errno.h>
#include <securec.h>

#include "ascend_hal_define.h"

#include "svm_pub.h"
#include "svm_log.h"
#include "svm_ioctl_ex.h"
#include "svm_sys_cmd.h"
#include "svm_user_adapt.h"
#include "malloc_mng.h"
#include "memcpy_msg.h"
#include "svm_memcpy_local.h"
#include "svm_umc_client.h"
#include "svm_sub_event_type.h"
#include "svm_dbi.h"
#include "svm_apbi.h"
#include "svm_memcpy.h"
#include "svm_memcpy_local_client.h"

static int svm_memcpy_local_event(u32 devid, u64 dst, u64 dst_max, u64 src, u64 count)
{
    struct svm_umc_msg_head head;
    struct svm_memcpy_local_msg memcpy_local_msg = {.src = src, .dst = dst, .size = count};
    struct svm_umc_msg msg = {
        .msg_in = (char *)(uintptr_t)&memcpy_local_msg,
        .msg_in_len = sizeof(struct svm_memcpy_local_msg),
        .msg_out = NULL,
        .msg_out_len = 0
    };
    struct svm_apbi apbi;
    int ret;
    SVM_UNUSED(dst_max);

    ret = svm_apbi_query(devid, DEVDRV_PROCESS_CP1, &apbi);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    svm_umc_msg_head_pack(devid, apbi.tgid, apbi.grp_id, SVM_MEMCPY_LOCAL_EVENT, &head);
    ret = svm_umc_h2d_send(&head, &msg);
    if (ret == DRV_ERROR_NO_PROCESS) {
        (void)svm_apbi_clear(devid, DEVDRV_PROCESS_CP1);
    }

    return ret;
}

int svm_memcpy_local_client(u32 devid, u64 dst, u64 dst_max, u64 src, u64 count)
{
    if (devid != svm_get_host_devid()) {
        return svm_memcpy_local_event(devid, dst, dst_max, src, count);
    } else {
        return svm_memcpy_local(dst, dst_max, src, count);
    }
}
