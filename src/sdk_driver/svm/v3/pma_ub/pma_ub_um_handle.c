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
#include <linux/types.h>

#include "pbl_uda.h"

#include "svm_urma_seg_flag.h"
#include "svm_kern_log.h"
#include "svm_um_handle.h"
#include "urma_seg_msg_head.h"
#include "pma_ub_core.h"
#include "pma_ub_ubdevshm_wrapper.h"
#include "pma_ub_um_handle.h"

static int um_urma_seg_register_pre_handle(u32 udevid, int master_tgid, int slave_tgid,
    void *msg, u32 msg_len)
{
    struct svm_urma_seg_msg_head *msg_head = (struct svm_urma_seg_msg_head *)msg;
    int ret;

    if ((msg_len < sizeof(struct svm_urma_seg_msg_head)) || (udevid == uda_get_host_id())) {
        svm_err("Invalid para. (udevid=%u; master_tgid=%d; slave_tgid=%d; msg_len=%u)\n",
            udevid, master_tgid, slave_tgid, msg_len);
        return -EINVAL;
    }

    if (svm_urma_seg_flag_is_self_user(msg_head->flag)) {
        ret = pma_ub_ubdevshm_register_segment(udevid, master_tgid, msg_head->va, msg_head->size);
        if (ret != 0) {
            svm_err("Pma_ub register seg failed. (ret=%d; udevid=%u; tgid=%d; va=0x%llx; size=%llu)\n",
                ret, udevid, master_tgid, msg_head->va, msg_head->size);
            return ret;
        }
    }

    return 0;
}

static void um_urma_seg_register_pre_cancel_handle(u32 udevid, int master_tgid, int slave_tgid,
    void *msg, u32 msg_len)
{
    struct svm_urma_seg_msg_head *msg_head = (struct svm_urma_seg_msg_head *)msg;
    int ret;

    if ((msg_len < sizeof(struct svm_urma_seg_msg_head)) || (udevid == uda_get_host_id())) {
        svm_err("Invalid para. (udevid=%u; master_tgid=%d; slave_tgid=%d; msg_len=%u)\n",
            udevid, master_tgid, slave_tgid, msg_len);
        return;
    }

    if (svm_urma_seg_flag_is_self_user(msg_head->flag)) {
        ret = pma_ub_ubdevshm_unregister_segment(udevid, master_tgid, msg_head->va, msg_head->size);
        if (ret != 0) {
            svm_err("Pma_ub unregister seg failed. (ret=%d; udevid=%u; tgid=%d; va=0x%llx; size=%llu)\n",
                ret, udevid, master_tgid, msg_head->va, msg_head->size);
        }
    }
}

static int um_urma_seg_register_post_handle(u32 udevid, int master_tgid, int slave_tgid,
    void *msg, u32 msg_len)
{
    struct svm_urma_seg_msg_head *msg_head = (struct svm_urma_seg_msg_head *)msg;
    int ret;

    if ((msg_len < sizeof(struct svm_urma_seg_msg_head)) || (udevid == uda_get_host_id())) {
        svm_err("Invalid para. (udevid=%u; master_tgid=%d; slave_tgid=%d; msg_len=%u)\n",
            udevid, master_tgid, slave_tgid, msg_len);
        return -EINVAL;
    }

    if (svm_urma_seg_flag_is_self_user(msg_head->flag)) {
        ret = pma_ub_register_seg(udevid, master_tgid, msg_head->va, msg_head->size, msg_head->token_id);
        if (ret != 0) {
            svm_err("Pma_ub unregister seg failed. (ret=%d; udevid=%u; tgid=%d; va=0x%llx; size=%llu)\n",
                ret, udevid, master_tgid, msg_head->va, msg_head->size);
            (void)pma_ub_ubdevshm_unregister_segment(udevid, master_tgid, msg_head->va, msg_head->size);
            return ret;
        }
    }

    return 0;
}

static int um_urma_seg_unregister_pre_handle(u32 udevid, int master_tgid, int slave_tgid,
    void *msg, u32 msg_len)
{
    struct svm_urma_seg_msg_head *msg_head = (struct svm_urma_seg_msg_head *)msg;
    u64 start, size;
    u32 token_id;
    int ret;

    if ((msg_len < sizeof(struct svm_urma_seg_msg_head)) || (udevid == uda_get_host_id())) {
        svm_err("Invalid para. (udevid=%u; master_tgid=%d; slave_tgid=%d; msg_len=%u)\n",
            udevid, master_tgid, slave_tgid, msg_len);
        return -EINVAL;
    }

    if (svm_urma_seg_flag_is_self_user(msg_head->flag)) {
        ret = pma_ub_get_register_seg_info(udevid, master_tgid, msg_head->va, &start, &size, &token_id);
        if (ret != 0) {
            svm_err("Pma_ub get seg info failed. (ret=%d; udevid=%u; tgid=%d; va=0x%llx)\n",
                ret, udevid, master_tgid, msg_head->va);
            return ret;
        }
        if (msg_head->va != start) {
            svm_err("Pma_ub get seg info mismatch. (udevid=%u; tgid=%d; va=0x%llx; start=0x%llx)\n",
                udevid, master_tgid, msg_head->va, start);
            return -EINVAL;
        }

        ret = pma_ub_unregister_seg(udevid, master_tgid, start, size);
        if (ret != 0) {
            svm_err("Pma_ub del seg failed. (ret=%d; udevid=%u; tgid=%d; va=0x%llx; size=%llu)\n",
                ret, udevid, master_tgid, start, size);
            return ret;
        }

        ret = pma_ub_ubdevshm_unregister_segment(udevid, master_tgid, start, size);
        if (ret != 0) {
            svm_err("Pma_ub unregister seg failed. (ret=%d; udevid=%u; tgid=%d; va=0x%llx; size=%llu)\n",
                ret, udevid, master_tgid, start, size);
            return ret;
        }

        msg_head->size = size;
    }

    return 0;
}

void pma_ub_um_handle_init(void)
{
    svm_um_register_handle(SVM_URMA_SEG_REGISTER_EVENT, um_urma_seg_register_pre_handle,
        um_urma_seg_register_pre_cancel_handle, um_urma_seg_register_post_handle);
    svm_um_register_handle(SVM_URMA_SEG_UNREGISTER_EVENT, um_urma_seg_unregister_pre_handle, NULL, NULL);
}
