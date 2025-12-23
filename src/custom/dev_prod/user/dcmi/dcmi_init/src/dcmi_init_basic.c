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
#include "securec.h"
#include "dsmi_common_interface_custom.h"
#include "dcmi_product_judge.h"
#include "dcmi_fault_manage_intf.h"
#include "dcmi_log.h"
#include "dcmi_common.h"
#include "dcmi_i2c_operate.h"
#ifndef _WIN32
#include "ascend_hal.h"
#endif
#include "dcmi_environment_judge.h"
#include "dcmi_inner_cfg_persist.h"
#include "dcmi_inner_info_get.h"
#include "dcmi_init_basic.h"

struct dcmi_board_details_info g_board_details = {0};
struct dcmi_mainboard_info g_mainboard_info = {0};
struct dcmi_slot_pcie_map g_pcie_map_info;

void dcmi_run_env_init(void)
{
    int is_not_root = dcmi_is_not_root_user();
    int is_in_vm = dcmi_is_in_virtual_machine();
    int is_in_docker = dcmi_is_in_docker();
    dcmi_set_env_value(is_not_root, is_in_vm, is_in_docker);
}
 
void dcmi_init_ok(void)
{
    dcmi_set_init_flag(TRUE);
}

STATIC void dcmi_set_board_type(int board_type)
{
    g_board_details.board_type = board_type;
}

STATIC void dcmi_set_sub_board_type(int sub_board_type)
{
    g_board_details.sub_board_type = sub_board_type;
}

STATIC void dcmi_set_chip_type(int chip_type)
{
    g_board_details.chip_type = chip_type;
}

STATIC void dcmi_set_product_type(int product_type)
{
    g_board_details.product_type = product_type;
}

STATIC void dcmi_set_device_count_in_one_card(int device_count)
{
    g_board_details.device_count_in_one_card = device_count;
}

void dcmi_init_board_details_default(void)
{
    (void)memset_s(&g_board_details, sizeof(g_board_details), 0, sizeof(g_board_details));
    g_board_details.board_type = DCMI_BOARD_TYPE_INVALID;
    g_board_details.sub_board_type = DCMI_BOARD_TYPE_INVALID;
    g_board_details.product_type = DCMI_PRODUCT_TYPE_INVALID;
    g_board_details.chip_type = DCMI_CHIP_TYPE_INVALID;
    g_board_details.board_id = DCMI_INVALID_BOARD_ID;
    g_board_details.bom_id = DCMI_INVALID_BOM_ID;
    g_board_details.mcu_access_chan = DCMI_MCU_ACCESS_INVALID;
    g_board_details.device_count_in_one_card = 1;
}

STATIC int dcmi_get_board_type_by_config(void)
{
    char info_line[DCMI_CFG_LINE_MAX_LEN] = {0};
    char *tmp_str = NULL;
    size_t read_count;
    FILE *fp = NULL;

    fp = fopen(BOARD_CONFIG_FILE, "r");
    if (fp == NULL) {
        return DCMI_ERR_CODE_FILE_OPERATE_FAIL;
    }

    if (fseek(fp, 0, SEEK_SET) != 0) {
        gplog(LOG_ERR, "call fseek failed.");
        (void)fclose(fp);
        return DCMI_ERR_CODE_FILE_OPERATE_FAIL;
    }

    read_count = fread(info_line, 1, sizeof(info_line) - 1, fp);
    if (read_count <= 0) {
        gplog(LOG_ERR, "call fread failed.read_count=%zu", read_count);
        (void)fclose(fp);
        return DCMI_ERR_CODE_FILE_OPERATE_FAIL;
    }

    info_line[DCMI_CFG_LINE_MAX_LEN - 1] = '\0';
    tmp_str = strstr(info_line, BOARD_TYPE_KEY);
    if (tmp_str == NULL) {
        (void)fclose(fp);
        return DCMI_ERR_CODE_FILE_OPERATE_FAIL;
    }

    // key要全字匹配
    if (info_line != tmp_str && *(tmp_str - 1) != '\n') {
        (void)fclose(fp);
        return DCMI_ERR_CODE_FILE_OPERATE_FAIL;
    }

    // 500 A2在rc流程设置单板信息
    if (strncmp(tmp_str + strlen(BOARD_TYPE_KEY), "A500_A2", strlen("A500_A2")) == 0) {
        (void)fclose(fp);
        return DCMI_ERR_CODE_INNER_ERR;
    }

    dcmi_set_chip_type(DCMI_CHIP_TYPE_D310);
    dcmi_set_board_type(DCMI_BOARD_TYPE_MODEL);
    dcmi_set_product_type(DCMI_A200_MODEL_3000);
    if (strncmp(tmp_str + strlen(BOARD_TYPE_KEY), BOARD_TYPE_HILENS, strlen(BOARD_TYPE_HILENS)) == 0) {
        dcmi_set_sub_board_type(DCMI_BOARD_TYPE_MODEL_HILENS);
    } else {
        dcmi_set_sub_board_type(DCMI_BOARD_TYPE_MODEL_SEI);
    }

    gplog(LOG_INFO, "board_type = %d sub_board_type = %d.", dcmi_get_board_type(), dcmi_get_sub_board_type());
    (void)fclose(fp);
    return DCMI_OK;
}

STATIC int dcmi_get_board_type_by_i2c(void)
{
#ifndef _WIN32
    int err;
    char buff[DCMI_BOARD_ID_LEN] = {0};

    err = dcmi_i2c_get_data_9555(I2C0_DEV_NAME, I2C_SLAVE_PCA9555_BOARDINFO, 0x0, buff, sizeof(buff));
    if (err < 0) {
        gplog(LOG_ERR, "call dcmi_i2c_get_data_9555 failed. err is %d.", err);
        return DCMI_ERR_CODE_INNER_ERR;
    }

    if (buff[0] == DCMI_310_DEVELOP_C_BOARD_ID || buff[0] == DCMI_310_DEVELOP_A_BOARD_ID) {
        dcmi_set_sub_board_type(DCMI_BOARD_TYPE_SOC_BASE);
    }
#endif
    return DCMI_OK;
}

STATIC void dcmi_init_product_type_for_d310_ep(int board_id)
{
    switch (board_id) {
        case DCMI_310_CARD_B_BOARD_ID:
        case DCMI_310_CARD_C_BOARD_ID:
        case DCMI_310_CARD_D_BOARD_ID:
            dcmi_set_product_type(DCMI_A300I_MODEL_3010);
            break;
        case DCMI_310_CARD_DMPB_BOARD_ID:
            dcmi_set_product_type(DCMI_A300I_MODEL_3000);
            break;
        case DCMI_310_MODEL_BOARD_ID:
            dcmi_set_product_type(DCMI_A200_MODEL_3000);
            break;
        default:
            dcmi_set_product_type(DCMI_PRODUCT_TYPE_INVALID);
            break;
    }
}

STATIC void dcmi_init_product_type_for_d310_rc(void)
{
    dcmi_set_product_type(DCMI_A200_MODEL_3000);
}

STATIC void dcmi_init_board_type_for_d310_ep(int board_id)
{
    int ret;

    switch (board_id) {
        case DCMI_310_CARD_B_BOARD_ID:
        case DCMI_310_CARD_C_BOARD_ID:
        case DCMI_310_CARD_D_BOARD_ID:
        case DCMI_310_CARD_DMPA_MCU_BOARD_ID:
        case DCMI_310_CARD_DMPB_BOARD_ID:
        case DCMI_310_CARD_DMPC_MCU_BOARD_ID:
            dcmi_set_board_type(DCMI_BOARD_TYPE_CARD);
            dcmi_set_sub_board_type(DCMI_BOARD_TYPE_CARD);
            dcmi_set_device_count_in_one_card(DCMI_310_CARD_DEVICE_COUNT);
            break;
        case DCMI_310_MODEL_BOARD_ID:
            dcmi_set_board_type(DCMI_BOARD_TYPE_MODEL);
            dcmi_set_sub_board_type(DCMI_BOARD_TYPE_MODEL_BASE);
            ret = dcmi_get_board_type_by_config();
            if (ret != DCMI_OK) {
                gplog(LOG_INFO, "dcmi_get_board_type_by_config ret is %d", ret);
            }
            break;
        default:
            dcmi_set_board_type(DCMI_BOARD_TYPE_INVALID);
            break;
    }
}

STATIC void dcmi_init_board_type_for_d310_rc(void)
{
    int ret;
    dcmi_set_board_type(DCMI_BOARD_TYPE_SOC);
    dcmi_set_sub_board_type(DCMI_BOARD_TYPE_SOC_BASE);
    ret = dcmi_get_board_type_by_i2c();
    if (ret != DCMI_OK) {
        gplog(LOG_INFO, "dcmi_get_board_type_by_i2c ret is %d", ret);
    }
}

STATIC void dcmi_init_product_type_for_d310B_ep(int board_id)
{
    if ((board_id >= DCMI_A200_A2_EP_MODEL_BOARD_ID_MIN && board_id <= DCMI_A200_A2_EP_MODEL_BOARD_ID_MAX) ||
        (board_id >= DCMI_A200_A2_EP_XP_BOARD_ID_MIN && board_id <= DCMI_A200_A2_EP_XP_BOARD_ID_MAX)) {
        dcmi_set_product_type(DCMI_A200_A2_EP);
    } else {
        dcmi_set_board_type(DCMI_BOARD_TYPE_INVALID);
    }
}

STATIC void dcmi_init_board_type_for_d310B_ep(int board_id)
{
    if ((board_id >= DCMI_A200_A2_EP_MODEL_BOARD_ID_MIN && board_id <= DCMI_A200_A2_EP_MODEL_BOARD_ID_MAX) ||
        (board_id >= DCMI_A200_A2_EP_XP_BOARD_ID_MIN && board_id <= DCMI_A200_A2_EP_XP_BOARD_ID_MAX)) {
        dcmi_set_board_type(DCMI_BOARD_TYPE_MODEL);
        dcmi_set_sub_board_type(DCMI_BOARD_TYPE_MODEL);
        dcmi_set_device_count_in_one_card(DCMI_310B_EP_CARD_DEVICE_COUNT);
    } else {
        dcmi_set_board_type(DCMI_BOARD_TYPE_INVALID);
    }
}

STATIC void dcmi_init_product_type_for_d310P(int board_id)
{
    switch (board_id) {
        case DCMI_310P_1P_CARD_BOARD_ID:
        case DCMI_310P_1P_CARD_BOARD_ID_V2:
            /* A300I Pro与A300V Pro通过mcu board id区分，在后续流程中通过dcmi_flush_product_type_for_d310P()实现 */
            dcmi_set_product_type(DCMI_PRODUCT_TYPE_INVALID);
            break;
        case DCMI_310P_1P_CARD_BOARD_ID_V3:
            dcmi_set_product_type(DCMI_A300V);
            break;
        case DCMI_310P_2P_CARD_BOARD_ID:
            dcmi_set_product_type(DCMI_A300I_DUO);
            break;
        case DCMI_310P_2P_HP_CARD_BOARD_ID:
            dcmi_set_product_type(DCMI_A300I_DUOA);
            break;
        case DCMI_310P_1P_XP_BOARD_ID:
            dcmi_set_product_type(DCMI_A200I_PRO);
            break;
        default:
            dcmi_set_board_type(DCMI_BOARD_TYPE_INVALID);
            break;
    }
}

STATIC void dcmi_init_board_type_for_d310P(int board_id)
{
    switch (board_id) {
        case DCMI_310P_1P_CARD_BOARD_ID:
        case DCMI_310P_1P_CARD_BOARD_ID_V2:
        case DCMI_310P_1P_CARD_BOARD_ID_V3:
        case DCMI_310P_1P_XP_BOARD_ID:
            dcmi_set_board_type(DCMI_BOARD_TYPE_CARD);
            dcmi_set_sub_board_type(DCMI_BOARD_TYPE_CARD);
            dcmi_set_device_count_in_one_card(DCMI_310P_1P_CARD_DEVICE_COUNT);
            break;
        case DCMI_310P_2P_CARD_BOARD_ID:
        case DCMI_310P_2P_HP_CARD_BOARD_ID:
            dcmi_set_board_type(DCMI_BOARD_TYPE_CARD);
            dcmi_set_sub_board_type(DCMI_BOARD_TYPE_CARD);
            dcmi_set_device_count_in_one_card(DCMI_310P_2P_CARD_DEVICE_COUNT);
            break;
        default:
            dcmi_set_board_type(DCMI_BOARD_TYPE_INVALID);
            break;
    }
}

STATIC void dcmi_init_product_type_for_d910(int board_id)
{
    /* 910的芯片不支持获取产品类型，预留接口，便于后续扩展 */
    switch (board_id) {
        case DCMI_910_CARD_256T_A_BOARD_ID:
        case DCMI_910_CARD_256T_B_BOARD_ID:
        case DCMI_910_CARD_280T_B_BOARD_ID:
            dcmi_set_product_type(DCMI_A300T_MODEL_9000);
            break;
        case DCMI_910_BOARD_256T_A_BOARD_ID:
        case DCMI_910_BOARD_256T_B_BOARD_ID:
        case DCMI_910_BOARD_280T_A_BOARD_ID:
        case DCMI_910_BOARD_280T_B_BOARD_ID:
        case DCMI_910_BOARD_320T_A_BOARD_ID:
            dcmi_set_product_type(DCMI_A800_SERVER);
            break;
        default:
            dcmi_set_board_type(DCMI_BOARD_TYPE_INVALID);
            break;
    }
}

STATIC void dcmi_init_board_type_for_d910(int board_id)
{
    switch (board_id) {
        case DCMI_910_CARD_256T_A_BOARD_ID:
        case DCMI_910_CARD_256T_B_BOARD_ID:
        case DCMI_910_CARD_280T_B_BOARD_ID:
            dcmi_set_board_type(DCMI_BOARD_TYPE_CARD);
            dcmi_set_sub_board_type(DCMI_BOARD_TYPE_CARD);
            dcmi_set_device_count_in_one_card(DCMI_910_CARD_DEVICE_COUNT);
            break;
        case DCMI_910_BOARD_256T_A_BOARD_ID:
        case DCMI_910_BOARD_256T_B_BOARD_ID:
        case DCMI_910_BOARD_280T_A_BOARD_ID:
        case DCMI_910_BOARD_280T_B_BOARD_ID:
        case DCMI_910_BOARD_320T_A_BOARD_ID:
            dcmi_set_board_type(DCMI_BOARD_TYPE_SERVER);
            dcmi_set_sub_board_type(DCMI_BOARD_TYPE_SERVER);
            break;
        default:
            dcmi_set_board_type(DCMI_BOARD_TYPE_INVALID);
            break;
    }
}

STATIC void dcmi_init_product_type_for_d910B(int board_id)
{
    switch (board_id) {
        case DCMI_A900T_POD_A1_BIN3_BOARD_ID:
        case DCMI_A900T_POD_A1_BIN3_P3_BOARD_ID:
        case DCMI_A900T_POD_A1_BIN0_BOARD_ID:
        case DCMI_A900T_POD_A1_BIN0_P3_BOARD_ID:
        case DCMI_A900T_POD_A1_BIN1_BOARD_ID:
        case DCMI_A900T_POD_A1_BIN1_P3_BOARD_ID:
        case DCMI_A900T_POD_A1_BIN2_BOARD_ID:
        case DCMI_A900T_POD_A1_BIN2_P3_BOARD_ID:
        case DCMI_A900T_POD_A1_BIN2X_BOARD_ID:
        case DCMI_A900T_POD_A1_BIN2X_P3_BOARD_ID:
        case DCMI_A900T_POD_A1_BIN2X_1_BOARD_ID:
        case DCMI_A900T_POD_A1_BIN2X_1_P3_BOARD_ID:
        case DCMI_A800I_POD_A2_BIN2_BOARD_ID:
        case DCMI_A800I_POD_A2_BIN2_1_BOARD_ID:
        case DCMI_A800T_POD_A2_BIN1_BOARD_ID:
        case DCMI_A800T_POD_A2_BIN0_BOARD_ID:
        case DCMI_A800I_POD_A2_BIN4_1_PCIE_BOARD_ID:
            dcmi_set_product_type(DCMI_A900T_POD_A1);
            break;
        case DCMI_A200T_BOX_A1_BIN3_BOARD_ID:
        case DCMI_A200T_BOX_A1_BIN0_BOARD_ID:
        case DCMI_A200T_BOX_A1_BIN2_BOARD_ID:
        case DCMI_A200T_BOX_A1_BIN1_BOARD_ID:
            dcmi_set_product_type(DCMI_A200T_BOX_A1);
            break;
        case DCMI_A300T_A1_BIN1_350W_BOARD_ID:
        case DCMI_A300T_A1_BIN2_BOARD_ID:
        case DCMI_A300T_A1_BIN1_300W_BOARD_ID:
        case DCMI_A300T_A1_BIN0_BOARD_ID:
            dcmi_set_product_type(DCMI_A300T_A1);
            break;
        case DCMI_A300I_A2_BIN2_BOARD_ID:
        case DCMI_A300I_A2_BIN2_64G_BOARD_ID:
            dcmi_set_product_type(DCMI_A300I_A2);
            break;
        default:
            dcmi_set_board_type(DCMI_BOARD_TYPE_INVALID);
            break;
    }
}

STATIC void dcmi_init_board_type_for_d910B(int board_id)
{
    switch (board_id) {
        case DCMI_A900T_POD_A1_BIN3_BOARD_ID:
        case DCMI_A900T_POD_A1_BIN3_P3_BOARD_ID:
        case DCMI_A900T_POD_A1_BIN0_BOARD_ID:
        case DCMI_A900T_POD_A1_BIN0_P3_BOARD_ID:
        case DCMI_A900T_POD_A1_BIN1_BOARD_ID:
        case DCMI_A900T_POD_A1_BIN1_P3_BOARD_ID:
        case DCMI_A900T_POD_A1_BIN2_BOARD_ID:
        case DCMI_A900T_POD_A1_BIN2_P3_BOARD_ID:
        case DCMI_A900T_POD_A1_BIN2X_BOARD_ID:
        case DCMI_A900T_POD_A1_BIN2X_P3_BOARD_ID:
        case DCMI_A900T_POD_A1_BIN2X_1_BOARD_ID:
        case DCMI_A900T_POD_A1_BIN2X_1_P3_BOARD_ID:
        case DCMI_A200T_BOX_A1_BIN3_BOARD_ID:
        case DCMI_A200T_BOX_A1_BIN0_BOARD_ID:
        case DCMI_A200T_BOX_A1_BIN2_BOARD_ID:
        case DCMI_A200T_BOX_A1_BIN1_BOARD_ID:
        case DCMI_A800I_POD_A2_BIN2_BOARD_ID:
        case DCMI_A800I_POD_A2_BIN2_1_BOARD_ID:
        case DCMI_A800T_POD_A2_BIN1_BOARD_ID:
        case DCMI_A800T_POD_A2_BIN0_BOARD_ID:
        case DCMI_A800I_POD_A2_BIN4_1_PCIE_BOARD_ID:
            dcmi_set_board_type(DCMI_BOARD_TYPE_SERVER);
            dcmi_set_sub_board_type(DCMI_BOARD_TYPE_SERVER);
            break;
        case DCMI_A300T_A1_BIN1_350W_BOARD_ID:
        case DCMI_A300T_A1_BIN2_BOARD_ID:
        case DCMI_A300T_A1_BIN1_300W_BOARD_ID:
        case DCMI_A300T_A1_BIN0_BOARD_ID:
        case DCMI_A300I_A2_BIN2_BOARD_ID:
        case DCMI_A300I_A2_BIN2_64G_BOARD_ID:
            dcmi_set_board_type(DCMI_BOARD_TYPE_CARD);
            dcmi_set_sub_board_type(DCMI_BOARD_TYPE_CARD);
            break;
        default:
            dcmi_set_board_type(DCMI_BOARD_TYPE_INVALID);
            dcmi_set_sub_board_type(DCMI_BOARD_TYPE_INVALID);
            break;
    }
}

STATIC void dcmi_init_product_type_for_d910_93(int board_id)
{
    switch (board_id) {
        case DCMI_A900_A3_SUPERPOD_BIN1_BOARD_ID:
        case DCMI_A900_A3_SUPERPOD_BIN2_BOARD_ID:
        case DCMI_A900_A3_SUPERPOD_BIN3_BOARD_ID:
        case DCMI_A3_560T_BIN1_BOARD_ID:
        case DCMI_A3_ZQ_752T_BOARD_ID:
        case DCMI_A3_ZQ_560T_BOARD_ID:
            dcmi_set_product_type(DCMI_A900_A3_SUPERPOD);
            break;
        default:
            dcmi_set_board_type(DCMI_BOARD_TYPE_INVALID);
            break;
    }
}

STATIC void dcmi_init_board_type_for_d910_93(int board_id)
{
    switch (board_id) {
        case DCMI_A900_A3_SUPERPOD_BIN1_BOARD_ID:
        case DCMI_A900_A3_SUPERPOD_BIN2_BOARD_ID:
        case DCMI_A900_A3_SUPERPOD_BIN3_BOARD_ID:
        case DCMI_A3_560T_BIN1_BOARD_ID:
        case DCMI_A3_ZQ_752T_BOARD_ID:
        case DCMI_A3_ZQ_560T_BOARD_ID:
            dcmi_set_board_type(DCMI_BOARD_TYPE_SERVER);
            dcmi_set_sub_board_type(DCMI_BOARD_TYPE_SERVER);
            dcmi_set_device_count_in_one_card(DCMI_910_93_CARD_DEVICE_COUNT);
            break;
        default:
            dcmi_set_board_type(DCMI_BOARD_TYPE_INVALID);
            dcmi_set_sub_board_type(DCMI_BOARD_TYPE_INVALID);
            break;
    }
}

STATIC void dcmi_init_board_id(struct tag_pcie_idinfo_all *pcie_id_info, struct dsmi_board_info_stru *board_info)
{
    if (pcie_id_info->venderid == DCMI_D_CHIP_VENDER_ID && pcie_id_info->deviceid == DCMI_D_910_DEVICE_ID) {
        // 接口返回的boardid有误，低4位是预留位，非boardid信息位，需要右移丢掉
        g_board_details.board_id = (int)((board_info->board_id) >> 4);
    } else {
        g_board_details.board_id = (int)board_info->board_id;
    }
}

STATIC void dcmi_init_board_type_for_d310P_rc(void)
{
    dcmi_set_board_type(DCMI_BOARD_TYPE_SOC);
    dcmi_set_sub_board_type(DCMI_BOARD_TYPE_SOC_BASE);
}

STATIC void dcmi_init_product_type_for_d310P_rc(void)
{
    dcmi_set_product_type(DCMI_A800D_G1);
}

STATIC void dcmi_init_product_type_for_d310P_rc_cdls(void)
{
    dcmi_set_product_type(DCMI_A800D_G1_CDLS);
}

static void dcmi_init_product_type_for_d310p_1u_rc(void)
{
    dcmi_set_product_type(DCMI_A200I_SOC_A1);
}

STATIC void dcmi_init_chip_board_product_for_rc(unsigned int board_id)
{
    dcmi_310B_trans_baseboard_id(&board_id);
    switch (board_id) {
        case DCMI_310P_1P_SOC_BOARD_ID:
        case DCMI_310P_2P_SOC_BOARD_ID:
            dcmi_set_chip_type(DCMI_CHIP_TYPE_D310P);
            dcmi_init_board_type_for_d310P_rc();
            dcmi_init_product_type_for_d310P_rc();
            break;
        case DCMI_310P_CDLS_BOARD_ID:
            dcmi_set_chip_type(DCMI_CHIP_TYPE_D310P);
            dcmi_init_board_type_for_d310P_rc();
            dcmi_init_product_type_for_d310P_rc_cdls();
            break;
        case DCMI_310P_1P_SOC_AG_BOARD_ID:
            dcmi_set_chip_type(DCMI_CHIP_TYPE_D310P);
            dcmi_init_board_type_for_d310P_rc();
            dcmi_init_product_type_for_d310p_1u_rc();
            break;
        case DCMI_A500_A2_BOARD_ID:
            dcmi_set_chip_type(DCMI_CHIP_TYPE_D310B);
            dcmi_set_board_type(DCMI_BOARD_TYPE_SOC);
            dcmi_set_sub_board_type(DCMI_BOARD_TYPE_SOC_BASE);
            dcmi_set_product_type(DCMI_A500_A2);
            break;
        case DCMI_A200_A2_MODEL_BOARD_ID:
        case DCMI_A200_A2_DK_BOARD_ID:
            dcmi_set_chip_type(DCMI_CHIP_TYPE_D310B);
            dcmi_set_board_type(DCMI_BOARD_TYPE_SOC);
            dcmi_set_sub_board_type(DCMI_BOARD_TYPE_SOC_BASE);
            dcmi_set_product_type(DCMI_A200_A2_MODEL);
            break;
        default:
            dcmi_set_chip_type(DCMI_CHIP_TYPE_D310);
            dcmi_init_board_type_for_d310_rc();
            dcmi_init_product_type_for_d310_rc();
            break;
    }
}

int dcmi_init_chip_board_product_type(struct tag_pcie_idinfo_all *pcie_id_info)
{
    if (pcie_id_info == NULL) {
        gplog(LOG_ERR, "pcie_id_info is NULL.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if (pcie_id_info->venderid == DCMI_D_CHIP_VENDER_ID && pcie_id_info->deviceid == DCMI_D_310_DEVICE_ID) {
        dcmi_set_chip_type(DCMI_CHIP_TYPE_D310);
        dcmi_init_board_type_for_d310_ep(g_board_details.board_id);
        dcmi_init_product_type_for_d310_ep(g_board_details.board_id);
    } else if (pcie_id_info->venderid == DCMI_D_CHIP_VENDER_ID && pcie_id_info->deviceid == DCMI_D_310P_DEVICE_ID) {
        dcmi_set_chip_type(DCMI_CHIP_TYPE_D310P);
        dcmi_init_board_type_for_d310P(g_board_details.board_id);
        dcmi_init_product_type_for_d310P(g_board_details.board_id);
    } else if (pcie_id_info->venderid == DCMI_D_CHIP_VENDER_ID && pcie_id_info->deviceid == DCMI_D_910_DEVICE_ID) {
        dcmi_set_chip_type(DCMI_CHIP_TYPE_D910);
        dcmi_init_board_type_for_d910(g_board_details.board_id);
        dcmi_init_product_type_for_d910(g_board_details.board_id);
    } else if (pcie_id_info->venderid == DCMI_D_CHIP_VENDER_ID && pcie_id_info->deviceid == DCMI_D_910B_DEVICE_ID) {
        dcmi_set_chip_type(DCMI_CHIP_TYPE_D910B);
        dcmi_init_board_type_for_d910B(g_board_details.board_id);
        dcmi_init_product_type_for_d910B(g_board_details.board_id);
    } else if (pcie_id_info->venderid == DCMI_D_CHIP_VENDER_ID && pcie_id_info->deviceid == DCMI_D_910_93_DEVICE_ID) {
        dcmi_set_chip_type(DCMI_CHIP_TYPE_D910_93);
        dcmi_init_board_type_for_d910_93(g_board_details.board_id);
        dcmi_init_product_type_for_d910_93(g_board_details.board_id);
    } else if (pcie_id_info->venderid == DCMI_D_CHIP_VENDER_ID && pcie_id_info->deviceid == DCMI_D_310B_EP_DEVICE_ID) {
        dcmi_set_chip_type(DCMI_CHIP_TYPE_D310B);
        dcmi_init_board_type_for_d310B_ep(g_board_details.board_id);
        dcmi_init_product_type_for_d310B_ep(g_board_details.board_id);
    } else {
        dcmi_set_board_type(DCMI_BOARD_TYPE_INVALID);
    }

    return DCMI_OK;
}

int dcmi_get_board_info_handle(int device_logic_id, struct dsmi_board_info_stru *board_info)
{
    int ret;
    unsigned int phy_id, mainboard_id;
    ret = dsmi_get_board_info(device_logic_id, board_info);
    if (ret != 0) {
        gplog(LOG_ERR, "dsmi_get_board_info failed. ret is %d", ret);
        return dcmi_convert_error_code(ret);
    }

    ret = dcmi_get_device_phyid_from_logicid(device_logic_id, &phy_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_phyid_from_logicid failed. ret is %d", ret);
        return ret;
    }

    ret = dsmi_get_mainboard_id(phy_id, &mainboard_id);
    if (ret != 0) {
        gplog(LOG_ERR, "dsmi_get_mainboard_id failed. ret is %d", ret);
        return dcmi_convert_error_code(ret);
    }

    g_mainboard_info.mainboard_id = mainboard_id;
    return DCMI_OK;
}

int dcmi_init_board_type(const int *device_logic_id, int device_count)
{
    struct tag_pcie_idinfo_all pcie_id_info = {0};
    struct dsmi_board_info_stru board_info = {0};
    int ret, i;
    unsigned int mode = 0;

    dcmi_init_board_details_default();
    /* current dsmi lib for Ascend 310 Model can not get pcie info */
    ret = dcmi_get_board_type_by_config();
    if (ret == DCMI_OK) {
        return DCMI_OK;
    }

    ret = dcmi_get_rc_ep_mode(&mode);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_rc_ep_mode failed. ret is %d", ret);
        return dcmi_convert_error_code(ret);
    }

    for (i = 0; i < device_count; i++) {
        if (dcmi_get_boot_status(mode, device_logic_id[i]) != DCMI_OK) {
            continue;
        }

        ret = dcmi_get_board_info_handle(device_logic_id[i], &board_info);
        if (ret == DCMI_OK) {
            break;
        }
    }

    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dsmi_get_board_info failed. ret is %d", ret);
        return ret;
    }

    if (mode == DCMI_PCIE_RC_MODE) {
        dcmi_init_chip_board_product_for_rc(board_info.board_id);
        g_board_details.board_id = (int)board_info.board_id;
        return DCMI_OK;
    }

#ifndef _WIN32
    ret = dsmi_get_pcie_info_v2(device_logic_id[i], &pcie_id_info);
#else
    ret = dcmi_get_pcie_info_win(device_logic_id[i], &pcie_id_info);
#endif
    if (ret != DSMI_OK) {
        gplog(LOG_ERR, "dsmi get pcie info failed. ret is %d", ret);
        return dcmi_convert_error_code(ret);
    }
    dcmi_init_board_id(&pcie_id_info, &board_info);
    ret = dcmi_init_chip_board_product_type(&pcie_id_info);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_init_chip_board_product_type failed. ret is %d", ret);
    }

    return DCMI_OK;
}

/* A300I Pro与A300V Pro只能通过mcu board id区分 */
STATIC void dcmi_flush_product_type_for_d310P(unsigned int board_id)
{
    if (board_id == DCMI_A300I_PRO_MCU_BOARD_ID) {
        dcmi_set_product_type(DCMI_A300I_PRO);
    } else if (board_id == DCMI_A300V_PRO_MCU_BOARD_ID) {
        dcmi_set_product_type(DCMI_A300V_PRO);
    } else {
        return;
    }
}

STATIC void dcmi_flush_card_info_mcu_id(struct dcmi_card_info *card_info)
{
    card_info->mcu_id = (g_board_details.is_has_mcu == TRUE) ? card_info->device_count : -1;
}

STATIC void dcmi_flush_card_info_cpu_id(struct dcmi_card_info *card_info)
{
    bool board_type_supported = dcmi_board_type_is_station() ||
        dcmi_board_type_is_hilens() || dcmi_board_type_is_soc_develop();
    if (board_type_supported) {
        if ((card_info->mcu_id == -1) && (!dcmi_board_type_is_hilens())) {
            card_info->cpu_id = card_info->device_count;
        } else {
            card_info->cpu_id = card_info->device_count + 1;
        }
        if (dcmi_check_run_in_docker()) {
            card_info->cpu_id = -1;
        }
    } else {
        card_info->cpu_id = -1;
    }
}

STATIC void dcmi_flush_card_info_elabel_pos(struct dcmi_card_info *card_info)
{
    if (card_info->elabel_pos == DCMI_ELABEL_ACCESS_BY_MCU) {
        card_info->elabel_pos = card_info->mcu_id;
    } else if (card_info->elabel_pos == DCMI_ELABEL_ACCESS_BY_CPU) {
        card_info->elabel_pos = card_info->cpu_id;
    } else {
        card_info->elabel_pos = 0;
    }
}

STATIC void dcmi_flush_card_info_board_id_pos(struct dcmi_card_info *card_info)
{
    struct dcmi_board_info board_info = {0};
    int err;

    if (card_info->board_id_pos == DCMI_BOARD_ID_ACCESS_BY_MCU) {
        card_info->board_id_pos = card_info->mcu_id;
        err = dcmi_mcu_get_board_info(card_info->card_id, &board_info);
        if (err != DCMI_OK) {
            gplog(LOG_ERR, "dcmi_mcu_get_board_info failed. err is %d", err);
            return;
        }
        card_info->card_board_id = (int)board_info.board_id;
        dcmi_flush_product_type_for_d310P(board_info.board_id);
    } else if (card_info->board_id_pos == DCMI_BOARD_ID_ACCESS_BY_CPU) {
        card_info->board_id_pos = card_info->cpu_id;
    } else {
        card_info->board_id_pos = 0;
    }
}

int dcmi_flush_device_id(void)
{
    struct dcmi_card_info *card_info = NULL;
    int num_id;

    for (num_id = 0; num_id < g_board_details.card_count; num_id++) {
        card_info = &g_board_details.card_info[num_id];
        dcmi_flush_card_info_mcu_id(card_info);
        dcmi_flush_card_info_cpu_id(card_info);
        dcmi_flush_card_info_elabel_pos(card_info);
        dcmi_flush_card_info_board_id_pos(card_info);
    }

    return DCMI_OK;
}

STATIC bool dcmi_310_card_is_need_get_pcie_slot_info(void)
{
    switch (g_board_details.board_id) {
        case DCMI_310_CARD_DMPB_BOARD_ID:
            return TRUE;
        default:
            return FALSE;
    }
}

STATIC bool dcmi_310P_card_is_need_get_pcie_slot_info(void)
{
    switch (g_board_details.board_id) {
        case DCMI_310P_1P_CARD_BOARD_ID:
        case DCMI_310P_1P_CARD_BOARD_ID_V2:
        case DCMI_310P_1P_CARD_BOARD_ID_V3:
        case DCMI_310P_2P_CARD_BOARD_ID:
        case DCMI_310P_2P_HP_CARD_BOARD_ID:
            return TRUE;
        default:
            return FALSE;
    }
}

STATIC bool dcmi_910_card_is_need_get_pcie_slot_info(void)
{
    switch (g_board_details.board_id) {
        case DCMI_910_CARD_256T_A_BOARD_ID:
        case DCMI_910_CARD_280T_B_BOARD_ID:
        case DCMI_910_CARD_256T_B_BOARD_ID:
            return TRUE;
        default:
            return FALSE;
    }
}

STATIC bool dcmi_910B_card_is_need_get_pcie_slot_info(void)
{
    switch (g_board_details.board_id) {
        case DCMI_A300T_A1_BIN1_350W_BOARD_ID:
        case DCMI_A300T_A1_BIN2_BOARD_ID:
        case DCMI_A300T_A1_BIN1_300W_BOARD_ID:
        case DCMI_A300T_A1_BIN0_BOARD_ID:
        case DCMI_A300I_A2_BIN2_BOARD_ID:
        case DCMI_A300I_A2_BIN2_64G_BOARD_ID:
            return TRUE;
        default:
            return FALSE;
    }
}

STATIC bool dcmi_card_is_need_get_pcie_slot_info(void)
{
    int chip_type = dcmi_get_board_chip_type();

    switch (chip_type) {
        case DCMI_CHIP_TYPE_D310:
            return dcmi_310_card_is_need_get_pcie_slot_info();
        case DCMI_CHIP_TYPE_D310P:
            return dcmi_310P_card_is_need_get_pcie_slot_info();
        case DCMI_CHIP_TYPE_D910:
            return dcmi_910_card_is_need_get_pcie_slot_info();
        case DCMI_CHIP_TYPE_D910B:
            return dcmi_910B_card_is_need_get_pcie_slot_info();
        default:
            return FALSE;
    }
}

STATIC char *dcmi_pcie_slot_map_cmp(struct dcmi_single_solt_pcie_map *pcie_map_info, struct dcmi_card_info *card_info)
{
    int i;
    char *find_flag = NULL;
    bool check_result;

    if (pcie_map_info == NULL || card_info == NULL) {
        return NULL;
    }

    check_result = dcmi_card_is_need_get_pcie_slot_info();
    if (check_result) {
        for (i = 0; i < card_info->device_count; i++) {
            find_flag = strstr(&pcie_map_info->pcie_info[0], &card_info->device_info[i].chip_pcieinfo[0]);
            if (find_flag != NULL) {
                break;
            }
        }
    } else {
        find_flag = strstr(&pcie_map_info->pcie_info[0], &card_info->pcie_info_pre[0]);
    }

    return find_flag;
}

STATIC char *dcmi_init_dmidecode_info_slot_id_parse(const char *msginfo)
{
    char *info_start_index = NULL;
    char *find_flag = NULL;

    find_flag = strstr(&msginfo[0], "ID: ");
    if (find_flag != NULL) {
        info_start_index = find_flag + strlen("ID: ");
        return info_start_index;
    }

    find_flag = strstr(&msginfo[0], "Designation: Slot");
    if (find_flag != NULL) {
        info_start_index = find_flag + strlen("Designation: Slot");
        return info_start_index;
    }

    find_flag = strstr(&msginfo[0], "Designation: SLOT_");
    if (find_flag != NULL) {
        info_start_index = find_flag + strlen("Designation: SLOT_");
        return info_start_index;
    }

    find_flag = strstr(&msginfo[0], "Designation: SLOT");
    if (find_flag != NULL) {
        info_start_index = find_flag + strlen("Designation: SLOT");
        return info_start_index;
    }

    return info_start_index;
}

STATIC int dcmi_init_pcie_slot_info_from_dmidecode_info(FILE *p_file)
{
    int length, slot_id;
    char msginfo[MAX_PCIE_INFO_LENTH] = {0};
    char *find_flag = NULL;
    char *info_start_index = NULL;
    char *in_use = NULL;
    struct dcmi_single_solt_pcie_map *pcie_self_info = NULL;
    char *end_ptr = NULL;
    int has_find = -1;

    while (!feof(p_file)) {
        if (fgets(&msginfo[0], MAX_PCIE_INFO_LENTH, p_file) == NULL) {
            break;
        }

        if (in_use == NULL) {
            in_use = strstr(&msginfo[0], "Current Usage: In Use");
        }

        if (has_find == -1) {
            info_start_index = dcmi_init_dmidecode_info_slot_id_parse(msginfo);
            if (info_start_index == NULL) {
                continue;
            }
            slot_id = strtol(info_start_index, &end_ptr, DCMI_NUMBER_BASE);
            has_find = 1;
        } else {
            find_flag = strstr(&msginfo[0], "Bus Address: ");
            if (find_flag == NULL) {
                continue;
            }

            if (in_use == NULL) {
                has_find = -1;
                continue;
            }

            pcie_self_info = &g_pcie_map_info.single_slot_pcie_map[g_pcie_map_info.pcie_num];
            pcie_self_info->slot_id = slot_id;
            length = (int)strlen(find_flag);
            if ((length <= 0) || (length >= MAX_PCIE_INFO_LENTH)) {
                continue;
            }

            if (memcpy_s(&pcie_self_info->pcie_info[0], MAX_PCIE_INFO_LENTH, find_flag, length) != DCMI_OK) {
                gplog(LOG_ERR, "dcmi_init_pcie_slot_info_from_dmidecode_info call memcpy_s failed");
            }
            pcie_self_info->pcie_info[length] = '\n';
            g_pcie_map_info.pcie_num++;

            has_find = -1;
            in_use = NULL;
        }
    }
    return DCMI_OK;
}

STATIC int dcmi_init_pcie_slot_info(void)
{
#ifndef _WIN32
    int ret;
    FILE *p_file = NULL;
    if (dcmi_get_board_type() != DCMI_BOARD_TYPE_CARD) {
        return DCMI_OK;
    }

    if (dcmi_check_run_in_vm()) {
        gplog(LOG_ERR, "running in VM, can't get slot info.");
        return DCMI_OK;
    }

    p_file = popen("/usr/sbin/dmidecode -t9 2>&1", "r");
    if (p_file == NULL) {
        gplog(LOG_ERR, "Failed to run the dmidecode command on the popen.\n");
        return DCMI_ERR_CODE_INNER_ERR;
    }

    ret = dcmi_init_pcie_slot_info_from_dmidecode_info(p_file);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_init_pcie_slot_info_from_dmidecode_info failed. ret is %d", ret);
        (void)pclose(p_file);
        return DCMI_ERR_CODE_INNER_ERR;
    }
    (void)pclose(p_file);
    return DCMI_OK;
#else
    return DCMI_OK;
#endif
}

STATIC void dcmi_pcie_slotid_cardid_init_by_dsmi(void)
{
    int card_index, ret;
    struct dcmi_card_info *card_info = NULL;

    for (card_index = 0; card_index < g_board_details.card_count; card_index++) {
        card_info = &g_board_details.card_info[card_index];
        struct dcmi_board_info board_info = { 0 };
        ret = get_card_board_info(card_info, &(board_info.slot_id));
        if (ret == DCMI_OK) {
            card_info->slot_id = (int)board_info.slot_id;
            card_info->card_id = (int)board_info.slot_id;
            continue;
        }

        card_info->slot_id = -1;
        card_info->card_id = -1;
        if (ret != DCMI_ERR_CODE_RESOURCE_OCCUPIED) {
            gplog(LOG_ERR, "dcmi_get_device_board_info failed. ret is %d.", ret);
        }
    }
    return;
}

STATIC void dcmi_pcie_slotid_cardid_other_init(void)
{
    int card_index, pcie_index;
    struct dcmi_card_info *card_info = NULL;
    struct dcmi_single_solt_pcie_map *pcie_map_info = NULL;

    for (card_index = 0; card_index < g_board_details.card_count; card_index++) {
        card_info = &g_board_details.card_info[card_index];
        card_info->slot_id = -1;

        for (pcie_index = 0; pcie_index < g_pcie_map_info.pcie_num; pcie_index++) {
            pcie_map_info = &g_pcie_map_info.single_slot_pcie_map[pcie_index];
            if (dcmi_pcie_slot_map_cmp(pcie_map_info, card_info) != NULL) {
                card_info->slot_id = pcie_map_info->slot_id;
                card_info->card_id = pcie_map_info->slot_id;
                break;
            }
        }
    }
    return;
}

STATIC int cmp_device_info(const void *a, const void *b)
{
    int logic_id_a = ((struct dcmi_device_info *)a)->logic_id;
    int logic_id_b = ((struct dcmi_device_info *)b)->logic_id;
    return (logic_id_a > logic_id_b) ? 1 : -1;
}

STATIC int cmp_card_info(const void *a, const void *b)
{
    int card_id_a = ((struct dcmi_card_info *)a)->card_id;
    int card_id_b = ((struct dcmi_card_info *)b)->card_id;
    return (card_id_a > card_id_b) ? 1 : -1;
}

void dcmi_card_info_sort(void)
{
    int card_index;
    struct dcmi_card_info *card_info = NULL;

    qsort(g_board_details.card_info, g_board_details.card_count, sizeof(struct dcmi_card_info), cmp_card_info);
    for (card_index = 0; card_index < g_board_details.card_count; card_index++) {
        card_info = &g_board_details.card_info[card_index];
        qsort(card_info->device_info, card_info->device_count, sizeof(struct dcmi_device_info), cmp_device_info);
    }

    return;
}

int dcmi_pcie_slot_map_init(void)
{
    int ret;
    (void)memset_s(&g_pcie_map_info, sizeof(struct dcmi_slot_pcie_map), 0, sizeof(struct dcmi_slot_pcie_map));

    if (dcmi_get_board_type() != DCMI_BOARD_TYPE_CARD) {
        return DCMI_OK;
    }

    if (dcmi_board_chip_type_is_ascend_910() || dcmi_board_chip_type_is_ascend_910b() ||
        dcmi_board_chip_type_is_ascend_910_93()) {
        dcmi_pcie_slotid_cardid_init_by_dsmi();
        return DCMI_OK;
    }

    ret = dcmi_init_pcie_slot_info();
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "Call dcmi_init_pcie_slot_info failed. ret is %d.", ret);
    }
    dcmi_pcie_slotid_cardid_other_init();
    return DCMI_OK;
}

// 310B通过海思接口获取到的board id是一个范围，将其转为唯一的底板硬件board id
void dcmi_310B_trans_baseboard_id(unsigned int *board_id)
{
    // 非310B环境直接返回
    if (*board_id < DCMI_A200_A2_MODEL_BOARD_ID_MIN) {
        return;
    } else if ((*board_id >= DCMI_A200_A2_MODEL_BOARD_ID_MIN) && (*board_id < DCMI_A500_A2_BOARD_ID_MIN)) {
        *board_id = DCMI_A200_A2_MODEL_BOARD_ID;
    } else if ((*board_id >= DCMI_A500_A2_BOARD_ID_MIN) && (*board_id <= DCMI_A500_A2_BOARD_ID_MAX)) {
        *board_id = DCMI_A500_A2_BOARD_ID;
    } else if ((*board_id >= DCMI_A200_A2_4G_DK_BOARD_ID_MIN) && (*board_id <= DCMI_A200_A2_4G_DK_BOARD_ID_MAX)) {
        *board_id = DCMI_A200_A2_DK_BOARD_ID;
    } else if ((*board_id >= DCMI_A200_A2_12G_DK_BOARD_ID_MIN) && (*board_id <= DCMI_A200_A2_12G_DK_BOARD_ID_MAX)) {
        *board_id = DCMI_A200_A2_DK_BOARD_ID;
    } else if ((*board_id > DCMI_A200_A2_12G_DK_BOARD_ID_MAX) && (*board_id <= DCMI_A200_A2_MODEL_BOARD_ID_MAX)) {
        *board_id = DCMI_A200_A2_MODEL_BOARD_ID;
    }
    return;
}

#if defined DCMI_VERSION_2
int dcmi_board_init(void)
{
    int err;
    int device_count = 0;
    int device_id_list[MAX_DEVICE_NUM] = {0};
 
    err = dcmi_get_npu_device_list((int *)&device_id_list[0], MAX_DEVICE_NUM, &device_count);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_list failed. err is %d.", err);
        return err;
    }
 
    err = dcmi_init_board_type((const int *)&device_id_list[0], device_count);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_init_board_type failed. err is %d.", err);
        return err;
    }
 
    switch (dcmi_get_board_type()) {
        case DCMI_BOARD_TYPE_MODEL:
            err = dcmi_init_for_model((const int *)&device_id_list[0], device_count);
            break;
        case DCMI_BOARD_TYPE_CARD:
            err = dcmi_init_for_card((const int *)&device_id_list[0], device_count);
            break;
        case DCMI_BOARD_TYPE_SERVER:
            err = dcmi_init_for_server(&device_id_list[0], device_count);
            break;
        case DCMI_BOARD_TYPE_SOC:
            err = dcmi_init_for_soc((const int *)&device_id_list[0], device_count);
            break;
 
        default:
            err = DCMI_ERR_CODE_INNER_ERR;
            break;
    }
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi init failed. err is %d", err);
        return err;
    }
 
    gplog(LOG_INFO, "dcmi board init success. device_count=%d.", device_count);
    return DCMI_OK;
}
 
int dcmi_init(void)
{
    int err;
    bool check_result;
 
    dcmi_run_env_init();
    (void)dcmi_cfg_create_lock_dir(DCMI_CFG_VNPU_LOCK_DIR);
    (void)dcmi_cfg_create_lock_dir(DCMI_CFG_SYSLOG_LOCK_DIR);
    (void)dcmi_cfg_create_lock_dir(DCMI_CFG_CUSTOM_OP_LOCK_DIR);
 
    err = dcmi_board_init();
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_board_init failed. err is %d", err);
        return err;
    }
 
    dcmi_init_ok();
 
    check_result = (dcmi_board_type_is_station() || dcmi_board_type_is_hilens());
    if (!check_result) {
        err = dcmi_flush_device_id();
        if (err != DCMI_OK) {
            gplog(LOG_ERR, "dcmi_flush_device_id failed. err is %d.", err);
        }
 
        err = dcmi_pcie_slot_map_init();
        if (err != DCMI_OK) {
            gplog(LOG_ERR, "dcmi_pcie_slot_map_init failed. err is %d.", err);
        }
        dcmi_card_info_sort();
    }
 
    gplog(LOG_INFO, "dcmi init all success.");
    return DCMI_OK;
}
#endif