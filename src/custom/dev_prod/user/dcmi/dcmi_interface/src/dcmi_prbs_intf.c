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
#include <unistd.h>
 
#include "securec.h"
#include "dsmi_common_interface_custom.h"
#include "dsmi_network_interface.h"
#include "dcmi_interface_api.h"
#include "dcmi_init_basic.h"
#include "dcmi_fault_manage_intf.h"
#include "dcmi_log.h"
#include "dcmi_product_judge.h"
#include "dcmi_environment_judge.h"
#include "dcmi_prbs_intf.h"


#if defined DCMI_VERSION_1

int check_prbs_operate_para(DCMI_PRBS_OPERATE_PARAM operate_para, void *buf)
{
    unsigned int serdes_prbs_type = SERDES_PRBS_TYPE_MAX;
    unsigned int serdes_prbs_direction = SERDES_PRBS_DIRECTION_MAX;
    unsigned int serdes_prbs_macro_id, serdes_prbs_start_lane_id, serdes_prbs_lane_count;

    if ((operate_para.sub_cmd >= SERDES_PRBS_SUB_CMD_MAX) || (operate_para.main_cmd >= DSMI_SERDES_CMD_MAX)) {
        gplog(LOG_ERR, "dcmi_prbs_operate prams is invalid, sub_cmd=[%u], main_cmd=[%u]",
            operate_para.sub_cmd, operate_para.main_cmd);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if (operate_para.sub_cmd == SERDES_PRBS_SET_CMD) {
        serdes_prbs_type = operate_para.operate_para.set_param.serdes_prbs_type;
        serdes_prbs_direction = operate_para.operate_para.set_param.serdes_prbs_direction;
        serdes_prbs_macro_id = operate_para.operate_para.set_param.param_base.serdes_prbs_macro_id;
        serdes_prbs_start_lane_id = operate_para.operate_para.set_param.param_base.serdes_prbs_start_lane_id;
        serdes_prbs_lane_count = operate_para.operate_para.set_param.param_base.serdes_prbs_lane_count;
        if ((serdes_prbs_type != SERDES_PRBS_TYPE_END && serdes_prbs_type != SERDES_PRBS_TYPE_31) ||
            (serdes_prbs_direction >= SERDES_PRBS_DIRECTION_MAX)) {
            gplog(LOG_ERR, "serdes_prbs para is invalid, type=[%u], direction=[%u]",
                serdes_prbs_type, serdes_prbs_direction);
            return DCMI_ERR_CODE_INVALID_PARAMETER;
        }
    }

    if ((operate_para.sub_cmd == SERDES_PRBS_GET_RESULT_CMD) || (operate_para.sub_cmd == SERDES_PRBS_GET_STATUS_CMD)) {
        serdes_prbs_macro_id = operate_para.operate_para.get_param.serdes_prbs_macro_id;
        serdes_prbs_start_lane_id = operate_para.operate_para.get_param.serdes_prbs_start_lane_id;
        serdes_prbs_lane_count = operate_para.operate_para.get_param.serdes_prbs_lane_count;
    }

    // 当前仅支持对eth口进行prbs打流，910 A3天成/服务器产品 以及 A2产品eth口均为m0
    if (serdes_prbs_macro_id > NET_MACRO_ID_MAX) {
        gplog(LOG_ERR, "serdes_prbs_macro_id[%u] not support", serdes_prbs_macro_id);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if ((serdes_prbs_start_lane_id >= MACRO_LANE_MAX_NUM) || (serdes_prbs_lane_count > MACRO_LANE_MAX_NUM) ||
        (serdes_prbs_lane_count == 0) || ((serdes_prbs_start_lane_id + serdes_prbs_lane_count) > MACRO_LANE_MAX_NUM)) {
        gplog(LOG_ERR, "operate_para is invalid, SET_CMD=[%u], direc=[%u], m_id=[%u], start_id=[%u], count=[%u]",
            serdes_prbs_type, serdes_prbs_direction, serdes_prbs_macro_id,
            serdes_prbs_start_lane_id, serdes_prbs_lane_count);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if (buf == NULL) {
        gplog(LOG_ERR, "dcmi_prbs operate_result is NULL");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    return 0;
}

int check_cdr_exist(unsigned int device_logic_id, unsigned int mainboard_id, bool *if_contain_cdr_flag)
{
    int ret = 0;
    struct dsmi_board_info_stru board_info;
    // 判定是否存在cdr，若不存在则不进行cdr的操作
    if (dcmi_mainboard_is_910_93(mainboard_id)) {
        *if_contain_cdr_flag = true;              // 天成
    } else {
        ret = dsmi_get_board_info(device_logic_id, &board_info);
        if ((ret != DSMI_OK) && (ret != DSMI_ERR_NOT_SUPPORT)) {
            gplog(LOG_ERR, "call dsmi_get_board_info failed. err is %d.", ret);
            return DCMI_ERR_CODE_INVALID_PARAMETER;
        }
        if ((board_info.board_id & BOARD_ID_MASK) == A_X_BOARD_ID) {     // A+X 环境
            *if_contain_cdr_flag = true;     // 有cdr
        } else if ((board_info.board_id & BOARD_ID_MASK) == A_K_OR_POD_BOARD_ID) {      // A+K 或者POD
            if ((mainboard_id != POD_A_MAINBOARD_ID) && (mainboard_id != POD_C_MAINBOARD_ID)) {     // A+K
                *if_contain_cdr_flag = true;     // 有cdr
            }
        }
    }

    return ret;
}

int dcmi_prbs_tx_adapt_in_order(unsigned int mainboard_id, unsigned int device_logic_id,
    int convert_logic_id, unsigned char master_flag)
{
    bool is_cdr_exist = false;
    int ret = check_cdr_exist(device_logic_id, mainboard_id, &is_cdr_exist);
    if (ret != 0) {
        gplog(LOG_ERR, "check_cdr_exist failed, device_logic_id=[%u], ret=[%d]", device_logic_id, ret);
        return ret;
    }

    // CDR存在时，进行CDR自适应
    // CDR不存在时，直接进行serdes开关信号，触发光模块自适应
    if (is_cdr_exist == false) {
        return dsmi_prbs_adapt_in_order(PRBS_TX_SERDES_TRIGGER, device_logic_id, master_flag);
    }

    return dsmi_prbs_adapt_in_order(PRBS_TX_RETIMER_HOST_ADAPT, convert_logic_id, master_flag);
}

int dcmi_prbs_rx_adapt_in_order(unsigned int mainboard_id, int device_logic_id, int convert_logic_id,
    unsigned char master_flag)
{
#define PRBS_RX_SLEEP_TIME 3
    bool is_cdr_exist = false;
    int ret = check_cdr_exist(device_logic_id, mainboard_id, &is_cdr_exist);
    if (ret != 0) {
        gplog(LOG_ERR, "check_cdr_exist failed, device_logic_id=[%d], ret=[%d]", device_logic_id, ret);
        return ret;
    }

    if (is_cdr_exist == false) {
        gplog(LOG_INFO, "this device has no cdr, do serdes adapt, device_logic_id=[%d], convert_logic_id=[%d]ret=[%d]",
            device_logic_id, convert_logic_id, ret);
        goto serdes_adapt;
    }

    // cdr自适应（对于主从die互助款型，主die帮助从die做）
    ret = dsmi_prbs_adapt_in_order(PRBS_RX_RETIMER_MEDIA_ADAPT, convert_logic_id, master_flag);
    if (ret != 0) {
        gplog(LOG_ERR, "dsmi_prbs_rx_retimer_media_adapt failed, device_logic_id=[%d], convert_logic_id=[%d, ]ret=[%d]",
            device_logic_id, convert_logic_id, ret);
        return ret;
    }

    sleep(PRBS_RX_SLEEP_TIME);
serdes_adapt:
    // npu serdes自适应
    return dsmi_prbs_adapt_in_order(PRBS_RX_SERDES_MEDIA_ADAPT, device_logic_id, master_flag);
}

STATIC int dcmi_a900_a3_convert_logic_id(unsigned int logic_id, unsigned int mainboard_id,
    unsigned int *convert_logic_id, unsigned char *master_flag)
{
    int ret = 0;
    unsigned int phy_id = 0;

    if (!dcmi_mainboard_is_910_93(mainboard_id)) {
        *master_flag = true;
        *convert_logic_id = logic_id;
        return 0;
    }

    ret = dsmi_get_phyid_from_logicid(logic_id, &phy_id);
    if (ret) {
        gplog(LOG_ERR, "get phyid failed, logic_id=[%u]", logic_id);
        return ret;
    }

    *convert_logic_id = dcmi_910_93_logic_id_convert(phy_id);
    if (*convert_logic_id == INVALID_LOGIC_ID) {
        return DCMI_ERR_CODE_INNER_ERR;
    }
    *master_flag = dcmi_910_93_phy_id_convert(phy_id) == (int)phy_id ? 1 : 0;

    return ret;
}

STATIC int dcmi_set_cdr_mode_operate(int device_logic_id, int port, unsigned char mode, unsigned int mainboard_id)
{
    int ret = 0;
    unsigned int convert_logic_id = (unsigned int)device_logic_id;
    int retry_count = DCMI_PRBS_RETRY_TIMES;
    struct ds_cdr_mode_info cdr_mode_info = {0};

    cdr_mode_info.master_flag = 1;
    cdr_mode_info.mode = mode;

    ret = dcmi_a900_a3_convert_logic_id(device_logic_id, mainboard_id, &convert_logic_id, &(cdr_mode_info.master_flag));
    if (ret) {
        gplog(LOG_ERR, "get phyid failed, device_logic_id=[%d]", device_logic_id);
        return ret;
    }

    while (retry_count--) {
        ret = dsmi_set_cdr_mode_cmd(convert_logic_id, port, &cdr_mode_info);
        if (ret == DSMI_ERR_WAIT_TIMEOUT) {
            gplog(LOG_INFO, "cdr i2c is busy, retry, device_logic_id=[%d], mode=[%d], port=[%d], ret=[%d], count=[%d]",
                  device_logic_id, mode, port, ret, retry_count);
            usleep(DCMI_PRBS_RETRY_WATI_TIME);
            continue;
        }

        if (ret != 0 && ret != EOPNOTSUPP) {
            gplog(LOG_ERR, "cdr adaptive set failed, device_logic_id=[%d], mode=[%d], port=[%d], ret=[%d], count=[%d]",
                device_logic_id, mode, port, ret, retry_count);
            return ret;
        } else {
            break;
        }
    }

    return ret;
}

STATIC int dcmi_prbs_set_open_adapt(int device_logic_id, unsigned int mainboard_id,
                                    DCMI_PRBS_OPERATE_PARAM operate_para)
{
    int ret;
    unsigned int convert_logic_id;
    bool if_contain_cdr_flag = false;
    unsigned char master_flag = true;

    if (device_logic_id < 0) {
        gplog(LOG_ERR, "npu adaptive open failed, device_logic_id=[%d], is invalid", device_logic_id);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }
    convert_logic_id = (unsigned int)device_logic_id;

    ret = check_cdr_exist(device_logic_id, mainboard_id, &if_contain_cdr_flag);
    if (ret != 0) {
        gplog(LOG_ERR, "check_cdr_exist failed, device_logic_id=[%d], ret=[%d]", device_logic_id, ret);
        return ret;
    }

    // 先打开npu自适应
    ret = dsmi_set_prbs_flag(device_logic_id, DCMI_SERDES_NPU_ADAPTIVE_OPEN);
    if (ret != 0) {
        gplog(LOG_ERR, "npu adaptive open failed, device_logic_id=[%d], ret=[%d]\n", device_logic_id, ret);
        return ret;
    }

    // 不存在cdr的款型不需要做后续操作
    if (!if_contain_cdr_flag) {
        return DCMI_OK;
    }

    // 再打开cdr自适应
    ret = dcmi_set_cdr_mode_operate(device_logic_id, 0, 1, mainboard_id);
    if (ret != 0 && ret != EOPNOTSUPP) {
        gplog(LOG_ERR, "cdr adaptive open failed, device_logic_id=[%d], ret=[%d]\n", device_logic_id, ret);
        return ret;
    }

    // 对于存在主从die网口互助款型，需要对从die的logicid做一次转换
    ret = dcmi_a900_a3_convert_logic_id(device_logic_id, mainboard_id, &convert_logic_id, &master_flag);
    if (ret != 0) {
        gplog(LOG_ERR, "dcmi_a900_a3_convert_logic_id failed, logic_id=[%d], main_cmd=[%u], sub_cmd=[%u] ret=[%d]",
            device_logic_id, operate_para.main_cmd, operate_para.sub_cmd, ret);
        return ret;
    }

    // 开启cdr信号
    ret = dsmi_prbs_adapt_in_order(PRBS_RETIMER_TX_ENABLE, convert_logic_id, master_flag);
    if (ret != 0) {
        gplog(LOG_ERR, "cdr tx enable failed, device_logic_id=[%d], ret=[%d]\n", device_logic_id, ret);
        return ret;
    }

    return DCMI_OK;
}

STATIC int dcmi_prbs_set_close_adapt(int device_logic_id, unsigned int mainboard_id,
                                     DCMI_PRBS_OPERATE_PARAM operate_para)
{
    int ret;
    bool if_contain_cdr_flag = false;
    ret = check_cdr_exist(device_logic_id, mainboard_id, &if_contain_cdr_flag);
    if (ret != 0) {
        gplog(LOG_ERR, "check_cdr_exist failed, device_logic_id=[%d], ret=[%d]", device_logic_id, ret);
        return ret;
    }

    // 先关闭cdr自适应
    if (if_contain_cdr_flag) {
        ret = dcmi_set_cdr_mode_operate(device_logic_id, 0, 0, mainboard_id);
        if (ret != 0 && ret != EOPNOTSUPP) {
            gplog(LOG_ERR, "cdr adaptive close failed, device_logic_id=[%d], ret=[%d]\n", device_logic_id, ret);
            return ret;
        }
    }

    // 再关闭npu自适应
    ret = dsmi_set_prbs_flag(device_logic_id, DCMI_SERDES_NPU_ADAPTIVE_CLOSE);
    if (ret != 0) {
        gplog(LOG_ERR, "npu adaptive close failed, device_logic_id=[%d], ret=[%d]\n", device_logic_id, ret);
        (void)dcmi_set_cdr_mode_operate(device_logic_id, 0, 1, mainboard_id);
        return ret;
    }
    sleep(5); // 等待5秒，让自适应完全关闭

    return DCMI_OK;
}

STATIC int dcmi_prbs_open_operate(int device_logic_id, unsigned int mainboard_id, DCMI_PRBS_OPERATE_PARAM operate_para,
                                  void *buf, unsigned int buf_size)
{
    unsigned int serdes_prbs_direction = operate_para.operate_para.set_param.serdes_prbs_direction;
    unsigned int convert_logic_id = (unsigned int)device_logic_id;
    unsigned char master_flag = true;

    // 关闭cdr和npu自适应
    int ret = dcmi_prbs_set_close_adapt(device_logic_id, mainboard_id, operate_para);
    if (ret != 0) {
        gplog(LOG_ERR, "dcmi_prbs_set_close_adapt failed, logic_id=[%d], main_cmd=[%u], sub_cmd=[%u] ret=[%d]",
            device_logic_id, operate_para.main_cmd, operate_para.sub_cmd, ret);
        return ret;
    }

    // 对于存在主从die网口互助款型，需要对从die的logicid做一次转换
    ret = dcmi_a900_a3_convert_logic_id(device_logic_id, mainboard_id, &convert_logic_id, &master_flag);
    if (ret != 0) {
        gplog(LOG_ERR, "dcmi_a900_a3_convert_logic_id failed, logic_id=[%d], main_cmd=[%u], sub_cmd=[%u] ret=[%d]",
            device_logic_id, operate_para.main_cmd, operate_para.sub_cmd, ret);
        return ret;
    }

    // TX: 先开启发送码流，再做自适应保序
    if (serdes_prbs_direction == SERDES_PRBS_DIRECTION_TX) {
        ret = dsmi_set_serdes_info(device_logic_id, operate_para.main_cmd, operate_para.sub_cmd, buf, buf_size);
        if (ret != 0) {
            gplog(LOG_ERR, "dsmi_set_serdes_info failed, logic_id=[%d], main_cmd=[%u], sub_cmd=[%u] ret=[%d]",
                device_logic_id, operate_para.main_cmd, operate_para.sub_cmd, ret);
            return ret;
        }

        ret = dcmi_prbs_tx_adapt_in_order(mainboard_id, device_logic_id, convert_logic_id, master_flag);
        if (ret != 0) {
            gplog(LOG_ERR, "dcmi_prbs_tx_adapt_in_order failed, logic_id=[%d], main_cmd=[%u], sub_cmd=[%u] ret=[%d]",
                device_logic_id, operate_para.main_cmd, operate_para.sub_cmd, ret);
            return ret;
        }
    }

    // RX：先做自适应保序，再开启接收码流
    if (serdes_prbs_direction == SERDES_PRBS_DIRECTION_RX) {
        ret = dcmi_prbs_rx_adapt_in_order(mainboard_id, device_logic_id, convert_logic_id, master_flag);
        if (ret != 0) {
            gplog(LOG_ERR, "dcmi_prbs_rx_adapt_in_order failed, logic_id=[%d], main_cmd=[%u], sub_cmd=[%u] ret=[%d]",
                device_logic_id, operate_para.main_cmd, operate_para.sub_cmd, ret);
            return ret;
        }

        ret = dsmi_set_serdes_info(device_logic_id, operate_para.main_cmd, operate_para.sub_cmd, buf, buf_size);
        if (ret != 0) {
            gplog(LOG_ERR, "dsmi_set_serdes_info failed, logic_id=[%d], main_cmd=[%u], sub_cmd=[%u] ret=[%d]",
                device_logic_id, operate_para.main_cmd, operate_para.sub_cmd, ret);
            return ret;
        }
    }

    return 0;
}

STATIC int dcmi_prbs_set_operate(int device_logic_id, unsigned int mainboard_id, DCMI_PRBS_OPERATE_PARAM operate_para,
                                 void *buf, unsigned int buf_size)
{
    int ret;
    unsigned int prbs_type = operate_para.operate_para.set_param.serdes_prbs_type;

    if (prbs_type == SERDES_PRBS_TYPE_END) {  // 关闭打流场景，需要先关闭打流再打开自适应
        // 关闭打流
        ret = dsmi_set_serdes_info(device_logic_id, operate_para.main_cmd, operate_para.sub_cmd, buf, buf_size);
        if (ret != 0) {
            gplog(LOG_ERR, "dsmi_set_serdes_info failed, logic_id=[%d], main_cmd=[%u], sub_cmd=[%u] ret=[%d]",
                device_logic_id, operate_para.main_cmd, operate_para.sub_cmd, ret);
            return ret;
        }

        // 开启cdr和npu自适应
        ret = dcmi_prbs_set_open_adapt(device_logic_id, mainboard_id, operate_para);
        if (ret != 0) {
            gplog(LOG_ERR, "dcmi_prbs_set_open_adapt failed, logic_id=[%d], main_cmd=[%u], sub_cmd=[%u] ret=[%d]",
                device_logic_id, operate_para.main_cmd, operate_para.sub_cmd, ret);
            return ret;
        }
    } else {    // 开启打流，需要先关闭自适应再开启打流
        return dcmi_prbs_open_operate(device_logic_id, mainboard_id, operate_para, buf, buf_size);
    }

    return 0;
}

int dcmi_prbs_get_dev_mainboard_id(int card_id, int device_id, int *device_logic_id, unsigned int *mainboard_id)
{
    int ret;
    ret = dcmi_get_device_logic_id(device_logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. card_id=[%d], device_id=[%d], ret=[%d]",
              card_id, device_id, ret);
        return ret;
    }

    ret = dcmi_get_mainboard_id(card_id, device_id, mainboard_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "Failed to query main board id of card. err is %d", ret);
        return ret;
    }
    gplog(LOG_INFO, "mainboard_id:%u", *mainboard_id);

    return 0;
}

int dcmi_prbs_operate(int card_id, int device_id, DCMI_PRBS_OPERATE_PARAM operate_para,
                      DCMI_PRBS_OPERATE_RESULT *operate_result)
{
    int ret;
    unsigned int mainboard_id = 0;
    int device_logic_id = 0;
    unsigned int buf_size = sizeof(DCMI_PRBS_OPERATE_RESULT);
    void *buf = (void *)operate_result;

    if (!(dcmi_is_in_privileged_docker_root() || dcmi_is_in_phy_machine_root() || dcmi_is_in_vm_root())) {
        gplog(LOG_OP, "Operation not permitted, only root user on physical or virtual machine"
            " or privileged docker can call this api.");
        return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
    }

    ret = check_prbs_operate_para(operate_para, buf);
    if (ret != DCMI_OK) {
        return ret;
    }

    ret = dcmi_prbs_get_dev_mainboard_id(card_id, device_id, &device_logic_id, &mainboard_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "Failed to query board id and main board id of card. err is %d", ret);
        return ret;
    }

    if (dcmi_mainboard_is_a9000_a3_superpod(mainboard_id) || (mainboard_id == POD_A_MAINBOARD_ID)
            || dcmi_board_chip_type_is_ascend_910b_300i_a2()) {
        gplog(LOG_ERR, "This device does not support prbs operate.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    ret = memcpy_s(buf, sizeof(DCMI_PRBS_OPERATE_RESULT), &(operate_para.operate_para),
                   sizeof(operate_para.operate_para));
    if (ret != 0) {
        gplog(LOG_ERR, "memcpy failed, ret=[%d]", ret);
        return DCMI_ERR_CODE_MEM_OPERATE_FAIL;
    }
    gplog(LOG_OP, "operate prbs, card_id:%d, device_id:%d, device_logic_id:%d\n", card_id, device_id, device_logic_id);
    if (operate_para.sub_cmd == SERDES_PRBS_SET_CMD) {
        ret = dcmi_prbs_set_operate(device_logic_id, mainboard_id, operate_para, buf, buf_size);
        if (ret != 0) {
            gplog(LOG_ERR, "dcmi_prbs_set_operate failed, logic_id=[%d], main_cmd=[%u], sub_cmd=[%u] ret=[%d]",
                device_logic_id, operate_para.main_cmd, operate_para.sub_cmd, ret);
            return dcmi_convert_error_code(ret);
        }
    } else {
        ret = dsmi_get_serdes_info(device_logic_id, operate_para.main_cmd, operate_para.sub_cmd, buf, &buf_size);
        if (ret != 0) {
            gplog(LOG_ERR, "dsmi_get_serdes_info failed, logic_id=[%d], main_cmd=[%u], sub_cmd=[%u] ret=[%d]",
                  device_logic_id, operate_para.main_cmd, operate_para.sub_cmd, ret);
            return dcmi_convert_error_code(ret);
        }
    }

    gplog(LOG_OP, "operate prbs success");
    return 0;
}
#endif