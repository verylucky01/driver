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

#include "securec.h"
#include "dsmi_common_interface_custom.h"
#include "dcmi_os_adapter.h"
#include "dcmi_fault_manage_intf.h"
#include "dcmi_log.h"
#include "dcmi_common.h"
#include "dcmi_inner_info_get.h"
#include "dcmi_product_judge.h"
#include "dcmi_environment_judge.h"

STATIC unsigned int g_device_phy_card_id_map[MAX_DEVICE_NUM] = {0};
STATIC unsigned int g_max_phy_id = 0;

STATIC void dcmi_init_server_get_device_in_card(int card_id, int* device_logic_id_list,
    int* device_logic_phy_id_map)
{
    int i, j, device_phy_id_base, card_count;

    card_count = 0;
    i = 0;
    // 找到对应实际起来的物理cardid
    while (1) {
        if (g_device_phy_card_id_map[i] == 1) {
            card_count++;
            if (card_count == card_id + 1) {
                break;
            }
        }
        i++;
    }
    // 计算对应物理卡的起始物理dieid
    device_phy_id_base = i * g_board_details.device_count_in_one_card;
    for (i = 0; i <= (int)g_max_phy_id; i++) {
        for (j = 0; j < g_board_details.device_count_in_one_card; j++) {
            if (device_logic_phy_id_map[i] == device_phy_id_base + j) {
                device_logic_id_list[j] = i;
            }
        }
    }
    return ;
}

STATIC int dcmi_init_bdf_info(int *device_logic_phy_id_map, int card_count)
{
    struct dcmi_card_info *card_info = NULL;
    int ret, i, j;
    int *device_logic_id_list = NULL;
    device_logic_id_list = (int *)malloc(sizeof(int) * g_board_details.device_count_in_one_card);
    if (device_logic_id_list == NULL) {
        gplog(LOG_ERR, "malloc device_logic_id_list failed.");
        return DCMI_ERR_CODE_INNER_ERR;
    }
    /* 此函数只在服务器场景使用，一个910对应一个card */
    for (i = 0; i < card_count; i++) {
        dcmi_init_server_get_device_in_card(i, device_logic_id_list, device_logic_phy_id_map);
        for (j = 0; j < g_board_details.device_count_in_one_card; j++) {
            ret = dsmi_get_pcie_info(device_logic_id_list[j],
                (struct tag_pcie_idinfo *)&g_board_details.card_info[i].str_pci_info);
            if (ret != DSMI_OK && ret != DSMI_ERR_RESOURCE_OCCUPIED) {
                    gplog(LOG_INFO, "call dsmi_get_pcie_info get chip %d pcie info failed %d.",
                        device_logic_id_list[i], ret);
            }
            if (ret != DSMI_OK) {
                continue;
            }

            card_info = &g_board_details.card_info[i];
            ret = snprintf_s(card_info->device_info[j].chip_pcieinfo, MAX_PCIE_INFO_LENTH, MAX_PCIE_INFO_LENTH - 1,
                "%02x:%02x.%1x", card_info->str_pci_info.bdf_busid, card_info->str_pci_info.bdf_deviceid,
                card_info->str_pci_info.bdf_funcid);
            if (ret <= 0) {
                gplog(LOG_ERR, "call snprintf_s failed %d.", ret);
                continue;
            }
        }
    }
    free(device_logic_id_list);
    device_logic_id_list = NULL;
    return DCMI_OK;
}

void dcmi_init_board_info_exit_abnormally(int index)
{
    g_board_details.card_info[index].card_id = -1;
    g_board_details.card_info[index].slot_id = -1;
    g_board_details.card_info[index].mcu_id = -1;
    g_board_details.card_info[index].cpu_id = -1;
    g_board_details.card_info[index].board_id_pos = DCMI_BOARD_ID_ACCESS_BY_NPU;
}

STATIC void dcmi_init_server_per_device(int *device_logic_phy_id_map, int card_id,
    struct dsmi_board_info_stru *board_info, int* init_abnomal_flag)
{
    int ret, i;
    unsigned int device_phy_id;
    unsigned int health = HEALTH_UNKNOWN;
    int *device_logic_id_list = NULL;
    device_logic_id_list = (int *)malloc(sizeof(int) * g_board_details.device_count_in_one_card);
    if (device_logic_id_list == NULL) {
        gplog(LOG_ERR, "malloc device_logic_id_list failed.");
        *init_abnomal_flag = 1;
        (void)dcmi_init_board_info_exit_abnormally(card_id);
        return;
    }
    for (i = 0; i < g_board_details.device_count_in_one_card; i++) {
        device_logic_id_list[i] = -1;
    }
    dcmi_init_server_get_device_in_card(card_id, device_logic_id_list, device_logic_phy_id_map);

    for (i = 0; i < g_board_details.device_count_in_one_card; i++) {
        ret = dsmi_get_device_health(device_logic_id_list[i], &health);
        if (ret != DSMI_OK || health == DCMI_DEVICE_NOT_EXIST) {
            (void)dcmi_init_board_info_exit_abnormally(card_id);
            gplog(LOG_ERR, "call dsmi_get_device_health error.(logic_id=%d, health=%u, ret=%d).",
                device_logic_id_list[i], health, ret);
            *init_abnomal_flag = 1;
            continue;
        }
        
        ret = dsmi_get_board_info(device_logic_id_list[i], board_info);  // 每个芯片均获得board_info
        if (ret != DSMI_OK) {
            (void)dcmi_init_board_info_exit_abnormally(card_id);
            if (ret != DSMI_ERR_RESOURCE_OCCUPIED) {
                gplog(LOG_ERR, "call dsmi_get_board_info get chip %d board info failed %d.",
                    device_logic_id_list[i], ret);
            }
            *init_abnomal_flag = 1;
            continue;
        }
        g_board_details.card_info[card_id].device_info[i].logic_id = device_logic_id_list[i];
        ret = dsmi_get_phyid_from_logicid((unsigned int)device_logic_id_list[i], &device_phy_id);
        if (ret != DSMI_OK) {
            gplog(LOG_ERR, "dsmi_get_phyid_from_logicid failed. err is %d.", ret);
            *init_abnomal_flag = 1;
            continue;
        }
        g_board_details.card_info[card_id].device_info[i].phy_id = device_phy_id;
        g_board_details.card_info[card_id].device_info[i].chip_slot = i; // ascend910卡或ascend910板的NPU单元都只有一个npu芯片
    }
    free(device_logic_id_list);
    device_logic_id_list = NULL;
    return;
}

STATIC int dcmi_init_board_info(int *device_logic_phy_id_map, int card_count)
{
    int i, init_abnomal_flag;
    struct dsmi_board_info_stru board_info = { 0 };
    bool support_chip_type = (dcmi_board_chip_type_is_ascend_910b() || dcmi_board_chip_type_is_ascend_910_93());

    /* 服务器场景每个芯片作为一个card */
    for (i = 0; i < card_count; i++) {
        init_abnomal_flag = 0;
        dcmi_init_server_per_device(device_logic_phy_id_map, i, &board_info, &init_abnomal_flag);
        if (init_abnomal_flag != 0) {
            // 如果有某个die初始化失败，跳过下面赋值流程，910_93双die状态保持一直，不需单独拆分
            continue;
        }
        g_board_details.card_info[i].card_id = (int)board_info.slot_id;
        g_board_details.card_info[i].slot_id = (int)board_info.slot_id;
        g_board_details.card_info[i].mcu_id = -1;
        g_board_details.card_info[i].cpu_id = -1;
        g_board_details.card_info[i].board_id_pos = (support_chip_type ?
            DCMI_BOARD_ID_ACCESS_BY_MCU : DCMI_BOARD_ID_ACCESS_BY_NPU);
        g_board_details.card_info[i].device_count = g_board_details.device_count_in_one_card;
    }
    return DCMI_OK;
}

STATIC int dcmi_init_phy_id_list(int *device_id_list, int *device_logic_phy_id_map)
{
    int ret, i;
    unsigned int temp_phy_id;
    unsigned int max_phy_id = 0;

    ret = memset_s(device_logic_phy_id_map, sizeof(int) * MAX_DEVICE_NUM, -1, sizeof(int) * MAX_DEVICE_NUM);
    if (ret != DCMI_OK) {
            gplog(LOG_ERR, "memset_s device_logic_phy_id_map failed. ret is %d.", ret);
            return DCMI_ERR_CODE_INNER_ERR;
    }
    ret = memset_s(g_device_phy_card_id_map, sizeof(int) * MAX_DEVICE_NUM, -1, sizeof(int) * MAX_DEVICE_NUM);
    if (ret != DCMI_OK) {
            gplog(LOG_ERR, "memset_s device_phy_card_id_map failed. ret is %d.", ret);
            return DCMI_ERR_CODE_INNER_ERR;
    }
    for (i = 0; i < g_board_details.device_count; i++) {
        ret = dsmi_get_phyid_from_logicid((unsigned int)device_id_list[i], &temp_phy_id);
        if (ret != DSMI_OK) {
            gplog(LOG_ERR, "dsmi_get_phyid_from_logicid failed. err is %d, logic_id is %d.",
                ret, device_id_list[i]);
            continue;
        }
        device_logic_phy_id_map[i] = (int)temp_phy_id;
        max_phy_id = max_phy_id < temp_phy_id ? temp_phy_id : max_phy_id;
        g_device_phy_card_id_map[(int)temp_phy_id / g_board_details.device_count_in_one_card] = 1;
    }
    g_max_phy_id = max_phy_id;
    return DCMI_OK;
}

int dcmi_init_for_server(int *device_id_list, int device_count)
{
    int ret;
    int device_logic_phy_id_map[MAX_DEVICE_NUM] = {0};
    bool support_chip_type = (dcmi_board_chip_type_is_ascend_910b() || dcmi_board_chip_type_is_ascend_910_93());
    g_board_details.is_has_mcu = (support_chip_type ? TRUE : FALSE);
    g_board_details.is_has_npu = TRUE;
    g_board_details.mcu_access_chan = (support_chip_type ? DCMI_MCU_ACCESS_BY_NPU : DCMI_MCU_ACCESS_INVALID);
    g_board_details.card_count = (device_count + g_board_details.device_count_in_one_card - 1) /
                                 g_board_details.device_count_in_one_card;
    g_board_details.device_count = device_count;

    ret = dcmi_init_phy_id_list(device_id_list, device_logic_phy_id_map);
    if (ret != DCMI_OK) {
        return ret;
    }

    ret = dcmi_init_board_info(device_logic_phy_id_map, g_board_details.card_count);
    if (ret != DCMI_OK) {
        return ret;
    }

    ret = dcmi_init_bdf_info(device_logic_phy_id_map, g_board_details.card_count);
    if (ret != DCMI_OK) {
        return ret;
    }

    return DCMI_OK;
}