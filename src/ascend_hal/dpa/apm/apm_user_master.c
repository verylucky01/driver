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

#include "dpa/dpa_apm.h"
#include "apm_ioctl.h"

drvError_t drvGetProcessSign(struct process_sign *sign)
{
    struct apm_cmd_get_sign para;
    int ret;

    if (sign == NULL) {
        apm_err("Sign is NULL.\n");
        return DRV_ERROR_PARA_ERROR;
    }

    ret = apm_cmd_ioctl(APM_GET_SIGN, &para);
    if (ret != 0) {
        apm_err("Get proc sign fail. (ret=%d)\n", ret);
        return ret;
    }

    sign->tgid = (pid_t)para.cur_tgid;
    (void)memset_s((void *)sign->sign, PROCESS_SIGN_LENGTH, 0, PROCESS_SIGN_LENGTH);

    return DRV_ERROR_NONE;
}

int apm_query_slave_pid(unsigned long cmd, int master_pid, unsigned int devid, enum devdrv_process_type proc_type,
    int *slave_pid)
{
    struct apm_cmd_query_slave_pid para;
    int ret;

    if (slave_pid == NULL) {
        apm_err("slave_pid is NULL\n");
        return DRV_ERROR_PARA_ERROR;
    }

    para.devid = devid;
    para.master_pid = master_pid;
    para.proc_type = proc_type;
    ret = apm_cmd_ioctl(cmd, &para);
    if (ret != 0) {
        apm_warn("Query pid warn. (ret=%d; cmd=%u; devid=%u; master_pid=%d; proc_type=%d)\n",
            ret, _IOC_NR(cmd), para.devid, para.master_pid, para.proc_type);
        return ret;
    }

    *slave_pid = para.slave_pid;

    return DRV_ERROR_NONE;
}

drvError_t halQuerySlavePid(int master_pid, unsigned int devid, enum devdrv_process_type proc_type,
    int *slave_pid)
{
    return apm_query_slave_pid(APM_QUERY_SLAVE_PID, master_pid, devid, proc_type, slave_pid);
}

drvError_t halQuerySlavePidByLocalMaster(int master_pid, unsigned int devid, enum devdrv_process_type proc_type,
    int *slave_pid)
{
    return apm_query_slave_pid(APM_QUERY_SLAVE_PID_BY_LOCAL_MASTER, master_pid, devid, proc_type, slave_pid);
}

drvError_t halQueryDevpid(struct halQueryDevpidInfo info, pid_t *dev_pid)
{
    if (info.vfid != 0) {
        apm_err("Invalid param. (vfid=%d)\n", info.vfid);
        return DRV_ERROR_PARA_ERROR;
    }

    return halQuerySlavePid(info.hostpid, info.devid, info.proc_type, dev_pid);
}

drvError_t halQuerySlaveProcMeminfo(int master_pid, unsigned int devid, processType_t processType,
    processMemType_t memType, unsigned long long *size)
{
    struct apm_cmd_slave_meminfo para;
    drvError_t ret;

    if (size == NULL) {
        apm_err("Size is NULL.\n");
        return DRV_ERROR_INVALID_VALUE;
    }

    para.master_pid = master_pid;
    para.devid = devid;
    para.proc_type = processType;
    para.type = memType;
    ret = apm_cmd_ioctl(APM_QUERY_SLAVE_MEMINFO, &para);
    if (ret != 0) {
        return ret;
    }

    *size = para.size;
    return DRV_ERROR_NONE;
}

