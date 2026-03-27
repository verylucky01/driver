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
#include "ka_base_pub.h"
#include "ka_compiler_pub.h"
#include "ka_ioctl_pub.h"

#include "apm_task_group_def.h"
#include "apm_slave_ssid.h"

int _apm_query_slave_ssid(u32 udevid, int slave_tgid, int *ssid)
{
    return -EOPNOTSUPP;
}

int _apm_query_slave_ssid_by_master(u32 udevid, int master_tgid, processType_t proc_type, u32 *ssid)
{
    struct apm_msg_query_slave_ssid msg;
    int ret;

    apm_msg_fill_header(&msg.header, APM_MSG_TYPE_QUERY_SLAVE_SSID);
    msg.master_tgid = master_tgid;
    msg.proc_type = proc_type;

    ret = apm_msg_send(udevid, &msg.header, sizeof(msg));
    if (ret == 0) {
        *ssid = msg.ssid;
    }

    return ret;
}

static int apm_fops_query_slave_ssid_by_master(u32 cmd, unsigned long arg)
{
    struct apm_cmd_slave_ssid *usr_arg = (struct apm_cmd_slave_ssid __ka_user *)(uintptr_t)arg;
    struct apm_cmd_slave_ssid para;
    int ret;

    ret = (int)ka_base_copy_from_user(&para, usr_arg, sizeof(para));
    if (ret != 0) {
        apm_err("Copy from user failed. (ret=%d)\n", ret);
        return ret;
    }

    ret = apm_devid_to_udevid(para.devid, &para.devid);
    if (ret != 0) {
        apm_err("Get udevid failed. (ret=%d; devid=%d)\n", ret, para.devid);
        return ret;
    }

    ret = hal_kernel_apm_query_slave_ssid_by_master(para.devid, ka_task_get_current_tgid(), PROCESS_CP1, &para.ssid);
    if (ret != 0) {
        apm_err("Query failed. (ret=%d; udevid=%u; master_tgid=%d)\n", ret, para.devid, ka_task_get_current_tgid());
        return ret;
    }

    /* ssid not support return to user space */
    return ka_base_put_user(ka_task_get_current_tgid(), &usr_arg->ssid);
}

int apm_slave_ssid_init(void)
{
    apm_register_ioctl_cmd_func(_KA_IOC_NR(APM_QUERY_SSID), apm_fops_query_slave_ssid_by_master);
    return 0;
}
DECLAER_FEATURE_AUTO_INIT(apm_slave_ssid_init, FEATURE_LOADER_STAGE_8);

