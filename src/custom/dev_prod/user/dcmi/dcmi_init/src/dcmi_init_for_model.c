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
#include <fcntl.h>
#ifndef _WIN32
#include <unistd.h>
#include <sys/mman.h>
#endif

#include "securec.h"
#include "dsmi_common_interface_custom.h"
#include "dcmi_os_adapter.h"
#include "dcmi_fault_manage_intf.h"
#include "dcmi_log.h"
#include "dcmi_mcu_intf.h"
#include "dcmi_common.h"
#include "dcmi_inner_info_get.h"
#include "dcmi_product_judge.h"
#include "dcmi_environment_judge.h"
#include "dcmi_init_basic.h"


STATIC void dcmi_init_board_info_by_minid_present(int present)
{
    g_board_details.device_count = (present ? 1 : 0);
    g_board_details.card_info[0].device_count = (present ? 1 : 0);
    g_board_details.is_has_npu = (present ? TRUE : FALSE);
}

STATIC int dcmi_get_mcu_flag(void)
{
    FILE *fp = NULL;
    char buf[DCMI_CFG_LINE_MAX_LEN] = {0};
    char *p = NULL;
    char *end_str = NULL;
    int read_count, mcu_flag;
    long result;

    fp = fopen(BOARD_CONFIG_FILE, "r");
    if (fp == NULL) {
        return 1; // In order to be compatible with the old version of station, when the file is not exist, return 1
    }

    if (fseek(fp, 0, SEEK_SET) != 0) {
        gplog(LOG_ERR, "call fseek failed.");
        (void)fclose(fp);
        return DCMI_ERR_CODE_FILE_OPERATE_FAIL;
    }

    read_count = (int)fread(buf, 1, sizeof(buf) - 1, fp);
    if (read_count <= 0) {
        gplog(LOG_ERR, "call fread failed.read_count=%d", read_count);
        (void)fclose(fp);
        return DCMI_ERR_CODE_FILE_OPERATE_FAIL;
    }

    buf[DCMI_CFG_LINE_MAX_LEN - 1] = '\0';
    p = strstr(buf, "mcu_flag=");
    if (p != NULL) {
        p = p + strlen("mcu_flag=");
        result = strtol(p, &end_str, DCMI_NUMBER_BASE);
        mcu_flag = (result == TRUE) ? TRUE : FALSE;
    } else {
        mcu_flag = FALSE;
    }

    (void)fclose(fp);
    fp = NULL;

    return mcu_flag;
}

STATIC int dcmi_init_for_station(void)
{
    int ret;

    g_board_details.mcu_access_chan = DCMI_MCU_ACCESS_DIRECT;
    g_board_details.card_count = 1;
    g_board_details.card_info[0].card_id = 0;
    g_board_details.card_info[0].device_count = 1;
    g_board_details.card_info[0].device_info[0].logic_id = 0;
    g_board_details.card_info[0].board_id_pos = DCMI_BOARD_ID_ACCESS_BY_CPU;
    g_board_details.card_info[0].elabel_pos = DCMI_ELABEL_ACCESS_BY_CPU;

    if ((dcmi_board_type_is_station() == TRUE) && (dcmi_get_mcu_flag() == TRUE)) {
        g_board_details.is_has_mcu = TRUE;
        if (dcmi_check_run_in_docker() == TRUE) {
            g_board_details.is_has_mcu = FALSE;
        }
    } else {
        g_board_details.is_has_mcu = FALSE;
    }

    ret = dcmi_flush_device_id();
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_flush_device_id failed. ret is %d", ret);
        return ret;
    }

    if (access("/run/minid_not_present", F_OK) == 0) {
        dcmi_init_board_info_by_minid_present(DCMI_MODEL_NOT_PRESENT);
    } else {
        dcmi_init_board_info_by_minid_present(DCMI_MODEL_PRESENT);
    }

    if (dcmi_get_sub_board_type() != DCMI_BOARD_TYPE_MODEL_HILENS) {
        /* hilens 单板无MCU，必然失败，执行npu-smi命令时存在异常打印，此处只给hilens以外单板用 */
        ret = dcmi_mcu_get_board_id(0, (unsigned int *)(&g_board_details.board_id));
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "call dcmi_mcu_get_board_id fail! ret = %d.", ret);
        }

        ret = dcmi_mcu_get_bom_id(0, &g_board_details.bom_id);
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "call dcmi_mcu_get_bom_id fail! ret = %d.", ret);
        }
    }

    return DCMI_OK;
}

int dcmi_init_for_model(const int *device_id_list, int device_count)
{
    int ret, num_id, device_id_logic;
    struct dsmi_board_info_stru board_info = {0};
    unsigned int device_phy_id;
    unsigned int health = HEALTH_UNKNOWN;

    if ((dcmi_board_type_is_station() == TRUE) || (dcmi_board_type_is_hilens() == TRUE)) {
        return dcmi_init_for_station();
    }

    g_board_details.is_has_mcu = FALSE;
    g_board_details.is_has_npu = TRUE;
    g_board_details.mcu_access_chan = DCMI_MCU_ACCESS_INVALID;
    g_board_details.device_count = device_count;

    for (num_id = 0; num_id < device_count; num_id++) {
        device_id_logic = device_id_list[num_id];
        ret = dsmi_get_device_health(device_id_logic, &health);
        if (ret != DSMI_OK || health == DCMI_DEVICE_NOT_EXIST) {
            gplog(LOG_ERR, "dsmi_get_device_health error.(logic_id=%d, health=%u, ret=%d).",
                device_id_logic, health, ret);
            continue;
        }

        ret = dsmi_get_board_info(device_id_logic, &board_info);
        if (ret != DSMI_OK) {
            gplog(LOG_ERR,
                "dsmi_get_board_info failed.%d. %s %d %s.", ret, "get chip", device_id_logic,
                "board information failed. Reboot OS to repair if necessary");
            continue;
        }

        if (num_id == 0) {
            g_board_details.board_id = (int)board_info.board_id;
        }
        g_board_details.card_info[num_id].card_id = num_id;
        g_board_details.card_info[num_id].device_count = 1;
        g_board_details.card_info[num_id].device_info[0].logic_id = device_id_logic;
        ret = dsmi_get_phyid_from_logicid((unsigned int)device_id_logic, &device_phy_id);
        if (ret != DSMI_OK) {
            gplog(LOG_ERR, "dsmi_get_phyid_from_logicid failed. err is %d.", ret);
            return dcmi_convert_error_code(ret);
        }
        g_board_details.card_info[num_id].device_info[0].phy_id = device_phy_id;
        g_board_details.card_info[num_id].elabel_pos =
            (dcmi_board_type_is_soc_develop()) ? DCMI_ELABEL_ACCESS_BY_CPU : DCMI_ELABEL_ACCESS_BY_NPU;
        g_board_details.card_info[num_id].board_id_pos =
            (dcmi_board_type_is_soc_develop()) ? DCMI_ELABEL_ACCESS_BY_CPU : DCMI_ELABEL_ACCESS_BY_NPU;
        g_board_details.card_count++;
    }
    return DCMI_OK;
}