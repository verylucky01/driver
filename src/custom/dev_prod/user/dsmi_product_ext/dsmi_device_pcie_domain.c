/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <stdio.h>
#include "dsmi_dmp_command.h"
#include "dms/dms_cmd_def.h"
#include "dms/dms_soc_interface.h"
#include "dev_mon_log.h"
#include "devdrv_ioctl.h"
#include "errno.h"
#include "dms_devdrv_info_comm.h"
#include "ascend_hal_error.h"
#include "mmpa_linux.h"
#include "mmpa_api.h"
#include "dmc_log_user.h"
#include "dev_mon_cmd_manager.h"
#include "dms_user_common.h"
#include "dsmi_device_pcie_domain.h"

static int dmanageCommonIoctl(int ioctlCmd, void *ioctlArg)
{
    int ret;
    int fd;

    fd = devdrv_open_device_manager();
    if (fd < 0) {
        DEV_MON_ERR("Open davinci manager failed. (fd=%d)\n", fd);
        return DRV_ERROR_INVALID_HANDLE;
    }

    ret = ioctl(fd, (unsigned long)ioctlCmd, ioctlArg);
    return (ret == -1) ? errno_to_user_errno(-errno) : errno_to_user_errno(ret);
}

static int DmsVirtGetPcieIdInfoAll(unsigned int devId, struct dmanage_pcie_id_info_all *pcie_idinfo)
{
#ifndef AOS_LLVM_BUILD
    int ret;
    dms_pcie_id_info_t pcie_info = {0};

    pcie_info.davinci_id = devId;
    ret = dmanageCommonIoctl(DEVDRV_MANAGER_GET_PCIE_ID_INFO, &pcie_info);
    if (ret) {
        DEV_MON_ERR("Dms virt get pcie info all ioctl failed. (dev_id=%u; ret=%d)\n", devId, ret);
        return ret;
    }
    pcie_idinfo->davinci_id = devId;
    pcie_idinfo->venderid = pcie_info.venderid;
    pcie_idinfo->subvenderid = pcie_info.subvenderid;
    pcie_idinfo->deviceid = pcie_info.deviceid;
    pcie_idinfo->subdeviceid = pcie_info.subdeviceid;
    pcie_idinfo->bus = pcie_info.bus;
    pcie_idinfo->device = pcie_info.device;
    pcie_idinfo->fn = pcie_info.fn;

#endif

    return DRV_ERROR_NONE;
}

static int DmsPhyGetPcieIdInfoAll(unsigned int devId, struct dmanage_pcie_id_info_all *pcie_idinfo)
{
    struct dms_ioctl_arg ioarg = {0};
    int ret;

    ioarg.main_cmd = DMS_MAIN_CMD_PRODUCT;
    ioarg.sub_cmd = DMS_SUBCMD_GET_PCIE_ID_INFO_ALL;
    ioarg.filter_len = 0;
    ioarg.input = (void *)&devId;
    ioarg.input_len = sizeof(unsigned int);
    ioarg.output = (void *)pcie_idinfo;
    ioarg.output_len = sizeof(struct dmanage_pcie_id_info_all);

    ret = DmsIoctl(DMS_IOCTL_CMD, &ioarg);
    if (ret) {
        DEV_MON_ERR("Dms phy get pcie info all failed. (dev_id=%u; ret=%d)\n", devId, ret);
    }

    return ret;
}

drvError_t drvDeviceGetPcieIdInfoAll(unsigned int devId, struct tag_pcie_idinfo_all *pcie_idinfo)
{
    struct dmanage_pcie_id_info_all id_info = {0};
    int ret;

    if ((devId >= DEVDRV_MAX_DAVINCI_NUM) || (pcie_idinfo == NULL)) {
        DEV_MON_ERR("Invalid devid or pcie_idinfo is NULL.(devid = %u)\n", devId);
        return DRV_ERROR_INVALID_VALUE;
    }

    ret = DMS_VIRT_ADAPT_FUNC(DmsVirtGetPcieIdInfoAll, DmsPhyGetPcieIdInfoAll)(devId, &id_info);
    if (ret) {
        DEV_MON_ERR("Get pcie info all failed. (dev_id=%u; ret=%d)\n", devId, ret);
        return ret;
    }

    pcie_idinfo->venderid = id_info.venderid;
    pcie_idinfo->subvenderid = id_info.subvenderid;
    pcie_idinfo->subdeviceid = id_info.subdeviceid;
    pcie_idinfo->deviceid = id_info.deviceid;
    pcie_idinfo->domain = id_info.domain;
    pcie_idinfo->bdf_busid = id_info.bus;
    pcie_idinfo->bdf_deviceid = id_info.device;
    pcie_idinfo->bdf_funcid = id_info.fn;

    return DRV_ERROR_NONE;
}

int dsmi_get_pcie_info_v2(int device_id, struct tag_pcie_idinfo_all *pcie_idinfo)
{
#ifndef CFG_SOC_PLATFORM_RC
    int ret;

    /* get domain, bus, deviceid and func */
    ret = drvDeviceGetPcieIdInfoAll((unsigned int)device_id, pcie_idinfo);
    if (ret) {
        DEV_MON_ERR("devid %d drvDeviceGetPcieIdInfoAll call error ret = %d!\n", device_id, ret);
        return ret;
    }
    return 0;
#else
    return DRV_ERROR_NOT_SUPPORT;
#endif
}
