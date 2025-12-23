/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef __DCMI_PRODUCT_JUDGE_H__
#define __DCMI_PRODUCT_JUDGE_H__

#include <stdbool.h>
#include "dcmi_interface_api.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#ifdef DT_FLAG
#define STATIC
#else
#define STATIC static
#endif

#define INVALID_LOGIC_ID   (-1)

#define DCMI_A900_A3_SUPERPOD_MAIN_BOARD_ID1    0x18
#define DCMI_A900_A3_SUPERPOD_MAIN_BOARD_ID2    0x19
#define A900_A3_SUPERPOD_MAX_FRONT_CARD_NUM     4   // 天成产品一个板共8张npu卡，分两组，每组4张

#define MAX_FRONT_NPU_NUM 8
#define EVEN_NUM          2

#define DCMI_310_DEVELOP_A_BOARD_ID         0xCF
#define DCMI_310_DEVELOP_C_BOARD_ID         0xCE
#define DCMI_310_SOC_BOARD_ID               0x3EC
#define DCMI_310_SANTACHI_SOC_BOARD_ID      0xBBC    // 金三立SOC BOARDID
#define DCMI_310_MODEL_BOARD_ID             0x04
#define DCMI_310_CARD_B_BOARD_ID            0x01
#define DCMI_310_CARD_C_BOARD_ID            0x03
#define DCMI_310_CARD_D_BOARD_ID            0x05
#define DCMI_310_CARD_DMPB_BOARD_ID         0xC8

#define DCMI_310_CARD_DMPA_MCU_BOARD_ID     0xFC    // 300-3010
#define DCMI_310_CARD_DMPB_MCU_BOARD_ID     0xAA    // 300-3000
#define DCMI_310_CARD_DMPC_MCU_BOARD_ID     0xAC    // 300-3010

// 310B EP ID转换范围区间
#define DCMI_A200_A2_EP_MODEL_BOARD_ID_MIN    0x64
#define DCMI_A200_A2_EP_MODEL_BOARD_ID_MAX    0xA0
#define DCMI_A200_A2_EP_XP_BOARD_ID_MIN       0xC8
#define DCMI_A200_A2_EP_XP_BOARD_ID_MAX       0x12C // 芯片场景预留id 200——300

#define DCMI_310P_1P_CARD_BOARD_ID      0x64
/* 310p vrd 连续性改版board type */
#define DCMI_310P_1P_CARD_BOARD_ID_V2   0x68
/* 310p 高功耗芯片board type Atlas 300V */
#define DCMI_310P_1P_CARD_BOARD_ID_V3   0x6E
#define DCMI_310P_1P_XP_BOARD_ID        0xC8    // 310p芯片场景id 200
#define DCMI_310P_2P_CARD_BOARD_ID      0x97
/* 310p 高功耗芯片（HP）2P卡board type */
#define DCMI_310P_2P_HP_CARD_BOARD_ID   0x99
#define DCMI_310P_1P_SOC_BOARD_ID       0x1c2
#define DCMI_310P_2P_SOC_BOARD_ID       0x1db
#define DCMI_310P_1P_SOC_AG_BOARD_ID    0x1c7
#define DCMI_310P_CDLS_BOARD_ID         0x191

#define DCMI_A300I_PRO_MCU_BOARD_ID    0xAB     /* A300I Pro单板MCU侧board id */
#define DCMI_A300V_PRO_MCU_BOARD_ID    0xAF     /* A300V Pro单板MCU侧board id */
#define DCMI_A300V_MCU_BOARD_ID        0xB6     /* A300V单板MCU侧board id */
#define DCMI_A300I_DUO_MCU_BOARD_ID    0xB1     /* A300I Duo单板MCU侧board id */
#define DCMI_A300I_DUOA_MCU_BOARD_ID   0xB7     /* A300I Duoa单板MCU侧board id */

#define DCMI_910_CARD_256T_A_BOARD_ID  0x1
#define DCMI_910_CARD_256T_B_BOARD_ID  0x3
#define DCMI_910_CARD_280T_B_BOARD_ID  0x6
#define DCMI_910_BOARD_256T_A_BOARD_ID 0x2
#define DCMI_910_BOARD_256T_B_BOARD_ID 0x21
#define DCMI_910_BOARD_280T_A_BOARD_ID 0x27
#define DCMI_910_BOARD_280T_B_BOARD_ID 0x28
#define DCMI_910_BOARD_320T_A_BOARD_ID 0x24

// A500 A2的MCU和310b获取的BOARD ID相同
#define DCMI_A200_A2_MODEL_BOARD_ID    0x42
#define DCMI_A200_A2_DK_BOARD_ID       0x45
#define DCMI_A500_A2_BOARD_ID          0x46

// 通过海思获取的board id是一个范围区间 转化为对应的底板board id
#define DCMI_A200_A2_MODEL_BOARD_ID_MIN       0x2774 // 10100
#define DCMI_A500_A2_BOARD_ID_MIN             0xC3E6 // 50150
#define DCMI_A500_A2_BOARD_ID_MAX             0xC449 // 50249
#define DCMI_A200_A2_4G_DK_BOARD_ID_MIN       0xC79C // 51100
#define DCMI_A200_A2_4G_DK_BOARD_ID_MAX       0xC7CD // 51149
#define DCMI_A200_A2_12G_DK_BOARD_ID_MIN      0xC7CE // 51150
#define DCMI_A200_A2_12G_DK_BOARD_ID_MAX      0xC831 // 51249
#define DCMI_A200_A2_MODEL_BOARD_ID_MAX       0x183B1 // 99249

// A900T_POD_A1与A200T_BOX_A1有多个BOARD ID且为连续值,区别为芯片bin不同
#define DCMI_A900T_POD_A1_BIN3_BOARD_ID  0x30
#define DCMI_A900T_POD_A1_BIN0_BOARD_ID  0x31
#define DCMI_A900T_POD_A1_BIN1_BOARD_ID  0x32
#define DCMI_A900T_POD_A1_BIN2_BOARD_ID  0x33
#define DCMI_A900T_POD_A1_BIN2X_BOARD_ID 0x34
#define DCMI_A900T_POD_A1_BIN2X_1_BOARD_ID 0x38
// P3表示3个PMU
#define DCMI_A900T_POD_A1_BIN3_P3_BOARD_ID  0x39
#define DCMI_A900T_POD_A1_BIN0_P3_BOARD_ID  0x3a
#define DCMI_A900T_POD_A1_BIN1_P3_BOARD_ID  0x3b
#define DCMI_A900T_POD_A1_BIN2_P3_BOARD_ID  0x3c
#define DCMI_A900T_POD_A1_BIN2X_P3_BOARD_ID 0x3d
#define DCMI_A900T_POD_A1_BIN2X_1_P3_BOARD_ID 0x3e
#define DCMI_A200T_BOX_A1_BIN3_BOARD_ID  0x50
#define DCMI_A200T_BOX_A1_BIN0_BOARD_ID  0x51
#define DCMI_A200T_BOX_A1_BIN2_BOARD_ID  0x52
#define DCMI_A200T_BOX_A1_BIN1_BOARD_ID  0x53
// A300T_A1 有2个bin1，功率不同
#define DCMI_A300T_A1_BIN1_350W_BOARD_ID 0x10
#define DCMI_A300T_A1_BIN2_BOARD_ID      0x11
#define DCMI_A300T_A1_BIN1_300W_BOARD_ID 0x12
#define DCMI_A300T_A1_BIN0_BOARD_ID      0x13

// Atlas 900 A3 SuperPoD 有多个BOARD ID且为连续值，区别为芯片bin不同
#define DCMI_A900_A3_SUPERPOD_BIN1_BOARD_ID       0XB0
#define DCMI_A900_A3_SUPERPOD_BIN2_BOARD_ID       0XB1
#define DCMI_A900_A3_SUPERPOD_BIN3_BOARD_ID       0XB2
// 适配A3_9362（560T算力NPU训练模组）
#define DCMI_A3_560T_BIN1_BOARD_ID                0xB3
// ZQ器件替代
#define DCMI_A3_ZQ_752T_BOARD_ID                  0xD1
#define DCMI_A3_ZQ_560T_BOARD_ID                  0xD3
// Atlas 9000 A3 SuperPoD
#define Atlas_9000_A3_SuperPoD_MAIN_BOARD_ID1     0x1C
#define Atlas_9000_A3_SuperPoD_MAIN_BOARD_ID2     0x1D
// A300I Duo A2
#define DCMI_A300I_A2_BIN2_BOARD_ID  0x28
// A300I Duo A2 64G Version
#define DCMI_A300I_A2_BIN2_64G_BOARD_ID  0x29

#define DCMI_A_K_910_93_MAIN_BOARD_ID 0X14
#define DCMI_A_X_910_93_MAIN_BOARD_ID 0X15
#define DCMI_A900_A3_SUPERPOD_MAIN_BOARD_ID1    0x18
#define DCMI_A900_A3_SUPERPOD_MAIN_BOARD_ID2    0x19
#define BOARD_ID_MASK       0xF0
#define A_X_BOARD_ID        0x50
#define A_K_OR_POD_BOARD_ID 0x30 // 特别的，A+K与POD共用一种BOARD ID，需要用main区分
#define POD_A_MAINBOARD_ID  0X0
#define POD_C_MAINBOARD_ID  0X4

#define DCMI_A800I_POD_A2_BIN2_BOARD_ID  0x40
#define DCMI_A800I_POD_A2_BIN2_1_BOARD_ID  0x41
#define DCMI_A800T_POD_A2_BIN1_BOARD_ID  0x42
#define DCMI_A800T_POD_A2_BIN0_BOARD_ID  0x43
#define DCMI_A800I_POD_A2_BIN4_1_PCIE_BOARD_ID 0x37 // Atlas 800I A2 280T-64G no-HCCS规格

struct dcmi_product_type_table {
    int product_type;
    const char *product_type_str;
};

struct dcmi_card_product_type_table {
    unsigned int npu_board_id;
    unsigned int mcu_board_id;
    const char *product_type_str;
};

int dcmi_board_type_is_card(void);

int dcmi_board_type_is_station(void);

int dcmi_board_type_is_hilens(void);

int dcmi_board_type_is_server(void);

int dcmi_board_type_is_model(void);

int dcmi_board_type_is_soc(void);

int dcmi_board_type_is_soc_develop(void);

int dcmi_board_chip_type_is_ascend_310(void);

int dcmi_board_chip_type_is_ascend_310p(void);

int dcmi_board_chip_type_is_ascend_310p_200i_pro(void);

int dcmi_board_chip_type_is_ascend_310b(void);

int dcmi_board_chip_type_is_ascend_910(void);

int dcmi_board_chip_type_is_ascend_910b(void);

int dcmi_board_chip_type_is_ascend_910_93(void);

int dcmi_board_chip_type_is_ascend_910b_300i_a2(void);

int dcmi_board_chip_type_is_ascend_910b_200t_box_a2(void);

int dcmi_board_chip_type_is_ascend_310p_300v(void);

int dcmi_board_type_is_a300i_duo(void);

int dcmi_board_type_is_310p_duo_chips(void);

int dcmi_board_type_is_A500_A2(void);

int dcmi_310p_chip_is_ag(void);

int check_pkcs_support_product_type(void);

int dcmi_get_product_type_str(int card_id, int device_id, char *product_type_str, int max_len);

void dcmi_init_product_type_inner(int card_id, int device_id);

int dcmi_mainboard_is_910_93(unsigned int main_board_id);

int dcmi_mainboard_is_a9000_a3_superpod(unsigned int main_board_id);

int dcmi_mainboard_is_arm_910_93(unsigned int main_board_id);

int dcmi_a900_a3_superpod_fp_card_id_convert(int card_id, int device_id);

int dcmi_910_93_logic_id_convert(int phy_id);

int dcmi_910_93_phy_id_convert(int phy_id);

int dcmi_check_card_id(int card_id);

int dcmi_is_has_pcieinfo(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __DCMI_PRODUCT_JUDGE_H__ */