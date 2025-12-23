/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>

#include "dsmi_dmp_command.h"
#include "dsmi_product.h"
#include "dsmi_common_interface_custom.h"
#include "dev_mon_ops_manager.h"
#include "dev_mon_log.h"
#include "dms_devdrv_info_comm.h"
#include "dsmi_inner_interface.h"
#include "dsmi_device_share.h"
#include "dms/dms_cmd_def.h"
#include "dms_user_common.h"
#include "dsmi_device_info.h"

#ifdef STATIC_SKIP
#define STATIC
#else
#define STATIC static
#endif

STATIC int dsmi_cmd_set_device_info_conv(unsigned int device_id, unsigned int main_cmd, unsigned int sub_cmd,
    void *buf, unsigned int *size)
{
    return dsmi_cmd_set_device_info(device_id, main_cmd, sub_cmd, (const void *)buf, *size);
}

#if (defined(CFG_SOC_PLATFORM_CLOUD) || defined(CFG_SOC_PLATFORM_CLOUD_V2))
STATIC int dmanage_get_device_computing_info(unsigned int device_id, unsigned int main_cmd,
    unsigned int sub_cmd, void *buf, unsigned int *size)
{
    int ret;
    if ((buf == NULL) || (size == NULL)) {
        DEV_MON_ERR("Param is NULL. (size=%pK)\n", size);
        return DRV_ERROR_PARA_ERROR;
    }
    ret = halGetDeviceInfo(device_id, (int)main_cmd, (int)sub_cmd, (int64_t *)(intptr_t)buf);
    if (ret != 0) {
        DEV_MON_ERR("Get device info failed. (device_id=%u; main_cmd=%u; sub_cmd=%u; ret=%d)\n",
                    device_id, main_cmd, sub_cmd, ret);
        return ret;
    }
    *size = sizeof(int64_t);
    return 0;
}
#endif
#ifdef CFG_SOC_PLATFORM_MINIV3
STATIC int dsmi_cmd_get_vrd_info(unsigned int device_id, unsigned int main_cmd, unsigned int sub_cmd,
    void *buf, unsigned int *size)
{
    struct dms_ioctl_arg ioarg = {0};
    int ret;

    ioarg.main_cmd = DMS_MAIN_CMD_PRODUCT;
    ioarg.sub_cmd = DMS_SUBCMD_GET_VRD_INFO;
    ioarg.filter_len = 0;
    ioarg.input = &device_id;
    ioarg.input_len = sizeof(int);
    ioarg.output = (void *)buf;
    ioarg.output_len = sizeof(DeviceVrdStatusInfo);

    ret = DmsIoctl(DMS_IOCTL_CMD, &ioarg);
    if (ret != 0) {
        DEV_MON_ERR("Dms get work mode failed. (ret=%d)\n", ret);
    }

    return ret;
}
#endif
STATIC const struct dev_mon_device_info_command g_dev_info_cmd_list[] = {
#if (defined(CFG_SOC_PLATFORM_CLOUD) || defined(CFG_SOC_PLATFORM_CLOUD_V2))
    {DSMI_MAIN_CMD_EX_COMPUTING, DSMI_EX_COMPUTING_SUB_CMD_TOKEN, USER_PROP, GUEST_PROP,
        DEV_MON_REQUEST_COMMAND, {0}, dmanage_get_device_computing_info},
#endif
#if (defined(CFG_SOC_PLATFORM_MINI) || defined(CFG_FEATURE_DEVICE_SHARE))
    {DSMI_MAIN_CMD_EX_CONTAINER, DSMI_EX_CONTAINER_SUB_CMD_SHARE, ROOT_PROP, ADMIN_PROP,
        DEV_MON_SETTING_COMMAND, {0}, dmanage_set_device_share},
    {DSMI_MAIN_CMD_EX_CONTAINER, DSMI_EX_CONTAINER_SUB_CMD_SHARE, USER_PROP, GUEST_PROP,
        DEV_MON_REQUEST_COMMAND, {0}, dmanage_get_device_share},
#endif
#ifdef CFG_SOC_PLATFORM_MINI
    {DSMI_MAIN_CMD_GPIO, DSMI_GPIO_SUB_CMD_GET_VALUE, USER_PROP, GUEST_PROP,
        DEV_MON_REQUEST_COMMAND, {0}, dsmi_cmd_get_device_info},
    {DSMI_MAIN_CMD_GPIO, DSMI_GPIO_SUB_CMD_SET_VALUE, ROOT_PROP, GUEST_PROP,
        DEV_MON_SETTING_COMMAND, {0}, dsmi_cmd_set_device_info_conv},
    {DSMI_MAIN_CMD_GPIO, DSMI_GPIO_SUB_CMD_DIRECT_INPUT, ROOT_PROP, GUEST_PROP,
        DEV_MON_SETTING_COMMAND, {0}, dsmi_cmd_set_device_info_conv},
    {DSMI_MAIN_CMD_GPIO, DSMI_GPIO_SUB_CMD_DIRECT_OUTPUT, ROOT_PROP, GUEST_PROP,
        DEV_MON_SETTING_COMMAND, {0}, dsmi_cmd_set_device_info_conv},
#endif
#if (defined(CFG_SOC_PLATFORM_MINIV2) || defined(CFG_SOC_PLATFORM_CLOUD_V2))
    {DSMI_MAIN_CMD_EX_CERT, DSMI_CERT_SUB_CMD_INIT_TLS_PUB_KEY, ROOT_PROP, ADMIN_PROP, DEV_MON_REQUEST_COMMAND,
        {0}, dsmi_cmd_get_device_info},
    {DSMI_MAIN_CMD_EX_CERT, DSMI_CERT_SUB_CMD_TLS_CERT_INFO, USER_PROP, GUEST_PROP, DEV_MON_REQUEST_COMMAND,
        {0}, dsmi_cmd_get_device_info},
    {DSMI_MAIN_CMD_EX_CERT, DSMI_CERT_SUB_CMD_TLS_CERT_INFO, ROOT_PROP, ADMIN_PROP, DEV_MON_SETTING_COMMAND,
        {0}, dsmi_cmd_set_device_info_conv},
#endif
#ifdef CFG_SOC_PLATFORM_MINIV3
    {DMS_MAIN_CMD_PRODUCT, DMS_SUBCMD_GET_VRD_INFO, ROOT_PROP, ADMIN_PROP, DEV_MON_REQUEST_COMMAND,
        {0}, dsmi_cmd_get_vrd_info},
#endif
    {DSMI_MAIN_CMD_EN_DECRYPTION, DSMI_STATE_SUB_CMD_ENCRYPT, ROOT_PROP, ADMIN_PROP,
        DEV_MON_REQUEST_COMMAND, {0}, dsmi_cmd_get_device_info},
    {DSMI_MAIN_CMD_EN_DECRYPTION, DSMI_STATE_SUB_CMD_DECRYPT, ROOT_PROP, ADMIN_PROP,
        DEV_MON_REQUEST_COMMAND, {0}, dsmi_cmd_get_device_info},

#ifdef CFG_FEATURE_PCIE_HCCS_BANDWIDTH
    {DSMI_MAIN_CMD_PCIE_BANDWIDTH, DSMI_PCIE_CMD_GET_BANDWIDTH, USER_PROP, GUEST_PROP,
        DEV_MON_REQUEST_COMMAND, {0}, dsmi_cmd_get_device_info},
#endif
#ifdef CFG_FEATURE_HCCS_BANDWIDTH
    {DSMI_MAIN_CMD_HCCS_BANDWIDTH, DSMI_HCCS_CMD_GET_BANDWIDTH, USER_PROP, GUEST_PROP,
        DEV_MON_REQUEST_COMMAND, {0}, dsmi_cmd_get_device_info},
#endif
    {DSMI_MAIN_CMD_MAX, 0, 0, 0, 0, {0}, NULL},
};

STATIC const struct dev_mon_device_info_command *find_host_device_info_handle(DSMI_MAIN_CMD main_cmd,
    unsigned int sub_cmd, unsigned int command_type)
{
    unsigned int i;
    const struct dev_mon_device_info_command *command = NULL;

    for (i = 0; i < sizeof(g_dev_info_cmd_list) / sizeof(g_dev_info_cmd_list[0]); i++) {
        command = &g_dev_info_cmd_list[i];
        if ((command->main_cmd == main_cmd) && (command->sub_cmd == sub_cmd) &&
            (command->command_type == command_type)) {
            return command;
        }
    }
    return NULL;
}

STATIC int check_phy_mach_scenes(unsigned int device_id)
{
    int ret;
    unsigned int container_flag;

    /* Check whether it is a physical machine(Non-VM). */
    ret = drv_get_phy_mach_flag((int)device_id);
    if (ret) {
        DEV_MON_ERR("Get phy mach flag failed. (device_id=%u; ret=%d)\n", device_id, ret);
        return ret;
    }

    /* Check whether it is a container. */
    ret = dmanage_get_container_flag(&container_flag);
    if (ret) {
        DEV_MON_ERR("Get container flag failed. (device_id=%u; ret=%d)\n", device_id, ret);
        return ret;
    }

    if (container_flag != 0) {
        DEV_MON_ERR("Check container flag failed. (dev_id=%u; container_flag=%d)\n", device_id, container_flag);
        return DRV_ERROR_OPER_NOT_PERMITTED;
    }

    return 0;
}

STATIC int dsmi_product_config_device_info(unsigned int device_id, DSMI_MAIN_CMD main_cmd, unsigned int sub_cmd,
    void *buf, unsigned int *buf_size, unsigned int cmd_type)
{
    int ret, uid;
    dev_mon_device_info_handle handle = NULL;
    const struct dev_mon_device_info_command *command = NULL;

    if (buf == NULL || buf_size == NULL) {
        DEV_MON_ERR("Input buf is null.\n");
        return DRV_ERROR_PARA_ERROR;
    }

    command = find_host_device_info_handle(main_cmd, sub_cmd, cmd_type);
    if (command == NULL || command->device_info_ops == NULL) {
        DEV_MON_ERR("Host dev info command not found. (dev_id=%u; main_cmd=0x%x; command=%d)\n",
                    device_id, main_cmd, (int)(command == NULL));
        return DRV_ERROR_NOT_SUPPORT;
    }
    handle = (dev_mon_device_info_handle)command->device_info_ops;

    ret = dsmi_check_device_id((int)device_id);
    if (ret != 0) {
        DEV_MON_ERR("Check dev id invalid. (dev_id=%u; ret=%d)\n", device_id, ret);
        return DRV_ERROR_INVALID_DEVICE;
    }

    // user permission check
    if (command->uid == ROOT_PROP) {
        uid = user_prop_check();
        if (uid != ROOT_PROP) {
            DEV_MON_ERR("Invalid permission. (dev_id=%u; uid=%d)\n", device_id, uid);
            return DRV_ERROR_OPER_NOT_PERMITTED;
        }
    }

    // run env check
    if (command->scenes == ADMIN_PROP) {
        /* Check whether the current environment is a physical machine. */
        ret = check_phy_mach_scenes(device_id);
        if (ret != 0) {
            DEV_MON_ERR("Invalid permission. (dev_id=%u; ret=%d)\n", device_id, ret);
            return DRV_ERROR_OPER_NOT_PERMITTED;
        }
    }
    ret = handle(device_id, main_cmd, sub_cmd, buf, buf_size);
    if (ret != 0) {
        DEV_MON_ERR("Host callback function failed. (dev_id=%u; main_cmd=0x%x; ret=%d)\n", device_id, main_cmd, ret);
        return ret;
    }
    return DRV_ERROR_NONE;
}

int dms_get_work_mode(unsigned int *work_mode)
{
    struct dms_ioctl_arg ioarg = {0};
    int ret;

    ioarg.main_cmd = DMS_MAIN_CMD_PRODUCT;
    ioarg.sub_cmd = DMS_SUBCMD_GET_WORK_MODE;
    ioarg.filter_len = 0;
    ioarg.input = NULL;
    ioarg.input_len = 0;
    ioarg.output = (void *)work_mode;
    ioarg.output_len = sizeof(unsigned int);

    ret = DmsIoctl(DMS_IOCTL_CMD, &ioarg);
    if (ret != 0) {
        DEV_MON_ERR("Dms get work mode failed. (ret=%d)\n", ret);
    }

    return ret;
}

int dsmi_product_set_device_info(unsigned int device_id, DSMI_MAIN_CMD main_cmd, unsigned int sub_cmd,
    const void *buf, unsigned int size)
{
    return dsmi_product_config_device_info(device_id, main_cmd, sub_cmd, (void *)(uintptr_t)buf, &size, DEV_MON_SETTING_COMMAND);
}

int dsmi_product_get_device_info(unsigned int device_id, DSMI_MAIN_CMD main_cmd, unsigned int sub_cmd,
    void *buf, unsigned int *size)
{
    return dsmi_product_config_device_info(device_id, main_cmd, sub_cmd, buf, size, DEV_MON_REQUEST_COMMAND);
}

int dsmi_get_work_mode(unsigned int *work_mode)
{
    return dms_get_work_mode(work_mode);
}

int dms_get_mainboard_id(unsigned int device_id, unsigned int *mainboard_id)
{
    int ret = 0;
#ifdef CFG_SOC_PLATFORM_CLOUD_V4
    long int mainboard_id_tmp;
 
    ret = halGetDeviceInfo(device_id, MODULE_TYPE_SYSTEM, INFO_TYPE_MAINBOARD_ID, &mainboard_id_tmp);
    if (ret != 0) {
        DEV_MON_ERR("halGetDeviceInfo failed. (ret=%d)\n", ret);
        return ret;
    }

    *mainboard_id = (unsigned int)mainboard_id_tmp;
#else
    struct dms_ioctl_arg ioarg = {0};
 
    ioarg.main_cmd = DMS_MAIN_CMD_PRODUCT;
    ioarg.sub_cmd = DMS_SUBCMD_GET_MAINBOARD_ID;
    ioarg.filter_len = 0;
    ioarg.input = (void*)&device_id;
    ioarg.input_len = sizeof(unsigned int);
    ioarg.output = (void *)mainboard_id;
    ioarg.output_len = sizeof(unsigned int);
 
    ret = DmsIoctl(DMS_IOCTL_CMD, &ioarg);
    if (ret != 0) {
        DEV_MON_ERR("Dms get main board id failed. (ret=%d)\n", ret);
    }
#endif

    return ret;
}
 
int dsmi_get_mainboard_id(unsigned int device_id, unsigned int *mainboard_id)
{
    if (mainboard_id == NULL) {
        DEV_MON_ERR("The mainboard_id is null.\n");
        return DRV_ERROR_PARA_ERROR;
    }
    if (device_id >= DEVDRV_MAX_DAVINCI_NUM) {
        DEV_MON_ERR("The device_id is too large.\n");
        return DRV_ERROR_PARA_ERROR;
    }
#ifdef CFG_SOC_PLATFORM_CLOUD_V4
    // A5上需要通过logicid获取mainboardid
    unsigned int logic_id = 0;
    int ret;
 
    ret = dsmi_get_logicid_from_phyid(device_id, &logic_id);
    if (ret != 0) {
        DEV_MON_ERR("dsmi_get_phyid_from_logicid failed. (ret=%d)\n", ret);
        return ret;
    }
 
    device_id = logic_id;
#endif

#if !defined(CFG_SOC_PLATFORM_CLOUD_V2) && !defined(CFG_SOC_PLATFORM_CLOUD_V4)
    *mainboard_id = 0;
    return DRV_ERROR_NONE;
#else
    return dms_get_mainboard_id(device_id, mainboard_id);
#endif
}