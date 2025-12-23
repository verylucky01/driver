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
#include "dcmi_fault_manage_intf.h"
#include "dcmi_os_adapter.h"
#include "dcmi_log.h"
#include "dcmi_product_judge.h"
#include "dcmi_mcu_intf.h"
#include "dcmi_init_basic.h"
#include "dcmi_inner_info_get.h"
#include "dcmi_product_judge.h"
#include "dcmi_environment_judge.h"
#ifndef _WIN32
#include "ascend_hal.h"
#endif
#ifdef _WIN32
struct dcmi_win_pcie_node g_pcie_node[MAX_PCIE_DEVICE_NUM] = {0};
unsigned short g_pcie_node_cnt = 0;
#endif

#ifdef _WIN32
STATIC int is_d_card(PTCHAR pszDesc, size_t len)
{
    size_t str_len = wcslen(D_CARD_HARDWAREID_STR);
    if (str_len > len) {
        return FALSE;
    }
    int ret = wcsncmp(pszDesc, D_CARD_HARDWAREID_STR, wcslen(D_CARD_HARDWAREID_STR));
    return ret == 0;
}

STATIC int get_d_card_pcie(HDEVINFO dev_info, PSP_DEVINFO_DATA pdev_info_data)
{
    DWORD size, reg_data_type;
    TCHAR desc_info[DCMI_BUFFER_SIZE] = {0};
    int pci_bdf_1, pci_bdf_2, ret;
    const unsigned int pci_str_cnt = 2;
    unsigned int lh_pci_sign = 0;

    pdev_info_data->cbSize = sizeof(SP_DEVINFO_DATA);
    if (SetupDiGetDeviceRegistryProperty(dev_info, pdev_info_data, SPDRP_LOCATION_INFORMATION, &reg_data_type,
        (BYTE*)desc_info, sizeof(desc_info), &size) == FALSE) {
        gplog(LOG_ERR, "call SetupDiGetDeviceRegistryProperty for SPDRP_LOCATION_INFORMATION failed. err=%d",
            GetLastError());
        return DCMI_ERR_CODE_INNER_ERR;
    }

    /* CH PCI 总线 220、设备 0、功能 0
       PCI bus 179, device 0, function 0 */
    ret = swscanf_s(desc_info, L"%*s%*s%d%*s%d%*s%d", &g_pcie_node[g_pcie_node_cnt].bus_id,
        &g_pcie_node[g_pcie_node_cnt].dev_id, &g_pcie_node[g_pcie_node_cnt].fun_id);
    if (ret != BDF_STR_CNT) {
        ret = swscanf_s(desc_info, L"%*s%*s%d%*s%*s%d%*s%*s%d", &g_pcie_node[g_pcie_node_cnt].bus_id,
            &g_pcie_node[g_pcie_node_cnt].dev_id, &g_pcie_node[g_pcie_node_cnt].fun_id);
    }

    if (ret != BDF_STR_CNT) {
        gplog(LOG_ERR, "get BDF info failed. ret=%d", ret);
        return DCMI_ERR_CODE_SECURE_FUN_FAIL;
    }

    if (SetupDiGetDeviceRegistryProperty(dev_info, pdev_info_data, SPDRP_LOCATION_PATHS, &reg_data_type,
        (BYTE*)desc_info, sizeof(desc_info), &size) == FALSE) {
        gplog(LOG_ERR, "call SetupDiGetDeviceRegistryProperty for SPDRP_LOCATION_PATHS failed. err=%d",
            GetLastError());
        return DCMI_ERR_CODE_INNER_ERR;
    }

    /* PCIROOT(D7)#PCI(0000)#PCI(0000)#PCI(0200)#PCI(0000) */
    ret = swscanf_s(desc_info, L"%*[PCIROOT(]%02X%*[)#PCI(]%04u", &pci_bdf_1, &pci_bdf_2);
    if (ret == pci_str_cnt) {
        if (pci_bdf_1 == lh_pci_sign) {
            g_pcie_node[g_pcie_node_cnt].pci_root = pci_bdf_2;
        } else {
            g_pcie_node[g_pcie_node_cnt].pci_root = pci_bdf_1;
        }
    } else {
        gplog(LOG_ERR, "get pci_root info failed. ret=%d", ret);
        return DCMI_ERR_CODE_SECURE_FUN_FAIL;
    }

    g_pcie_node_cnt++;
    return DCMI_OK;
}

STATIC int dcmi_save_pcie_info(FILE **pci_file)
{
    HDEVINFO dev_info;
    SP_DEVINFO_DATA dev_info_data;
    DWORD size, reg_data_type;
    TCHAR desc_info[DCMI_BUFFER_SIZE];
    int dev_index = 0;

    if (g_pcie_node_cnt) {
        return DCMI_OK;
    }

    (void)setlocale(LC_ALL, "");
    dev_info = SetupDiGetClassDevs(NULL, TEXT("PCI"), NULL, DIGCF_ALLCLASSES | DIGCF_PRESENT);
    if (dev_info == INVALID_HANDLE_VALUE) {
        gplog(LOG_ERR, "call SetupDiGetClassDevs failed, err=%d", GetLastError());
        return DCMI_ERR_CODE_INNER_ERR;
    }
    dev_info_data.cbSize = sizeof(SP_DEVINFO_DATA);
    while (SetupDiEnumDeviceInfo(dev_info, dev_index, &dev_info_data)) {
        if (SetupDiGetDeviceRegistryProperty(dev_info, &dev_info_data, SPDRP_HARDWAREID, &reg_data_type,
            (BYTE *)desc_info, sizeof(desc_info), &size) == FALSE) {
            gplog(LOG_ERR, "call SetupDiGetDeviceRegistryProperty failed. err=%d", GetLastError());
            continue;
        }
        if (is_d_card(desc_info, sizeof(desc_info))) {
            if (get_d_card_pcie(dev_info, &dev_info_data) != DCMI_OK) {
                gplog(LOG_ERR, "call get_d_card_pcie failed.");
                continue;
            }
        }
        dev_index++;
    }

    SetupDiDestroyDeviceInfoList(dev_info);
    return DCMI_OK;
}
#else
STATIC int dcmi_save_pcie_info(FILE **pci_file)
{
    int ret;
    char mat_string_new[MAX_PCIE_INFO_LENTH] = {0};
    char mat_string[] =
        "/usr/bin/find /sys/devices/ 2>/dev/null -name \"[0-f][0-f][0-f][0-f]:[0-f][0-f]:[0-f][0-f].[0-f]\" | grep "
        "\"[0-f][0-f][0-f][0-f]:[0-f][0-f]:[0-f][0-f].[0-f]/[0-f][0-f][0-f][0-f]:[0-f][0-f]:[0-f][0-f].[0-f]\" | awk "
        "-F \"pci[0-f][0-f][0-f][0-f]:\" '{print $2}' | cut -d \"/\" -f 2-$NF";
    char mat_string_vm[] =
        "/usr/bin/find /sys/devices/ 2>/dev/null -name \"[0-f][0-f][0-f][0-f]:[0-f][0-f]:[0-f][0-f].[0-f]\""
        "|grep \"pci[0-f][0-f][0-f][0-f]:\"";

    // 虚拟机、容器场景均调用mat_string_vm实现pcie设备查找，以适配虚拟机容器场景
    if (dcmi_check_run_in_vm() || dcmi_check_run_in_docker()) {
        ret = snprintf_s(&mat_string_new[0], MAX_PCIE_INFO_LENTH, MAX_PCIE_INFO_LENTH - 1, "%s", &mat_string_vm[0]);
    } else {
        ret = snprintf_s(&mat_string_new[0], MAX_PCIE_INFO_LENTH, MAX_PCIE_INFO_LENTH - 1, "%s", &mat_string[0]);
    }
    if (ret <= 0) {
        gplog(LOG_ERR, "call snprintf_s failed ret is %d.", ret);
        return DCMI_ERR_CODE_SECURE_FUN_FAIL;
    }

    *pci_file = popen(mat_string_new, "r");
    if (*pci_file == NULL) {
        gplog(LOG_ERR, "Failed to run the find pcie command on the popen.\n");
        return DCMI_ERR_CODE_INNER_ERR;
    }

    return DCMI_OK;
}
#endif

STATIC int dcmi_card_is_without_switch(void)
{
    switch (dcmi_get_board_chip_type()) {
        case DCMI_CHIP_TYPE_D310:
            return (g_board_details.board_id == DCMI_310_CARD_DMPB_BOARD_ID);
        case DCMI_CHIP_TYPE_D310P:
            return (g_board_details.board_id == DCMI_310P_1P_CARD_BOARD_ID ||
                g_board_details.board_id == DCMI_310P_1P_CARD_BOARD_ID_V2 ||
                g_board_details.board_id == DCMI_310P_1P_CARD_BOARD_ID_V3 ||
                g_board_details.board_id == DCMI_310P_1P_XP_BOARD_ID ||
                g_board_details.board_id == DCMI_310P_2P_CARD_BOARD_ID ||
                g_board_details.board_id == DCMI_310P_2P_HP_CARD_BOARD_ID);
        case DCMI_CHIP_TYPE_D910:
            return (g_board_details.board_id == DCMI_910_CARD_256T_A_BOARD_ID ||
                g_board_details.board_id == DCMI_910_CARD_256T_B_BOARD_ID ||
                g_board_details.board_id == DCMI_910_CARD_280T_B_BOARD_ID);
        case DCMI_CHIP_TYPE_D910B:
            return (g_board_details.board_id == DCMI_A300T_A1_BIN1_350W_BOARD_ID ||
                g_board_details.board_id == DCMI_A300T_A1_BIN2_BOARD_ID ||
                g_board_details.board_id == DCMI_A300T_A1_BIN1_300W_BOARD_ID ||
                g_board_details.board_id == DCMI_A300T_A1_BIN0_BOARD_ID ||
                g_board_details.board_id == DCMI_A300I_A2_BIN2_BOARD_ID ||
                g_board_details.board_id == DCMI_A300I_A2_BIN2_64G_BOARD_ID);
        case DCMI_CHIP_TYPE_D910_93:
            return (g_board_details.board_id == DCMI_A900_A3_SUPERPOD_BIN1_BOARD_ID ||
                g_board_details.board_id == DCMI_A900_A3_SUPERPOD_BIN2_BOARD_ID ||
                g_board_details.board_id == DCMI_A900_A3_SUPERPOD_BIN3_BOARD_ID ||
                g_board_details.board_id == DCMI_A3_560T_BIN1_BOARD_ID ||
                g_board_details.board_id == DCMI_A3_ZQ_752T_BOARD_ID ||
                g_board_details.board_id == DCMI_A3_ZQ_560T_BOARD_ID);
        default:
            return FALSE;
    }
}


#ifdef _WIN32
STATIC int dcmi_get_pcie_before_info(char *msginfo, int msginfo_len, struct dcmi_card_pcie_info *card_pcie_info,
    struct dcmi_pcie_pre_index pcie_pre_index, struct dcmi_device_id_info *device_id_info)
{
    int domain, bus_id, dev_id, fun_id, ret, i;
    ret = sscanf_s(card_pcie_info->d_chip_pcie_info, "%04x:%02x:%02x.%1x", &domain, &bus_id, &dev_id, &fun_id);
    if (ret != DBDF_STR_CNT) {
        gplog(LOG_ERR, "get BDF info failed. ret=%d", ret);
        return DCMI_ERR_CODE_SECURE_FUN_FAIL;
    }

    for (i = 0; i < g_pcie_node_cnt; i++) {
        if ((bus_id == g_pcie_node[i].bus_id) && (dev_id == g_pcie_node[i].dev_id) &&
            (fun_id == g_pcie_node[i].fun_id)) {
            ret = sprintf_s(card_pcie_info->root_pcie_info, MAX_PCIE_INFO_LENTH, "%04X:%02X:%02X.%1X",
                0, g_pcie_node[i].pci_root, 0, 0);
            if (ret < 0) {
                gplog(LOG_ERR, "call sprintf_s failed. ret=%d", ret);
                return DCMI_ERR_CODE_SECURE_FUN_FAIL;
            }
            ret = sprintf_s(card_pcie_info->switch_pcie_info, MAX_PCIE_INFO_LENTH, "%04X:%02X:%02X.%1X",
                0, g_pcie_node[i].pci_root, 0, 0);
            if (ret < 0) {
                gplog(LOG_ERR, "call sprintf_s failed. ret=%d", ret);
                return DCMI_ERR_CODE_SECURE_FUN_FAIL;
            }
            return DCMI_OK;
        }
    }

    return DCMI_ERR_CODE_INNER_ERR;
}
#else
STATIC int dcmi_get_pcie_info_list(char *info, char **pci_info_list, int *list_len)
{
    char *str_tmp = NULL;
    int index = 0;
    int ret, str_num;
    char *info_inner = info;

    /* 虚拟机中最前面的为'/'字符，需要跳过 */
    if (info_inner[0] == '/') {
        info_inner = info_inner + 1;
    }

    while ((str_tmp = strsep(&info_inner, "/")) != NULL) {
        pci_info_list[index] = (char *)malloc(MAX_PCIE_INFO_LENTH);

        if (pci_info_list[index] == NULL) {
            gplog(LOG_ERR, "Failed to alloc usages info memory.");
            for (str_num = 0; str_num < index; str_num++) {
                free(pci_info_list[str_num]);
            }
            return DCMI_ERR_CODE_MEM_OPERATE_FAIL;
        }
        index++;
        ret = memset_s(pci_info_list[index - 1], MAX_PCIE_INFO_LENTH, 0, MAX_PCIE_INFO_LENTH);
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "call memset_s failed.%d.", ret);
            for (str_num = 0; str_num < index; str_num++) {
                free(pci_info_list[str_num]);
            }
            return DCMI_ERR_CODE_MEM_OPERATE_FAIL;
        }

        ret = strcpy_s(pci_info_list[index - 1], MAX_PCIE_INFO_LENTH, str_tmp);
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "call strcpy_s failed.%d.", ret);
            for (str_num = 0; str_num < index; str_num++) {
                free(pci_info_list[str_num]);
            }
            return DCMI_ERR_CODE_MEM_OPERATE_FAIL;
        }
    }

    *list_len = index;
    return DCMI_OK;
}

STATIC int dcmi_get_pcie_before_info(char *msginfo, int msginfo_len, struct dcmi_card_pcie_info *card_pcie_info,
    struct dcmi_pcie_pre_index pcie_pre_index, struct dcmi_device_id_info *device_id_info)
{
    char *pci_info_list[128] = {0}; // pcie的bus信息预留128个层级，提前申请栈上空间。
    int str_index = 0;
    int ret, str_num;
    char msginfo_new[MAX_PCIE_INFO_LENTH] = {0};
    ret = snprintf_s(msginfo_new, MAX_PCIE_INFO_LENTH, MAX_PCIE_INFO_LENTH - 1, "%s", msginfo);
    if (ret <= 0) {
        gplog(LOG_ERR, "call snprintf_s failed ret is %d.", ret);
        return DCMI_ERR_CODE_SECURE_FUN_FAIL;
    }
    if (strstr(msginfo_new, card_pcie_info->d_chip_pcie_info) != NULL) {
        ret = dcmi_get_pcie_info_list(msginfo_new, pci_info_list, &str_index);
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "Failed to dcmi_get_pcie_info_list.");
            return ret;
        }
        (device_id_info->find_device_num)++;
    } else {
        return DCMI_ERR_CODE_INNER_ERR;
    }
    if (str_index <= pcie_pre_index.pci_pre_index || str_index <= pcie_pre_index.switch_pre_index) {
        gplog(LOG_ERR, "Index out of bounds.");
        return DCMI_ERR_CODE_INNER_ERR;
    }  // 防止数组访问越界
    ret = strcpy_s(card_pcie_info->root_pcie_info, MAX_PCIE_INFO_LENTH,
        pci_info_list[str_index - pcie_pre_index.pci_pre_index - 1]);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call strcpy_s failed.%d.", ret);
    }
    ret = strcpy_s(card_pcie_info->switch_pcie_info, MAX_PCIE_INFO_LENTH,
        pci_info_list[str_index - pcie_pre_index.switch_pre_index - 1]);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call strcpy_s failed.%d.", ret);
    }

    for (str_num = 0; str_num < str_index; str_num++) {
        free(pci_info_list[str_num]);
    }

    return DCMI_OK;
}
#endif

STATIC int dcmi_get_pcie_info_inner(char *msginfo, int msginfo_len, struct dcmi_device_id_info *device_id_info,
    struct dcmi_card_pcie_info *card_pcie_info)
{
    int ret;
    int device_id_logic = device_id_info->device_id_logic;
    struct dcmi_pcie_pre_index pcie_pre_index;
#ifndef _WIN32
    ret = dsmi_get_pcie_info_v2(device_id_logic, (struct tag_pcie_idinfo_all *)&card_pcie_info->pcie_info_curr);
#else
    ret = dcmi_get_pcie_info_win(device_id_logic, (struct tag_pcie_idinfo_all *)&card_pcie_info->pcie_info_curr);
#endif
    if (ret != DCMI_OK) {
        gplog(LOG_INFO, "dsmi_get_pcie_info failed.%d. %s %d %s.", ret, "Get chip", device_id_logic,
            "PCIE information failed");
        return dcmi_convert_error_code(ret);
    }

    ret = sprintf_s(card_pcie_info->d_chip_pcie_info, MAX_PCIE_INFO_LENTH - 1, "%04x:%02x:%02x.%1x",
        card_pcie_info->pcie_info_curr.domain,
        card_pcie_info->pcie_info_curr.bdf_busid,
        card_pcie_info->pcie_info_curr.bdf_deviceid,
        card_pcie_info->pcie_info_curr.bdf_funcid);
    if (ret <= DCMI_OK) {
        gplog(LOG_ERR, "call sprintf_s failed.%d", ret);
        return DCMI_ERR_CODE_SECURE_FUN_FAIL;
    }

    if (dcmi_card_is_without_switch()) {
        /* 消A D卡的switch这级已经不存在，因而card和switch的index都相等为1 */
        if (dcmi_check_run_in_vm()) {
            /* 与公有云确认虚拟机中只有一级PCIE，此处直接返回pcie设备地址 */
            pcie_pre_index.pci_pre_index = CHIP_PCIE_INFO_INDEX;
            pcie_pre_index.switch_pre_index = CHIP_PCIE_INFO_INDEX;
        } else {
            pcie_pre_index.pci_pre_index = SWITCH_PCIE_INFO_INDEX;
            pcie_pre_index.switch_pre_index = SWITCH_PCIE_INFO_INDEX;
        }
        ret = dcmi_get_pcie_before_info(msginfo, msginfo_len, card_pcie_info, pcie_pre_index, device_id_info);
    } else {
        if (dcmi_check_run_in_vm()) {
            /* 与公有云确认虚拟机中只有一级PCIE，此处直接返回pcie设备地址 */
            pcie_pre_index.pci_pre_index = CHIP_PCIE_INFO_INDEX;
            pcie_pre_index.switch_pre_index = CHIP_PCIE_INFO_INDEX;
        } else {
            pcie_pre_index.pci_pre_index = CARD_PCIE_INFO_INDEX;
            pcie_pre_index.switch_pre_index = SWITCH_PCIE_INFO_INDEX;
        }
        ret = dcmi_get_pcie_before_info(msginfo, msginfo_len, card_pcie_info, pcie_pre_index, device_id_info);
    }
    if (ret != DCMI_OK) {
        return DCMI_ERR_CODE_INNER_ERR;
    }

    return DCMI_OK;
}


STATIC int dcmi_init_card_info_for_new(
    int card_index, int card_id, struct dcmi_card_pcie_info *card_pcie_info, int logic_id, int chip_slot)
{
    int ret, device_id_curr;
    unsigned int device_phy_id;

    device_id_curr = g_board_details.card_info[card_index].device_count++;
    if (dcmi_get_board_chip_type() == DCMI_CHIP_TYPE_D910_93) {
        g_board_details.card_info[card_index].card_id = chip_slot;
        g_board_details.card_info[card_index].slot_id = chip_slot;
    } else {
        g_board_details.card_info[card_index].card_id = card_id;
    }
    g_board_details.card_info[card_index].elabel_pos = DCMI_ELABEL_ACCESS_BY_MCU;
    g_board_details.card_info[card_index].board_id_pos = DCMI_BOARD_ID_ACCESS_BY_MCU;
    g_board_details.card_info[card_index].str_pci_info = card_pcie_info->pcie_info_curr;
    g_board_details.card_info[card_index].device_info[device_id_curr].logic_id = logic_id;

    ret = dsmi_get_phyid_from_logicid((unsigned int)logic_id, &device_phy_id);
    if (ret != DSMI_OK) {
        gplog(LOG_ERR, "dsmi_get_phyid_from_logicid failed. err is %d.", ret);
        return dcmi_convert_error_code(ret);
    }
    g_board_details.card_info[card_index].device_info[device_id_curr].phy_id = device_phy_id;

    if (dcmi_board_chip_type_is_ascend_910()) {
        g_board_details.card_info[card_index].device_info[device_id_curr].chip_slot = device_id_curr;
    } else {
        g_board_details.card_info[card_index].device_info[device_id_curr].chip_slot = chip_slot;
    }

    ret = memcpy_s(&g_board_details.card_info[card_index].pcie_info_pre[0], MAX_PCIE_INFO_LENTH,
        &card_pcie_info->root_pcie_info[0], strlen(&card_pcie_info->root_pcie_info[0]) + 1);
    if (ret != EOK) {
        gplog(LOG_ERR, "call memcpy_s failed.%d.", ret);
        return DCMI_ERR_CODE_SECURE_FUN_FAIL;
    }

    ret = memcpy_s(&g_board_details.card_info[card_index].device_info[device_id_curr].chip_pcieinfo[0],
        MAX_PCIE_INFO_LENTH, &card_pcie_info->d_chip_pcie_info[0], strlen(&card_pcie_info->d_chip_pcie_info[0]) + 1);
    if (ret != EOK) {
        gplog(LOG_ERR, "call memcpy_s failed.%d.", ret);
        return DCMI_ERR_CODE_SECURE_FUN_FAIL;
    }

    ret = memcpy_s(&g_board_details.card_info[card_index].device_info[device_id_curr].switch_pcieinfo[0],
        MAX_PCIE_INFO_LENTH, &card_pcie_info->switch_pcie_info[0], strlen(&card_pcie_info->switch_pcie_info[0]) + 1);
    if (ret != EOK) {
        gplog(LOG_ERR, "call memcpy_s failed.%d.", ret);
        return DCMI_ERR_CODE_SECURE_FUN_FAIL;
    }

    return ret;
}

STATIC int dcmi_init_card_info_for_exist(
    int card_index, struct dcmi_card_pcie_info *card_pcie_info, int logic_id, int chip_slot)
{
    int ret, device_id_curr;
    unsigned int device_phy_id;

    device_id_curr = g_board_details.card_info[card_index].device_count;
    g_board_details.card_info[card_index].device_count++;
    g_board_details.card_info[card_index].elabel_pos = DCMI_ELABEL_ACCESS_BY_MCU;
    g_board_details.card_info[card_index].board_id_pos = DCMI_BOARD_ID_ACCESS_BY_MCU;
    g_board_details.card_info[card_index].device_info[device_id_curr].logic_id = logic_id;
    g_board_details.card_info[card_index].device_info[device_id_curr].chip_slot = chip_slot;

    ret = dsmi_get_phyid_from_logicid((unsigned int)logic_id, &device_phy_id);
    if (ret != DSMI_OK) {
        gplog(LOG_ERR, "dsmi_get_phyid_from_logicid failed. err is %d.", ret);
        return dcmi_convert_error_code(ret);
    }
    g_board_details.card_info[card_index].device_info[device_id_curr].phy_id = device_phy_id;

    ret = memcpy_s(&g_board_details.card_info[card_index].device_info[device_id_curr].chip_pcieinfo[0],
        MAX_PCIE_INFO_LENTH,
        &card_pcie_info->d_chip_pcie_info[0],
        strlen(&card_pcie_info->d_chip_pcie_info[0]) + 1);
    if (ret != EOK) {
        gplog(LOG_ERR, "call memcpy_s failed.%d.", ret);
    }

    ret = memcpy_s(&g_board_details.card_info[card_index].device_info[device_id_curr].switch_pcieinfo[0],
        MAX_PCIE_INFO_LENTH, &card_pcie_info->switch_pcie_info[0],
        strlen(&card_pcie_info->switch_pcie_info[0]) + 1);
    if (ret != EOK) {
        gplog(LOG_ERR, "call memcpy_s failed.%d.", ret);
    }

    return ret;
}

STATIC void dcmi_init_card_info(
    int card_id_curr, int bus_id, struct dcmi_card_pcie_info *card_pcie_info, int logic_id, int chip_slot)
{
    int ret, tmp_card_id;

    tmp_card_id = card_id_curr;
    if (tmp_card_id == DCMI_INVALID_CARD_ID) {  // 如果在当前的前一级中找不到，则另立门户
        tmp_card_id = g_board_details.card_count;
        g_board_details.card_count++;

        ret = dcmi_init_card_info_for_new(tmp_card_id, bus_id, card_pcie_info, logic_id, chip_slot);
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "dcmi_init_card_info_for_new. ret is %d.", ret);
        }
    } else {
        ret = dcmi_init_card_info_for_exist(tmp_card_id, card_pcie_info, logic_id, chip_slot);
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "dcmi_init_card_info_for_exist. ret is %d.", ret);
        }
    }
}

STATIC void dcmi_find_card_id_with_pcie(int *card_id, const char *pcie_info)
{
    int find_flag, card_index;
    char *pcie_curr = NULL;

    for (card_index = 0; card_index < g_board_details.card_count; card_index++) {
        pcie_curr = &g_board_details.card_info[card_index].pcie_info_pre[0];
        find_flag = strcmp(pcie_curr, pcie_info);
        if (find_flag == DCMI_OK) {
            *card_id = card_index;
            return;
        }
    }

    *card_id = DCMI_INVALID_CARD_ID;
}

STATIC void dcmi_find_card_id_with_bus_id(int *card_id, int bus_id)
{
    int card_index, card_id_curr;

    for (card_index = 0; card_index < g_board_details.card_count; card_index++) {
        card_id_curr = g_board_details.card_info[card_index].card_id;
        if (card_id_curr == bus_id) {
            *card_id = card_index;
            return;
        }
    }

    *card_id = DCMI_INVALID_CARD_ID;
}

STATIC void dcmi_find_card_id_with_slot_id(int *card_id, int slot_id)
{
    int card_index, card_id_curr;

    for (card_index = 0; card_index < g_board_details.card_count; card_index++) {
        card_id_curr = g_board_details.card_info[card_index].card_id;
        if (card_id_curr == slot_id) {
            *card_id = card_index;
            return;
        }
    }

    *card_id = DCMI_INVALID_CARD_ID;
}

STATIC int dcmi_calc_card_id(struct dcmi_pcie_info_all *pcie_info, int *card_id, int is_without_switch)
{
    unsigned int domain = (unsigned int)pcie_info->domain;
    unsigned int bus = pcie_info->bdf_busid;
    unsigned int rc = pcie_info->bdf_deviceid;
    unsigned int func = pcie_info->bdf_funcid;
    unsigned int chip_num_in_one_card = (unsigned int)dcmi_get_device_count_in_one_card();
    if (chip_num_in_one_card == 0) {
        gplog(LOG_ERR, "chip_num_in_one_card is zero.");
        return DCMI_ERR_CODE_INNER_ERR;
    }

    /* card id is named in this way */
    if (is_without_switch == TRUE && g_board_details.board_id == DCMI_310_CARD_DMPB_BOARD_ID) {
        *card_id = (domain << (unsigned int)DCMI_DOMAIN_SHIFT) + (bus << (unsigned int)DCMI_BUS_ID_SHIFT) +
                   ((rc / chip_num_in_one_card) << (unsigned int)DCMI_RC_ID_SHIFT) + func;
    } else {
        *card_id = (domain << (unsigned int)DCMI_DOMAIN_SHIFT) + (bus << (unsigned int)DCMI_BUS_ID_SHIFT) +
                   (rc << (unsigned int)DCMI_RC_ID_SHIFT) + func;
    }

    return DCMI_OK;
}

int dcmi_trans_pcie_common_id(const char *bus_id_str, int common_len, int common_pos, unsigned int *common_num)
{
    char common_curr[DCMI_PCIE_ID_MIN_LEN] = {0};
    unsigned int tran_common = 0;
    if (common_len >= DCMI_PCIE_ID_MIN_LEN) {
        gplog(LOG_ERR, "common_len exceeds buffer size. common_len: %d, buffer size: %d",
            common_len, DCMI_PCIE_ID_MIN_LEN - 1);
        return DCMI_ERR_CODE_SECURE_FUN_FAIL;
    }  // 防止数组访问越界
    int ret = memcpy_s(&common_curr[0], DCMI_PCIE_ID_MIN_LEN, bus_id_str + common_pos, common_len);
    if (ret != EOK) {
        gplog(LOG_ERR, "call memcpy_s failed.%d.", ret);
        return DCMI_ERR_CODE_SECURE_FUN_FAIL;
    }
    common_curr[common_len] = '\0';

    ret = sscanf_s(common_curr, "%x", &tran_common);
    if (ret <= EOK) {
        gplog(LOG_ERR, "common_curr call sscanf_s failed.%d.", ret);
        return DCMI_ERR_CODE_SECURE_FUN_FAIL;
    }
    *common_num = tran_common;
    return DCMI_OK;
}

STATIC int dcmi_trans_pcie_bus_id_without_switch(const char *bus_id_str, int *value, int is_without_switch)
{
    int ret;
    struct dcmi_pcie_info_all pcie_info = {0};

    if (strlen(bus_id_str) < DCMI_PCIE_ID_MIN_LEN) {
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    ret = dcmi_trans_pcie_common_id(bus_id_str, DCMI_DOMAIN_LEN, 0, (unsigned int *)&pcie_info.domain);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_trans_pcie_common_id failed.%d", ret);
        return ret;
    }
    ret = dcmi_trans_pcie_common_id(bus_id_str, DCMI_BUS_ID_LEN, DCMI_BUS_ID_POS, &pcie_info.bdf_busid);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_trans_pcie_common_id failed.%d", ret);
        return ret;
    }
    ret = dcmi_trans_pcie_common_id(bus_id_str, DCMI_RC_ID_LEN, DCMI_RC_ID_POS, &pcie_info.bdf_deviceid);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_trans_pcie_common_id failed.%d", ret);
        return ret;
    }

    ret = dcmi_trans_pcie_common_id(bus_id_str, DCMI_FUNC_ID_LEN, DCMI_FUNC_ID_POS, &pcie_info.bdf_funcid);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_trans_pcie_common_id failed.%d", ret);
        return ret;
    }

    return dcmi_calc_card_id(&pcie_info, value, is_without_switch);
}

STATIC int dcmi_get_bus_id(const char *pcie_info_pre, int *card_id)
{
    if (dcmi_card_is_without_switch()) {
        return dcmi_trans_pcie_bus_id_without_switch(pcie_info_pre, card_id, TRUE);
    }

    return dcmi_trans_pcie_bus_id_without_switch((const char *)pcie_info_pre, card_id, FALSE);
}

STATIC void dcmi_get_bus_id_from_root_pcie_info(int device_id_logic, struct dcmi_card_pcie_info *card_pcie_info)
{
    int ret, bus_id;
    int card_id_curr = 0;
    struct dsmi_board_info_stru board_info = {0};
    unsigned int health = HEALTH_UNKNOWN;

    ret = dsmi_get_device_health(device_id_logic, &health);
    if (ret != DSMI_OK || health == DCMI_DEVICE_NOT_EXIST) {
        gplog(LOG_ERR, "dsmi_get_device_health error.(logic_id=%d, health=%u, ret=%d).", device_id_logic,
            health, ret);
        return;
    }
    
    ret = dsmi_get_board_info(device_id_logic, &board_info);
    if (ret != DSMI_OK) {
        if (ret != DSMI_ERR_RESOURCE_OCCUPIED) {
                gplog(LOG_ERR, "dsmi_get_board_info failed.%d. %s %d %s.", ret, "get chip", device_id_logic,
                    "board information failed. Reboot OS to repair if necessary");
            }
            return;
        }

    ret = dcmi_get_bus_id((const char *)card_pcie_info->root_pcie_info, &bus_id);
    if (ret != DCMI_OK) {
        bus_id = (int)card_pcie_info->pcie_info_curr.bdf_busid;
    }

    if (dcmi_card_is_without_switch()) {
        if (dcmi_get_board_chip_type() == DCMI_CHIP_TYPE_D910_93) {
            (void)dcmi_find_card_id_with_slot_id(&card_id_curr, board_info.slot_id);
        } else {
            (void)dcmi_find_card_id_with_bus_id(&card_id_curr, bus_id);
        }
    } else {
        (void)dcmi_find_card_id_with_pcie(&card_id_curr, (const char *)&card_pcie_info->root_pcie_info[0]);
    }
    dcmi_init_card_info(card_id_curr, bus_id, card_pcie_info, device_id_logic, board_info.slot_id);
    return;
}

STATIC bool dcmi_matched_pcie_info_exist(char *msginfo, char **matched_d_chip_pcie_info, int matched_size)
{
    for (int i = 0; i < matched_size; i++) {
        if ((matched_d_chip_pcie_info[i] != NULL) && (strstr(msginfo, matched_d_chip_pcie_info[i]) != NULL)) {
            return true;
        }
    }
    return false;
}

STATIC int dcmi_add_matched_pcie_info(char *d_chip_pcie_info, char **matched_d_chip_pcie_info, int *matched_size)
{
    int ret;

    char *curr_pcie_info = (char *)malloc(MAX_PCIE_INFO_LENTH);
    if (curr_pcie_info == NULL) {
        gplog(LOG_ERR, "curr_pcie_info malloc failed.");
        return DCMI_ERR_CODE_INNER_ERR;
    }

    ret = sprintf_s(curr_pcie_info, MAX_PCIE_INFO_LENTH - 1, "%s", d_chip_pcie_info);
    if (ret <= DCMI_OK) {
        gplog(LOG_ERR, "call sprintf_s failed. ret is %d", ret);
        free(curr_pcie_info);
        return DCMI_ERR_CODE_SECURE_FUN_FAIL;
    }

    matched_d_chip_pcie_info[(*matched_size)] = curr_pcie_info;
    (*matched_size)++;
    return DCMI_OK;
}

STATIC void dcmi_free_matched_pcie_info(char **matched_d_chip_pcie_info, int matched_size)
{
    if (matched_d_chip_pcie_info != NULL) {
        for (int i = 0; i < matched_size; i++) {
            if (matched_d_chip_pcie_info[i] != NULL) {
                free(matched_d_chip_pcie_info[i]);
                matched_d_chip_pcie_info[i] = NULL;
            }
        }
        free(matched_d_chip_pcie_info);
        matched_d_chip_pcie_info = NULL;
    }
}

// 当前接口为static仅内部调用，card_pcie_info来源可控，不做重复检查以控制函数在50行内。后续使用需注意
STATIC int dcmi_get_card_id_from_bus_id(const int *device_id_list, int device_count,
    struct dcmi_card_pcie_info *card_pcie_info)
{
    int ret, num_id;
    FILE *pci_file = NULL;
    char msginfo[MAX_PCIE_INFO_LENTH] = {0};
    struct dcmi_device_id_info device_id_info = {0};
    char **matched_d_chip_pcie_info = (char **)malloc(device_count * sizeof(char *));
    int matched_size = 0;

    if (matched_d_chip_pcie_info == NULL) {
        gplog(LOG_ERR, "matched_d_chip_pcie_info malloc failed.");
        return DCMI_ERR_CODE_INNER_ERR;
    }

    ret = dcmi_save_pcie_info(&pci_file);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_save_pcie_info. ret is %d", ret);
        free(matched_d_chip_pcie_info);
        return ret;
    }
#ifndef _WIN32
    while (!feof(pci_file)) {
        if (matched_size >= device_count) {
            gplog(LOG_INFO, "matched_size >= device_count, end reading pci_file.");
            break;
        }
        if ((fgets(msginfo, MAX_PCIE_INFO_LENTH, pci_file) == NULL) ||
            (device_id_info.find_device_num == device_count)) {
            break;
        }
#endif
        // 检查当前PCIE设备文件是否已在之前匹配过
        if (dcmi_matched_pcie_info_exist(msginfo, matched_d_chip_pcie_info, matched_size)) {
            continue;
        }
        for (num_id = 0; num_id < device_count; num_id++) {
            device_id_info.device_id_logic = device_id_list[num_id];
            ret = dcmi_get_pcie_info_inner(msginfo, MAX_PCIE_INFO_LENTH, &device_id_info, card_pcie_info);
            if (ret != DCMI_OK) {
                continue;
            }
            // 将匹配成功的PCIE设备号记入列表
            (void)dcmi_add_matched_pcie_info(card_pcie_info->d_chip_pcie_info, matched_d_chip_pcie_info, &matched_size);
            (void)dcmi_get_bus_id_from_root_pcie_info(device_id_info.device_id_logic, card_pcie_info);
        }
#ifndef _WIN32
    }
    (void)pclose(pci_file);
#endif
    dcmi_free_matched_pcie_info(matched_d_chip_pcie_info, matched_size);
    return DCMI_OK;
}

STATIC void dcmi_get_same_split_num_from_str(char *str_info, const char *split_info, int *num)
{
    char *src_info = str_info;
    char *find_curr = NULL;
    size_t split_len = strlen(split_info);
    int find_num = 0;

    while (1) {
        find_curr = strstr(src_info, split_info);
        if (find_curr == NULL) {
            break;
        }
        src_info = find_curr + split_len;
        find_num++;
    }
    *num = find_num;
    return;
}

STATIC int dcmi_find_board_pcie_num(char *msginfo, int msginfo_len, const char *pci_curr_info, int *device_num)
{
#ifndef _WIN32
    char *find_flag = NULL;
    int split_num = 0;
    int device_num_curr = 0;
    find_flag = strstr(msginfo, pci_curr_info);
    if (find_flag == NULL) {
        return DCMI_ERR_CODE_INNER_ERR;
    }
    dcmi_get_same_split_num_from_str(find_flag, "/", &split_num);
    if (split_num == DCMI_SPLIT_NUM) {
        device_num_curr++;
    }
    *device_num = device_num_curr;
#else
    const int board_pcie_num = 2;
    *device_num = board_pcie_num;
#endif
    return DCMI_OK;
}

STATIC void dcmi_flush_pcie_device()
{
    int ret, card_index;
    int device_num = 0;
    struct dcmi_card_info *card_info = NULL;
    char msginfo[MAX_PCIE_INFO_LENTH] = {0};
    FILE *pci_file = NULL;

    if (dcmi_get_board_type() != DCMI_BOARD_TYPE_CARD) {
        return;
    }
#ifndef _WIN32
    ret = dcmi_save_pcie_info(&pci_file);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_save_pcie_info. ret is %d", ret);
        return;
    }
    while (!feof(pci_file)) {
        if ((fgets(msginfo, MAX_PCIE_INFO_LENTH, pci_file) == NULL)) {
            break;
        }
#endif
        for (card_index = 0; card_index < g_board_details.card_count; card_index++) {
            card_info = &g_board_details.card_info[card_index];
            ret = dcmi_find_board_pcie_num(msginfo, MAX_PCIE_INFO_LENTH, &card_info->pcie_info_pre[0], &device_num);
            if ((ret == 0) && (device_num > card_info->device_count)) {
                card_info->device_loss = device_num - card_info->device_count;
            } else if (ret == DCMI_ERR_CODE_INNER_ERR) {
                continue;
            } else {
                card_info->device_loss = 0;
            }
        }
#ifndef _WIN32
    }
    (void)pclose(pci_file);
#endif
    return;
}

int dcmi_init_for_card(const int *device_id_list, int device_count)
{
    int ret;
    struct dcmi_card_pcie_info card_pcie_info = { { 0 } };
    g_board_details.device_count = device_count;

    ret = dcmi_get_card_id_from_bus_id(device_id_list, device_count, &card_pcie_info);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_card_id_from_bus_id failed. ret is %d", ret);
        return ret;
    }

    dcmi_flush_pcie_device();
    g_board_details.is_has_mcu = !(dcmi_board_chip_type_is_ascend_310p_200i_pro() ||
        (dcmi_board_chip_type_is_ascend_310() && !dcmi_is_in_phy_machine()));
    g_board_details.is_has_npu = TRUE;
    g_board_details.mcu_access_chan = DCMI_MCU_ACCESS_BY_NPU;
    return DCMI_OK;
}