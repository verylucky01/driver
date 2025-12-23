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
#include <time.h>
#include <limits.h>
#include <errno.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "securec.h"
#include "dsmi_common_interface_custom.h"
#include "dcmi_interface_api.h"
#include "dcmi_common.h"
#include "dcmi_init_basic.h"
#include "dcmi_fault_manage_intf.h"
#include "dcmi_log.h"
#include "dcmi_inner_info_get.h"
#include "dcmi_product_judge.h"
#include "dcmi_environment_judge.h"
#include "dcmi_flash_intf.h"

#if defined DCMI_VERSION_2
int dcmi_get_device_flash_count(int card_id, int device_id, unsigned int *flash_count)
{
    int err;
    enum dcmi_unit_type device_type = NPU_TYPE;

    if (flash_count == NULL) {
        gplog(LOG_ERR, "flash_count is invalid.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    err = dcmi_get_device_type(card_id, device_id, &device_type);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d.", err);
        return err;
    }

    if (device_type == NPU_TYPE) {
        return dcmi_get_npu_device_flash_count(card_id, device_id, flash_count);
    } else {
        gplog(LOG_ERR, "device_type %d is not support.", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
}

int dcmi_get_device_flash_info_v2(int card_id, int device_id, unsigned int flash_index,
    struct dcmi_flash_info *flash_info)
{
    int err;
    enum dcmi_unit_type device_type = NPU_TYPE;

    if (flash_info == NULL) {
        gplog(LOG_ERR, "flash_info is invalid.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    err = dcmi_get_device_type(card_id, device_id, &device_type);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d.", err);
        return err;
    }

    if (device_type == NPU_TYPE) {
        return dcmi_get_npu_device_flash_info(card_id, device_id, flash_index, flash_info);
    } else {
        gplog(LOG_ERR, "device_type %d is error.", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
}

int dcmi_get_npu_device_flash_count(int card_id, int device_id, unsigned int *flash_count)
{
    int ret;
    int device_logic_id = 0;

    ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed.%d.\n", ret);
        return ret;
    }

    ret = dsmi_get_device_flash_count(device_logic_id, flash_count);
    if ((ret != DSMI_OK) && (ret != DSMI_ERR_NOT_SUPPORT)) {
        gplog(LOG_ERR, "call dsmi_get_device_flash_count failed. err is %d.", ret);
    }

    return dcmi_convert_error_code(ret);
}

int dcmi_get_npu_device_flash_info(
    int card_id, int device_id, unsigned int flash_index, struct dcmi_flash_info *flash_info)
{
    int ret;
    int device_logic_id = 0;

    ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", ret);
        return ret;
    }

    ret = dsmi_get_device_flash_info(device_logic_id, flash_index, (struct dm_flash_info_stru *)flash_info);
    if ((ret != DSMI_OK) && (ret != DSMI_ERR_NOT_SUPPORT)) {
        gplog(LOG_ERR, "call dsmi_get_device_flash_info failed. err is %d.", ret);
    }

    return dcmi_convert_error_code(ret);
}
#endif

#if defined DCMI_VERSION_1
int dcmi_get_device_flash_info(
    int card_id, int device_id, unsigned int flash_index, struct dcmi_flash_info_stru *flash_info)
{
    return dcmi_get_device_flash_info_v2(card_id, device_id, flash_index, (struct dcmi_flash_info *)flash_info);
}
#endif

STATIC int dcmi_check_vnpu_in_docker(int card_id, int device_id)
{
    int ret;
    unsigned int env_flag = ENV_PHYSICAL;
    struct dcmi_chip_info_v2 chip_info = { { 0 } };

    ret = dcmi_get_npu_chip_info(card_id, device_id, &chip_info);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_npu_chip_info failed. ret is %d.", ret);
        return ret;
    }
    ret = dcmi_get_environment_flag(&env_flag);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "get environment flag failed. ret is %d", ret);
        return ret;
    }

    if (strstr((char *)chip_info.chip_name, "vir") &&
        (env_flag == ENV_PHYSICAL_PLAIN_CONTAINER || env_flag == ENV_VIRTUAL_PLAIN_CONTAINER)) {
        return DCMI_ERR_CODE_NOT_SUPPORT_IN_CONTAINER;
    }
    return DCMI_OK;
}

int dcmi_get_multi_ecc_time_info_v2(int card_id, int device_id, struct dcmi_multi_ecc_time_data *multi_ecc_time_data)
{
    int ret;
    int device_logic_id = 0;

    if (multi_ecc_time_data == NULL) {
        gplog(LOG_ERR, "multi_ecc_time_data is NULL\n");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if (dcmi_check_vnpu_in_docker(card_id, device_id) == DCMI_ERR_CODE_NOT_SUPPORT_IN_CONTAINER) {
        gplog(LOG_OP, "card_id %d is in vnpu mode can not get multi ecc time info.\n", card_id);
        return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
    }

    if (dcmi_board_chip_type_is_ascend_310() || dcmi_board_chip_type_is_ascend_310b()) {
        gplog(LOG_OP, "This device does not support get multi ecc time info.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. ret is %d.", ret);
        return ret;
    }

    ret = dsmi_get_multi_ecc_time_info(device_logic_id, (struct dsmi_multi_ecc_time_data *)multi_ecc_time_data);
    if (ret) {
        gplog(LOG_ERR, "call dsmi_get_multi_ecc_time_info failed. ret is %d.", ret);
        return dcmi_convert_error_code(ret);
    }
    return ret;
}

int dcmi_get_multi_ecc_time_info(int card_id, struct dcmi_multi_ecc_time_data *multi_ecc_time_data)
{
    if (dcmi_board_chip_type_is_ascend_910_93()) {
        gplog(LOG_OP, "This device does not support this api.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
    return dcmi_get_multi_ecc_time_info_v2(card_id, 0, multi_ecc_time_data);
}

int dcmi_get_multi_ecc_record_info_v2(int card_id, int device_id, struct dcmi_ecc_record_type type,
    unsigned int *ecc_count, struct dcmi_ecc_common_data *ecc_common_data_s)
{
    int ret;
    int device_logic_id = 0;

    if (ecc_common_data_s == NULL || ecc_count == NULL) {
        gplog(LOG_ERR, "ecc_common_data_s or ecc_count is NULL\n");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if ((type.read_type != SINGLE_ECC_INFO_READ) && (type.read_type != MULTI_ECC_INFO_READ)) {
        // 仅支持获取单比特和多比特ecc信息
        gplog(LOG_OP, "This read_type does not support get multi ecc record info.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
    if (dcmi_check_vnpu_in_docker(card_id, device_id) == DCMI_ERR_CODE_NOT_SUPPORT_IN_CONTAINER) {
        gplog(LOG_OP, "card_id %d is in vnpu mode can not get multi ecc record info.\n", card_id);
        return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
    }
    if (dcmi_board_chip_type_is_ascend_310() || dcmi_board_chip_type_is_ascend_310b()) {
        gplog(LOG_OP, "This device does not support get multi ecc record info.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
    if ((dcmi_board_chip_type_is_ascend_310p()) && (type.module_type == (enum dcmi_device_type)DSMI_DEVICE_TYPE_HBM)) {
        gplog(LOG_ERR, "module_type is not support. module_type=%d",  type.module_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
    ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. ret is %d.", ret);
        return ret;
    }

    ret = dsmi_get_multi_ecc_record_info(device_logic_id, ecc_count,
        type.read_type, type.module_type, (struct dsmi_ecc_common_data *)ecc_common_data_s);
    if (ret) {
        gplog(LOG_ERR, "call dsmi_get_multi_ecc_record_info failed. ret is %d.", ret);
        return dcmi_convert_error_code(ret);
    }
    return ret;
}

int dcmi_get_multi_ecc_record_info(int card_id, unsigned int *ecc_count, unsigned char read_type,
    unsigned char module_type, struct dcmi_ecc_common_data *ecc_common_data_s)
{
    struct dcmi_ecc_record_type type;

    if (dcmi_board_chip_type_is_ascend_910_93()) {
        gplog(LOG_OP, "This device does not support this api.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
    type.read_type = read_type;
    type.module_type = module_type;
    return dcmi_get_multi_ecc_record_info_v2(card_id, 0, type, ecc_count, ecc_common_data_s);
}

STATIC int dcmi_check_hbm_manufacturer_id(unsigned int manufacturer_id)
{
    unsigned char check = 0;
    unsigned int data = manufacturer_id;

    check ^= (data >> CHECK_THREE_BYTE_BIT) & 0xFF;
    check ^= (data >> CHECK_TWO_BYTE_BIT) & 0xFF;
    check ^= (data >> CHECK_ONE_BYTE_BIT) & 0xFF;
    check ^= data & 0xFF;

    if (check == 0) {
        return DCMI_OK;
    } else {
        gplog(LOG_ERR, "The manufacturer_id check error. (manufacturer_id=0x%x)\n", manufacturer_id);
        return DCMI_ERR_CODE_INNER_ERR;
    }
}

int dcmi_get_device_hbm_product_info(int card_id, int device_id, struct dcmi_hbm_product_info *hbm_product_info)
{
    int ret;
    enum dcmi_unit_type device_type = NPU_TYPE;
    unsigned int manufacturer_id = 0xffffffff;
    int device_logic_id = 0;

    if (hbm_product_info == NULL) {
        gplog(LOG_ERR, "hbm_product_info is NULL.\n");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    ret = dcmi_get_device_type(card_id, device_id, &device_type);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. ret is %d.", ret);
        return ret;
    }

    if (!dcmi_board_chip_type_is_ascend_910b() && !dcmi_board_chip_type_is_ascend_910_93()) {
        gplog(LOG_ERR, "This device does not support get hbm product info.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    if (device_type == NPU_TYPE) {
        ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. ret is %d.", ret);
            return ret;
        }

        ret = dsmi_get_hbm_manufacturer_id(device_logic_id, &manufacturer_id);
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "call dsmi_get_hbm_manufacturer_id failed. manufacturer_id is 0x%X, ret is %d.",
                manufacturer_id, ret);
            return dcmi_convert_error_code(ret);
        }

        ret = dcmi_check_hbm_manufacturer_id(manufacturer_id);
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "Check hbm manufacturer_id failed. card_id=%d, device_id=%d, ret is %d",
                card_id, device_id, ret);
            return ret;
        }

        hbm_product_info->manufacturer_id = (unsigned short)(manufacturer_id & 0xFF);
        return ret;
    } else {
        gplog(LOG_ERR, "device_type %d is not support.", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
}