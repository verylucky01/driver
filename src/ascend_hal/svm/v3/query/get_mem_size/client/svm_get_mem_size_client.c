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
#include "svm_sub_event_type.h"
#include "svm_umc_client.h"
#include "svm_user_adapt.h"
#include "svm_apbi.h"

int svm_get_mem_size_info(u32 devid, u32 type, struct MemPhyInfo *phy_info)
{
    struct svm_umc_msg_head head;
    struct svm_umc_msg msg = {
        .msg_in = (char *)&type,
        .msg_in_len = sizeof(u32),
        .msg_out = (char *)phy_info,
        .msg_out_len = sizeof(struct MemPhyInfo)
    };
    struct svm_apbi apbi;
    int ret;

    ret = svm_apbi_query(devid, DEVDRV_PROCESS_CP1, &apbi);
    if (ret != DRV_ERROR_NONE) {
        return ret;
    }

    svm_umc_msg_head_pack(devid, apbi.tgid, apbi.grp_id, SVM_GET_MEMINFO_EVENT, &head);
    ret = svm_umc_h2d_send(&head, &msg);
    if (ret == DRV_ERROR_NO_PROCESS) {
        (void)svm_apbi_clear(devid, DEVDRV_PROCESS_CP1);
    }

    return ret;
}
