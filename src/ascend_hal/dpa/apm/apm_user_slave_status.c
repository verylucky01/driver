/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include "ascend_hal.h"
#include "dpa/dpa_apm.h"
#include "apm_ioctl.h"

drvError_t apm_query_slave_status(unsigned int devid, processType_t process_type,
    enum apm_cmd_slave_status_type type, int *status)
{
    struct apm_cmd_slave_status para;
    int ret;

    para.devid = devid;
    para.proc_type = process_type;
    para.type = type;

    ret = apm_cmd_ioctl(APM_QUERY_SLAVE_STATUS, &para);
    if (ret != 0) {
        return ret;
    }

    *status = para.status;

    return DRV_ERROR_NONE;
}

/* status read clear */
drvError_t apm_query_slave_oom_status(unsigned int devid, processType_t process_type, int *status)
{
    return apm_query_slave_status(devid, process_type, CMD_SLAVE_STATUS_TYPE_OOM, status);
}

drvError_t halCheckProcessStatus(DVdevice device, processType_t processType, processStatus_t status, bool *isMatched)
{
    drvError_t ret;
    int oom_status;

    if ((processType < PROCESS_CP1) || (processType >= PROCESS_USER) ||
        (status != STATUS_NOMEM) || (isMatched == NULL)) {
        apm_err("Invalid para. (devId=%u; procType=%d; status=%d; isMatched=%p)\n",
            device, processType, status, isMatched);
        return DRV_ERROR_INVALID_VALUE;
    }

    ret = apm_query_slave_oom_status(device, processType, &oom_status);
    if (ret != 0) {
        return ret;
    }

    *isMatched = (oom_status == 0) ? false : true;
    return 0;
}
