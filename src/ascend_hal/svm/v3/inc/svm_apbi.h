/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef SVM_APBI_H
#define SVM_APBI_H

#include "svm_pub.h"

struct svm_apbi {
    int tgid;
    u32 grp_id;
};

/* if agent process is not exist, return DRV_ERROR_NO_PROCESS */
int svm_apbi_query(u32 devid, int task_type, struct svm_apbi *apbi);
int svm_apbi_update(u32 devid, int task_type);
void svm_apbi_clear(u32 devid, int task_type);

static inline int svm_apbi_query_tgid(u32 devid, int task_type, int *tgid)
{
    struct svm_apbi apbi;
    int ret;

    ret = svm_apbi_query(devid, task_type, &apbi);
    if (ret == DRV_ERROR_NONE) {
        *tgid = apbi.tgid;
    }
    return ret;
}

static inline int svm_apbi_query_grp_id(u32 devid, int task_type, u32 *grp_id)
{
    struct svm_apbi apbi;
    int ret;

    ret = svm_apbi_query(devid, task_type, &apbi);
    if (ret == DRV_ERROR_NONE) {
        *grp_id = apbi.grp_id;
    }
    return ret;
}

#endif
