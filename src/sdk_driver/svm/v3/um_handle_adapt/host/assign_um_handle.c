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
#include "mpl_msg.h"
#include "mpl_flag.h"
#include "svm_smp.h"
#include "pma.h"
#include "assign_um_handle.h"
#include "mms_kern.h"

static int um_mpl_populate_pre_handle(u32 udevid, int master_tgid, int slave_tgid, void *msg, u32 msg_len)
{
    struct svm_mpl_populate_msg *populate_msg = (struct svm_mpl_populate_msg *)msg;
    int ret;

    if ((msg_len != sizeof(*populate_msg)) || (udevid == uda_get_host_id())) {
        svm_err("Invalid para. (udevid=%u; master_tgid=%d; slave_tgid=%d; msg_len=%u)\n",
            udevid, master_tgid, slave_tgid, msg_len);
        return -EINVAL;
    }

    ret = svm_smp_check_mem_exists(udevid, master_tgid, populate_msg->va, populate_msg->size);
    if (ret == 0) {
        svm_err("Smp existed, not allow to populate. (udevid=%u; master_tgid=%d; va=0x%llx; size=%llu)\n",
            udevid, master_tgid, populate_msg->va, populate_msg->size);
        return -EADDRINUSE;
    }
    return 0;
}

static void um_mpl_populate_pre_cancel_handle(u32 udevid, int master_tgid, int slave_tgid, void *msg, u32 msg_len)
{
    svm_mms_dev_show(udevid);
}

static int um_mpl_populate_post_handle(u32 udevid, int master_tgid, int slave_tgid, void *msg, u32 msg_len)
{
    struct svm_mpl_populate_msg *populate_msg = (struct svm_mpl_populate_msg *)msg;
    u32 flag = 0;
    int ret;

    if ((msg_len != sizeof(*populate_msg)) || (udevid == uda_get_host_id())) {
        svm_err("Invalid para. (udevid=%u; master_tgid=%d; slave_tgid=%d; msg_len=%u)\n",
            udevid, master_tgid, slave_tgid, msg_len);
        return -EINVAL;
    }

    flag |= ((populate_msg->flag & SVM_MPL_FLAG_DEV_CP_ONLY) != 0) ? SVM_SMP_FLAG_DEV_CP_ONLY : 0;

    ret = svm_smp_add_mem(udevid, master_tgid, populate_msg->va, populate_msg->size, flag);
    if (ret != 0) {
        svm_err("Smp add mem failed in mpl um post handle. (udevid=%u; master_tgid=%d; va=0x%llx; size=%llu)\n",
            udevid, master_tgid, populate_msg->va, populate_msg->size);
    }
    return ret;
}

static int um_mpl_depopulate_pre_handle(u32 udevid, int master_tgid, int slave_tgid, void *msg, u32 msg_len)
{
    struct svm_mpl_depopulate_msg *depopulate_msg = (struct svm_mpl_depopulate_msg *)msg;
    int ret;

    if ((msg_len != sizeof(*depopulate_msg)) || (udevid == uda_get_host_id())) {
        svm_err("Invalid para. (udevid=%u; master_tgid=%d; slave_tgid=%d; msg_len=%u)\n",
            udevid, master_tgid, slave_tgid, msg_len);
        return -EINVAL;
    }

    ret = svm_smp_del_mem(udevid, master_tgid, depopulate_msg->va);
    if (ret == -EBUSY) {
        depopulate_msg->is_busy = 1;
    }

    pma_mem_recycle_notify(udevid, master_tgid, depopulate_msg->va, depopulate_msg->size);

    return ((ret == -EBUSY) ? 0 : ret);
}

static int um_mpl_populate_no_pin_pre_handle(u32 udevid, int master_tgid, int slave_tgid, void *msg, u32 msg_len)
{
    struct svm_mpl_populate_msg *populate_msg = (struct svm_mpl_populate_msg *)msg;
    int ret;

    if ((msg_len != sizeof(*populate_msg)) || (udevid == uda_get_host_id())) {
        svm_err("Invalid para. (udevid=%u; master_tgid=%d; slave_tgid=%d; msg_len=%u)\n",
            udevid, master_tgid, slave_tgid, msg_len);
        return -EINVAL;
    }

    ret = svm_smp_check_mem(udevid, master_tgid, populate_msg->va, populate_msg->size);
    if (ret != 0) {
        ret = svm_smp_check_dev_cp_only_mem(udevid, master_tgid, populate_msg->va, populate_msg->size);
    }

    return ret;
}

static int um_mpl_depopulate_no_unpin_pre_handle(u32 udevid, int master_tgid, int slave_tgid, void *msg, u32 msg_len)
{
    struct svm_mpl_depopulate_msg *depopulate_msg = (struct svm_mpl_depopulate_msg *)msg;
    int ret;

    if ((msg_len != sizeof(*depopulate_msg)) || (udevid == uda_get_host_id())) {
        svm_err("Invalid para. (udevid=%u; master_tgid=%d; slave_tgid=%d; msg_len=%u)\n",
            udevid, master_tgid, slave_tgid, msg_len);
        return -EINVAL;
    }

    ret = svm_smp_check_mem(udevid, master_tgid, depopulate_msg->va, depopulate_msg->size);
    if (ret != 0) {
        ret = svm_smp_check_dev_cp_only_mem(udevid, master_tgid, depopulate_msg->va, depopulate_msg->size);
    }
    if (ret == 0) {
        pma_mem_recycle_notify(udevid, master_tgid, depopulate_msg->va, depopulate_msg->size);
    }

    return ret;
}

void assign_um_handle_init(void)
{
    svm_um_register_handle(SVM_MPL_POPULATE_EVENT, um_mpl_populate_pre_handle, um_mpl_populate_pre_cancel_handle, um_mpl_populate_post_handle);
    svm_um_register_handle(SVM_MPL_DEPOPULATE_EVENT, um_mpl_depopulate_pre_handle, NULL, NULL);
    svm_um_register_handle(SVM_MPL_POPULATE_NO_PIN_EVENT, um_mpl_populate_no_pin_pre_handle, NULL, NULL);
    svm_um_register_handle(SVM_MPL_DEPOPULATE_NO_UNPIN_EVENT, um_mpl_depopulate_no_unpin_pre_handle, NULL, NULL);
}

