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
#include "dev_mon_log.h"
#include "dev_mon_cmd_manager.h"
#include "devdrv_ioctl.h"
#include "dsmi_common_interface_custom.h"

#ifdef STATIC_SKIP
#define STATIC
#else
#define STATIC static
#endif

#define DEVDRV_GET_DEVICE_SHARE 0
#define DEVDRV_SET_DEVICE_SHARE 1
struct device_share_para {
    unsigned int dev_id;
    unsigned int opcode;
    int dev_share_flag;
};

STATIC int dmanage_common_ioctl(int cmd, void *arg)
{
    int ret;
    int fd;

    fd = devdrv_open_device_manager();
    if (fd < 0) {
        DEV_MON_ERR("Open device manager failed. (fd=%d)\n", fd);
        return DRV_ERROR_INVALID_HANDLE;
    }

    ret = ioctl(fd, (unsigned long)cmd, arg);
    return (ret == -1) ? errno_to_user_errno(-errno) : errno_to_user_errno(ret);
}

int dmanage_set_device_share(unsigned int device_id, unsigned int main_cmd,
    unsigned int sub_cmd, const void *buf, unsigned int *size)
{
    int ret;
    struct device_share_para para = {0};
    unsigned int devid = 0;

    if ((buf == NULL) || (size == NULL)) {
        DEV_MON_ERR("Param is NULL.\n");
        return DRV_ERROR_PARA_ERROR;
    }

    ret = dsmi_get_phyid_from_logicid(device_id, &devid);
    if (ret != 0) {
        DEV_MON_ERR("get devid error. (ret=%d)\n", ret);
        return ret;
    }

    para.dev_id = devid;
    para.opcode = DEVDRV_SET_DEVICE_SHARE;
    para.dev_share_flag = *(int *)buf;

    ret = dmanage_common_ioctl(DEVDRV_MANAGER_CONFIG_DEVICE_SHARE, (void*)&para);
    if (ret != 0) {
        DEV_MON_ERR("Ioctl set device share failed. (errno=%d)\n", errno);
        return ret;
    }
    return 0;
}

int dmanage_get_device_share(unsigned int device_id, unsigned int main_cmd,
    unsigned int sub_cmd, void *buf, unsigned int *size)
{
    int ret;
    struct device_share_para para = {0};
    unsigned int devid = 0;

    if ((buf == NULL) || (size == NULL)) {
        DEV_MON_ERR("Param is NULL.\n");
        return DRV_ERROR_PARA_ERROR;
    }

    ret = dsmi_get_phyid_from_logicid(device_id, &devid);
    if (ret != 0) {
        DEV_MON_ERR("get devid error. (ret=%d)\n", ret);
        return ret;
    }

    para.dev_id = devid;
    para.opcode = DEVDRV_GET_DEVICE_SHARE;

    ret = dmanage_common_ioctl(DEVDRV_MANAGER_CONFIG_DEVICE_SHARE, (void*)&para);
    if (ret != 0) {
        DEV_MON_ERR("Ioctl for getting device share failed. (errno=%d)\n", errno);
        return ret;
    }
    *(int *)buf = para.dev_share_flag;
    *size = sizeof(int);
    return 0;
}
