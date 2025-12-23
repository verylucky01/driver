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
#include "ascend_hal.h"
#include "dms/dms_devdrv_info_comm.h"
#include "dms_user_common.h"
#include "pbl_urd_sub_cmd_common.h"
#include "ascend_dev_num.h"
#include "udis_user.h"

#ifdef STATIC_SKIP
#define STATIC
#else
#define STATIC static
#endif

STATIC int udis_check_is_vdev(unsigned int dev_id, unsigned int *vdev_flag)
{
    int ret;
    unsigned int physical_id = 0;
    unsigned int split_mode = 0;
    unsigned int container_flag = 0;
#ifdef CFG_EDGE_HOST
    (void) physical_id;
    ret = halGetDeviceSplitMode(dev_id, &split_mode);
    if (ret != 0) {
        DMS_ERR("Get split mode failed. (dev_id=%u; ret=%d)\n", dev_id, ret);
        return ret;
    }

    if ((split_mode == 0) || (split_mode == VMNG_VIRTUAL_SPLIT_MODE)) {
        *vdev_flag = (split_mode == 0 ? 0 : 1);
        return 0;
    }

    ret = dmanage_get_container_flag(&container_flag);
    if (ret != 0) {
        DMS_ERR("Get container flag failed. (dev_id=%u; ret=%d)\n", dev_id, ret);
        return ret;
    }

    *vdev_flag = ((container_flag == 0 && dev_id < ASCEND_PDEV_MAX_NUM )? 0 : 1);
#else
    (void) split_mode;
    (void) container_flag;
    ret = drvDeviceGetPhyIdByIndex(dev_id, &physical_id);
    if (ret != 0) {
        DMS_ERR("Get physical_id failed. (dev_id=%u; ret=%d)\n", dev_id, ret);
        return ret;
    }

    *vdev_flag = (physical_id >= ASCEND_PDEV_MAX_NUM ? 1 : 0);
#endif
    return 0;
}

STATIC int udis_is_supported(unsigned int dev_id)
{
    int ret;
    halChipInfo chip_info = {{0}, {0}, {0}};
    const char *chip_type = "Ascend";
    const char *cloud_v3_name = "910_93";
    unsigned int vdev_flag = 0;

    ret = halGetChipInfo(dev_id, &chip_info);
    if (ret != 0) {
        DMS_ERR("Get chip info failed. (dev_id=%u; ret=%d)\n", dev_id, ret);
        return ret;
    }

    if (strncmp((const char *)chip_info.type, chip_type, strlen(chip_type)) != 0) {
        return DRV_ERROR_NOT_SUPPORT;
    }

    if (strncmp((const char *)chip_info.name, cloud_v3_name, strlen(cloud_v3_name)) != 0) {
        return 0;
    }

    ret = udis_check_is_vdev(dev_id, &vdev_flag);
    if (ret != 0) {
        DMS_ERR("Get vdev flag failed. (dev_id=%u; ret=%d)\n", dev_id, ret);
        return ret;
    }

    if (vdev_flag == 1) {
        return DRV_ERROR_NOT_SUPPORT;
    }

    return 0;
}

STATIC int udis_input_check(unsigned int dev_id, const struct udis_dev_info *info)
{
    size_t name_len = 0;

    if (info == NULL) {
        DMS_ERR("Invalid para. info is NULL (dev_id=%u)\n", dev_id);
        return DRV_ERROR_PARA_ERROR;
    }

    if (info->module_type >= UDIS_MODULE_MAX) {
        DMS_ERR("Invalid module type. (dev_id=%u; module_type=%d; max_module_type=%u)\n",
            dev_id, info->module_type, UDIS_MODULE_MAX - 1);
        return DRV_ERROR_PARA_ERROR;
    }

    name_len = strnlen(info->name, UDIS_MAX_NAME_LEN);
    if ((name_len == 0) ||  (name_len >= UDIS_MAX_NAME_LEN)) {
        DMS_ERR("Invalid para, name length invalid. (dev_id=%u; module_type=%u; name_len=%u; max_name_len=%u)\n",
            dev_id, info->module_type, name_len, UDIS_MAX_NAME_LEN - 1);
        return DRV_ERROR_PARA_ERROR;
    }

    return 0;
}

int udis_get_device_info(unsigned int dev_id, struct udis_dev_info *info)
{
    int ret = 0;
    struct urd_cmd cmd = {0};
    struct urd_cmd_para cmd_para = {0};
    struct udis_get_ioctl_in in = {0};
    struct udis_get_ioctl_out out = {0};

    ret = udis_is_supported(dev_id);
    if (ret != 0) {
        DMS_EX_NOTSUPPORT_ERR(ret, "Udis support check failed. (dev_id=%u; ret=%d)", dev_id, ret);
        return ret;
    }

    ret = udis_input_check(dev_id, info);
    if (ret != 0) {
        return ret;
    }

    in.module_type = info->module_type;
    ret = strcpy_s(in.name, UDIS_MAX_NAME_LEN, info->name);
    if (ret != 0) {
        DMS_ERR("Failed to copy info name, (dev_id=%u)\n", dev_id);
        return DRV_ERROR_PARA_ERROR;
    }
    in.data = info->data;
    in.in_len = sizeof(info->data);

    urd_usr_cmd_fill(&cmd, DMS_GET_UDIS_DEVICE_INFO_CMD, ZERO_CMD, NULL, 0);
    urd_usr_cmd_para_fill(&cmd_para, (void*)&in, sizeof(struct udis_get_ioctl_in),
        &out, sizeof(struct udis_get_ioctl_out));
    ret = urd_dev_usr_cmd(dev_id, &cmd, &cmd_para);
    if (ret != 0) {
        DMS_EX_NOTSUPPORT_ERR(ret, "Get info from udis failed. (dev_id=%u; module_type=%u; name=%s; ret=%d)",
            dev_id, info->module_type, info->name, ret);
        return ret;
    }

    info->data_len = out.out_len;
    info->acc_ctrl = out.acc_ctrl;
    info->last_update_time = out.last_update_time;

    return 0;
}

int udis_set_device_info(unsigned int dev_id, const struct udis_dev_info *info)
{
    int ret = 0;
    struct urd_cmd cmd = {0};
    struct urd_cmd_para cmd_para = {0};
    struct udis_set_ioctl_in in = {0};

    ret = udis_is_supported(dev_id);
    if (ret != 0) {
        DMS_EX_NOTSUPPORT_ERR(ret, "Udis support check failed. (dev_id=%u; ret=%d)", dev_id, ret);
        return ret;
    }

    ret = udis_input_check(dev_id, info);
    if (ret != 0) {
        return ret;
    }

    if ((info->data_len == 0) || (info->data_len > UDIS_MAX_DATA_LEN)) {
        DMS_ERR("Invalid data len. (dev_id=%u; module_type=%u; name=%s; data_len=%u; max_data_len=%u)\n",
            dev_id, info->module_type, info->name, info->data_len, UDIS_MAX_DATA_LEN);
        return DRV_ERROR_PARA_ERROR;
    }

    in.module_type = info->module_type;
    ret = strcpy_s(in.name, UDIS_MAX_NAME_LEN, info->name);
    if (ret != 0) {
        DMS_ERR("Failed to copy info name, (dev_id=%u)\n", dev_id);
        return DRV_ERROR_PARA_ERROR;
    }
    in.data = info->data;
    in.data_len = info->data_len;
    in.acc_ctrl = info->acc_ctrl;

    urd_usr_cmd_fill(&cmd, DMS_SET_UDIS_DEVICE_INFO_CMD, ZERO_CMD, NULL, 0);
    urd_usr_cmd_para_fill(&cmd_para, (void *)&in, sizeof(struct udis_set_ioctl_in), NULL, 0);
    ret = urd_dev_usr_cmd(dev_id, &cmd, &cmd_para);
    if (ret != 0) {
        DMS_EX_NOTSUPPORT_ERR(ret, "Set info from udis failed. (dev_id=%u; module_type=%u; name=%s; ret=%d)",
            dev_id, info->module_type, info->name, ret);
        return ret;
    }

    DMS_INFO("Set udis info success. (dev_id=%u; module_type=%u; name=%s)\n",
        dev_id, info->module_type, info->name);

    return 0;
}