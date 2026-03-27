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
#include "ka_ioctl_pub.h"

#include "pbl_uda.h"

#include "svm_ioctl_ex.h"
#include "framework_cmd.h"
#include "svm_kern_log.h"
#include "mpl_ioctl.h"
#include "svm_smp.h"
#include "assign_cmd_handle.h"
#include "mms_kern.h"

static int cmd_mpl_populate_post_handle(u32 udevid, u32 cmd, void *para)
{
    struct svm_mpl_populate_para *populate_para = (struct svm_mpl_populate_para *)para;
    u32 flag = 0;

    if (udevid != uda_get_host_id()) {
        svm_err("Invalid para. (udevid=%u; cmd=%u)\n", udevid, cmd);
        return -EINVAL;
    }

    return svm_smp_add_mem(udevid, ka_task_get_current_tgid(), populate_para->va, populate_para->size, flag);
}

static int cmd_mpl_populate_pre_handle(u32 udevid, u32 cmd, void *para)
{
    return 0;
}

static void cmd_mpl_populate_pre_cancle_handle(u32 udevid, u32 cmd, void *para)
{
    svm_mms_dev_show(udevid);
}

static int cmd_mpl_depopulate_pre_handle(u32 udevid, u32 cmd, void *para)
{
    struct svm_mpl_depopulate_para *depopulate_para = (struct svm_mpl_depopulate_para *)para;

    if (udevid != uda_get_host_id()) {
        svm_err("Invalid para. (udevid=%u; cmd=%u)\n", udevid, cmd);
        return -EINVAL;
    }

    return svm_smp_del_mem(udevid, ka_task_get_current_tgid(), depopulate_para->va);
}

/* for host mem pin */
void assign_cmd_handle_init(void)
{
    svm_register_ioctl_cmd_post_handle(_IOC_NR(SVM_MPL_POPULATE), cmd_mpl_populate_post_handle);
    svm_register_ioctl_cmd_pre_handle(_IOC_NR(SVM_MPL_POPULATE), cmd_mpl_populate_pre_handle);
    svm_register_ioctl_cmd_pre_cancle_handle(_IOC_NR(SVM_MPL_POPULATE), cmd_mpl_populate_pre_cancle_handle);
    svm_register_ioctl_cmd_pre_handle(_IOC_NR(SVM_MPL_DEPOPULATE), cmd_mpl_depopulate_pre_handle);
}
