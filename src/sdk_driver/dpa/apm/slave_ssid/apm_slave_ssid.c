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
#include "ka_kernel_def_pub.h"

#include "apm_task_group_def.h"
#include "apm_slave_ssid.h"
#include "apm_master_domain.h"
#include "apm_slave_domain.h"

/*
 * HOST:        query form master domain, or apm msg query form svm
 * DEVICE EP:   query from host master proxy domain, or query form svm
 * DEVICE RC:   query form master domain, or query form svm
 */
int hal_kernel_apm_query_slave_ssid_by_master(u32 udevid, int master_tgid, processType_t proc_type, u32 *ssid)
{
    struct apm_cmd_slave_ssid para;
    int ret;

    para.devid = udevid;
    para.proc_type = proc_type;
    ret = apm_master_query_domain_get_slave_ssid(master_tgid, &para);
    if (ret == 0) {
        goto query_success;
    }

    if (ret == -ENOSYS) { /* try to query ssid from slave */
        ret = _apm_query_slave_ssid_by_master(udevid, master_tgid, proc_type, &para.ssid);
        if (ret == 0) {
            (void)apm_master_query_domain_set_slave_ssid(master_tgid, &para);
            goto query_success;
        }
    }

    apm_warn("Query slave ssid. (master_tgid=%d; udevid=%u; proc_type=%u; ret=%d)\n",
        master_tgid, udevid, proc_type, ret);
    return ret;

query_success:
    *ssid = para.ssid;
    return ret;
}
KA_EXPORT_SYMBOL_GPL(hal_kernel_apm_query_slave_ssid_by_master);

/*
 * HOST:    not support
 * DEVICE:  query form slave domain, or query form svm
 */
int apm_query_slave_ssid(u32 udevid, int slave_tgid, int *ssid)
{
    struct apm_cmd_slave_ssid para;
    int ret;

    para.devid = udevid;
    ret = apm_slave_domain_get_ssid(slave_tgid, &para);
    if (ret == 0) {
        goto query_success;
    } else {
        ret = _apm_query_slave_ssid(udevid, slave_tgid, &para.ssid);
        if (ret == 0) {
            (void)apm_slave_domain_set_ssid(slave_tgid, &para);
            goto query_success;
        }
    }

    apm_warn("Query slave ssid. (udevid=%u; slave_tgid=%d; ret=%d)\n", udevid, slave_tgid, ret);
    return ret;

query_success:
    *ssid = para.ssid;
    return 0;
}
KA_EXPORT_SYMBOL_GPL(apm_query_slave_ssid);

