/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef SVM_EVENT_GRP_ID_H
#define SVM_EVENT_GRP_ID_H

#include "ascend_hal.h"

#include "svm_pub.h"
#include "svm_log.h"

static inline int svm_event_get_grp_id(u32 devid, int pid, enum esched_query_type type, u32 *grp_id)
{
    struct esched_query_gid_output gid_out = {0};
    struct esched_query_gid_input gid_in = {.pid = pid, .grp_name = EVENT_DRV_MSG_GRP_NAME};
    struct esched_output_info out_put = {.outBuff = &gid_out, .outLen = sizeof(struct esched_query_gid_output)};
    struct esched_input_info in_put = {.inBuff = &gid_in, .inLen = sizeof(struct esched_query_gid_input)};
    int ret;

    ret = halEschedQueryInfo(devid, type, &in_put, &out_put);
    if (ret != DRV_ERROR_NONE) {
        svm_err_if((ret != DRV_ERROR_NO_PROCESS),
            "Get grpid failed. (ret=%d; devid=%u; pid=%d).\n", ret, devid, pid);
        return (ret != DRV_ERROR_NO_PROCESS) ? DRV_ERROR_INNER_ERR : ret;
    }

    *grp_id = gid_out.grp_id;
    return DRV_ERROR_NONE;
}

static inline int svm_event_get_remote_grp_id(u32 devid, int pid, u32 *grp_id)
{
    return svm_event_get_grp_id(devid, pid, QUERY_TYPE_REMOTE_GRP_ID, grp_id);
}

static inline int svm_event_get_local_grp_id(u32 devid, int pid, u32 *grp_id)
{
    return svm_event_get_grp_id(devid, pid, QUERY_TYPE_LOCAL_GRP_ID, grp_id);
}

#endif
