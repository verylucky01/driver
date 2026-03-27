/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */
#include "pbl_uda.h"

#include "svm_um_handle.h"
#include "svm_kern_log.h"
#include "memset_msg.h"
#include "memcpy_msg.h"
#include "urma_seg_msg_head.h"
#include "svm_smp.h"
#include "ksvmm.h"
#include "op_um_handle.h"

static int um_memset_pre_handle(u32 udevid, int master_tgid, int slave_tgid, void *_msg, u32 msg_len)
{
    struct svm_memset_msg *msg = (struct svm_memset_msg *)_msg;
    int ret;

    if ((msg_len != sizeof(*msg)) || (udevid == uda_get_host_id())) {
        svm_err("Invalid para. (udevid=%u; master_tgid=%d; slave_tgid=%d; msg_len=%u)\n",
            udevid, master_tgid, slave_tgid, msg_len);
        return -EINVAL;
    }

    ret = svm_smp_check_mem_exists(udevid, master_tgid, msg->va, msg->size);
    if (ret != 0) {
        ret = ksvmm_check_range(udevid, master_tgid, msg->va, msg->size);
        if (ret != 0) {
            svm_err("Invalid para. (udevid=%u; va=0x%llx; size=0x%llx)\n", udevid, msg->va, msg->size);
            return -EINVAL;
        }
    }

    return 0;
}

static int um_memcpy_local_pre_handle(u32 udevid, int master_tgid, int slave_tgid, void *_msg, u32 msg_len)
{
    struct svm_memcpy_local_msg *msg = (struct svm_memcpy_local_msg *)_msg;
    int ret;

    if ((msg_len != sizeof(*msg)) || (udevid == uda_get_host_id())) {
        svm_err("Invalid para. (udevid=%u; master_tgid=%d; slave_tgid=%d; msg_len=%u)\n",
            udevid, master_tgid, slave_tgid, msg_len);
        return -EINVAL;
    }

    ret = svm_smp_check_mem_exists(udevid, master_tgid, msg->src, msg->size);
    if (ret != 0) {
        ret = ksvmm_check_range(udevid, master_tgid, msg->src, msg->size);
        if (ret != 0) {
            svm_err("Invalid src para. (udevid=%u; src=0x%llx; size=0x%llx)\n", udevid, msg->src, msg->size);
            return -EINVAL;
        }
    }

    ret = svm_smp_check_mem_exists(udevid, master_tgid, msg->dst, msg->size);
    if (ret != 0) {
        ret = ksvmm_check_range(udevid, master_tgid, msg->dst, msg->size);
        if (ret != 0) {
            svm_err("Invalid dst para. (udevid=%u; dst=0x%llx; size=0x%llx)\n", udevid, msg->dst, msg->size);
            return -EINVAL;
        }
    }

    return 0;
}

void op_um_handle_init(void)
{
    svm_um_register_handle(SVM_MEMSET_EVENT, um_memset_pre_handle, NULL, NULL);
    svm_um_register_handle(SVM_MEMCPY_LOCAL_EVENT, um_memcpy_local_pre_handle, NULL, NULL);
}

