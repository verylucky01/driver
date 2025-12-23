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
#include "dsmi_common_interface_custom.h"
#include "dcmi_os_adapter.h"
#include "dcmi_fault_manage_intf.h"
#include "dcmi_log.h"
#include "dcmi_init_basic.h"
#include "dcmi_mcu_intf.h"
#include "dcmi_inner_info_get.h"
#include "dcmi_product_judge.h"
#include "dcmi_environment_judge.h"

STATIC int dcmi_init_for_soc310p_310b(const int *device_id_list, int device_count)
{
    int ret, num_id, device_id_logic;
    unsigned int device_phy_id;
    struct dsmi_board_info_stru board_info = { 0 };

    g_board_details.is_has_mcu = TRUE;
    g_board_details.is_has_npu = TRUE;
    g_board_details.mcu_access_chan = dcmi_board_chip_type_is_ascend_310b() ?
                                      DCMI_MCU_ACCESS_DIRECT : DCMI_MCU_ACCESS_BY_NPU;
    g_board_details.device_count = device_count;

    g_board_details.card_info[0].card_id = 0;
    g_board_details.card_info[0].device_count = device_count;
    g_board_details.card_info[0].board_id_pos = DCMI_BOARD_ID_ACCESS_BY_MCU;
    g_board_details.card_info[0].elabel_pos = dcmi_board_chip_type_is_ascend_310b() ?
                                              DCMI_ELABEL_ACCESS_BY_NPU : DCMI_ELABEL_ACCESS_BY_MCU;
    g_board_details.card_count = 1;

    for (num_id = 0; num_id < device_count; num_id++) {
        device_id_logic = device_id_list[num_id];
        ret = dsmi_get_board_info(device_id_logic, &board_info);
        if (ret != DSMI_OK) {
            gplog(LOG_ERR, "dsmi_get_board_info failed.%d. %s %d %s.", ret, "get chip", device_id_logic,
                "board information failed. Reboot OS to repair if necessary");
            continue;
        }

        if (num_id == 0) {
            g_board_details.board_id = (int)board_info.board_id;
        }

        g_board_details.card_info[0].device_info[num_id].logic_id = device_id_logic;
        ret = dsmi_get_phyid_from_logicid((unsigned int)device_id_logic, &device_phy_id);
        if (ret != DSMI_OK) {
            gplog(LOG_ERR, "dsmi_get_phyid_from_logicid failed. err is %d.", ret);
            return dcmi_convert_error_code(ret);
        }
        g_board_details.card_info[0].device_info[num_id].phy_id = device_phy_id;
    }

    return DCMI_OK;
}

int dcmi_init_for_soc(const int *device_id_list, int device_count)
{
    unsigned int board_id;
    board_id = (unsigned int)dcmi_get_board_id_inner();
    dcmi_310B_trans_baseboard_id(&board_id);
    switch (board_id) {
        case DCMI_310P_1P_SOC_BOARD_ID:
        case DCMI_310P_2P_SOC_BOARD_ID:
        case DCMI_310P_1P_SOC_AG_BOARD_ID:
        case DCMI_310P_CDLS_BOARD_ID:
        case DCMI_A500_A2_BOARD_ID:
            return dcmi_init_for_soc310p_310b(device_id_list, device_count);
        default:
            return dcmi_init_for_model(device_id_list, device_count);
    }
}