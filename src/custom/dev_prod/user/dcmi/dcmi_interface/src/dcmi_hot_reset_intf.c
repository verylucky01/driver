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
#include "dcmi_init_basic.h"
#include "dcmi_fault_manage_intf.h"
#include "dcmi_log.h"
#include "dcmi_permission_judge.h"
#include "dcmi_virtual_intf.h"
#include "dcmi_ipmi.h"
#include "dcmi_os_adapter.h"
#include "dcmi_product_judge.h"
#include "dcmi_npu_link_intf.h"
#include "dcmi_inner_cfg_manage.h"
#include "dcmi_inner_info_get.h"
#include "dcmi_environment_judge.h"
#include "dcmi_hot_reset_intf.h"

int g_brother_card_id[MAX_RESET_CARD_NUM] = {-1, -1, -1, -1, -1, -1, -1, -1};
#if defined DCMI_VERSION_1
int dcmi_pre_reset_soc(int card_id, int device_id)
{
    return dcmi_set_device_pre_reset(card_id, device_id);
}

int dcmi_rescan_soc(int card_id, int device_id)
{
    return dcmi_set_device_rescan(card_id, device_id);
}

int dcmi_reset_device(int card_id, int device_id)
{
    return dcmi_set_device_reset(card_id, device_id, OUTBAND_CHANNEL);
}

STATIC int dcmi_reset_device_inband_param_check(int card_id, int device_id, enum dcmi_unit_type *device_type)
{
    int err;

    if (!dcmi_is_in_phy_machine_root()) {
        gplog(LOG_OP, "Operation not permitted, only root user on physical machine can call this api.");
        return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
    }

    err = dcmi_get_device_type(card_id, device_id, device_type);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d.", err);
        return err;
    }

    if (dcmi_board_chip_type_is_ascend_910() || dcmi_board_chip_type_is_ascend_910b() ||
        dcmi_board_chip_type_is_ascend_910_93()) {
        gplog(LOG_OP, "The device does not support this api");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    if (dcmi_board_type_is_model()) {
        gplog(LOG_OP, "The device does not support in-band reset.card_id %d device_id %d", card_id, device_id);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    // 310p双芯片卡为smp模式，仅支持对device id 0进行预复位
    if ((dcmi_board_type_is_310p_duo_chips() == TRUE) && (device_id != 0)) {
        gplog(LOG_OP, "The device does not support in-band reset. device_id %d", device_id);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    return DCMI_OK;
}

int dcmi_reset_device_inband(int card_id, int device_id)
{
    int err;
    int device_logic_id = 0;
    enum dcmi_unit_type device_type = NPU_TYPE;

    err = dcmi_reset_device_inband_param_check(card_id, device_id, &device_type);
    if (err != DCMI_OK) {
        return err;
    }

    if (device_type == NPU_TYPE) {
        err = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
        if (err != DCMI_OK) {
            gplog(LOG_ERR, "dcmi_get_device_logic_id failed. err is %d.", err);
            return err;
        }
        dcmi_npu_msn_env_clean(card_id);
#ifndef _WIN32
        err = dsmi_hot_reset_atomic(device_logic_id, DSMI_SUBCMD_HOTRESET_ASSEMBLE);
        if (err != DSMI_OK) {
            gplog(LOG_OP, "call dsmi_hot_reset_atomic failed.%d.", err);
            return dcmi_convert_error_code(err);
        }
#else
        err = dsmi_hot_reset_soc(device_logic_id);
        if (err != DSMI_OK) {
            gplog(LOG_OP, "call dsmi_hot_reset_soc failed.%d.", err);
            return dcmi_convert_error_code(err);
        }
#endif
        gplog(LOG_OP, "reset device inband success. card_id=%d, device_id=%d", card_id, device_id);
        return DCMI_OK;
    } else {
        gplog(LOG_OP, "device_type %d is not support.", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
}
#endif

int dcmi_set_device_pre_reset(int card_id, int device_id)
{
    int err;
    enum dcmi_unit_type device_type = NPU_TYPE;
    unsigned int main_board_id = 0;
    
    // 支持物理机和A3特权容器场景
    main_board_id = dcmi_get_maindboard_id_inner();
    if (!dcmi_is_in_phy_machine_root() && !(dcmi_is_in_privileged_docker_root() &&
        dcmi_mainboard_is_arm_910_93(main_board_id))) {
        gplog(LOG_OP, "Operation not permitted, only root user on physical machine can call this api.");
        return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
    }
    
    if (!dcmi_mainboard_is_arm_910_93(main_board_id)) {
        if (dcmi_check_device_reset_vnpu_mode(card_id) == DCMI_OK) {
            gplog(LOG_OP, "card_id %d is in vnpu mode can not reset.\n", card_id);
            return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
        }
    }

    err = dcmi_get_device_type(card_id, device_id, &device_type);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d.", err);
        return err;
    }

    // 310p双芯片卡为smp模式，仅支持对device id0进行复位
    if ((dcmi_board_type_is_310p_duo_chips()) && (device_id != 0)) {
        gplog(LOG_OP, "device_id %d is not support.", device_id);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    if (device_type == NPU_TYPE) {
        err = dcmi_set_npu_device_pre_reset(card_id, device_id);
        if (err != DCMI_OK) {
            gplog(LOG_OP, "set pre reset failed. card_id=%d, device_id=%d, err=%d", card_id, device_id, err);
            return err;
        }

        gplog(LOG_OP, "set pre reset success. card_id=%d, device_id=%d", card_id, device_id);
        return DCMI_OK;
    } else {
        gplog(LOG_OP, "device_type %d is not support.", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
}

STATIC int dcmi_check_permission_by_channel_type(const enum dcmi_reset_channel channel_type)
{
    if (channel_type != INBAND_CHANNEL) {
        if (!dcmi_board_chip_type_is_ascend_910_93() || !dcmi_is_in_privileged_docker_root()) {
            gplog(LOG_OP, "Operation not permitted, only inband mode is supported on virtual machine or container.");
            return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
        }
    }
    return DCMI_OK;
}

int dcmi_check_device_reset_permission(const enum dcmi_reset_channel channel_type)
{
    int ret;

    if (dcmi_check_run_not_root()) {
        gplog(LOG_OP, "Operation not permitted, only root user can call this api.");
        return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
    }

    if (!dcmi_is_in_phy_machine()) {
        ret = dcmi_check_permission_by_channel_type(channel_type);
        if (ret != DCMI_OK) {
            gplog(LOG_OP, "call dcmi_check_permission_by_channel_type failed. err is %d", ret);
            return ret;
        }

        if ((!dcmi_board_chip_type_is_ascend_310()) &&
            (!dcmi_board_chip_type_is_ascend_310p()) &&
            (!dcmi_board_chip_type_is_ascend_910b()) &&
            (!dcmi_board_chip_type_is_ascend_910()) &&
            (!dcmi_board_chip_type_is_ascend_910_93())) {
            gplog(LOG_OP, "Operation not permitted, this device can't call this api on virtual machine or container.");
            return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
        }
    }

    /* 910B 支持物理机 + 特权容器 */
    if (dcmi_check_run_in_docker() &&
        (dcmi_board_chip_type_is_ascend_910b() || dcmi_board_chip_type_is_ascend_910_93())) {
        ret = dcmi_check_a2_a3_device_reset_docker_permission();
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "call dcmi_check_a2_a3_device_reset_docker_permission failed. err is %d.", ret);
            return ret;
        }
    }

    return DCMI_OK;
}

int dcmi_check_device_reset_vnpu_mode(int card_id)
{
    int ret = 0;
    int hccs_status = HCCS_OFF;
    unsigned int env_flag;
    int vir_flag;

    if (dcmi_check_run_in_docker() || dcmi_check_run_in_vm()) {
        // 在虚拟机或容器里，通过chip name判断是否算力切分
        // 获取当前场景信息
        ret = dcmi_get_environment_flag(&env_flag);
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "Failed to get dcmi_get_environment_flag.\n");
            return DCMI_ERR_CODE_INNER_ERR;
        }

        // 获取是否算力切分
        ret = dcmi_check_vnpu_chip_is_vir(card_id, 0, &vir_flag);
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "Failed to get dcmi_check_vnpu_chip_is_vir.\n");
            return DCMI_ERR_CODE_INNER_ERR;
        }

        if ((env_flag == ENV_PHYSICAL_PLAIN_CONTAINER) && (vir_flag == 1)) {
            return DCMI_OK;
        }
    } else {
        if (dcmi_board_chip_type_is_ascend_910b()) {
            // 当前只有910B需要查询hccs互联状态
            ret = dcmi_get_hccs_status(card_id, 0, &hccs_status);
            if (ret != DCMI_OK) {
                gplog(LOG_ERR, "Failed to get dcmi_get_hccs_status.\n");
                return DCMI_ERR_CODE_INNER_ERR;
            }
        }

        if (hccs_status == HCCS_ON) {
            if (dcmi_check_card_is_split_phy(0xff) == TRUE) {
                return DCMI_OK;
            }
        } else {
            if (dcmi_check_card_is_split_phy(card_id) == TRUE) {
                return DCMI_OK;
            }
        }
    }

    return DCMI_ERR_CODE_INNER_ERR;
}

int dcmi_set_device_reset(int card_id, int device_id, enum dcmi_reset_channel channel_type)
{
    int err;
    enum dcmi_unit_type device_type = NPU_TYPE;

    err = dcmi_check_device_reset_permission(channel_type);
    if (err != DCMI_OK) {
        return err;
    }

    if (card_id == ALL_DEVICE_RESET_CARD_ID && channel_type != INBAND_CHANNEL) {
        gplog(LOG_ERR, "This card_id is not supported in this scenario.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }
    
    if (card_id != ALL_DEVICE_RESET_CARD_ID || !dcmi_board_chip_type_is_ascend_910_93()) {
        err = dcmi_get_device_type(card_id, device_id, &device_type);
        if (err != DCMI_OK) {
            gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d.", err);
            return err;
        }
    }
    
    // Atlas 300i-duo为smp模式，仅支持对device id0进行复位
    if ((dcmi_board_type_is_310p_duo_chips()) && (device_id != 0)) {
        gplog(LOG_OP, "device_id %d is not support.", device_id);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    if (device_type == NPU_TYPE) {
        switch (channel_type) {
            case INBAND_CHANNEL:
                err = dcmi_set_npu_device_reset_inband(card_id, device_id);
                break;
            case OUTBAND_CHANNEL:
                err = dcmi_set_npu_device_reset_outband(card_id, device_id);
                break;
            default:
                gplog(LOG_ERR, "channel_type %d is error.", channel_type);
                return DCMI_ERR_CODE_NOT_SUPPORT;
        }
    } else {
        gplog(LOG_OP, "device_type %d is not support.", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    if (err != DCMI_OK) {
        gplog(LOG_OP, "reset failed. card_id=%d, device_id=%d, channel_type=%d, err=%d", card_id, device_id,
            channel_type, err);
        return err;
    }

    gplog(LOG_OP, "reset success. card_id=%d, device_id=%d, channel_type=%d", card_id, device_id, channel_type);
    return DCMI_OK;
}

int dcmi_set_device_rescan(int card_id, int device_id)
{
    int err;
    enum dcmi_unit_type device_type = NPU_TYPE;
    unsigned int main_board_id = 0;

    // 支持物理机和A3特权容器场景
    main_board_id = dcmi_get_maindboard_id_inner();
    if (!dcmi_is_in_phy_machine_root() && !(dcmi_is_in_privileged_docker_root() &&
        dcmi_mainboard_is_arm_910_93(main_board_id))) {
        gplog(LOG_OP, "Operation not permitted, only root user on physical machine can call this api.");
        return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
    }

    err = dcmi_get_device_type(card_id, device_id, &device_type);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d.", err);
        return err;
    }

    // Atlas 300i-duo为smp模式，仅支持对device id0进行复位;
    if ((dcmi_board_type_is_310p_duo_chips()) && (device_id != 0)) {
        gplog(LOG_OP, "device_id %d is not support.", device_id);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    if (device_type == NPU_TYPE) {
        err = dcmi_set_npu_device_rescan(card_id, device_id);
        if (err != DCMI_OK) {
            gplog(LOG_OP, "set device rescan failed. card_id=%d, device_id=%d, err=%d", card_id, device_id, err);
            return err;
        }

        gplog(LOG_OP, "set device rescan success. card_id=%d, device_id=%d", card_id, device_id);
        return DCMI_OK;
    } else {
        gplog(LOG_OP, "device_type %d is not support.", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
}

static int dcmi_get_npu_outband_channel_state(int *channel_state)
{
#ifdef _WIN32
    return DCMI_ERR_CODE_NOT_SUPPORT;
#else
    int ret;

    // 用该函数查询复位结果,用以判断通路状态共获取两次
    ret = dcmi_ipmi_get_npu_outband_channel_state(channel_state);
    if (ret != DCMI_OK) {
        sleep(1); // 延迟1s
        ret = dcmi_ipmi_get_npu_outband_channel_state(channel_state);
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "Call dcmi_ipmi_get_npu_outband_channel_state failed. err is %d.", ret);
            return ret;
        }
    }

    return DCMI_OK;
#endif
}

int dcmi_get_npu_outband_reset_state(int card_id, int device_id, unsigned char *reset_state)
{
#ifdef _WIN32
    return DCMI_ERR_CODE_NOT_SUPPORT;
#else
    int ret;
    int pcie_slot_id; /* pcie卡的槽位号 */

    if (dcmi_board_type_is_card() != TRUE) {
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
    // 获取pcie卡的槽位号
    ret = dcmi_get_card_pcie_slot(card_id, &pcie_slot_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "Call dcmi_get_pcie_slot failed. %d.", ret);
        return ret;
    }

    // 用该函数查询复位结果,用以判断通路状态共获取两次
    ret = dcmi_ipmi_get_npu_reset_state(pcie_slot_id, device_id, reset_state);
    if (ret != DCMI_OK) {
        sleep(1); // 延迟1s
        ret = dcmi_ipmi_get_npu_reset_state(pcie_slot_id, device_id, reset_state);
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "Call dcmi_ipmi_get_npu_reset_state failed. err is %d.", ret);
            return ret;
        }
    }

    return DCMI_OK;
#endif
}

STATIC int dcmi_get_device_npu_outband_channel_state(int card_id, int device_id, int *channel_state)
{
    int ret = 0;
    unsigned int main_board_id = 0;
    unsigned char reset_state = BMC_RESET_CHIP_FAILED; /* 默认是复位失败 */
    const int CHANNEL_STATUS_SUCCESS = 1;

    main_board_id = dcmi_get_maindboard_id_inner();
    if (dcmi_mainboard_is_arm_910_93(main_board_id)) {
        // 910_93场景下bmc不支持直接获取复位状态
        ret = dcmi_get_npu_outband_channel_state(channel_state);
        if (ret != DCMI_OK || *channel_state != CHANNEL_STATUS_SUCCESS) {
            gplog(LOG_ERR, "call dcmi_get_npu_outband_channel_state failed. err is %d.", ret);
            return ret;
        }
        gplog(LOG_OP, "call dcmi_get_npu_outband_channel_state success. card_id=%d, device_id=%d", card_id,
            device_id);
    } else {
        ret = dcmi_get_npu_outband_reset_state(card_id, device_id, &reset_state);
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "call dcmi_get_npu_outband_reset_state failed. err is %d.", ret);
            return ret;
        }
        bool check_result = ((reset_state == BMC_RESET_CHIP_SUCCESS) || (reset_state == BMC_RESET_CHIP_FAILED) ||
            (reset_state == BMC_RESET_CHIP_UNKNOWN));
        *channel_state = check_result ? CHANNEL_STATUS_SUCCESS : *channel_state;
    }

    return DCMI_OK;
}

int dcmi_get_device_outband_channel_state(int card_id, int device_id, int *channel_state)
{
    enum dcmi_unit_type device_type = NPU_TYPE;
    int ret;
    unsigned int main_board_id = 0;

    // 支持物理机和A3特权容器场景
    main_board_id = dcmi_get_maindboard_id_inner();
    if (!dcmi_is_in_phy_machine_root() && !(dcmi_is_in_privileged_docker_root() &&
        dcmi_mainboard_is_arm_910_93(main_board_id))) {
        gplog(LOG_ERR, "Operation not permitted, only root user on physical machine can call this api.");
        return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
    }

    if (channel_state == NULL) {
        gplog(LOG_ERR, "channel state is invalid.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    ret = dcmi_get_device_type(card_id, device_id, &device_type);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_type failed.%d.", ret);
        return ret;
    }

    // 310p双芯片卡为smp模式，仅支持对device id 0进行预复位
    if ((device_type == NPU_TYPE) && ((dcmi_board_type_is_310p_duo_chips() != TRUE) || (device_id == 0))) {
        ret = dcmi_get_device_npu_outband_channel_state(card_id, device_id, channel_state);
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "call dcmi_get_device_npu_outband_channel_state failed, card_id=%d, device_id=%d. err is %d",
                card_id, device_id, ret);
            return ret;
        }
    } else {
        gplog(LOG_ERR, "device_type %d is error.", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    return DCMI_OK;
}

int dcmi_get_upstream_port_offset(int board_id, unsigned int *upstream_port_offset)
{
    if (board_id == DCMI_310_CARD_DMPC_MCU_BOARD_ID) {
        *upstream_port_offset = ASM_PCIE_UPSTREAM_BUS_ADDR;
        return DCMI_OK;
    } else {
#ifdef X86
        if (dcmi_board_chip_type_is_ascend_310()) {
            *upstream_port_offset = NPU_310_PCIE_UPSTREAM_BUS_ADDR;
        } else if (dcmi_board_chip_type_is_ascend_310p()) {
            *upstream_port_offset = NPU_310P_PCIE_UPSTREAM_BUS_ADDR;
        } else if (dcmi_board_chip_type_is_ascend_910()) {
            *upstream_port_offset = NPU_910_PCIE_UPSTREAM_BUS_ADDR;
        } else {
            gplog(LOG_ERR, "call dcmi_get_upstream_port_offset chip type (%d) is invalid.", dcmi_get_board_chip_type());
            return DCMI_ERR_CODE_INNER_ERR;
        }
        return DCMI_OK;
#else
        *upstream_port_offset = PCIE_UPSTREAM_BUS_ADDR;
        return DCMI_OK;
#endif
    }
}

int dcmi_set_npu_device_close_pcie_upstream(int card_id, int device_id, struct dcmi_card_info *card_info)
{
    int ret, board_id = 0;
    unsigned int port_control_offset = 0;

    ret = dcmi_get_card_board_id(card_id, &board_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call get_card_board_id failed.err is %d.", ret);
        return ret;
    }
 
    ret = dcmi_get_upstream_port_offset(board_id, &port_control_offset);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_upstream_port_offset failed.err is %d.", ret);
        return ret;
    }
 
    ret = dcmi_pci_write_conf_byte(card_info->device_info[device_id].switch_pcieinfo, port_control_offset,
                                   CLOSE_PCIE_UPSTREAM);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call pcidr_write_conf_byte failed. err is %d.", ret);
    }
    return ret;
}

void dcmi_get_brother_card_id(int card_id, int *brother_card_id)
{
    *brother_card_id = g_brother_card_id[card_id];
}

int dcmi_check_device_cpld_version(int card_id)
{
    int ret = 0;
    unsigned char fru_id = 0;
    unsigned char cpld_version[CPLD_VERSION_SIZE] = {0};

    ret = dcmi_ipmi_get_npu_fru_id(card_id, &fru_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_ipmi_get_npu_fru_id failed, id=%d. err is %d.", card_id, ret);
        return ret;
    }

    ret = dcmi_ipmi_get_cpld_version(fru_id, cpld_version);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_ipmi_get_cpld_version failed, id=%d. err is %d.", card_id, ret);
        return ret;
    }

    if (strcmp((const char *)cpld_version, MIN_CPLD_VERSION) < 0) {
        gplog(LOG_ERR, "The CPLD firmware version (%s) on card %d does not meet the minimum requirement (%s).",
            cpld_version, card_id, MIN_CPLD_VERSION);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    return ret;
}

int dcmi_pre_reset_brother_card_outbind(int card_id, int brother_card_id)
{
    int ret ;    
    int master_logic_id, slaver_logic_id, brother_master_logic_id, brother_slave_logic_id;

    ret = dcmi_get_device_logic_id(&master_logic_id, card_id, 0);
    ret |= dcmi_get_device_logic_id(&slaver_logic_id, card_id, 1);
    if (brother_card_id != -1) {
        ret |= dcmi_get_device_logic_id(&brother_master_logic_id, brother_card_id, 0);
        ret |= dcmi_get_device_logic_id(&brother_slave_logic_id, brother_card_id, 1);
    }
    if (ret != 0) {
        gplog(LOG_ERR, "get_logicid failed. err is %d. card_id =%d, brother_card_id=%d", ret, card_id, brother_card_id);
        return DCMI_ERR_CODE_INNER_ERR;
    }
 /* 依次判断4个device是否可以复位 */
    ret = dsmi_hot_reset_atomic(master_logic_id, DSMI_SUBCMD_HOTRESET_SETFLAG);
    if (ret != DSMI_OK) {
        gplog(LOG_ERR, "call dsmi_hot_reset_atomic failed. err is %d.master_logic_id =%d", ret, master_logic_id);
        return dcmi_convert_error_code(ret);       
    }
    ret = dsmi_hot_reset_atomic(slaver_logic_id, DSMI_SUBCMD_HOTRESET_SETFLAG);
    if (ret != DSMI_OK) {
        dsmi_hot_reset_atomic(master_logic_id, DSMI_SUBCMD_HOTRESET_CLEARFLAG);
        gplog(LOG_ERR, "call dsmi_hot_reset_atomic failed. err is %d.slaver_logic_id =%d", ret, slaver_logic_id);
        return dcmi_convert_error_code(ret);        
    }
    if (brother_card_id != -1) {
        ret = dsmi_hot_reset_atomic(brother_master_logic_id, DSMI_SUBCMD_HOTRESET_SETFLAG);
        if (ret != DSMI_OK) {
            dsmi_hot_reset_atomic(master_logic_id, DSMI_SUBCMD_HOTRESET_CLEARFLAG);
            dsmi_hot_reset_atomic(slaver_logic_id, DSMI_SUBCMD_HOTRESET_CLEARFLAG);                
            gplog(LOG_ERR, "call dsmi_hot_reset_atomic failed. err is %d.brother_master_logic_id =%d", ret, brother_master_logic_id);
            return dcmi_convert_error_code(ret);            
        }
        ret = dsmi_hot_reset_atomic(brother_slave_logic_id, DSMI_SUBCMD_HOTRESET_SETFLAG);
        if (ret != DSMI_OK) {
            dsmi_hot_reset_atomic(master_logic_id, DSMI_SUBCMD_HOTRESET_CLEARFLAG);
            dsmi_hot_reset_atomic(slaver_logic_id, DSMI_SUBCMD_HOTRESET_CLEARFLAG);
            dsmi_hot_reset_atomic(brother_master_logic_id, DSMI_SUBCMD_HOTRESET_CLEARFLAG);               
            gplog(LOG_ERR, "call dsmi_hot_reset_atomic failed. err is %d.brother_slave_logic_id =%d", ret, brother_slave_logic_id);
            return dcmi_convert_error_code(ret);           
        }        
    }
    dcmi_npu_msn_env_clean(card_id);
    dsmi_hot_reset_atomic(master_logic_id, DSMI_SUBCMD_HOTRESET_UNBIND);
    dsmi_hot_reset_atomic(slaver_logic_id, DSMI_SUBCMD_HOTRESET_UNBIND);
    if (brother_card_id != -1) {
        dcmi_npu_msn_env_clean(brother_card_id);        
        dsmi_hot_reset_atomic(brother_master_logic_id, DSMI_SUBCMD_HOTRESET_UNBIND);
        dsmi_hot_reset_atomic(brother_slave_logic_id, DSMI_SUBCMD_HOTRESET_UNBIND);        
    }
    dsmi_hot_reset_atomic(slaver_logic_id, DSMI_SUBCMD_HOTRESET_REMOVE);
    dsmi_hot_reset_atomic(master_logic_id, DSMI_SUBCMD_HOTRESET_REMOVE);    
    if (brother_card_id != -1) {    
        dsmi_hot_reset_atomic(brother_slave_logic_id, DSMI_SUBCMD_HOTRESET_REMOVE);
        dsmi_hot_reset_atomic(brother_master_logic_id, DSMI_SUBCMD_HOTRESET_REMOVE);        
    }
    return 0;
   
}

int dcmi_pre_reset_device(int card_id, int brother_card_id, int id)
{
    int ret;
    unsigned int main_board_id = 0;

#ifndef _WIN32
    main_board_id = dcmi_get_maindboard_id_inner();
    if (dcmi_mainboard_is_arm_910_93(main_board_id)) {
        // A3场景下判断cpld版本是否支持
        ret = dcmi_check_device_cpld_version(card_id);
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "call dcmi_check_device_cpld_version failed, id=%d. err is %d.", card_id, ret);
            return ret;
        }
        if (brother_card_id != -1) {
            ret = dcmi_check_device_cpld_version(brother_card_id);
            if (ret != DCMI_OK) {
                gplog(LOG_ERR, "call dcmi_check_device_cpld_version failed, id=%d. err is %d.", brother_card_id, ret);
                return ret;
            }
        }        
        ret = dcmi_pre_reset_brother_card_outbind(card_id, brother_card_id);
        return ret;        
    }
    ret = dsmi_hot_reset_atomic(id, DSMI_SUBCMD_PRERESET_ASSEMBLE);
#else
    ret = dsmi_pre_reset_soc(id, 0);
#endif
    if (ret != DSMI_OK) {
        gplog(LOG_ERR, "pre reset soc failed, id=%d. err is %d.", id, ret);
        return dcmi_convert_error_code(ret);
    }

    return ret;
}

int dcmi_set_npu_device_pre_reset(int card_id, int device_id)
{
    unsigned int device_phy_id = 0, main_board_id = 0;
    struct dcmi_card_info *card_info = NULL;
    int brother_card_id = -1;

    main_board_id = dcmi_get_maindboard_id_inner();
    if (dcmi_board_type_is_card() != TRUE && !dcmi_mainboard_is_arm_910_93(main_board_id)) {
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
    // 获取卡的信息
    int ret = dcmi_get_card_info(card_id, &card_info);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_card_info failed, id=%d. err is %d.", card_id, ret);
        return ret;
    }
    // A3场景下获取网口互助卡的信息
    if (dcmi_mainboard_is_arm_910_93(main_board_id)) {
        ret = dcmi_get_netdev_brother_device(card_id, device_id, &brother_card_id);
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "call dcmi_get_net_dev_brother_device failed. err is %d.", ret);
            return ret;
        }
        g_brother_card_id[card_id] = brother_card_id;
    }

    int device_logic_id = card_info->device_info[device_id].logic_id;
    ret = dsmi_get_phyid_from_logicid(device_logic_id, &device_phy_id);
    if (ret != DSMI_OK) {
        gplog(LOG_ERR, "call dsmi_get_phyid_from_logicid failed. err is %d.", ret);
        return dcmi_convert_error_code(ret);
    }

    int id = (dcmi_board_chip_type_is_ascend_310()) ? (int)device_phy_id : device_logic_id;
    ret = dcmi_pre_reset_device(card_id, brother_card_id, id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_pre_reset_device failed, id=%d. err is %d.", card_id, ret);
        return ret;
    }

    sleep(1);
#ifndef _WIN32
    // 关闭上游端口
    if (!dcmi_mainboard_is_arm_910_93(main_board_id)) {
        ret = dcmi_set_npu_device_close_pcie_upstream(card_id, device_id, card_info);
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "call dcmi_set_npu_device_close_pcie_upstream failed, id=%d. err is %d.", card_id, ret);
            return ret;
        }
    }
#endif

    return ret;
}

int dcmi_set_npu_device_open_pcie_upstream(int card_id, int device_id, struct dcmi_card_info *card_info)
{
    int ret;
    int board_id = 0;
    unsigned int port_control_offset = 0;

    ret = dcmi_get_card_board_id(card_id, &board_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call get_card_board_id failed.err is %d.", ret);
        return ret;
    }

    ret = dcmi_get_upstream_port_offset(board_id, &port_control_offset);
    if (ret != DCMI_OK) {
        return ret;
    }

    ret = dcmi_pci_write_conf_byte(card_info->device_info[device_id].switch_pcieinfo,
                                   port_control_offset, OPEN_PCIE_UPSTREAM);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call pcidr_write_conf_byte failed.%d.\n", ret);
        return ret;
    }

    return DCMI_OK;
}

int dcmi_rescan_card(int card_id, int device_id, struct dcmi_card_info *card_info)
{
    int ret;
    int logic_id, sub_logic_id;
    logic_id = card_info->device_info[0].logic_id;
    sub_logic_id = card_info->device_info[1].logic_id;

    ret = dsmi_hot_reset_atomic(sub_logic_id, DSMI_SUBCMD_HOTRESET_RESCAN);
    if ((ret != DSMI_OK) && (ret != DSMI_ERR_NOT_SUPPORT)) {
        gplog(LOG_ERR, "call dsmi_hot_reset_atomic failed, id=%d. err is %d.", sub_logic_id, ret);
        return dcmi_convert_error_code(ret);
    }

    ret = dsmi_hot_reset_atomic(logic_id, DSMI_SUBCMD_HOTRESET_RESCAN);
    if ((ret != DSMI_OK) && (ret != DSMI_ERR_NOT_SUPPORT)) {
        gplog(LOG_ERR, "call dsmi_hot_reset_atomic failed, id=%d. err is %d.", logic_id, ret);
        return dcmi_convert_error_code(ret);
    }

    return DCMI_OK;
}

int dcmi_set_double_npu_device_rescan(int card_id, int device_id, struct dcmi_card_info *card_info)
{
    int ret, brother_card_id = -1;
    struct dcmi_card_info *bro_card_info = NULL;

    dcmi_get_brother_card_id(card_id, &brother_card_id);

    if (brother_card_id != -1) {
        ret = dcmi_get_card_info(brother_card_id, &bro_card_info);
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "call dcmi_get_card_info failed, card_id=%d. err is %d.", brother_card_id, ret);
            return ret;
        }

        ret = dcmi_rescan_card(brother_card_id, device_id, bro_card_info);
        if (ret != 0) {
            gplog(LOG_ERR, "call dcmi_rescan_card brother failed, card_id=%d. err is %d.", brother_card_id, ret);
            return ret;
        }
        gplog(LOG_OP, "call dcmi_rescan_card succuss, card_id=%d.", brother_card_id);
    }

    ret = dcmi_rescan_card(card_id, device_id, card_info);
    if (ret != 0) {
        gplog(LOG_ERR, "call dcmi_rescan_card failed, card_id=%d. err is %d.", card_id, ret);
        return ret;
    }

    return DCMI_OK;
}

int dcmi_set_npu_device_rescan(int card_id, int device_id)
{
    int ret;
    int device_logic_id;
    unsigned int device_phy_id = 0;
    unsigned int main_board_id = 0;

    main_board_id = dcmi_get_maindboard_id_inner();
    struct dcmi_card_info *card_info = NULL;
    if (dcmi_board_type_is_card() != TRUE && !dcmi_mainboard_is_arm_910_93(main_board_id)) {
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
    // 获取卡的信息
    ret = dcmi_get_card_info(card_id, &card_info);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_card_info failed, card_id=%d. err is %d.", card_id, ret);
        return ret;
    }

#ifndef _WIN32
    if (dcmi_mainboard_is_arm_910_93(main_board_id)) { // 910A3不需要打开上游端口
        ret = dcmi_set_double_npu_device_rescan(card_id, device_id, card_info);
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "call dcmi_set_double_npu_device_rescan failed.%d.\n", ret);
            return ret;
        }
        return ret;
    } else {
        ret = dcmi_set_npu_device_open_pcie_upstream(card_id, device_id, card_info); // 打开上游端口
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "call dcmi_set_npu_device_open_pcie_upstream failed. err is %d.", ret);
            return ret;
        }
    }
#endif
    sleep(1);

    device_logic_id = card_info->device_info[device_id].logic_id;
    ret = dsmi_get_phyid_from_logicid(device_logic_id, &device_phy_id);
    if (ret != DSMI_OK) {
        gplog(LOG_ERR, "call dsmi_get_phyid_from_logicid failed. err is %d.", ret);
        return dcmi_convert_error_code(ret);
    }

    int id = (dcmi_board_chip_type_is_ascend_310()) ? (int)device_phy_id : device_logic_id;

    ret = dsmi_hot_reset_atomic(id, DSMI_SUBCMD_HOTRESET_RESCAN);
    if ((ret != DSMI_OK) && (ret != DSMI_ERR_NOT_SUPPORT)) {
        gplog(LOG_ERR, "call dsmi_hot_reset_atomic failed, id=%d. err is %d.", id, ret);
    }

    return dcmi_convert_error_code(ret);
}

// 910的SMP架构时为dcmi_set_npu_smp_hot_reset
int dcmi_set_all_npu_hot_reset()
{
    int ret;
    int device_id;
    if ((dcmi_board_chip_type_is_ascend_310()) ||
        (dcmi_board_chip_type_is_ascend_310p()) ||
        (dcmi_board_chip_type_is_ascend_910())) {
        device_id = ALL_DEVICE_RESET_FLAG_OLD;
    } else {
        device_id = ALL_DEVICE_RESET_FLAG;
    }
    gplog(LOG_OP, "Reset all device. device_id is %d.", device_id);

#ifndef _WIN32
    // 0为flag，预留字段，当前无意义。
    ret = dsmi_hot_reset_atomic(device_id, DSMI_SUBCMD_HOTRESET_ASSEMBLE);
    if (ret != DSMI_OK) {
        gplog(LOG_ERR, "call dsmi_hot_reset_atomic failed. ret is %d.", ret);
    }
#else
    ret = dsmi_hot_reset_soc(device_id);
    if (ret != DSMI_OK) {
        gplog(LOG_ERR, "call dsmi_hot_reset_soc failed. ret is %d.", ret);
    }
#endif
    else {
        gplog(LOG_OP, "Resetting all npu successfully.");
    }

    return dcmi_convert_error_code(ret);
}

#define MAX_LINE_LENGTH 256
void dcmi_npu_msn_env_clean(int cardId)
{
    int ret, logicId = 0;
    char clean_cmd[MAX_LINE_LENGTH] = {0};
    dcmi_get_device_logic_id(&logicId, cardId, 0);
    if (cardId == -1) {
        // kill 所有 msn日志传输进程
        ret = sprintf_s(clean_cmd, MAX_LINE_LENGTH,
        "kill -9 $(ps -ef | awk '/msnpureport report --permanent -d/&&!/awk/{print $2}')"
        " > /dev/null 2>&1");
    } else {
        // kill 一个 msn日志传输进程
        ret = dcmi_get_device_logic_id(&logicId, cardId, 0); // 仅使用die0去拉起msn日志传输进程，故只清除die0相关进程
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", ret);
            return;
        }
        ret = sprintf_s(clean_cmd, MAX_LINE_LENGTH,
        "kill -9 $(ps -ef | awk '/msnpureport report --permanent -d %d/&&!/awk/{print $2}')"
        " > /dev/null 2>&1", logicId);
    }
    
    if (ret < 0) {
        gplog(LOG_ERR, "Call sprintf_s failed. ret=%d", ret);
        return;
    }
 
    ret = system(clean_cmd);
    if (ret == -1) {
        gplog(LOG_ERR, "Run clean_cmd failed.");
        return;
    }
    return;
}

int dcmi_reset_brother_card(int card_id, int device_id, int device_logic_id)
{
    int ret ;
    int brother_card_id;
    int master_logic_id, slaver_logic_id, brother_master_logic_id, brother_slave_logic_id;
    ret = dcmi_get_netdev_brother_device(card_id, device_id, &brother_card_id);
    if (ret != 0) {
        gplog(LOG_ERR, "get_net_dev_brother_device failed. err is %d.", ret);
        return ret;
    }
    ret = dcmi_get_device_logic_id(&master_logic_id, card_id, 0);
    ret |= dcmi_get_device_logic_id(&slaver_logic_id, card_id, 1);
    if (brother_card_id != -1) {
        ret |= dcmi_get_device_logic_id(&brother_master_logic_id, brother_card_id, 0);
        ret |= dcmi_get_device_logic_id(&brother_slave_logic_id, brother_card_id, 1);
    } 
    if (ret != 0) {
        gplog(LOG_ERR, "get_logicid failed. err is %d. card_id=%d, brother_card_id=%d", ret, card_id, brother_card_id);
        return DCMI_ERR_CODE_INNER_ERR;
    }    
    /* 依次判断4个device是否可以复位 */
    ret = dsmi_hot_reset_atomic(master_logic_id, DSMI_SUBCMD_HOTRESET_SETFLAG);
    if (ret != DSMI_OK) {
        gplog(LOG_ERR, "call dsmi_hot_reset_atomic failed. err is %d.master_logic_id =%d", ret, master_logic_id);
        return dcmi_convert_error_code(ret);        
    }
    ret = dsmi_hot_reset_atomic(slaver_logic_id, DSMI_SUBCMD_HOTRESET_SETFLAG);
    if (ret != DSMI_OK) {
        dsmi_hot_reset_atomic(master_logic_id, DSMI_SUBCMD_HOTRESET_CLEARFLAG);
        gplog(LOG_ERR, "call dsmi_hot_reset_atomic failed. err is %d.slaver_logic_id =%d", ret, slaver_logic_id);
        return dcmi_convert_error_code(ret);        
    }
    if (brother_card_id != -1) {
        ret = dsmi_hot_reset_atomic(brother_master_logic_id, DSMI_SUBCMD_HOTRESET_SETFLAG);
        if (ret != DSMI_OK) {
            dsmi_hot_reset_atomic(master_logic_id, DSMI_SUBCMD_HOTRESET_CLEARFLAG);
            dsmi_hot_reset_atomic(slaver_logic_id, DSMI_SUBCMD_HOTRESET_CLEARFLAG);                
            gplog(LOG_ERR, "call dsmi_hot_reset_atomic failed. err is %d.brother_master_logic_id =%d", ret, brother_master_logic_id);
            return dcmi_convert_error_code(ret);            
        }
        ret = dsmi_hot_reset_atomic(brother_slave_logic_id, DSMI_SUBCMD_HOTRESET_SETFLAG);
        if (ret != DSMI_OK) {
            dsmi_hot_reset_atomic(master_logic_id, DSMI_SUBCMD_HOTRESET_CLEARFLAG);
            dsmi_hot_reset_atomic(slaver_logic_id, DSMI_SUBCMD_HOTRESET_CLEARFLAG);
            dsmi_hot_reset_atomic(brother_master_logic_id, DSMI_SUBCMD_HOTRESET_CLEARFLAG);               
            gplog(LOG_ERR, "call dsmi_hot_reset_atomic failed. err is %d.brother_slave_logic_id =%d", ret, brother_slave_logic_id);
            return dcmi_convert_error_code(ret);            
        }        
    }
    dcmi_npu_msn_env_clean(card_id);
    dsmi_hot_reset_atomic(master_logic_id, DSMI_SUBCMD_HOTRESET_UNBIND);
    dsmi_hot_reset_atomic(slaver_logic_id, DSMI_SUBCMD_HOTRESET_UNBIND);
    if (brother_card_id != -1) {
        dcmi_npu_msn_env_clean(brother_card_id);        
        dsmi_hot_reset_atomic(brother_master_logic_id, DSMI_SUBCMD_HOTRESET_UNBIND);
        dsmi_hot_reset_atomic(brother_slave_logic_id, DSMI_SUBCMD_HOTRESET_UNBIND);        
    }   
    dsmi_hot_reset_atomic(slaver_logic_id, DSMI_SUBCMD_HOTRESET_REMOVE);
    if (brother_card_id != -1) {    
        dsmi_hot_reset_atomic(brother_slave_logic_id, DSMI_SUBCMD_HOTRESET_REMOVE);
    }
    dsmi_hot_reset_atomic(master_logic_id, DSMI_SUBCMD_HOTRESET_RESET);

    if (brother_card_id != -1) {    
        dsmi_hot_reset_atomic(brother_master_logic_id, DSMI_SUBCMD_HOTRESET_RESET);
    }
    /* 兄弟卡复位多增加2秒 */
    sleep(2);
    dsmi_hot_reset_atomic(master_logic_id, DSMI_SUBCMD_HOTRESET_REMOVE);
    if (brother_card_id != -1) {    
        dsmi_hot_reset_atomic(brother_master_logic_id, DSMI_SUBCMD_HOTRESET_REMOVE);
    }

    dsmi_hot_reset_atomic(slaver_logic_id, DSMI_SUBCMD_HOTRESET_RESCAN);
    dsmi_hot_reset_atomic(master_logic_id, DSMI_SUBCMD_HOTRESET_RESCAN);
    if (brother_card_id != -1) {
        dsmi_hot_reset_atomic(brother_slave_logic_id, DSMI_SUBCMD_HOTRESET_RESCAN);         
        dsmi_hot_reset_atomic(brother_master_logic_id, DSMI_SUBCMD_HOTRESET_RESCAN);
    } 
    return 0;
}

int hccs_reset_all()
{
    int ret;
    // -1表示 关闭所有msn日志传输进程
    dcmi_npu_msn_env_clean(-1);
    ret = dcmi_set_all_npu_hot_reset();
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "Reset all device failed. err is %d.", ret);
        return ret;
    }
    return DCMI_OK;
}

STATIC int dcmi_get_hccs_status_inband(int card_id, int device_id, int *hccs_status)
{
    int ret;
    
    if (hccs_status == NULL) {
        gplog(LOG_ERR, "Input para error.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if (card_id == ALL_DEVICE_RESET_CARD_ID) {
        if (device_id < 0 || device_id > DCMI_DEVICE_MAX_NUM) {
            gplog(LOG_ERR, "This device_id is not supported in this scenario.");
            return DCMI_ERR_CODE_INVALID_PARAMETER;
        }

        if (!(dcmi_board_chip_type_is_ascend_910_93() && dcmi_is_in_phy_machine_root())) {
            gplog(LOG_ERR, "This card_id is not supported in this scenario.");
            return DCMI_ERR_CODE_INVALID_PARAMETER;
        }
        *hccs_status = HCCS_ON;
        return DCMI_OK;
    }
    
    if (dcmi_board_chip_type_is_ascend_910b()) {
        ret = dcmi_get_hccs_status(card_id, device_id, hccs_status);
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "Get hccs status fail. err is %d.", ret);
            return ret;
        }
    } else if (dcmi_board_chip_type_is_ascend_910_93()) {
        // 910_93不支持虚拟机内热复位
        if (!dcmi_is_in_phy_machine() && !dcmi_check_run_in_privileged_docker()) {
            gplog(LOG_OP, "VMs or dockers users are not support in-band reset.");
            return DCMI_ERR_CODE_NOT_SUPPORT;
        }
        *hccs_status = HCCS_OFF;
    }
 
    return DCMI_OK;
}

void dcmi_smp_rst_clr_all(int *dev_list, int dev_cnt)
{
    for (int i = 0; i < dev_cnt; i++) {
        if (dev_list[i] != DCMI_SMP_INVALID_ID) {
            (void)dsmi_hot_reset_atomic(dev_list[i], DSMI_SUBCMD_HOTRESET_CLEARFLAG);
        }
    }
}

int dcmi_smp_rst_btch_ops(int *dev_list, int dev_cnt, int sub_cmd, char *cmd_str)
{
    int ret;

    for (int i = 0; i < dev_cnt; i++) {
        if (dev_list[i] != DCMI_SMP_INVALID_ID) {
            ret = dsmi_hot_reset_atomic(dev_list[i], sub_cmd);
            if (ret != DSMI_OK) {
                gplog(LOG_ERR, "call %s failed. err is %d, dev_id=%d", cmd_str, ret, dev_list[i]);
                return ret;
            }
        }
    }

    return DSMI_OK;
}

int dcmi_smp_rst_prepare(int master_card_id, int *dev_list)
{
    int ret;
    int master_logic_id, slaver_logic_id1, slaver_logic_id2, slaver_logic_id3;

    ret = dcmi_get_device_logic_id(&master_logic_id, master_card_id, 0);
    ret |= dcmi_get_device_logic_id(&slaver_logic_id1, master_card_id + DCMI_SMP_SLAVER_NUM0, 0);
    ret |= dcmi_get_device_logic_id(&slaver_logic_id2, master_card_id + DCMI_SMP_SLAVER_NUM2, 0);
    ret |= dcmi_get_device_logic_id(&slaver_logic_id3, master_card_id + DCMI_SMP_SLAVER_NUM2, 0);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "get_logic_id failed. err is %d, master_card_id=%d", ret, master_card_id);
        return ret;
    }
   /* 依次判断4个device是否可以复位 */
    ret = dsmi_hot_reset_atomic(slaver_logic_id1, DSMI_SUBCMD_HOTRESET_SETFLAG);
    if (ret != DSMI_OK) {
        gplog(LOG_ERR, "call set flg failed. err is %d, slaver_logic_id1=%d", ret, slaver_logic_id1);
        return ret;
    }
    dev_list[DCMI_SMP_SLAVER_NUM0] = slaver_logic_id1;

    ret = dsmi_hot_reset_atomic(slaver_logic_id2, DSMI_SUBCMD_HOTRESET_SETFLAG);
    if (ret != DSMI_OK) {
        (void)dsmi_hot_reset_atomic(slaver_logic_id1, DSMI_SUBCMD_HOTRESET_CLEARFLAG);
        gplog(LOG_ERR, "call set flg failed. err is %d, slaver_logic_id2=%d", ret, slaver_logic_id2);
        return ret;
    }
    dev_list[DCMI_SMP_SLAVER_NUM1] = slaver_logic_id2;

    ret = dsmi_hot_reset_atomic(slaver_logic_id3, DSMI_SUBCMD_HOTRESET_SETFLAG);
    if (ret != DSMI_OK) {
        dcmi_smp_rst_clr_all(dev_list, DCMI_SMP_DEVICE_NUMBER);
        gplog(LOG_ERR, "call set flg failed. err is %d, slaver_logic_id3=%d", ret, slaver_logic_id3);
        return ret;
    }
    dev_list[DCMI_SMP_SLAVER_NUM2] = slaver_logic_id3;    

    ret = dsmi_hot_reset_atomic(master_logic_id, DSMI_SUBCMD_HOTRESET_SETFLAG);
    if (ret != DSMI_OK) {
        dcmi_smp_rst_clr_all(dev_list, DCMI_SMP_DEVICE_NUMBER);
        gplog(LOG_ERR, "call set flg failed. err is %d, master_card_id=%d", ret, master_card_id);
        return ret;
    }
    dev_list[DCMI_SMP_MASTER_NUM] = master_logic_id;

    return DSMI_OK;
}

int dcmi_reset_smp_card(int card_id, int device_id)
{
    int ret;
    int master_card_id, master_logic_id;
    int dev_list[DCMI_SMP_DEVICE_NUMBER] = {DCMI_SMP_INVALID_ID};

    gplog(LOG_OP, "dcmi_reset_smp_card card_id=%d device_id=%d", card_id, device_id);

    master_card_id = (card_id >= DCMI_SMP_MASTER_CARD_ID2) ? DCMI_SMP_MASTER_CARD_ID2 : DCMI_SMP_MASTER_CARD_ID1;
    ret = dcmi_smp_rst_prepare(master_card_id, dev_list);
    if (ret != DSMI_OK) {
        gplog(LOG_ERR, "dcmi_smp_rst_prepare failed. err is %d, master_card_id=%d", ret, master_card_id);
        return dcmi_convert_error_code(ret);
    }

    dcmi_npu_msn_env_clean(master_card_id);

    ret = dcmi_smp_rst_btch_ops(dev_list, DCMI_SMP_DEVICE_NUMBER, DSMI_SUBCMD_HOTRESET_UNBIND, "unbind");
    if (ret != DSMI_OK) {
        dcmi_smp_rst_clr_all(dev_list, DCMI_SMP_DEVICE_NUMBER);
        return dcmi_convert_error_code(ret);
    }

    /*先remove从设备*/
    master_logic_id = dev_list[DCMI_SMP_MASTER_NUM];
    dev_list[DCMI_SMP_MASTER_NUM] = DCMI_SMP_INVALID_ID;
    ret = dcmi_smp_rst_btch_ops(dev_list, DCMI_SMP_DEVICE_NUMBER, DSMI_SUBCMD_HOTRESET_REMOVE, "remove");
    dev_list[DCMI_SMP_MASTER_NUM] = master_logic_id;
    if (ret != DSMI_OK) {
        dcmi_smp_rst_clr_all(dev_list, DCMI_SMP_DEVICE_NUMBER);
        return dcmi_convert_error_code(ret);
    }

    /*复位主设备后再remove*/
    ret = dsmi_hot_reset_atomic(master_logic_id, DSMI_SUBCMD_HOTRESET_RESET);
    if (ret != DSMI_OK) {
        dcmi_smp_rst_clr_all(dev_list, DCMI_SMP_DEVICE_NUMBER);
        gplog(LOG_ERR, "call reset failed. err is %d, master_logic_id=%d", ret, master_logic_id);
        return dcmi_convert_error_code(ret);
    }

    (void)dsmi_hot_reset_atomic(master_logic_id, DSMI_SUBCMD_HOTRESET_REMOVE);

    for (int i = 0; i < DCMI_SMP_DEVICE_NUMBER; i++) {
        (void)dsmi_hot_reset_atomic(dev_list[i], DSMI_SUBCMD_HOTRESET_RESCAN);
    }

    gplog(LOG_OP, "dcmi_reset_smp_card reset successfully! card_id=%d device_id=%d", card_id, device_id);
    return DSMI_OK;
}

int dcmi_set_npu_device_reset_inband(int card_id, int device_id)
{
    int ret, device_logic_id = 0, hccs_status = HCCS_OFF;

    if (dcmi_board_type_is_card() != TRUE && dcmi_board_type_is_server() != TRUE &&
        dcmi_board_type_is_model() != TRUE) {
        gplog(LOG_OP, "The device does not support in-band reset. card_id=%d device_id=%d", card_id, device_id);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    ret = dcmi_get_hccs_status_inband(card_id, device_id, &hccs_status);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_hccs_status_inband failed. err is %d.", ret);
        return ret;
    }

    if (card_id != ALL_DEVICE_RESET_CARD_ID) {
        ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", ret);
            return ret;
        }
    }

    /* hccs互联，走全片复位 */
    if (hccs_status == HCCS_ON) {
        return hccs_reset_all();
    }

    if (dcmi_board_chip_type_is_ascend_910_93()) {
        return dcmi_reset_brother_card(card_id, device_id, device_logic_id);
    }

    if (dcmi_board_chip_type_is_ascend_910()) {
        return dcmi_reset_smp_card(card_id, device_id);
    }

    dcmi_npu_msn_env_clean(card_id);
#ifndef _WIN32
    ret = dsmi_hot_reset_atomic(device_logic_id, DSMI_SUBCMD_HOTRESET_ASSEMBLE);
    if (ret != DSMI_OK) {
        gplog(LOG_ERR, "call dsmi_hot_reset_atomic failed. err is %d.", ret);
    }
#else
    ret = dsmi_hot_reset_soc(device_logic_id);
    if (ret != DSMI_OK) {
        gplog(LOG_ERR, "call dsmi_hot_reset_soc failed. err is %d.", ret);
    }
#endif
    else {
        gplog(LOG_OP, "Resetting npu successfully by inband. card_id=%d, device_id=%d.", card_id, device_id);
    }

    return dcmi_convert_error_code(ret);
}

int dcmi_get_npu_device_outband_id(int card_id, int device_id, int *outband_id)
{
    int chip_slot = 0;
    int ret;
    int dmpa_outband_id_table[MAX_DEVICE_IN_ALL_TYPE_CAR] = {0, 2, 1, 3, 0};
    int dmpb_outband_id_table[MAX_DEVICE_IN_ALL_TYPE_CAR] = {3, 2, 1, 0, 0};

    switch (dcmi_get_board_id_inner()) {
        case DCMI_310_CARD_B_BOARD_ID:
        case DCMI_310_CARD_C_BOARD_ID:
        case DCMI_310_CARD_D_BOARD_ID:
            ret = dcmi_get_device_chip_slot(card_id, device_id, &chip_slot);
            if (ret != DCMI_OK) {
                gplog(LOG_ERR, "call dcmi_get_chip_slot failed. err is %d.", ret);
                return ret;
            }

            if ((chip_slot < 0) || (chip_slot >= MAX_DEVICE_NUM_IN_CARD)) {
                gplog(LOG_ERR, "card(%d) device(%d) get invalid slot id(%d).", card_id, device_id, chip_slot);
                return DCMI_ERR_CODE_INNER_ERR;
            }

            *outband_id = dmpa_outband_id_table[chip_slot];
            break;
        case DCMI_310_CARD_DMPB_BOARD_ID:
            ret = dcmi_get_device_chip_slot(card_id, device_id, &chip_slot);
            if (ret != DCMI_OK) {
                gplog(LOG_ERR, "call dcmi_get_chip_slot failed. err is %d.", ret);
                return ret;
            }

            if (chip_slot >= MAX_DEVICE_NUM_IN_CARD) {
                gplog(LOG_ERR, "card(%d) device(%d) get invalid slot id(%d).", card_id, device_id, chip_slot);
                return DCMI_ERR_CODE_INNER_ERR;
            }

            *outband_id = dmpb_outband_id_table[chip_slot];
            break;

        default:
            /* 其他默认的保持device id，但是后续新单板建议在带外MCU侧就使用slot ID */
            *outband_id = device_id;
            break;
    }

    return DCMI_OK;
}

static int dcmi_set_npu_device_reset(int card_id, int device_id, int outband_id, int pcie_slot_id)
{
    unsigned char reset_state = 0x01; /* 默认是复位失败 */
    int retry_times;
    int set_ret;
    int get_ret = 0;
    for (retry_times = 0; retry_times < MAX_RETRY_CNT; retry_times++) {
        set_ret = dcmi_ipmi_reset_npu(pcie_slot_id, outband_id);
        if (set_ret == DCMI_OK) {
            get_ret = dcmi_ipmi_get_npu_reset_state(pcie_slot_id, outband_id, &reset_state);
            if (get_ret == DCMI_OK && reset_state == BMC_RESET_CHIP_SUCCESS) {
                break;
            }
        }

        gplog(LOG_INFO, "ipmi reset retry. card_id=%d, device_id=%d, set_ret=%d, get_ret=%d, reset_state=%u", card_id,
            device_id, set_ret, get_ret, reset_state);
        sleep(DCMI_RESET_MIN_DELAY); // 频繁复位会导致概率丢芯片
    }

    if (retry_times == MAX_RETRY_CNT) {
        gplog(LOG_ERR, "ipmi reset failed. card_id=%d, device_id=%d, set_ret=%d, get_ret=%d, reset_state=%d", card_id,
            device_id, set_ret, get_ret, reset_state);
        return DCMI_ERR_CODE_INNER_ERR;
    }
    return DCMI_OK;
}

int dcmi_set_npu_device_reset_910_93(int card_id)
{
    unsigned char reset_state = 0x01; /* 默认是复位失败 */
    int retry_times;
    int set_ret;
    for (retry_times = 0; retry_times < MAX_RETRY_CNT; retry_times++) {
        set_ret = dcmi_ipmi_reset_npu_910_93(card_id);
        if (set_ret == DCMI_OK) {
            break;
        }

        gplog(LOG_ERR, "ipmi reset failed. card_id=%d, set_ret=%d, reset_state=%d", card_id, set_ret, reset_state);
        sleep(DCMI_RESET_MIN_DELAY); // 频繁复位会导致概率丢芯片
    }

    if (retry_times == MAX_RETRY_CNT) {
        gplog(LOG_ERR, "ipmi reset failed. card_id=%d, set_ret=%d, reset_state=%d", card_id, set_ret, reset_state);
        return DCMI_ERR_CODE_INNER_ERR;
    }
    return DCMI_OK;
}

int dcmi_set_double_npu_device_reset(int card_id)
{
    int brother_card_id = -1;
    int ret;

    dcmi_npu_msn_env_clean(card_id);
    ret = dcmi_set_npu_device_reset_910_93(card_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_set_npu_device_reset_910_93 failed, card_id=%d. err is %d.", card_id, ret);
        return ret;
    }

    dcmi_get_brother_card_id(card_id, &brother_card_id);

    if (brother_card_id != -1) {
        dcmi_npu_msn_env_clean(brother_card_id);
        ret = dcmi_set_npu_device_reset_910_93(brother_card_id);
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "call dcmi_set_npu_device_reset_910_93 failed, card_id=%d. err is %d.", brother_card_id,
                ret);
            return ret;
        }
        gplog(LOG_OP, "call dcmi_set_npu_device_reset_910_93 success, card_id=%d.", brother_card_id);
    }

    if (ret == DCMI_OK) {
        sleep(RESET_WAIT_SECOND); // 带外热复位后需等待npu建链
    }

    return ret;
}

int dcmi_set_npu_device_reset_outband(int card_id, int device_id)
{
#ifdef _WIN32
    return DCMI_ERR_CODE_NOT_SUPPORT;
#else
    int ret, pcie_slot_id;
    int outband_id = device_id;
    unsigned int main_board_id = 0;

    main_board_id = dcmi_get_maindboard_id_inner();
    if (dcmi_board_type_is_card() != TRUE && !dcmi_mainboard_is_arm_910_93(main_board_id)) {
        gplog(LOG_OP, "The device does not support outband reset. card_id=%d, device_id=%d, main_board_id=%u", card_id,
            device_id, main_board_id);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    // 复位芯片,这里返回值只是表示BMC收到命令，并不代表动作执行成功
    if (dcmi_mainboard_is_arm_910_93(main_board_id)) {
        ret = dcmi_set_double_npu_device_reset(card_id);
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "Call dcmi_set_double_npu_device_reset failed. err is %d.", ret);
            return ret;
        }
        return ret;
    }

    if (dcmi_board_chip_type_is_ascend_310()) {
        ret = dcmi_get_npu_device_outband_id(card_id, device_id, &outband_id);
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "Call dcmi_get_device_outband_id failed. err is %d.", ret);
            return ret;
        }
    }
    // 获取pcie卡的槽位号
    ret = dcmi_get_pcie_slot(card_id, &pcie_slot_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "Call dcmi_get_pcie_slot failed. err is %d.", ret);
        return ret;
    }

    dcmi_npu_msn_env_clean(card_id);
    ret = dcmi_set_npu_device_reset(card_id, device_id, outband_id, pcie_slot_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "Call dcmi_set_npu_device_reset failed. err is %d.", ret);
        return ret;
    }

    gplog(LOG_INFO, "call dcmi_reset_device OK.card=%d chip=%d.", card_id, device_id);
    return DCMI_OK;
#endif
}