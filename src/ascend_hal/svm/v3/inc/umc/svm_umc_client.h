/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef SVM_UMC_CLIENT_H
#define SVM_UMC_CLIENT_H

#include "esched_user_interface.h"

#include "svm_pub.h"

struct svm_umc_msg {
    char *msg_in;
    u64 msg_in_len;
    char *msg_out;
    u64 msg_out_len;
};

struct svm_umc_msg_head {
    u32 devid;
    int pid;
    u32 grp_id;
    u32 subevent_id;
};

#define UMC_TO_LOCAL             (0x1U << 0U)
#define UMC_DEVICE_SUBMIT        (0x1U << 1U)

static inline void svm_umc_msg_head_pack(u32 devid, int pid, u32 grp_id, u32 subevent_id,
    struct svm_umc_msg_head *head)
{
    head->devid = devid;
    head->pid = pid;
    head->grp_id = grp_id;
    head->subevent_id = subevent_id;
}

int svm_umc_send(struct svm_umc_msg_head *head, u32 flag, struct svm_umc_msg *msg);
int svm_umc_h2d_send(struct svm_umc_msg_head *head, struct svm_umc_msg *msg);

static inline int svm_umc_local_send(struct svm_umc_msg_head *head, struct svm_umc_msg *msg)
{
    return svm_umc_send(head, UMC_TO_LOCAL, msg);
}

static inline int svm_umc_d2h_send(struct svm_umc_msg_head *head, struct svm_umc_msg *msg)
{
    return svm_umc_send(head, UMC_DEVICE_SUBMIT, msg);
}
#endif
