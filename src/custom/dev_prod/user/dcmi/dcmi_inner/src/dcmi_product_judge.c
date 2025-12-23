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
#include <stddef.h>
#include <time.h>
#ifndef _WIN32
#include <unistd.h>
#include <sys/mman.h>
#endif
#include "securec.h"
#include "dsmi_common_interface_custom.h"

#ifndef _WIN32
#include "ascend_hal.h"
#endif
#include "dcmi_fault_manage_intf.h"
#include "dcmi_log.h"
#include "dcmi_inner_info_get.h"
#include "dcmi_mcu_intf.h"
#include "dcmi_init_basic.h"
#include "dcmi_environment_judge.h"
#include "dcmi_product_judge.h"

int dcmi_board_type_is_card(void)
{
    return (dcmi_get_board_type() == DCMI_BOARD_TYPE_CARD) ? TRUE : FALSE;
}

int dcmi_board_type_is_station(void)
{
    return (dcmi_get_board_type() == DCMI_BOARD_TYPE_MODEL) && (dcmi_get_sub_board_type() == DCMI_BOARD_TYPE_MODEL_SEI);
}

int dcmi_board_type_is_hilens(void)
{
    return (dcmi_get_board_type() == DCMI_BOARD_TYPE_MODEL) &&
        (dcmi_get_sub_board_type() == DCMI_BOARD_TYPE_MODEL_HILENS);
}

int dcmi_board_type_is_server(void)
{
    return (dcmi_get_board_type() == DCMI_BOARD_TYPE_SERVER) && (dcmi_get_sub_board_type() == DCMI_BOARD_TYPE_SERVER);
}

int dcmi_board_type_is_model(void)
{
    if ((dcmi_get_board_type() == DCMI_BOARD_TYPE_MODEL) && (dcmi_get_sub_board_type() == DCMI_BOARD_TYPE_MODEL)) {
        return TRUE;
    }
    return FALSE;
}

int dcmi_board_type_is_soc(void)
{
    if ((dcmi_get_board_type() == DCMI_BOARD_TYPE_SOC) && (dcmi_get_sub_board_type() == DCMI_BOARD_TYPE_SOC_BASE)) {
        return TRUE;
    }
    return FALSE;
}

int dcmi_board_type_is_soc_develop(void)
{
    if ((dcmi_get_board_type() == DCMI_BOARD_TYPE_SOC) && (dcmi_get_sub_board_type() == DCMI_BOARD_TYPE_SOC_DEVELOP)) {
        return TRUE;
    }
    return FALSE;
}

int dcmi_board_chip_type_is_ascend_310(void)
{
    return (dcmi_get_board_chip_type() == DCMI_CHIP_TYPE_D310) ? TRUE : FALSE;
}

int dcmi_board_chip_type_is_ascend_310p(void)
{
    return (dcmi_get_board_chip_type() == DCMI_CHIP_TYPE_D310P) ? TRUE : FALSE;
}

int dcmi_board_chip_type_is_ascend_310p_200i_pro(void)
{
    return ((dcmi_get_board_chip_type() == DCMI_CHIP_TYPE_D310P) &&
    (dcmi_get_board_id_inner() == DCMI_310P_1P_XP_BOARD_ID)) ? TRUE : FALSE;
}

int dcmi_board_chip_type_is_ascend_310b(void)
{
    return (dcmi_get_board_chip_type() == DCMI_CHIP_TYPE_D310B) ? TRUE : FALSE;
}

int dcmi_board_chip_type_is_ascend_910(void)
{
    return (dcmi_get_board_chip_type() == DCMI_CHIP_TYPE_D910) ? TRUE : FALSE;
}

int dcmi_board_chip_type_is_ascend_910b(void)
{
    return (dcmi_get_board_chip_type() == DCMI_CHIP_TYPE_D910B) ? TRUE : FALSE;
}

int dcmi_board_chip_type_is_ascend_910_93(void)
{
    return (dcmi_get_board_chip_type() == DCMI_CHIP_TYPE_D910_93) ? TRUE : FALSE;
}

int dcmi_board_chip_type_is_ascend_910b_300i_a2(void)
{
    return ((dcmi_get_board_chip_type() == DCMI_CHIP_TYPE_D910B) &&
    ((dcmi_get_board_id_inner() == DCMI_A300I_A2_BIN2_BOARD_ID) ||
     (dcmi_get_board_id_inner() == DCMI_A300I_A2_BIN2_64G_BOARD_ID))) ? TRUE : FALSE;
}

int dcmi_board_chip_type_is_ascend_910b_200t_box_a2(void)
{
    return ((dcmi_get_board_chip_type() == DCMI_CHIP_TYPE_D910B) &&
        (dcmi_get_board_id_inner() >= DCMI_A200T_BOX_A1_BIN3_BOARD_ID) &&
        (dcmi_get_board_id_inner() <= DCMI_A200T_BOX_A1_BIN1_BOARD_ID)) ? TRUE : FALSE;
}

int dcmi_board_chip_type_is_ascend_310p_300v(void)
{
    return ((dcmi_get_board_chip_type() == DCMI_CHIP_TYPE_D310P) &&
    (dcmi_get_board_id_inner() == DCMI_310P_1P_CARD_BOARD_ID_V3)) ? TRUE : FALSE;
}

int dcmi_board_type_is_a300i_duo(void)
{
    return (dcmi_get_product_type_inner() == DCMI_A300I_DUO) ? TRUE : FALSE;
}

int dcmi_board_type_is_310p_duo_chips(void)
{
    return (dcmi_get_product_type_inner() == DCMI_A300I_DUO ||
        dcmi_get_product_type_inner() == DCMI_A300I_DUOA) ? TRUE : FALSE;
}

int dcmi_board_type_is_A500_A2(void)
{
    return (dcmi_get_product_type_inner() == DCMI_A500_A2) ? TRUE : FALSE;
}

int dcmi_310p_chip_is_ag(void)
{
    int i;
    enum dcmi_product_type ag_card_id_table[] = {
        DCMI_A300I_DUOA,
        DCMI_A800D_G1_CDLS,
        DCMI_A200I_SOC_A1,
    };

    for (i = 0; i < (int)(sizeof(ag_card_id_table) / sizeof(ag_card_id_table[0])); i++) {
        if (dcmi_get_product_type_inner() == ag_card_id_table[i]) {
            return TRUE;
        }
    }

    return FALSE;
}

// Atlas 300I DuoA卡从c83开始支持，因此不支持pkcs切换
int check_pkcs_support_product_type(void)
{
    switch (dcmi_get_product_type_inner()) {
        case DCMI_A300I_PRO:
        case DCMI_A300V_PRO:
        case DCMI_A300V:
        case DCMI_A300I_DUO:
            return TRUE;
        default:
            return FALSE;
    }
}

#ifdef ORIENT_CH
STATIC int dcmi_get_card_product_type_str(int card_id, int device_id, char *product_type_str, int max_len)
{
    struct dcmi_card_product_type_table card_product_type_table[] = {
        {DCMI_310P_1P_CARD_BOARD_ID,    DCMI_A300I_PRO_MCU_BOARD_ID,        "A300I Pro"},
        {DCMI_310P_1P_CARD_BOARD_ID_V2, DCMI_A300I_PRO_MCU_BOARD_ID,        "A300I Pro"},
        {DCMI_310P_1P_CARD_BOARD_ID,    DCMI_A300V_PRO_MCU_BOARD_ID,        "A300V Pro"},
        {DCMI_310P_1P_CARD_BOARD_ID_V2, DCMI_A300V_PRO_MCU_BOARD_ID,        "A300V Pro"},
        {DCMI_310P_1P_CARD_BOARD_ID_V3, DCMI_A300V_MCU_BOARD_ID,            "A300V"},
        {DCMI_310P_2P_CARD_BOARD_ID,    DCMI_A300I_DUO_MCU_BOARD_ID,        "A300I Duo"},
        {DCMI_310P_2P_HP_CARD_BOARD_ID, DCMI_A300I_DUOA_MCU_BOARD_ID,       "A300I DuoA"},
    };
    int ret;
    size_t index;
    size_t table_size;
    unsigned int mcu_board_id = 0;
    struct dcmi_board_info board_info = { 0 };
    ret = dcmi_get_device_board_info(card_id, device_id, &board_info);
    if ((ret != DCMI_OK) && (ret != DCMI_ERR_CODE_NOT_SUPPORT)) {
        gplog(LOG_ERR, "Failed to query board info of card %d.", card_id);
        return DCMI_ERR_CODE_INNER_ERR;
    }

    ret = dcmi_mcu_get_board_id(card_id, &mcu_board_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_mcu_get_board_id fail! ret = %d.", ret);
        return DCMI_ERR_CODE_INNER_ERR;
    }

    table_size = sizeof(card_product_type_table) / sizeof(card_product_type_table[0]);
    for (index = 0; index < table_size; index++) {
        if (board_info.board_id == card_product_type_table[index].npu_board_id &&
            mcu_board_id == card_product_type_table[index].mcu_board_id) {
            ret = strncpy_s(product_type_str, max_len, card_product_type_table[index].product_type_str,
                strlen(card_product_type_table[index].product_type_str));
            if (ret != EOK) {
                return DCMI_ERR_CODE_SECURE_FUN_FAIL;
            }
            return DCMI_OK;
        }
    }
    return DCMI_ERR_CODE_INNER_ERR;
}
#else
STATIC int dcmi_get_card_product_type_str(int card_id, int device_id, char *product_type_str, int max_len)
{
    struct dcmi_card_product_type_table card_product_type_table[] = {
        {DCMI_310P_1P_CARD_BOARD_ID,    DCMI_A300I_PRO_MCU_BOARD_ID,        "Atlas 300I Pro"},
        {DCMI_310P_1P_CARD_BOARD_ID_V2, DCMI_A300I_PRO_MCU_BOARD_ID,        "Atlas 300I Pro"},
        {DCMI_310P_1P_CARD_BOARD_ID,    DCMI_A300V_PRO_MCU_BOARD_ID,        "Atlas 300V Pro"},
        {DCMI_310P_1P_CARD_BOARD_ID_V2, DCMI_A300V_PRO_MCU_BOARD_ID,        "Atlas 300V Pro"},
        {DCMI_310P_1P_CARD_BOARD_ID_V3, DCMI_A300V_MCU_BOARD_ID,            "Atlas 300V"},
        {DCMI_310P_2P_CARD_BOARD_ID,    DCMI_A300I_DUO_MCU_BOARD_ID,        "Atlas 300I Duo"},
        {DCMI_310P_2P_HP_CARD_BOARD_ID, DCMI_A300I_DUOA_MCU_BOARD_ID,       "Atlas 300I DuoA"},
    };
    int ret;
    size_t index;
    size_t table_size;
    unsigned int mcu_board_id = 0;
    struct dcmi_board_info board_info = { 0 };
    ret = dcmi_get_device_board_info(card_id, device_id, &board_info);
    if ((ret != DCMI_OK) && (ret != DCMI_ERR_CODE_NOT_SUPPORT)) {
        gplog(LOG_ERR, "Failed to query board info of card %d.", card_id);
        return DCMI_ERR_CODE_INNER_ERR;
    }

    ret = dcmi_mcu_get_board_id(card_id, &mcu_board_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_mcu_get_board_id fail! ret = %d.", ret);
        return DCMI_ERR_CODE_INNER_ERR;
    }

    table_size = sizeof(card_product_type_table) / sizeof(card_product_type_table[0]);
    for (index = 0; index < table_size; index++) {
        if (board_info.board_id == card_product_type_table[index].npu_board_id &&
            mcu_board_id == card_product_type_table[index].mcu_board_id) {
            ret = strncpy_s(product_type_str, max_len, card_product_type_table[index].product_type_str,
                strlen(card_product_type_table[index].product_type_str));
            if (ret != EOK) {
                return DCMI_ERR_CODE_SECURE_FUN_FAIL;
            }
            return DCMI_OK;
        }
    }
    return DCMI_ERR_CODE_INNER_ERR;
}
#endif

#ifdef ORIENT_CH
int dcmi_get_product_type_str(int card_id, int device_id, char *product_type_str, int max_len)
{
    struct dcmi_product_type_table product_type_table[] = {
        {DCMI_A200_MODEL_3000, "A200 Model 3000"},
        {DCMI_A300I_MODEL_3000, "A300I Model 3000"},
        {DCMI_A300I_MODEL_3010, "A300I Model 3010"},
        {DCMI_A800D_G1, "A800D G1"},
        {DCMI_A800D_G1_CDLS, "A800D G1 CDLS"},
        {DCMI_A200I_SOC_A1, "A200I SoC A1"},
        {DCMI_A500_A2, "A500 A2"},
        {DCMI_A200_A2_MODEL, "A200I A2"},
        {DCMI_A900T_POD_A1, "A900T PoD A1"},
        {DCMI_A200T_BOX_A1, "A200T Box A1"},
        {DCMI_A300T_A1, "A300T A2"},
        {DCMI_A300I_A2, "A300I A2"},
        {DCMI_A900_A3_SUPERPOD, "A900 A3 SuperPoD"},
        {DCMI_A200I_PRO, "A200I Pro"},
    };
    int ret;
    size_t index, table_size;
    enum dcmi_product_type product_type = DCMI_PRODUCT_TYPE_INVALID;

    product_type = dcmi_get_product_type_inner();
    table_size = sizeof(product_type_table) / sizeof(product_type_table[0]);
    for (index = 0; index < table_size; index++) {
        if (product_type == product_type_table[index].product_type) {
            ret = strncpy_s(product_type_str, max_len, product_type_table[index].product_type_str,
                strlen(product_type_table[index].product_type_str));
            if (ret != EOK) {
                return DCMI_ERR_CODE_SECURE_FUN_FAIL;
            }
            return DCMI_OK;
        }
    }
    return dcmi_get_card_product_type_str(card_id, device_id, product_type_str, max_len);
}
#else
int dcmi_get_product_type_str(int card_id, int device_id, char *product_type_str, int max_len)
{
    struct dcmi_product_type_table product_type_table[] = {
        {DCMI_A200_MODEL_3000, "Atlas 200 Model 3000"},
        {DCMI_A300I_MODEL_3000, "Atlas 300I Model 3000"},
        {DCMI_A300I_MODEL_3010, "Atlas 300I Model 3010"},
        {DCMI_A800D_G1, "Atlas 800D G1"},
        {DCMI_A800D_G1_CDLS, "Atlas 800D G1 CDLS"},
        {DCMI_A200I_SOC_A1, "Atlas 200I SoC A1"},
        {DCMI_A500_A2, "Atlas 500 A2"},
        {DCMI_A200_A2_MODEL, "Atlas 200I A2"},
        {DCMI_A900T_POD_A1, "Atlas A900T PoD A1"},
        {DCMI_A200T_BOX_A1, "Atlas A200T Box A1"},
        {DCMI_A300T_A1, "Atlas 300T A2"},
        {DCMI_A300I_A2, "Atlas 300I A2"},
        {DCMI_A900_A3_SUPERPOD, "Atlas 900 A3 SuperPoD"},
        {DCMI_A200I_PRO, "Atlas 200I Pro"},
    };
    int ret;
    size_t index, table_size;
    enum dcmi_product_type product_type = DCMI_PRODUCT_TYPE_INVALID;

    product_type = dcmi_get_product_type_inner();
    table_size = sizeof(product_type_table) / sizeof(product_type_table[0]);
    for (index = 0; index < table_size; index++) {
        if (product_type == product_type_table[index].product_type) {
            ret = strncpy_s(product_type_str, max_len, product_type_table[index].product_type_str,
                strlen(product_type_table[index].product_type_str));
            if (ret != EOK) {
                return DCMI_ERR_CODE_SECURE_FUN_FAIL;
            }
            return DCMI_OK;
        }
    }
    return dcmi_get_card_product_type_str(card_id, device_id, product_type_str, max_len);
}
#endif

void dcmi_init_product_type_inner(int card_id, int device_id)
{
    int ret;
    struct dcmi_pcie_info_all pcie_info = { 0 };
    struct tag_pcie_idinfo_all tag_pcie_info = { 0 };
    struct dcmi_board_info board_info = { 0 };

    ret = dcmi_get_device_pcie_info_v2(card_id, device_id, &pcie_info);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_pcie_info_v2 failed. ret is %d", ret);
    }

    ret = dcmi_get_device_board_info(card_id, device_id, &board_info);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "Failed to query board info of card %d.", card_id);
    }
    g_board_details.board_id = (int)board_info.board_id;

    ret = memmove_s(&tag_pcie_info, sizeof(struct tag_pcie_idinfo_all), &pcie_info, sizeof(struct dcmi_pcie_info_all));
    if (ret < 0) {
        gplog(LOG_ERR, "call memmove_s failed. ret is %d", ret);
    }
    ret = dcmi_init_chip_board_product_type(&tag_pcie_info);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_init_chip_board_product_type failed. ret is %d", ret);
    }
    return;
}

int dcmi_mainboard_is_910_93(unsigned int main_board_id)
{
    return (main_board_id == DCMI_A900_A3_SUPERPOD_MAIN_BOARD_ID1) ||
        (main_board_id == DCMI_A900_A3_SUPERPOD_MAIN_BOARD_ID2) ||
        (main_board_id == DCMI_A_X_910_93_MAIN_BOARD_ID) ||
        (main_board_id == DCMI_A_K_910_93_MAIN_BOARD_ID);
}

int dcmi_mainboard_is_a9000_a3_superpod(unsigned int main_board_id)
{
    return (main_board_id == Atlas_9000_A3_SuperPoD_MAIN_BOARD_ID1) ||
        (main_board_id == Atlas_9000_A3_SuperPoD_MAIN_BOARD_ID2);
}

int dcmi_mainboard_is_arm_910_93(unsigned int main_board_id)
{
    return (main_board_id == DCMI_A900_A3_SUPERPOD_MAIN_BOARD_ID1) ||
        (main_board_id == DCMI_A900_A3_SUPERPOD_MAIN_BOARD_ID2) ||
        (main_board_id == Atlas_9000_A3_SuperPoD_MAIN_BOARD_ID1) ||
        (main_board_id == Atlas_9000_A3_SuperPoD_MAIN_BOARD_ID2) ||
        (main_board_id == DCMI_A_K_910_93_MAIN_BOARD_ID);
}

int dcmi_a900_a3_superpod_fp_card_id_convert(int card_id, int device_id)
{
    if (card_id < A900_A3_SUPERPOD_MAX_FRONT_CARD_NUM) {
        /* 前4张卡，device_id为1时为从die，映射的card_id需要加4 */
        return card_id + A900_A3_SUPERPOD_MAX_FRONT_CARD_NUM * (device_id % 0x2);
    } else {
        /* 后4张卡，device_id为0时为从die，映射的card_id需要减4 */
        return card_id - A900_A3_SUPERPOD_MAX_FRONT_CARD_NUM * ((device_id + 1) % 0x2);
    }
}

int dcmi_910_93_phy_id_convert(int phy_id)
{
    int parity_flag = 0, group_flag = 0, tmp_phy_id;

    if ((phy_id % EVEN_NUM) != 0) {
        parity_flag = 1;        // 1 indicates odd device
    }
    if ((phy_id / MAX_FRONT_NPU_NUM) != 0) {
        group_flag = 1;         // 1 indicates the rear 8P
    }
    
    if ((parity_flag == 1) && (group_flag == 0)) {
        tmp_phy_id = phy_id + MAX_FRONT_NPU_NUM;
    } else if ((parity_flag == 0) && (group_flag == 1)) {
        tmp_phy_id = phy_id - MAX_FRONT_NPU_NUM;
    } else {
        tmp_phy_id = phy_id;
    }
    return tmp_phy_id;
}

int dcmi_910_93_logic_id_convert(int phy_id)
{
    int ret;
    unsigned int logic_id = 0;
    int convert_phy_id;
    
    convert_phy_id = dcmi_910_93_phy_id_convert(phy_id);
    ret = dsmi_get_logicid_from_phyid(convert_phy_id, &logic_id);
    if (ret) {
        gplog(LOG_ERR, "DSMI get logicid from phyid fail. (ret=%d; phy_id=%d; convert_phy_id=%d)",
            ret, phy_id, convert_phy_id);
        return INVALID_LOGIC_ID;
    }

    return (int)logic_id;
}

int dcmi_check_card_id(int card_id)
{
    int num_id;
    struct dcmi_card_info *card_info = NULL;

    if (card_id < 0) {
        gplog(LOG_ERR, "card_id %d is invalid.", card_id);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if (dcmi_get_run_env_init_flag() != TRUE) {
        return DCMI_ERR_CODE_NOT_REDAY;
    }

    for (num_id = 0; num_id < g_board_details.card_count; num_id++) {
        card_info = &g_board_details.card_info[num_id];
        if (card_info->card_id == card_id) {
            return DCMI_OK;
        }
    }

    return DCMI_ERR_CODE_INVALID_PARAMETER;
}

int dcmi_is_has_pcieinfo(void)
{
    int board_type = dcmi_get_board_type();

    switch (board_type) {
        case DCMI_BOARD_TYPE_CARD:
        case DCMI_BOARD_TYPE_SERVER:
            return TRUE;
        case DCMI_BOARD_TYPE_MODEL:
            if ((dcmi_board_type_is_station()) || (dcmi_board_type_is_hilens())) {
                if (g_board_details.is_has_npu == FALSE) {
                    return FALSE;
                }
            }
            return TRUE;
        case DCMI_BOARD_TYPE_SOC:
            return FALSE;
        default:
            return FALSE;
    }
}