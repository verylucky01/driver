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

#include "pbl_feature_loader.h"
#include "pbl_soc_res.h"
#include "pbl_uda.h"
#include "svm_pub.h"
#include "svm_kern_log.h"
#include "svm_sub_event_type.h"
#include "va_reserve_msg.h"
#include "linear_mem.h"

#define AIC_ATU_CFG_NAME "AIC_ATU_CFG_BASE"

int linear_mem_enable(u32 udevid, int tgid, void *start_time)
{
    static bool linear_mem_enable_flag = false;
    int ret;

    if (linear_mem_enable_flag == false) {
        ret = uda_dev_ctrl(udevid, UDA_CTRL_AIC_ATU_CFG);
        if (ret != 0) {
            svm_err("uda_dev_ctrl_ex failed. (ret=%d; udevid=%u)\n", ret, udevid);
            return ret;
        }
        linear_mem_enable_flag = true;
    }

    return 0;
}
DECLAER_FEATURE_AUTO_INIT_TASK(linear_mem_enable, FEATURE_LOADER_STAGE_2);

static int linear_mem_set_addr_info(u32 udevid, u64 va, u64 size)
{
    struct soc_reg_base_info io_base;
    int ret;

    ret = soc_resmng_dev_get_reg_base(udevid, AIC_ATU_CFG_NAME, &io_base);
    if (ret == 0) {
        if ((io_base.io_base == va) && (io_base.io_base_size == size)) {
            return 0;
        }
        svm_warn("Repeat set different base. (udevid=%u)\n", udevid);
    }

    io_base.io_base = va;
    io_base.io_base_size = size;
    ret = soc_resmng_dev_set_reg_base(udevid, AIC_ATU_CFG_NAME, &io_base);
    if (ret != 0) {
        svm_err("Set linear_mem_base failed. (ret=%d; udevid=%u)\n", ret, udevid);
        return ret;
    }

    return 0;
}

static int um_va_reserve_post_handle(u32 udevid, int master_tgid, int slave_tgid, void *msg, u32 msg_len)
{
    struct svm_va_reserve_msg *reserve_msg = (struct svm_va_reserve_msg *)msg;
    int ret = 0;

    if (msg_len != sizeof(*reserve_msg)) {
        svm_err("Invalid para. (udevid=%u; master_tgid=%d; slave_tgid=%d; msg_len=%u)\n",
            udevid, master_tgid, slave_tgid, msg_len);
        return -EINVAL;
    }

    if (reserve_msg->status != VA_RESERVE_STATUS_OK) {
        return 0;
    }

    /* Use size for approximate identification of linear mem */
    if (((reserve_msg->flag & SVM_MMAP_FLAG_PRIVATE) != 0) && (reserve_msg->size > SVM_BYTES_PER_TB)) {
        ret = linear_mem_set_addr_info(udevid, reserve_msg->va, reserve_msg->size);
    }

    return ret;
}

int linear_mem_device_um_handle_init(void)
{
    svm_um_register_handle(SVM_VA_RESERVE_EVENT, NULL, NULL, um_va_reserve_post_handle);

    return 0;
}
DECLAER_FEATURE_AUTO_INIT(linear_mem_device_um_handle_init, FEATURE_LOADER_STAGE_2);

