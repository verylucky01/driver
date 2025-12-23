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
#include <stdlib.h>
#include <string.h>
#ifndef _WIN32
#include <sys/stat.h>
#include <limits.h>
#include <unistd.h>
#include <fcntl.h>
#endif
#include <errno.h>
#include "securec.h"
#include "dcmi_log.h"
#include "dsmi_common_interface_custom.h"
#include "dcmi_interface_api.h"
#include "dcmi_common.h"
#include "dcmi_virtual_intf.h"
#include "dcmi_mcu_intf.h"
#include "dcmi_inner_cfg_persist.h"
#include "dcmi_inner_cfg_manage.h"
#include "dcmi_product_judge.h"
#include "dcmi_environment_judge.h"
#include "dcmi_permission_judge.h"
#include "dcmi_inner_info_get.h"

#if defined DCMI_VERSION_1

int dcmi_config_ecc_enable(int card_id, int device_id, int enable_flag)
{
    return dcmi_set_device_ecc_enable(card_id, device_id, DCMI_DEVICE_TYPE_DDR, enable_flag);
}

STATIC int dcmi_check_user_config_support(const char *config_name)
{
    if (!(dcmi_board_chip_type_is_ascend_310p() || dcmi_board_chip_type_is_ascend_310b()) &&
        strcmp(config_name, "cpu_num_cfg") == 0) {
        gplog(LOG_ERR, "board_type is not 310p or 310b and does not support set %s.", config_name);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
 
    if (!dcmi_board_chip_type_is_ascend_310p() && strcmp(config_name, "p2p_mem_cfg") == 0) {
        gplog(LOG_ERR, "board_type is not 310p and does not support set %s.", config_name);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
 
    if (dcmi_board_chip_type_is_ascend_910b_300i_a2() == TRUE &&
        strcmp(config_name, "mac_info") == 0) {
        gplog(LOG_ERR, "board_type is 910b 300i a2 and does not support set %s.", config_name);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
 
    if (!dcmi_board_chip_type_is_ascend_310p_300v() && strcmp(config_name, "set_cpu_freq") == 0) {
        gplog(LOG_ERR, "board_type is not 310p 300v and does not support set %s.", config_name);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
 
    return DCMI_OK;
}

int dcmi_set_user_config(int card_id, int device_id, const char *config_name, unsigned int buf_size, unsigned char *buf)
{
    int err;
    enum dcmi_unit_type device_type = NPU_TYPE;

    if (!dcmi_is_in_phy_machine_root()) {
        gplog(LOG_OP, "Operation not permitted, only root user on physical machine can call this api.");
        return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
    }

    err = dcmi_check_user_config_parameter(config_name, buf_size, buf);
    if (err != DCMI_OK) {
        return err;
    }

    err = dcmi_check_user_config_support(config_name);
    if (err != DCMI_OK) {
        return err;
    }

    err = dcmi_get_device_type(card_id, device_id, &device_type);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d.", err);
        return err;
    }

    if (device_type == NPU_TYPE) {
        err = dcmi_set_npu_device_user_config(card_id, device_id, config_name, buf_size, buf);
        if (err != DCMI_OK) {
            gplog(LOG_OP, "set user config failed. card_id=%d, device_id=%d,err=%d", card_id, device_id, err);
            return err;
        }

        gplog(LOG_OP, "set user config success. card_id=%d, device_id=%d", card_id, device_id);
        return DCMI_OK;
    } else {
        gplog(LOG_OP, "device_type %d is not support.", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
}

int dcmi_get_user_config(int card_id, int device_id, const char *config_name, unsigned int buf_size, unsigned char *buf)
{
    int err;
    enum dcmi_unit_type device_type = NPU_TYPE;

    err = dcmi_check_user_config_parameter(config_name, buf_size, buf);
    if (err != DCMI_OK) {
        return err;
    }

    if (!(dcmi_board_chip_type_is_ascend_310p() || dcmi_board_chip_type_is_ascend_310b()) &&
        strcmp(config_name, "cpu_num_cfg") == 0) {
        gplog(LOG_ERR, "board_type is not 310p or 310b and does not support get %s.", config_name);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    if (!dcmi_board_chip_type_is_ascend_310p() && strcmp(config_name, "p2p_mem_cfg") == 0) {
        gplog(LOG_ERR, "board_type is not 310p and does not support get %s.", config_name);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    if (dcmi_board_chip_type_is_ascend_910b_300i_a2() == TRUE &&
        strcmp(config_name, "mac_info") == 0) {
        gplog(LOG_ERR, "board_type is 910b 300i a2 and does not support get %s.", config_name);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    if (!dcmi_board_chip_type_is_ascend_310p_300v() && strcmp(config_name, "get_cpu_freq") == 0) {
        gplog(LOG_ERR, "board_type is not 310p 300v and does not support get %s.", config_name);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
    
    err = dcmi_get_device_type(card_id, device_id, &device_type);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d.", err);
        return err;
    }

    if (device_type == NPU_TYPE) {
        return dcmi_get_npu_device_user_config(card_id, device_id, config_name, buf_size, buf);
    } else {
        gplog(LOG_ERR, "device_type %d is error.", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
}

int dcmi_clear_device_user_config(int card_id, int device_id, const char *config_name)
{
    int err;
    enum dcmi_unit_type device_type = NPU_TYPE;

    if (!dcmi_is_in_phy_machine_root()) {
        gplog(LOG_OP, "Operation not permitted, only root user on physical machine can call this api.");
        return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
    }

    if (dcmi_check_card_is_split_phy(card_id) == TRUE) {
        // 910B算力切分场景下，不支持该命令
        gplog(LOG_OP, "In the vNPU scenario, this device does not support dcmi_clear_device_user_config.");
        return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
    }

    if (config_name == NULL) {
        gplog(LOG_ERR, "config_name is invalid.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if (!(dcmi_board_chip_type_is_ascend_310p() || dcmi_board_chip_type_is_ascend_310b()) &&
        strcmp(config_name, "cpu_num_cfg") == 0) {
        gplog(LOG_ERR, "board_type is not 310p or 310b and does not support clear %s.", config_name);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    if (!dcmi_board_chip_type_is_ascend_310p() && strcmp(config_name, "p2p_mem_cfg") == 0) {
        gplog(LOG_ERR, "board_type is not 310p and does not support clear %s.", config_name);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    if (dcmi_board_chip_type_is_ascend_910b_300i_a2() == TRUE &&
        strcmp(config_name, "mac_info") == 0) {
        gplog(LOG_ERR, "board_type is 910b 300i a2 and does not support set %s.", config_name);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    err = dcmi_get_device_type(card_id, device_id, &device_type);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d.", err);
        return err;
    }

    if (device_type == NPU_TYPE) {
        err = dcmi_clear_npu_device_user_config(card_id, device_id, config_name);
        if (err != DCMI_OK) {
            gplog(LOG_OP, "clear user config failed. card_id=%d, device_id=%d, err=%d", card_id,
                device_id, err);
            return err;
        }

        gplog(LOG_OP, "clear user config success. card_id=%d, device_id=%d", card_id, device_id);
        return DCMI_OK;
    }
    gplog(LOG_OP, "device_type %d is not support.", device_type);
    return DCMI_ERR_CODE_NOT_SUPPORT;
}

int dcmi_set_device_ecc_enable(int card_id, int device_id, enum dcmi_device_type dcmi_device_type, int enable_flag)
{
    int err;
    enum dcmi_unit_type device_type = NPU_TYPE;

    if (!dcmi_is_in_phy_machine_root()) {
        gplog(LOG_OP, "Operation not permitted, only root user on physical machine can call this api.");
        return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
    }

    /* 设备类型，目前支持DCMI_DEVICE_TYPE_DDR */
    if (dcmi_device_type != DCMI_DEVICE_TYPE_DDR) {
        gplog(LOG_OP, "device_type %d is not support.", dcmi_device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    if (enable_flag != 0 && enable_flag != 1) {
        gplog(LOG_ERR, "enable_flag %d is invalid.", enable_flag);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    err = dcmi_get_device_type(card_id, device_id, &device_type);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d.", err);
        return err;
    }

    if (device_type == NPU_TYPE) {
        err = dcmi_set_npu_device_ecc_enable(card_id, device_id, dcmi_device_type, enable_flag);
        if (err != DCMI_OK) {
            gplog(LOG_OP,
                "set ecc enable failed. card_id=%d, device_id=%d, dcmi_device_type=%d, enable_flag=%d, err=%d", card_id,
                device_id, dcmi_device_type, enable_flag, err);
            return err;
        }

        gplog(LOG_OP, "set ecc enable success. card_id=%d, device_id=%d, dcmi_device_type=%d, enable_flag=%d", card_id,
            device_id, dcmi_device_type, enable_flag);
        return DCMI_OK;
    } else {
        gplog(LOG_OP, "device_type %d is not support.", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
}
#endif

int dcmi_set_device_mac(int card_id, int device_id, int mac_id, const char *mac_addr, unsigned int len)
{
    int err;
    enum dcmi_unit_type device_type = NPU_TYPE;

    if (!dcmi_is_in_phy_machine_root()) {
        gplog(LOG_OP, "Operation not permitted, only root user on physical machine can call this api.");
        return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
    }

    if (mac_addr == NULL) {
        gplog(LOG_ERR, "input para mac_addr is NULL.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }
    if (len != MAC_ADDR_LEN) {
        gplog(LOG_ERR, "input para len is invalid. len=%u.", len);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }
    if (mac_id < 0) {
        gplog(LOG_ERR, "input para mac_id is invalid, mac_id=%d.", mac_id);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    err = dcmi_get_device_type(card_id, device_id, &device_type);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d.", err);
        return err;
    }

    if (dcmi_board_chip_type_is_ascend_910b_300i_a2()) {
        gplog(LOG_ERR, "board_type is 910b 300i a2 and does not support set device mac.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    if (device_type == NPU_TYPE) {
        err = dcmi_set_npu_device_mac(card_id, device_id, mac_id, mac_addr, len);
        if (err != DCMI_OK) {
            gplog(LOG_OP, "set device mac failed.card_id=%d,device_id=%d,mac_id=%d,err=%d", card_id, device_id, mac_id,
                err);
            return err;
        }
        gplog(LOG_OP, "set device mac success. card_id=%d, device_id=%d mac_id=%d", card_id, device_id, mac_id);
    } else {
        gplog(LOG_OP, "device_type %d is not support.", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    return DCMI_OK;
}

int dcmi_set_device_share_enable(int card_id, int device_id, int enable_flag)
{
    int err;
    enum dcmi_unit_type device_type = NPU_TYPE;

    if (!dcmi_is_in_phy_machine_root()) {
        gplog(LOG_OP, "Operation not permitted, only root user on physical machine can call this api.");
        return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
    }

    if (enable_flag != 0 && enable_flag != 1) {
        gplog(LOG_ERR, "enable_flag %d is invalid.", enable_flag);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    err = dcmi_get_device_type(card_id, device_id, &device_type);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d.", err);
        return err;
    }

    if (device_type == NPU_TYPE) {
        err = dcmi_set_npu_device_share_enable(card_id, device_id, enable_flag);
        if (err != DCMI_OK) {
            gplog(LOG_OP, "set share enable failed. card_id=%d, device_id=%d, enable_flag=%d, err=%d",
                card_id, device_id, enable_flag, err);
            return err;
        }

        gplog(LOG_OP, "set share enable success. card_id=%d, device_id=%d, enable_flag=%d",
            card_id, device_id, enable_flag);
        return DCMI_OK;
    } else {
        gplog(LOG_OP, "device_type %d is not support.", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
}

int dcmi_set_nve_level(int card_id, int device_id, int nve_level)
{
    int err;

    if (!dcmi_is_in_phy_machine_root()) {
        gplog(LOG_OP, "Operation not permitted, only root user on physical machine can call this api.");
        return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
    }

    if (nve_level < DCMI_NVE_LOW || nve_level >= DCMI_NVE_LEVEL_INVALID) {
        gplog(LOG_ERR, "nve_level is invalid. nve_level=%d", nve_level);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if (dcmi_board_chip_type_is_ascend_310() && dcmi_board_type_is_soc()) {
        unsigned char level = (unsigned char)nve_level;
        err = dcmi_set_user_config(card_id, device_id, "set_nve_level", sizeof(unsigned char), &level);
        if (err != DCMI_OK) {
            gplog(LOG_OP, "set nve level failed. card_id=%d, device_id=%d, nve_level=%d, err=%d", card_id, device_id,
                nve_level, err);
            return err;
        }
        gplog(LOG_OP, "set nve level success. card_id=%d, device_id=%d, nve_level=%d", card_id, device_id, nve_level);
    } else {
        gplog(LOG_OP, "This device does not support set nve level.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    return DCMI_OK;
}

int dcmi_set_device_user_config(int card_id, int device_id, const char *config_name, unsigned int buf_size, char *buf)
{
    int err;
    enum dcmi_unit_type device_type = NPU_TYPE;

    if (!dcmi_is_in_phy_machine_root()) {
        gplog(LOG_OP, "Operation not permitted, only root user on physical machine can call this api.");
        return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
    }

    err = dcmi_check_user_config_parameter(config_name, buf_size, (unsigned char *)buf);
    if (err != DCMI_OK) {
        return err;
    }

    if (!(dcmi_board_chip_type_is_ascend_310p() || dcmi_board_chip_type_is_ascend_310b()) &&
        strcmp(config_name, "cpu_num_cfg") == 0) {
        gplog(LOG_ERR, "board_type is not 310p or 310b and does not support set %s.", config_name);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    if (!dcmi_board_chip_type_is_ascend_310p() && strcmp(config_name, "p2p_mem_cfg") == 0) {
        gplog(LOG_ERR, "board_type is not 310p and does not support set %s.", config_name);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    if (dcmi_board_chip_type_is_ascend_910b_300i_a2() == TRUE &&
        strcmp(config_name, "mac_info") == 0) {
        gplog(LOG_ERR, "board_type is 910b 300i a2 and does not support set %s.", config_name);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    if (!dcmi_board_chip_type_is_ascend_310p_300v() && strcmp(config_name, "set_cpu_freq") == 0) {
        gplog(LOG_ERR, "board_type is not 310p 300v and does not support set %s.", config_name);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    err = dcmi_get_device_type(card_id, device_id, &device_type);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d.", err);
        return err;
    }

    if (device_type == NPU_TYPE) {
        err = dcmi_set_npu_device_user_config(card_id, device_id, config_name, buf_size, (unsigned char *)buf);
        if (err != DCMI_OK) {
            gplog(LOG_OP, "set user config failed. card_id=%d, device_id=%d,err=%d", card_id, device_id, err);
            return err;
        }

        gplog(LOG_OP, "set user config success. card_id=%d, device_id=%d", card_id, device_id);
        return DCMI_OK;
    }
    gplog(LOG_OP, "device_type %d is not support.", device_type);
    return DCMI_ERR_CODE_NOT_SUPPORT;
}

int dcmi_set_device_clear_pcie_error(int card_id, int device_id)
{
    int err;
    enum dcmi_unit_type device_type = NPU_TYPE;
    if (!dcmi_is_in_phy_machine_root()) {
        gplog(LOG_OP, "Operation not permitted, only root user on physical machine can call this api.");
        return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
    }

    err = dcmi_get_device_type(card_id, device_id, &device_type);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d.", err);
        return err;
    }

    if (device_type == NPU_TYPE) {
        if (dcmi_is_has_pcieinfo() == TRUE) {
            err = dcmi_set_npu_device_clear_pcie_error(card_id, device_id);
        } else {
            gplog(LOG_OP, "This device does not support clear pcie error.");
            return DCMI_ERR_CODE_NOT_SUPPORT;
        }
    } else {
        gplog(LOG_OP, "device_type %d is not support.", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    if (err != DCMI_OK) {
        gplog(LOG_OP, "clear pcie error failed. card_id=%d, device_id=%d, err=%d", card_id, device_id, err);
        return err;
    }

    gplog(LOG_OP, "clear pcie error success. card_id=%d, device_id=%d", card_id, device_id);
    return DCMI_OK;
}

int dcmi_set_device_clear_ecc_statistics_info(int card_id, int device_id)
{
    int err;
    enum dcmi_unit_type device_type = NPU_TYPE;

    if (!dcmi_is_in_phy_machine_root()) {
        gplog(LOG_OP, "Operation not permitted, only root user on physical machine can call this api.");
        return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
    }

    if (dcmi_check_card_is_split_phy(card_id) == TRUE) {
        // 910B算力切分场景下，不支持该命令
        gplog(LOG_OP,
            "In the vNPU scenario, this device does not support dcmi_set_device_clear_ecc_statistics_info.\n");
        return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
    }

    err = dcmi_get_device_type(card_id, device_id, &device_type);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d.", err);
        return err;
    }

    if (device_type == NPU_TYPE) {
        err = dcmi_clear_npu_ecc_statistics_info(card_id, device_id);
        if (err != DCMI_OK) {
            gplog(LOG_OP, "clear ecc statistics info failed. card_id=%d, device_id=%d, err=%d", card_id, device_id,
                err);
            return err;
        }

        gplog(LOG_OP, "clear ecc statistics info success. card_id=%d, device_id=%d", card_id, device_id);
        return DCMI_OK;
    } else {
        gplog(LOG_OP, "device_type %d is not support.", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
}

int dcmi_set_device_gateway(int card_id, int device_id, enum dcmi_port_type input_type, int port_id,
    struct dcmi_ip_addr *gateway)
{
    int err;
    enum dcmi_unit_type device_type = NPU_TYPE;

    if (!dcmi_is_in_phy_machine_root()) {
        gplog(LOG_OP, "Operation not permitted, only root user on physical machine can call this api.");
        return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
    }

    if (dcmi_check_card_is_split_phy(card_id) == TRUE) {
        // 910B算力切分场景下，不支持该命令
        gplog(LOG_OP, "In the vNPU scenario, this device does not support dcmi_set_device_gateway.");
        return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
    }

    if (gateway == NULL) {
        gplog(LOG_ERR, "gateway is NULL");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if (input_type >= DCMI_INVALID_PORT || input_type < DCMI_VNIC_PORT) {
        gplog(LOG_ERR, "input_type is invalid. input_type=%d", input_type);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    err = dcmi_get_device_type(card_id, device_id, &device_type);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d.", err);
        return err;
    }

    if (dcmi_board_chip_type_is_ascend_910b_300i_a2() == TRUE || dcmi_board_chip_type_is_ascend_310b() == TRUE) {
        gplog(LOG_OP, "This device does not support set device gateway.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    if ((dcmi_board_chip_type_is_ascend_910() || dcmi_board_chip_type_is_ascend_910b()) &&
        input_type != DCMI_ROCE_PORT) {
        gplog(LOG_OP, "This device does not support this input_type. input_type=%d", input_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    if (device_type == NPU_TYPE) {
        err = dcmi_set_npu_device_gateway(card_id, device_id, input_type, port_id, gateway);
        if (err != DCMI_OK) {
            gplog(LOG_OP, "set device gateway failed. card_id=%d, device_id=%d, port_id=%d, err=%d", card_id, device_id,
                port_id, err);
            return err;
        }
        gplog(LOG_OP, "set device gateway success. card_id=%d, device_id=%d port_id=%d", card_id, device_id, port_id);
    } else {
        gplog(LOG_OP, "device_type %d is not support.", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    return DCMI_OK;
}

int dcmi_set_device_ip(int card_id, int device_id, enum dcmi_port_type input_type, int port_id, struct dcmi_ip_addr *ip,
    struct dcmi_ip_addr *mask)
{
    int err;
    enum dcmi_unit_type device_type = NPU_TYPE;

    if (!dcmi_is_in_phy_machine_root()) {
        gplog(LOG_OP, "Operation not permitted, only root user on physical machine can call this api.");
        return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
    }

    if (ip == NULL) {
        gplog(LOG_ERR, "input para ip is NULL.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }
    if (mask == NULL) {
        gplog(LOG_ERR, "input para mask is NULL.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }
    if (input_type >= DCMI_INVALID_PORT) {
        gplog(LOG_ERR, "input para input_type is invalid. input_type=%d.", input_type);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    err = dcmi_get_device_type(card_id, device_id, &device_type);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d.", err);
        return err;
    }

    if (dcmi_board_chip_type_is_ascend_310b() == TRUE) {
        gplog(LOG_OP, "This device does not support set device ip.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    if (dcmi_board_chip_type_is_ascend_910b_300i_a2() == TRUE) {
        gplog(LOG_OP, "This device does not support set device ip.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    if (device_type == NPU_TYPE) {
        err = dcmi_set_npu_device_ip(card_id, device_id, input_type, port_id, ip, mask);
        if (err != DCMI_OK) {
            gplog(LOG_OP, "set device ip failed. card_id=%d, device_id=%d, port_id=%d, err=%d", card_id, device_id,
                port_id, err);
            return err;
        }
        gplog(LOG_OP, "set device ip success. card_id=%d, device_id=%d port_id=%d", card_id, device_id, port_id);
    } else {
        gplog(LOG_OP, "device_type %d is not support.", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    return DCMI_OK;
}

int dcmi_set_device_cpu_num_config(int card_id, int device_id, unsigned char *buf, unsigned int buf_size)
{
    int err;
    enum dcmi_unit_type device_type = NPU_TYPE;

    if (!dcmi_is_in_phy_machine_root()) {
        gplog(LOG_ERR, "Operation not permitted, only root user on physical machine can call this api.");
        return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
    }

    if (buf == NULL || buf_size != DCMI_CPU_NUM_CFG_LEN) {
        gplog(LOG_ERR, "cpu_num_config is null or buf size %u is invaild", buf_size);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    // 支持310P和310B
    if (!(dcmi_board_chip_type_is_ascend_310p() || dcmi_board_chip_type_is_ascend_310b())) {
        gplog(LOG_OP, "This device does not support set cpu num cfg.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    err = dcmi_get_device_type(card_id, device_id, &device_type);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d.", err);
        return err;
    }

    if (device_type == NPU_TYPE) {
        err = dcmi_set_npu_device_user_config(card_id, device_id, "cpu_num_cfg", buf_size, buf);
        if (err != DCMI_OK) {
            gplog(LOG_OP, "set cpu num config failed. card_id=%d, device_id=%d,err=%d", card_id, device_id, err);
            return err;
        }

        gplog(LOG_OP, "set cpu num config success. card_id=%d, device_id=%d", card_id, device_id);
        return DCMI_OK;
    } else {
        gplog(LOG_ERR, "device_type is not support.%d.", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
}

int dcmi_create_capability_group(int card_id, int device_id, int ts_id, struct dcmi_capability_group_info *group_info)
{
    int err;
    enum dcmi_unit_type device_type = INVALID_TYPE;

    if (!dcmi_is_in_phy_machine_root()) {
        gplog(LOG_ERR, "Operation not permitted, only root user on physical machine can call this api.");
        return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
    }

    err = dcmi_check_capability_group_para(ts_id, group_info);
    if (err != DCMI_OK) {
        return err;
    }

    err = dcmi_get_device_type(card_id, device_id, &device_type);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d", err);
        return err;
    }

    if ((device_type == NPU_TYPE) && (dcmi_check_capability_group_support_type() == DCMI_OK)) {
        err = dcmi_npu_create_capability_group(card_id, device_id, ts_id, group_info);
        if (err != DCMI_OK) {
            gplog(LOG_ERR, "create capability group failed. card_id=%d, device_id=%d, ts_id=%d, group_id=%u, err=%d",
                card_id, device_id, ts_id, group_info->group_id, err);
            return err;
        }

        gplog(LOG_OP, "create capability group success. card_id=%d, device_id=%d, ts_id=%d, group_id=%u",
            card_id, device_id, ts_id, group_info->group_id);
        return DCMI_OK;
    } else {
        gplog(LOG_ERR, "The device does not support this api.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
}

int dcmi_delete_capability_group(int card_id, int device_id, int ts_id, int group_id)
{
    int err;
    enum dcmi_unit_type device_type = INVALID_TYPE;

    if (!dcmi_is_in_phy_machine_root()) {
        gplog(LOG_OP, "Operation not permitted, only root user on physical machine can call this api.");
        return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
    }

    if ((ts_id != DCMI_TS_AICORE) || (group_id < 0) || (group_id >= DCMI_CAPABILITY_GROUP_MAX_COUNT_NUM)) {
        gplog(LOG_ERR, "parameter invalid. ts_id=%d, group_id=%d", ts_id, group_id);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    err = dcmi_get_device_type(card_id, device_id, &device_type);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d.", err);
        return err;
    }

    if ((device_type == NPU_TYPE) && (dcmi_check_capability_group_support_type() == DCMI_OK)) {
        err = dcmi_npu_delete_capability_group(card_id, device_id, ts_id, group_id);
        if (err != DCMI_OK) {
            gplog(LOG_ERR, "delete capability group failed. card_id=%d, device_id=%d, ts_id=%d, group_id=%d, err=%d",
                card_id, device_id, ts_id, group_id, err);
            return err;
        }

        gplog(LOG_OP, "delete capability group success. card_id=%d, device_id=%d, ts_id=%d, group_id=%d",
            card_id, device_id, ts_id, group_id);
        return DCMI_OK;
    } else {
        gplog(LOG_ERR, "The device does not support this api.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
}

int dcmi_check_custom_op_config_recover_mode_is_permitted(const char *operate_mode)
{
    if (dcmi_board_chip_type_is_ascend_910_93() == TRUE) {
        if (!(dcmi_is_in_phy_machine_root())) {
            gplog(LOG_OP,
                "Operation not permitted, only root user on physical machine can call this api.");
            return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
        }
    } else {
        gplog(LOG_OP, "This device does not support %s custom-op config recover mode.", operate_mode);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
    return DCMI_OK;
}

int dcmi_set_custom_op_config_recover_mode(unsigned int mode)
{
    int err;

    err = dcmi_check_custom_op_config_recover_mode_is_permitted("set");
    if (err != DCMI_OK) {
        return err;
    }

    if ((mode != DCMI_CFG_RECOVER_ENABLE) && (mode != DCMI_CFG_RECOVER_DISABLE)) {
        gplog(LOG_ERR, "mode [%u] is invalid.", mode);
        gplog(LOG_OP, "int dcmi_set_custom_op_config_recover_mode(unsigned int mode) parameter mode [%u] is invalid.",
              mode);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    err = dcmi_cfg_set_custom_op_config_recover_mode(mode);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "set custom-op config recover mode %u failed. err=%d", mode, err);
        return DCMI_ERR_CODE_INNER_ERR;
    }

    gplog(LOG_OP, "dcmi set custom-op config recover mode %s success",
        (mode == DCMI_CFG_RECOVER_ENABLE) ? "enable" : "disable");
    return DCMI_OK;
}

#if defined DCMI_VERSION_2
int dcmi_set_card_customized_info(int card_id, char *info, int len)
{
    int err;

    if (!dcmi_is_in_phy_machine_root()) {
        gplog(LOG_OP, "Operation not permitted, only root user on physical machine can call this api.");
        return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
    }

    if (info == NULL || len <= 0) {
        gplog(LOG_ERR, "info is invalid.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    err = dcmi_check_card_id(card_id);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "check card id %d failed %d.", card_id, err);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    bool check_result =
        (dcmi_board_chip_type_is_ascend_310p() || (dcmi_board_type_is_card() && dcmi_board_chip_type_is_ascend_310()));
    if (!check_result) {
        gplog(LOG_OP, "This device does not support setting customized info.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    err = dcmi_set_check_customized_is_exist(card_id);
    if (err != DCMI_OK) {
        return err;
    }

    err = dcmi_mcu_set_customized_info(card_id, info, len);
    if (err != DCMI_OK) {
        gplog(LOG_OP, "set customized info failed. card_id=%d, err=%d", card_id, err);
        return err;
    }

    gplog(LOG_OP, "set customized info success. card_id=%d", card_id);
    return DCMI_OK;
}

#endif /* DCMI_VERSION_2 */

int dcmi_set_spod_node_status(int card_id, int device_id, unsigned int sdid, unsigned int status)
{
    int ret;
    struct dcmi_spod_node_status para = {0};

    para.sdid = sdid;
    para.status = status;

    ret = dcmi_set_device_info(card_id, device_id, DCMI_MAIN_CMD_CHIP_INF,
        DCMI_CHIP_INF_SUB_CMD_SPOD_NODE_STATUS, (void*)&para, sizeof(struct dcmi_spod_node_status));
    if ((ret != DCMI_OK) && (ret != DCMI_ERR_CODE_NOT_SUPPORT)) {
        gplog(LOG_ERR, "set spod node status failed. err is %d.", ret);
    }

    return ret;
}