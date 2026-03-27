/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include "securec.h"
#include "apm_ioctl.h"

static void drv_fill_bind_para(struct apm_cmd_bind *para, struct drvBindHostpidInfo *info)
{
    para->devid = info->chip_id;
    para->proc_type = (int)info->cp_type;
    para->mode = info->mode;
    para->master_pid = info->host_pid;
    (void)memcpy_s(&para->slave_pid, sizeof(int), info->sign, sizeof(int));
    if (para->slave_pid == 0) {
        para->slave_pid = getpid();
    }
}

drvError_t drvBindHostPid(struct drvBindHostpidInfo info)
{
    struct apm_cmd_bind para;
    int ret;

    if ((info.len != PROCESS_SIGN_LENGTH) || (info.vfid != 0)) {
        apm_err("Invalid param. (sig_len=%u; proc_type=%d; vfid=%d)\n", info.len, (int)info.cp_type, info.vfid);
        return DRV_ERROR_PARA_ERROR;
    }

    drv_fill_bind_para(&para, &info);

    ret = apm_cmd_ioctl(APM_BIND, &para);
    if (ret != 0) {
        apm_err("Bind pid fail. (ret=%d; devid=%u; master_pid=%d; proc_type=%d; mode=%d)\n",
            ret, para.devid, para.master_pid, para.proc_type, para.mode);
        return ret;
    }

    return DRV_ERROR_NONE;
}

drvError_t drvUnbindHostPid(struct drvBindHostpidInfo info)
{
    struct apm_cmd_bind para;
    int ret;

    if ((info.len != PROCESS_SIGN_LENGTH) || (info.vfid != 0)) {
        apm_err("Invalid param. (sig_len=%u; proc_type=%d; vfid=%d)\n", info.len, (int)info.cp_type, info.vfid);
        return DRV_ERROR_PARA_ERROR;
    }

    drv_fill_bind_para(&para, &info);

    ret = apm_cmd_ioctl(APM_UNBIND, &para);
    if (ret != 0) {
        apm_err("Unbind pid fail. (ret=%d; devid=%u; master_pid=%d; proc_type=%d)\n",
            ret, para.devid, para.master_pid, para.proc_type);
        return ret;
    }

    return DRV_ERROR_NONE;
}

static inline int apm_trans_first_proc_type_from_bitmap(unsigned int proc_type_bitmap)
{
    int proc_type;

    for (proc_type = 0; proc_type < PROCESS_CPTYPE_MAX; proc_type++) {
        if ((proc_type_bitmap & (unsigned int)(0x1 << proc_type)) != 0) {
            break;
        }
    }

    return proc_type;
}

int apm_query_master_pid(unsigned int cmd, int slave_pid,
    unsigned int *udevid, unsigned int *master_pid, unsigned int *proc_type)
{
    struct apm_cmd_query_master_info para = {.slave_pid = (unsigned int)slave_pid};
    int ret;

    if (cmd == APM_QUERY_MASTER_INFO_BY_DEVICE_SLAVE) {
        para.udevid = *udevid;
    }

    ret = apm_cmd_ioctl(cmd, &para);
    if (ret != 0) {
        apm_warn("Query bindpid warn. (ret=%d; slave_pid=%d)\n", ret, slave_pid);
        return ret;
    }

    if (udevid != NULL) {
        *udevid = para.udevid;
    }

    if (master_pid != NULL) {
        *master_pid = (unsigned int)para.master_pid;
    }

    if (proc_type != NULL) {
        *proc_type = (unsigned int)apm_trans_first_proc_type_from_bitmap(para.proc_type_bitmap);
    }

    return DRV_ERROR_NONE;
}

drvError_t drvQueryProcessHostPid(int pid, unsigned int *chip_id, unsigned int *vfid,
    unsigned int *host_pid, unsigned int *cp_type)
{
    if (vfid != NULL) {
        *vfid = 0;
    }

    return apm_query_master_pid(APM_QUERY_MASTER_INFO, pid, chip_id, host_pid, cp_type);
}

int halQueryMasterPidByDeviceSlave(unsigned int devid, int slave_pid, unsigned int *master_pid, unsigned int *proc_type)
{
    return apm_query_master_pid(APM_QUERY_MASTER_INFO_BY_DEVICE_SLAVE, slave_pid, &devid, master_pid, proc_type);
}

int halQueryMasterPidByHostSlave(int slave_pid, unsigned int *udevid, unsigned int *master_pid, unsigned int *proc_type)
{
    return apm_query_master_pid(APM_QUERY_MASTER_INFO_BY_HOST_SLAVE, slave_pid, udevid, master_pid, proc_type);
}
