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
#include <fcntl.h>
#include <math.h>
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
#include "dcmi_init_basic.h"
#include "dcmi_product_judge.h"
#include "dcmi_virtual_intf.h"
#include "dcmi_permission_judge.h"
#include "dcmi_i2c_operate.h"
#include "dcmi_environment_judge.h"
#include "dcmi_inner_cfg_manage.h"
#include "dcmi_inner_info_get.h"

static struct dcmi_computing_template g_310p_24G_template[DCMI_310P_TEMPLATE_NUM] = {
    // name            spec      aic   mem (MB)      aicpu    vpc    venc    vdec    jpegd    jpege    pngd
    {"vir01",          "1/8",    1,    0xC00,           1,       1,     0,      1,      2,       1,       0},
    {"vir02",          "1/4",    2,    0x1800,          2,       3,     1,      3,      4,       2,       0},
    {"vir02_1c",       "1/4",    2,    0x1800,          1,       3,     0,      3,      4,       2,       0},
    {"vir04",          "1/2",    4,    0x3000,          4,       6,     2,      6,      8,       4,       0},
    {"vir04_3c",       "1/2",    4,    0x3000,          3,       6,     1,      6,      8,       4,       0},
    {"vir04_3c_ndvpp", "1/2",    4,    0x3000,          3,       0,     0,      0,      0,       0,       0},
    {"vir04_4c_dvpp",  "1/2",    4,    0x3000,          4,       12,    3,      12,     16,      8,       0},
};

static struct dcmi_computing_template g_310p_48G_template[DCMI_310P_TEMPLATE_NUM] = {
    // name            spec      aic   mem (MB)      aicpu    vpc    venc    vdec    jpegd    jpege    pngd
    {"vir01",          "1/8",    1,    0x1800,          1,       1,     0,      1,      2,       1,       0},
    {"vir02",          "1/4",    2,    0x3000,          2,       3,     1,      3,      4,       2,       0},
    {"vir02_1c",       "1/4",    2,    0x3000,          1,       3,     0,      3,      4,       2,       0},
    {"vir04",          "1/2",    4,    0x6000,          4,       6,     2,      6,      8,       4,       0},
    {"vir04_3c",       "1/2",    4,    0x6000,          3,       6,     1,      6,      8,       4,       0},
    {"vir04_3c_ndvpp", "1/2",    4,    0x6000,          3,       0,     0,      0,      0,       0,       0},
    {"vir04_4c_dvpp",  "1/2",    4,    0x6000,          4,       12,    3,      12,     16,      8,       0},
};

static struct dcmi_computing_template g_910_32G_template[DCMI_910_TEMPLATE_NUM] = {
    // 2个CtrlCPU 14个AICPU
    // name            spec      aic   mem (MB)      aicpu    vpc    venc    vdec    jpegd    jpege    pngd
    {"vir02",          "1/16",   2,    0x800,           1,       1,     0,      1,      1,       0,       1.5},
    {"vir04",          "1/8",    4,    0x1000,          1,       2,     0,      2,      2,       1,       3},
    {"vir08",          "1/4",    8,    0x2000,          3,       4,     0,      4,      4,       2,       6},
    {"vir16",          "1/2",    16,   0x4000,          7,       8,     0,      8,      8,       4,       12},
};

static struct dcmi_computing_template g_910_16G_template[DCMI_910_TEMPLATE_NUM] = {
    // 2个CtrlCPU 14个AICPU
    // name            spec      aic   mem (MB)      aicpu    vpc    venc    vdec    jpegd    jpege    pngd
    {"vir02",          "1/16",   2,    0x400,           1,       1,     0,      1,      1,       0,       1.5},
    {"vir04",          "1/8",    4,    0x800,           1,       2,     0,      2,      2,       1,       3},
    {"vir08",          "1/4",    8,    0x1000,          3,       4,     0,      4,      4,       2,       6},
    {"vir16",          "1/2",    16,   0x2000,          7,       8,     0,      8,      8,       4,       12},
};

static struct dcmi_computing_template g_910B_bin0_show_template[DCMI_910B_BIN0_TEMPLATE_NUM] = {
    // name             spec      aic   mem (MB)      aicpu    vpc    venc    vdec    jpegd    jpege    pngd
    {"vir12_3c_32g",    "1/2",    12,   0x8000,        3,       5,     0,      1,      14,      2,       0},
    {"vir06_1c_16g",    "1/4",    6,    0x4000,        1,       2,     0,      0,      7,       1,       0},
};

static struct dcmi_computing_template g_910B_bin2_show_template[DCMI_910B_BIN2_TEMPLATE_NUM] = {
    // name                spec      aic   mem (MB)      aicpu    vpc    venc    vdec    jpegd    jpege    pngd
    {"vir10_3c_16g",       "1/2",    10,   0x4000,        3,       4,     0,      1,      12,      2,       0},
    {"vir10_4c_16g_m",     "1/2",    10,   0x4000,        4,       9,     0,      2,      24,      4,       0},
    {"vir10_3c_16g_nm",    "1/2",    10,   0x4000,        3,       0,     0,      0,      0,       0,       0},
    {"vir05_1c_8g",        "1/4",    5,    0x2000,        1,       2,     0,      0,      6,       1,       0},
};

static struct dcmi_computing_template g_910B_bin1_show_template[DCMI_910B_BIN1_TEMPLATE_NUM] = {
    // name             spec      aic   mem (MB)      aicpu    vpc    venc    vdec    jpegd    jpege    pngd
    {"vir10_3c_32g",    "1/2",    10,   0x8000,        3,       4,     0,      1,      12,      2,       0},
    {"vir05_1c_16g",    "1/4",    5,    0x4000,        1,       2,     0,      0,      6,       1,       0},
};

static struct dcmi_computing_template g_910B_bin0_template[DCMI_910B_BIN0_TEMPLATE_NUM] = {
    // name             spec      aic   mem (MB)      aicpu    vpc    venc    vdec    jpegd    jpege    pngd
    {"vir12_3c_32g",    "1/2",    12,   0x7800,        3,       5,     0,      1,      14,      2,       0},
    {"vir06_1c_16g",    "1/4",    6,    0x3C00,        1,       2,     0,      0,      7,       1,       0},
};

static struct dcmi_computing_template g_910B_bin2_template[DCMI_910B_BIN2_TEMPLATE_NUM] = {
    // name                 spec      aic   mem (MB)      aicpu    vpc    venc    vdec    jpegd    jpege    pngd
    {"vir10_3c_16g",       "1/2",    10,   0x3800,        3,       4,     0,      1,      12,      2,       0},
    {"vir10_4c_16g_m",     "1/2",    10,   0x3800,        4,       9,     0,      2,      24,      4,       0},
    {"vir10_3c_16g_nm",    "1/2",    10,   0x3800,        3,       0,     0,      0,      0,       0,       0},
    {"vir05_1c_8g",        "1/4",    5,    0x1C00,        1,       2,     0,      0,      6,       1,       0},
};

static struct dcmi_computing_template g_910B_bin1_template[DCMI_910B_BIN1_TEMPLATE_NUM] = {
    // name             spec      aic   mem (MB)      aicpu    vpc    venc    vdec    jpegd    jpege    pngd
    {"vir10_3c_32g",    "1/2",    10,   0x7800,        3,       4,     0,      1,      12,      2,       0},
    {"vir05_1c_16g",    "1/4",    5,    0x3C00,        1,       2,     0,      0,      6,       1,       0},
};

static struct dcmi_product_computing_template g_dcmi_product_template[] = {
    {DCMI_CHIP_TYPE_D310P, DCMI_MEM_SIZE_24G, DCMI_310P_TEMPLATE_NUM, g_310p_24G_template},
    {DCMI_CHIP_TYPE_D310P, DCMI_MEM_SIZE_48G, DCMI_310P_TEMPLATE_NUM, g_310p_48G_template},
    {DCMI_CHIP_TYPE_D910, DCMI_MEM_SIZE_16G, DCMI_910_TEMPLATE_NUM, g_910_16G_template},
    {DCMI_CHIP_TYPE_D910, DCMI_MEM_SIZE_32G, DCMI_910_TEMPLATE_NUM, g_910_32G_template},
};

static struct dcmi_product_computing_template_for_910B g_dcmi_product_template_for_910B[] = {
    {DCMI_910B_BIN0, DCMI_910B_BIN0_TEMPLATE_NUM, g_910B_bin0_template},
    {DCMI_910B_BIN2, DCMI_910B_BIN2_TEMPLATE_NUM, g_910B_bin2_template},
    {DCMI_910B_BIN1, DCMI_910B_BIN1_TEMPLATE_NUM, g_910B_bin1_template},
    {DCMI_910B_BIN3, DCMI_910B_BIN0_TEMPLATE_NUM, g_910B_bin0_template}, // 复用bin0的模板
};

static struct dcmi_product_computing_template_for_910B g_dcmi_product_show_template_for_910B[] = {
    {DCMI_910B_BIN0, DCMI_910B_BIN0_TEMPLATE_NUM, g_910B_bin0_show_template},
    {DCMI_910B_BIN2, DCMI_910B_BIN2_TEMPLATE_NUM, g_910B_bin2_show_template},
    {DCMI_910B_BIN1, DCMI_910B_BIN1_TEMPLATE_NUM, g_910B_bin1_show_template},
    {DCMI_910B_BIN3, DCMI_910B_BIN0_TEMPLATE_NUM, g_910B_bin0_show_template}, // 复用bin0的模板
};

int dcmi_get_card_info(int card_id, struct dcmi_card_info **card_info)
{
    int card_index;
    for (card_index = 0; card_index < g_board_details.card_count; card_index++) {
        if (card_id == g_board_details.card_info[card_index].card_id) {
            *card_info = &g_board_details.card_info[card_index];
            return DCMI_OK;
        }
    }
    gplog(LOG_ERR, "Can not find card %d info.", card_id);
    return DCMI_ERR_CODE_INVALID_PARAMETER;
}

int dcmi_get_board_id_inner(void)
{
    return g_board_details.board_id;
}

int dcmi_get_board_chip_type(void)
{
    return g_board_details.chip_type;
}

int dcmi_get_board_type(void)
{
    return g_board_details.board_type;
}

int dcmi_get_sub_board_type(void)
{
    return g_board_details.sub_board_type;
}

unsigned int dcmi_get_maindboard_id_inner(void)
{
    return g_mainboard_info.mainboard_id;
}

int dcmi_get_device_count_in_one_card()
{
    return g_board_details.device_count_in_one_card;
}

int get_card_board_info(struct dcmi_card_info *card_info, unsigned int *slot_id)
{
    int ret, i;
    struct dcmi_board_info board_info = { 0 };
    for (i = 0; i < card_info->device_count; i++) {
        ret = dcmi_get_device_board_info(card_info->card_id, i, &board_info);
        if ((ret == DCMI_OK) && (board_info.slot_id != -1)) {
            *slot_id = board_info.slot_id;
            return ret;
        }
    }

    return DCMI_ERR_CODE_INNER_ERR;
}

int dcmi_get_mcu_access_chan(void)
{
    return g_board_details.mcu_access_chan;
}

int dcmi_get_card_board_id(int card_id, int *board_id)
{
    int num_id;
    struct dcmi_card_info *card_info = NULL;

    if (dcmi_get_run_env_init_flag() != TRUE) {
        gplog(LOG_ERR, "dcmi is not init.");
        return DCMI_ERR_CODE_NOT_REDAY;
    }

    for (num_id = 0; num_id < g_board_details.card_count; num_id++) {
        card_info = &g_board_details.card_info[num_id];
        if (card_info->card_id == card_id) {
            *board_id = card_info->card_board_id;
            return DCMI_OK;
        }
    }

    return DCMI_ERR_CODE_INNER_ERR;
}

int dcmi_get_pcie_slot(int card_id, int *pcie_slot)
{
    int num_id;
    struct dcmi_card_info *card_info = NULL;

    for (num_id = 0; num_id < g_board_details.card_count; num_id++) {
        card_info = &g_board_details.card_info[num_id];
        if (card_info->card_id == card_id) {
            *pcie_slot = card_info->slot_id;
            return DCMI_OK;
        }
    }
    return DCMI_ERR_CODE_INNER_ERR;
}

int dcmi_get_board_id_pos_in_card(int card_id, int *board_id_pos)
{
    int num_id;
    struct dcmi_card_info *card_info = NULL;

    if (dcmi_get_run_env_init_flag() != TRUE) {
        gplog(LOG_ERR, "not init.\n");
        return DCMI_ERR_CODE_NOT_REDAY;
    }

    for (num_id = 0; num_id < g_board_details.card_count; num_id++) {
        card_info = &g_board_details.card_info[num_id];
        if (card_info->card_id == card_id) {
            *board_id_pos = card_info->board_id_pos;
            return DCMI_OK;
        }
    }
    return DCMI_ERR_CODE_INVALID_PARAMETER;
}

int dcmi_get_elabel_pos_in_card(int card_id, int *elabel_pos)
{
    int num_id;
    struct dcmi_card_info *card_info = NULL;

    if (dcmi_get_run_env_init_flag() != TRUE) {
        gplog(LOG_ERR, "not init.\n");
        return DCMI_ERR_CODE_NOT_REDAY;
    }

    for (num_id = 0; num_id < g_board_details.card_count; num_id++) {
        card_info = &g_board_details.card_info[num_id];
        if (card_info->card_id == card_id) {
            *elabel_pos = card_info->elabel_pos;
            return DCMI_OK;
        }
    }

    return DCMI_ERR_CODE_INVALID_PARAMETER;
}

void dcmi_get_elabel_item_it(int *sn_id, int *mf_id, int *pn_id, int *model_id)
{
    *sn_id = DCMI_ELABEL_SN;
    *mf_id = DCMI_ELABEL_MF;

    if (dcmi_board_type_is_card() || dcmi_board_type_is_server()) {
        *pn_id = DCMI_ELABEL_PN;
        *model_id = DCMI_ELABEL_MODEL;
    } else {
        *pn_id = DCMI_ELABEL_PN_MODEL_TYPE;
        *model_id = DCMI_ELABEL_MODEL_MODEL_TYPE;
    }
    return;
}

int dcmi_get_product_type_inner(void)
{
    return g_board_details.product_type;
}

// 获取310b芯片算力规格，invalid id按8T返回
int dcmi_get_310b_tops_type()
{
    int board_id = dcmi_get_board_id_inner();
    int model_id = board_id % 1000; // 取余1000得到后三位为模组id

    if ((model_id >= A200_A2_MODEL_20T_ID_MIN && model_id <= A200_A2_MODEL_20T_ID_MAX) ||
        (model_id >= A200_A2_XP_20T_ID_MIN && model_id <= A200_A2_XP_20T_ID_MAX)) {
        return IS_310B_20TOPS_TYPE;
    }
    return IS_310B_8TOPS_TYPE;
}

int dcmi_is_has_mcu(void)
{
    return g_board_details.is_has_mcu;
}

int dcmi_get_boot_status(unsigned int mode, int device_id)
{
    int ret;
    unsigned int health = HEALTH_UNKNOWN;
    struct tag_pcie_idinfo str_pci_info = { 0 };

    // rc能执行NPU-SMI时，D芯片已经启动OK
    if (mode == DCMI_PCIE_RC_MODE) {
        return DCMI_OK;
    }

    ret = dsmi_get_pcie_info(device_id, &str_pci_info);
    if ((ret != DSMI_OK) && (ret != DSMI_ERR_NOT_SUPPORT)) {
        gplog(LOG_ERR, "dcmi_get_boot_status ret %d  device %d", ret, device_id);
        return DCMI_ERR_CODE_INNER_ERR;
    }

    ret = dsmi_get_device_health(device_id, &health);  // 每个芯片均获的device_health
    if (ret != DSMI_OK || health == DCMI_DEVICE_NOT_EXIST) {
        gplog(LOG_ERR, "dsmi_get_device_health error.(logic_id=%d, health=%u, ret=%d).", device_id,
            health, ret);
        return DCMI_ERR_CODE_INNER_ERR;
    }

    return DCMI_OK;
}

STATIC int query_device_hccs_status_with_others(int device_logic_id, int device_logic_count,
    const int *device_logic_list, int *hccs_status)
{
    int ret;
    int i;
    int hccs_status_one_device = HCCS_ON;
    unsigned int mode = 0;

    ret = dcmi_get_rc_ep_mode(&mode);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "Failed to get rc ep mode. (ret=%d)", ret);
        return dcmi_convert_error_code(ret);
    }

    for (i = 0; i < device_logic_count; i++) {
        if (device_logic_list[i] == device_logic_id) {
            continue;
        }
        if (dcmi_get_boot_status(mode, device_logic_list[i]) != DCMI_OK) {
            continue;
        }
        // hccs_status_one_device HCCS_ON(1) 表示hccs互联状态， HCCS_OFF(0)表示hccs互联状态断开
        ret = dsmi_get_hccs_status(device_logic_id, device_logic_list[i], &hccs_status_one_device);
        if (ret != DSMI_OK) {
            gplog(LOG_ERR, "Get device hccs status failed. device_id1=%d, device_id2=%d, err=%d",
                device_logic_id, device_logic_list[i], ret);
            return ret;
        }
        // 有一个HCCS_ON就是HCCS_ON，只有HCCS全部断才能输出为HCCS_OFF
        if (hccs_status_one_device == HCCS_ON) {
            *hccs_status = HCCS_ON;
            return DCMI_OK;
        }
    }
    *hccs_status = HCCS_OFF;

    return DCMI_OK;
}

int dcmi_get_hccs_status(int card_id, int device_id, int *hccs_status)
{
    int ret;
    int device_logic_id = 0;
    int device_logic_count = 0;
    int *device_logic_list = NULL;

    ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "Failed to invoke dcmi_get_device_logic_id. err is %d.", ret);
        return ret;
    }

    ret = dsmi_get_device_count(&device_logic_count);
    if (ret != DSMI_OK || device_logic_count == 0) {
        gplog(LOG_ERR, "Failed to get device count. err is %d.", ret);
        return ret;
    }

    device_logic_list = (int *)malloc(device_logic_count * sizeof(int));
    if (device_logic_list == NULL) {
        gplog(LOG_ERR, "Failed to invoke malloc function.");
        return DCMI_ERR_CODE_MEM_OPERATE_FAIL;
    }

    ret = memset_s(device_logic_list, device_logic_count * sizeof(int), INVALID_DEVICE_ID,
        device_logic_count * sizeof(int));
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "Invoke memset_s failed. err is %d.", ret);
        goto device_list_resource_free;
    }

    ret = dsmi_list_device(device_logic_list, device_logic_count);
    if (ret != DSMI_OK) {
        gplog(LOG_ERR, "Failed to get device list. err is %d.", ret);
        goto device_list_resource_free;
    }

    // 查询device hccs互联状态，0表示互联
    ret = query_device_hccs_status_with_others(device_logic_id, device_logic_count, device_logic_list, hccs_status);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "Failed to query hccs_status of device. err is %d.", ret);
        goto device_list_resource_free;
    }

device_list_resource_free:
    free(device_logic_list);
    device_logic_list = NULL;
    return ret;
}

int dcmi_get_rc_ep_mode(unsigned int *mode)
{
    if (mode == NULL) {
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

#ifdef _WIN32
    *mode = DCMI_PCIE_EP_MODE;
#else
    int ret;

    ret = drvGetPlatformInfo(mode);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_rc_ep_mode failed. ret is %d", ret);
    }
    return ret;
#endif
    return DCMI_OK;
}

// 获取version.info的相关字段内容
STATIC int dcmi_version_info_by_field(FILE* fp, const char* field, unsigned char* item_out, unsigned int len)
{
    int ret;
    char line[COMPAT_ITEM_SIZE_MAX] = {0};
    char* item_field = NULL;
    char* item = NULL;

    if ((fp == NULL) || (field == NULL) || (item_out == NULL)) {
        gplog(LOG_ERR, "The input parameters are incorrect.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }
    (void)memset_s(item_out, sizeof(unsigned char) * len, 0, sizeof(unsigned char) * len);

    /* Read and judge fields by row */
    while (fgets(line, COMPAT_ITEM_SIZE_MAX, fp) != NULL) {
        item_field = strstr(line, field);
        if (item_field != NULL) {
            item = strchr(line, '=');
            break;
        }
    }

    if ((item_field == NULL) || (item == NULL) || (*(item + 1) == '\n')) {
        gplog(LOG_INFO, "Do not find field: %s", field);
        return DCMI_ERR_CODE_RECV_MSG_FAIL;
    }

    if ((strlen(item_field) > (COMPAT_ITEM_SIZE_MAX - 1)) || (strlen(item_field) > len)) {
        gplog(LOG_ERR, "The field content is too long.");
        return DCMI_ERR_CODE_INNER_ERR;
    }

    item++; // 跳过空格
    ret = strcpy_s((char*)item_out, len, item);
    if (ret != EOK) {
        return DCMI_ERR_CODE_SECURE_FUN_FAIL;
    }

    return DCMI_OK;
}

STATIC int dcmi_get_install_path(const char* field, unsigned char* item_out, unsigned int len)
{
    int ret;
    unsigned char buf_install_path[COMPAT_ITEM_SIZE_MAX] = {0};
    int str_len = 0;
    int i = 0;
    FILE* fp = NULL;

    if ((field == NULL) || (item_out == NULL)) {
        gplog(LOG_ERR, "Field or item_out is NULL.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }
    (void)memset_s(item_out, sizeof(unsigned char) * len, 0, sizeof(unsigned char) * len);

    // get install path of driver
    fp = fopen((const char *)DRV_INSTALL_PATH_INFO, "r");
    if (fp == NULL) {
        gplog(LOG_ERR, "Fopen file[%s] failed.", DRV_INSTALL_PATH_INFO);
        return DCMI_ERR_CODE_FILE_OPERATE_FAIL;
    }

    ret = dcmi_version_info_by_field(fp, field, buf_install_path, COMPAT_ITEM_SIZE_MAX);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_version_info_by_field failed,  ret %d.", ret);
        goto install_path_free_fp;
    }

    // 去掉换行
    str_len = strlen((const char*)buf_install_path);
    if (str_len >= COMPAT_ITEM_SIZE_MAX) {
        ret = DCMI_ERR_CODE_INNER_ERR;
        gplog(LOG_ERR, "path len failed,  ret %d.", ret);
        goto install_path_free_fp;
    }
    for (i = 0; i < str_len; i++) {
        if (buf_install_path[i] == '\n') {
            buf_install_path[i] = '\0';
            break;
        }
    }

    ret = sprintf_s((char*)item_out, len, "%s%s", buf_install_path, DRV_VERSION_INFO_PATH);
    if (ret < 0) {
        gplog(LOG_ERR, "Call sprintf_s failed. ret %d", ret);
        ret = DCMI_ERR_CODE_SECURE_FUN_FAIL;
        goto install_path_free_fp;
    }
    ret = DCMI_OK;

install_path_free_fp:
    (void)fclose(fp);
    fp = NULL;
    return ret;
}

// 获取驱动version.info的相关字段内容
int dcmi_version_info_of_drv_by_field(const char* field, unsigned char* item_out, unsigned int len)
{
    int ret;
    char drv_install_path[COMPAT_ITEM_SIZE_MAX] = {0};
    char drv_install_path_real[PATH_MAX + 1] = {0};
    FILE* fp = NULL;

    if ((field == NULL) || (item_out == NULL)) {
        gplog(LOG_ERR, "Field or item_out is NULL.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }
    (void)memset_s(item_out, sizeof(unsigned char) * len, 0, sizeof(unsigned char) * len);

    // get install path of drvier
    ret = dcmi_get_install_path(DRV_INSTALL_PATH_FIELD, (unsigned char*)drv_install_path, COMPAT_ITEM_SIZE_MAX);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "Get install path of driver failed.");
        return ret;
    }

    if (realpath(drv_install_path, drv_install_path_real) == NULL && errno != ENOENT) {
        gplog(LOG_ERR, "realpath error. errno is %d", errno);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    fp = fopen((const char *)drv_install_path_real, "r");
    if (fp == NULL) {
        gplog(LOG_ERR, "Fopen file[%s] failed.", drv_install_path_real);
        return DCMI_ERR_CODE_FILE_OPERATE_FAIL;
    }

    ret = dcmi_version_info_by_field(fp, field, item_out, len);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_version_info_by_field failed,  ret %d.", ret);
        goto free_fp;
    }
    ret = DCMI_OK;

free_fp:
    (void)fclose(fp);
    fp = NULL;
    return ret;
}

int dcmi_get_firmware_version(int card_id, int device_id, unsigned char* firmware_version, int len_firmware_version)
{
    int ret;
    // 查询一个组件版本---后续可支持查询多个组件版本
    enum dcmi_component_type comp = DCMI_UPGRADE_ALL_COMPONENT;
    unsigned int len_comp = sizeof(comp) / sizeof(enum dcmi_component_type);

    ret = dcmi_get_device_component_list(card_id, device_id, &comp, len_comp);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_component_list failed. card_id=%d, device_id=%d, err=%d",
            card_id, device_id, ret);
        return ret;
    }
    ret = dcmi_get_device_component_static_version(card_id, device_id, comp,
        firmware_version, len_firmware_version);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "get firmware version failed. card_id=%d, device_id=%d, err=%d", card_id, device_id, ret);
        return ret;
    }

    return DCMI_OK;
}

int dcmi_get_rootkey_status(int card_id, int device_id, unsigned int key_type, unsigned int *rootkey_status)
{
    int ret;
    int device_logic_id = 0;
    enum dcmi_unit_type device_type = INVALID_TYPE;

    if (rootkey_status == NULL) {
        gplog(LOG_ERR, "The rootkey_status is null.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }
    if (key_type != 0) {
        gplog(LOG_ERR, "The key_type %u is not support. card_id=%d, device_id=%d.",
            key_type, card_id, device_id);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    ret = dcmi_get_device_type(card_id, device_id, &device_type);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "Get device type failed. card_id=%d, device_id=%d, err=%d.", card_id, device_id, ret);
        return ret;
    }
    // 只支持910B
    if (dcmi_board_chip_type_is_ascend_910b() != TRUE) {
        gplog(LOG_ERR, "This device does not support querying rootkey status.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    // 仅支持NPU
    if (device_type == NPU_TYPE) {
        ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "Failed to invoke dcmi_get_device_logic_id. card_id=%d, device_id=%d, err=%d.",
                card_id, device_id, ret);
            return ret;
        }

        ret = dsmi_get_rootkey_status((unsigned int)device_logic_id, key_type, rootkey_status);
        if (ret != DSMI_OK) {
            gplog(LOG_ERR, "Failed to invoke dsmi_get_rootkey_status. card_id=%d, device_id=%d, err=%d.",
                card_id, device_id, ret);
            return DCMI_ERR_CODE_INNER_ERR;
        }
    } else {
        gplog(LOG_ERR, "The device_type %d is not support.", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    return DCMI_OK;
}

STATIC int dcmi_get_chip_memory_size(int card_id, unsigned int *mem_size)
{
    int chip_index;
    int device_count = 0;
    int mcu_id = 0;
    int cpu_id = 0;
    struct dcmi_get_memory_info_stru memory_info = { 0 };
    int ret = 0;
 
    ret = dcmi_get_device_id_in_card(card_id, &device_count, &mcu_id, &cpu_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_id_in_card card %d failed. err is %d", card_id, ret);
        return ret;
    }
 
    for (chip_index = 0; chip_index < device_count; chip_index++) {
        ret = dcmi_get_device_memory_info_v3(card_id, chip_index, &memory_info);
        if (ret == DCMI_OK) {
            break;
        }
    }
 
    if (ret == DCMI_OK) {
        if (memory_info.memory_size <= DCMI_MEM_16G_MB_VALUE) {
            *mem_size = DCMI_MEM_SIZE_16G;
        } else if ((memory_info.memory_size > DCMI_MEM_16G_MB_VALUE) &&
            (memory_info.memory_size <= DCMI_MEM_24G_MB_VALUE)) {
            *mem_size = DCMI_MEM_SIZE_24G;
        } else if ((memory_info.memory_size > DCMI_MEM_24G_MB_VALUE) &&
            (memory_info.memory_size <= DCMI_MEM_32G_MB_VALUE)) {
            *mem_size = DCMI_MEM_SIZE_32G;
        } else {
            *mem_size = DCMI_MEM_SIZE_48G;
        }
    }
    return ret;
}

STATIC int dcmi_get_template_info_for_310P_910(int card_id, struct dcmi_computing_template *template_out,
    unsigned int template_size, unsigned int *template_num)
{
    int chip_type;
    unsigned int mem_size = 0;
    unsigned int i;
    int ret;
 
    if (template_out == NULL || template_num == NULL) {
        gplog(LOG_ERR, "dcmi_get_template_info_for_310P_910 parameter is invalid.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }
 
    chip_type = dcmi_get_board_chip_type();
    if ((chip_type != DCMI_CHIP_TYPE_D310P) && (chip_type != DCMI_CHIP_TYPE_D910)) {
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
 
    ret = dcmi_get_chip_memory_size(card_id, &mem_size);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_chip_memory_size failed, err is %d.", ret);
        return DCMI_ERR_CODE_INNER_ERR;
    }
 
    unsigned int array_size = sizeof(g_dcmi_product_template) / sizeof(struct dcmi_product_computing_template);
    for (i = 0; i < array_size; i++) {
        if ((chip_type == (int)(g_dcmi_product_template[i].chip_type)) &&
            (mem_size == g_dcmi_product_template[i].mem_max_size)) {
            ret = memcpy_s(template_out, template_size, g_dcmi_product_template[i].split_template,
                g_dcmi_product_template[i].template_num * sizeof(struct dcmi_computing_template));
            if (ret != EOK) {
                gplog(LOG_ERR, "memcpy_s template_out failed. err is %d.", ret);
                return DCMI_ERR_CODE_SECURE_FUN_FAIL;
            } else {
                *template_num = g_dcmi_product_template[i].template_num;
                return DCMI_OK;
            }
        }
    }
    return DCMI_ERR_CODE_NOT_SUPPORT;
}

int dcmi_get_template_info_by_name(int card_id, const char *name, struct dcmi_computing_template *computing_template)
{
    int ret;
    struct dcmi_computing_template template_out[10] = { { { 0 } } };
    unsigned int template_num = 0;
    unsigned int j;
 
    if (name == NULL || computing_template == NULL) {
        gplog(LOG_ERR, "dcmi_get_template_info_by_name parameter is invalid.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }
 
    ret = dcmi_get_template_info_all(card_id, template_out, sizeof(template_out), &template_num);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_template_info_all failed. ret is %d.", ret);
        return DCMI_ERR_CODE_INNER_ERR;
    }
 
    for (j = 0; j < template_num; j++) {
        if (strcmp(name, template_out[j].name) == 0) {
            ret = memcpy_s(computing_template, sizeof(struct dcmi_computing_template), &template_out[j],
                sizeof(struct dcmi_computing_template));
            if (ret != EOK) {
                gplog(LOG_ERR, "memcpy_s computing_template failed. err is %d.", ret);
                return DCMI_ERR_CODE_SECURE_FUN_FAIL;
            } else {
                return DCMI_OK;
            }
        }
    }
 
    return DCMI_ERR_CODE_NOT_SUPPORT;
}

STATIC int dcmi_get_910b_bin_type(unsigned int *bin_type)
{
    switch (dcmi_get_board_id_inner()) {
        case DCMI_A900T_POD_A1_BIN0_BOARD_ID:
        case DCMI_A900T_POD_A1_BIN0_P3_BOARD_ID:
        case DCMI_A200T_BOX_A1_BIN0_BOARD_ID:
        case DCMI_A300T_A1_BIN0_BOARD_ID:
        case DCMI_A800T_POD_A2_BIN0_BOARD_ID:
            *bin_type = DCMI_910B_BIN0;
            break;
        case DCMI_A900T_POD_A1_BIN2_BOARD_ID:
        case DCMI_A900T_POD_A1_BIN2_P3_BOARD_ID:
        case DCMI_A900T_POD_A1_BIN2X_BOARD_ID:
        case DCMI_A900T_POD_A1_BIN2X_P3_BOARD_ID:
        case DCMI_A900T_POD_A1_BIN2X_1_BOARD_ID:
        case DCMI_A900T_POD_A1_BIN2X_1_P3_BOARD_ID:
        case DCMI_A200T_BOX_A1_BIN2_BOARD_ID:
        case DCMI_A300T_A1_BIN2_BOARD_ID:
        case DCMI_A300I_A2_BIN2_BOARD_ID:
        case DCMI_A300I_A2_BIN2_64G_BOARD_ID:
        case DCMI_A800I_POD_A2_BIN2_BOARD_ID:
        case DCMI_A800I_POD_A2_BIN2_1_BOARD_ID:
        case DCMI_A800I_POD_A2_BIN4_1_PCIE_BOARD_ID:
            *bin_type = DCMI_910B_BIN2;
            break;
        case DCMI_A900T_POD_A1_BIN1_BOARD_ID:
        case DCMI_A900T_POD_A1_BIN1_P3_BOARD_ID:
        case DCMI_A300T_A1_BIN1_300W_BOARD_ID:
        case DCMI_A300T_A1_BIN1_350W_BOARD_ID:
        case DCMI_A200T_BOX_A1_BIN1_BOARD_ID:
        case DCMI_A800T_POD_A2_BIN1_BOARD_ID:
            *bin_type = DCMI_910B_BIN1;
            break;
        case DCMI_A900T_POD_A1_BIN3_BOARD_ID:
        case DCMI_A900T_POD_A1_BIN3_P3_BOARD_ID:
        case DCMI_A200T_BOX_A1_BIN3_BOARD_ID:
            *bin_type = DCMI_910B_BIN3;
            break;
        default:
            return DCMI_ERR_CODE_INNER_ERR;
    }
    return DCMI_OK;
}

STATIC int dcmi_get_template_info_for_910B(struct dcmi_computing_template *template_out, unsigned int template_size,
    unsigned int *template_num)
{
    int chip_type;
    unsigned int i;
    int ret;
    unsigned int bin_type;
 
    if (template_out == NULL || template_num == NULL) {
        gplog(LOG_ERR, "dcmi_get_template_info_for_910B parameter is invalid.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }
 
    chip_type = dcmi_get_board_chip_type();
    if (chip_type != DCMI_CHIP_TYPE_D910B) {
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
 
    ret = dcmi_get_910b_bin_type(&bin_type);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_910b_bin_type error.");
        return ret;
    }
 
    unsigned int array_size =
        sizeof(g_dcmi_product_template_for_910B) / sizeof(struct dcmi_product_computing_template_for_910B);
    for (i = 0; i < array_size; i++) {
        if (bin_type == (unsigned int)(g_dcmi_product_template_for_910B[i].bin_type_910b)) {
            ret = memcpy_s(template_out, template_size, g_dcmi_product_template_for_910B[i].split_template,
                g_dcmi_product_template_for_910B[i].template_num * sizeof(struct dcmi_computing_template));
            if (ret != EOK) {
                gplog(LOG_ERR, "memcpy_s template_out failed. err is %d.", ret);
                return DCMI_ERR_CODE_SECURE_FUN_FAIL;
            } else {
                *template_num = g_dcmi_product_template_for_910B[i].template_num;
                return DCMI_OK;
            }
        }
    }
 
    return DCMI_ERR_CODE_NOT_SUPPORT;
}

STATIC int dcmi_get_show_template_info_for_910B(struct dcmi_computing_template *template_out,
    unsigned int template_size, unsigned int *template_num)
{
    int chip_type;
    unsigned int i;
    int ret;
    unsigned int bin_type;
 
    if (template_out == NULL || template_num == NULL) {
        gplog(LOG_ERR, "dcmi_get_template_info_for_910B parameter is invalid.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }
 
    chip_type = dcmi_get_board_chip_type();
    if (chip_type != DCMI_CHIP_TYPE_D910B) {
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
 
    ret = dcmi_get_910b_bin_type(&bin_type);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_910b_bin_type error.");
        return ret;
    }
 
    unsigned int array_size =
        sizeof(g_dcmi_product_show_template_for_910B) / sizeof(struct dcmi_product_computing_template_for_910B);
    for (i = 0; i < array_size; i++) {
        if (bin_type == (unsigned int)g_dcmi_product_show_template_for_910B[i].bin_type_910b) {
            ret = memcpy_s(template_out, template_size, g_dcmi_product_show_template_for_910B[i].split_template,
                g_dcmi_product_show_template_for_910B[i].template_num * sizeof(struct dcmi_computing_template));
            if (ret != EOK) {
                gplog(LOG_ERR, "memcpy_s template_out failed. err is %d.", ret);
                return DCMI_ERR_CODE_SECURE_FUN_FAIL;
            } else {
                *template_num = g_dcmi_product_show_template_for_910B[i].template_num;
                return DCMI_OK;
            }
        }
    }
 
    return DCMI_ERR_CODE_NOT_SUPPORT;
}
 
int dcmi_get_template_info_all(int card_id, struct dcmi_computing_template *template_out, unsigned int template_size,
    unsigned int *template_num)
{
    int ret;
    switch (dcmi_get_board_chip_type()) {
        case DCMI_CHIP_TYPE_D310P:
        case DCMI_CHIP_TYPE_D910:
            ret = dcmi_get_template_info_for_310P_910(card_id, template_out, template_size, template_num);
            break;
        case DCMI_CHIP_TYPE_D910B:
            ret = dcmi_get_template_info_for_910B(template_out, template_size, template_num);
            break;
        default:
            return DCMI_ERR_CODE_NOT_SUPPORT;
    }
    return ret;
}
 
int dcmi_get_show_template_info_all(int card_id, struct dcmi_computing_template *template_out,
    unsigned int template_size, unsigned int *template_num)
{
    int ret;
    switch (dcmi_get_board_chip_type()) {
        case DCMI_CHIP_TYPE_D310P:
        case DCMI_CHIP_TYPE_D910:
            ret = dcmi_get_template_info_for_310P_910(card_id, template_out, template_size, template_num);
            break;
        case DCMI_CHIP_TYPE_D910B:
            ret = dcmi_get_show_template_info_for_910B(template_out, template_size, template_num);
            break;
        default:
            return DCMI_ERR_CODE_NOT_SUPPORT;
    }
    return ret;
}

STATIC int dcmi_get_hex_single_value_by_key(const char *info_line, const char *info_key, int *info_value)
{
    char *end_ptr = NULL;
    size_t key_len = strlen(info_key);
    if (strncmp(info_line, info_key, key_len) == 0) {
        *info_value = strtol(info_line + key_len, &end_ptr, DCMI_HEX_TO_STR_BASE);
        return DCMI_OK;
    }
    return DCMI_ERR_CODE_INNER_ERR;
}

int dcmi_hilens_cpu_get_board_info(struct dcmi_board_info *board_info)
{
    char info_line[BOARD_INFO_LINE_LEN] = {0};
    int ret;
    FILE *fp = NULL;
    unsigned int read_flag = 0;
    const unsigned int ALL_READ_SUCCESS_FLAG = 7;      // 0x111
    const unsigned int BOARD_ID_READ_SUCCESS_FLAG = 1; // 0x001
    const unsigned int PCB_ID_READ_SUCCESS_FLAG = 2;   // 0x010
    const unsigned int BOM_ID_READ_SUCCESS_FLAG = 4;   // 0x100

    fp = fopen(BOARD_CONFIG_FILE, "r");
    if (fp == NULL) {
        return DCMI_ERR_CODE_FILE_OPERATE_FAIL;
    }

    while (fgets(info_line, BOARD_INFO_LINE_LEN, fp) != NULL) {
        if (dcmi_get_hex_single_value_by_key(info_line, "board_id=", (int *)(void *)&board_info->board_id) == DCMI_OK) {
            read_flag |= BOARD_ID_READ_SUCCESS_FLAG;
        } else if (dcmi_get_hex_single_value_by_key(info_line, "pcb_id=",
                                                    (int *)(void *)&board_info->pcb_id) == DCMI_OK) {
            read_flag |= PCB_ID_READ_SUCCESS_FLAG;
        } else if (dcmi_get_hex_single_value_by_key(info_line, "bom_id=",
                                                    (int *)(void *)&board_info->bom_id) == DCMI_OK) {
            read_flag |= BOM_ID_READ_SUCCESS_FLAG;
        }

        ret = memset_s(info_line, BOARD_INFO_LINE_LEN, 0, BOARD_INFO_LINE_LEN);
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "memset_s info_line failed. ret is %d.", ret);
            break;
        }
    }

    (void)fclose(fp);

    if (read_flag == ALL_READ_SUCCESS_FLAG) {
        return DCMI_OK;
    }

    gplog(LOG_ERR, "read board info failed!read_flag=(%u).", read_flag);
    return DCMI_ERR_CODE_INNER_ERR;
}

int dcmi_cpu_get_board_info(int card_id, struct dcmi_board_info *board_info)
{
    int err;
    unsigned char buff[2] = {0};
    const unsigned int DEVELOP_C_PCB_ID_SHIF = 3;
    const unsigned int DEVELOP_A_PCB_ID_SHIF = 3;

    if (dcmi_board_type_is_station()) {
        return dcmi_mcu_get_board_info(card_id, (struct dcmi_board_info *)board_info);
    } else if (dcmi_board_type_is_hilens()) {
        return dcmi_hilens_cpu_get_board_info(board_info);
    } else if (dcmi_board_type_is_soc_develop()) {
        err = dcmi_i2c_get_data_9555(I2C0_DEV_NAME, I2C_SLAVE_PCA9555_BOARDINFO, 0x0, (char *)buff, sizeof(buff));
        if (err < DCMI_OK) {
            gplog(LOG_ERR, "call driver_i2c_get_data_9555 failed. err is %d.", err);
            return DCMI_ERR_CODE_INNER_ERR;
        }

        board_info->board_id = buff[0];
        if (board_info->board_id == DCMI_310_DEVELOP_C_BOARD_ID) {
            /* C板 bomid为pca9555的13~15管脚, pcbid为16~18管脚, typeid为19~20管脚 */
            board_info->bom_id = buff[1] & DEVELOP_C_BOM_PCB_MASK;
            board_info->pcb_id = (buff[1] >> DEVELOP_C_PCB_ID_SHIF) & DEVELOP_C_BOM_PCB_MASK;
        } else {
            board_info->bom_id = buff[1] & DEVELOP_A_BOM_PCB_MASK;
            board_info->pcb_id = (buff[1] >> DEVELOP_A_PCB_ID_SHIF) & DEVELOP_A_BOM_PCB_MASK;
        }
        return DCMI_OK;
    } else {
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
}

int dcmi_cpu_get_board_id(int card_id, unsigned int *board_id)
{
    int err;
    struct dcmi_board_info board_info = {0};

    if (dcmi_board_type_is_soc_develop()) {
        err = dcmi_cpu_get_board_info(card_id, &board_info);
        if (err != DCMI_OK) {
            gplog(LOG_ERR, "call dcmi_cpu_get_board_info failed. err is %d.", err);
            return DCMI_ERR_CODE_INNER_ERR;
        }

        *board_id = board_info.board_id;
        return DCMI_OK;
    } else {
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
}

int dcmi_cpu_get_pcb_id(int card_id, int *pcb_id)
{
    int err;
    struct dcmi_board_info board_info = {0};

    if (dcmi_board_type_is_soc_develop()) {
        err = dcmi_cpu_get_board_info(card_id, &board_info);
        if (err != DCMI_OK) {
            gplog(LOG_ERR, "call dcmi_cpu_get_board_info failed. err is %d.", err);
            return DCMI_ERR_CODE_INNER_ERR;
        }

        *pcb_id = board_info.pcb_id;
        return DCMI_OK;
    } else {
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
}

int dcmi_cpu_get_health(int card_id, unsigned int *health)
{
    if (dcmi_board_type_is_soc_develop()) {
        return dcmi_get_npu_device_health(card_id, 0, health);
    } else {
        *health = DCMI_OK;
        return DCMI_OK;
    }
}

int dcmi_cpu_get_device_frequency(int card_id, int device_type, unsigned int *frequency)
{
    const unsigned int hi3559_cpu_freq = 1600;
    if (dcmi_board_type_is_station() || dcmi_board_type_is_hilens()) {
        *frequency = hi3559_cpu_freq; /* 海思Hi3559AV100 的主频是1.6GHz。在cpuinfo中读不到 */
        return DCMI_OK;
    }

    return DCMI_ERR_CODE_NOT_SUPPORT;
}

int dcmi_3559_himd(unsigned long phy_addr, struct dcmi_die_id *device_die)
{
#ifndef _WIN32
    int i;
    void* mem  = NULL;
    const unsigned int size = 24; // 6个die_id, 6*4=24
    unsigned int *data = NULL;
    unsigned long phy_addr_in_page;
    unsigned long page_diff;
    void *addr_memmap = NULL;
    static int fd = -1;
    static const char dev[] = "/dev/mem";
    unsigned long size_in_page;

    /* dev not opened yet, so open it */
    fd = open(dev, O_RDWR | O_SYNC);
    if (fd < 0) {
        gplog(LOG_ERR, "memmap():open %s error!", dev);
        return DCMI_ERR_CODE_FILE_OPERATE_FAIL;
    }

    /* addr align in page_size(4K) */
    phy_addr_in_page = phy_addr & PAGE_SIZE_MASK;
    page_diff = phy_addr - phy_addr_in_page;

    /* size in page_size */
    size_in_page = ((size + page_diff - 1) & PAGE_SIZE_MASK) + PAGE_SIZE;
    if (size_in_page > (PAGE_SIZE + PAGE_SIZE)) {
        close(fd);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    addr_memmap = mmap((void *)0, size_in_page, PROT_READ | PROT_WRITE, MAP_SHARED, fd, phy_addr_in_page);
    if (addr_memmap == MAP_FAILED) {
        gplog(LOG_ERR, "memmap():mmap @ 0x%x error!", (unsigned int)phy_addr_in_page);
        close(fd);
        return DCMI_ERR_CODE_MEM_OPERATE_FAIL;
    }

    mem = (addr_memmap + page_diff);
    data = mem;

    for (i = 0; i < DIE_ID_COUNT; ++i) {
        device_die->soc_die[i] = *(data++);
    }

    if (munmap(addr_memmap, size_in_page) != 0) {
        gplog(LOG_ERR, "memunmap(): munmap failed!");
    }

    close(fd);
    return DCMI_OK;
#else
    return DCMI_OK;
#endif
}

int dcmi_cpu_get_device_die(int card_id, int device_id, enum dcmi_die_type input_type, struct dcmi_die_id *die_id)
{
    if (dcmi_board_type_is_soc_develop()) {
        return dcmi_get_npu_device_die(card_id, 0, input_type, die_id);
    } else if (dcmi_board_type_is_station() || dcmi_board_type_is_hilens()) {
        if (input_type != VDIE) {
            return DCMI_ERR_CODE_NOT_SUPPORT;
        }

        int ret;
        unsigned long die_addr = 0x12020400;

        ret = dcmi_3559_himd(die_addr, die_id);
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "call dcmi_3559_himd failed.%d.\n", ret);
            return ret;
        }

        return DCMI_OK;
    } else {
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
}

int dcmi_cpu_get_memory_info(int card_id, struct dcmi_memory_info *memory_info)
{
    if (dcmi_board_type_is_station() || dcmi_board_type_is_hilens()) {
        int ret;
        FILE *fp = NULL;
        char buff[BOARD_INFO_LINE_LEN] = {0};
        int total = 0;

        fp = fopen("/proc/meminfo", "r");
        if (fp == NULL) {
            gplog(LOG_ERR, "open /proc/meminfo failed.");
            return DCMI_ERR_CODE_FILE_OPERATE_FAIL;
        }

        while (!feof(fp)) {
            if (fgets(buff, sizeof(buff), fp) == NULL) {
                break;
            }
            if (strncmp(buff, "MemTotal", strlen("MemTotal"))) {
                continue;
            }
            ret = sscanf_s(buff, "MemTotal: %d kB\n", &total);
            if (ret <= 0) {
                gplog(LOG_ERR, "call sscanf_s failed. ret is %d.", ret);
                (void)fclose(fp);
                return DCMI_ERR_CODE_SECURE_FUN_FAIL;
            }
        }

        // 读出来的结果是kb，转换成MB。对装备而言，需要的是总内存容量，向上取整
        memory_info->memory_size =
            (unsigned long)(ceil((double)total / BYTE_TO_KB_TRANS_VALUE / KB_TO_MB_TRANS_VALUE) * KB_TO_MB_TRANS_VALUE);

        (void)fclose(fp);
        return DCMI_OK;
    } else {
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
}

int dcmi_cpu_get_chip_info(int card_id, struct dcmi_chip_info_v2 *chip_info)
{
    int ret;

    ret = strncpy_s((char *)chip_info->chip_type, MAX_CHIP_NAME_LEN, "media", strlen("media"));
    if (ret != EOK) {
        gplog(LOG_ERR, "call strncpy_s failed.%d", ret);
    }

    ret = strncpy_s((char *)chip_info->chip_name, MAX_CHIP_NAME_LEN, "3559", strlen("3559"));
    if (ret != EOK) {
        gplog(LOG_ERR, "call strncpy_s failed.%d", ret);
    }

    ret = strncpy_s((char *)chip_info->chip_ver, MAX_CHIP_NAME_LEN, "V100", strlen("V100"));
    if (ret != EOK) {
        gplog(LOG_ERR, "call strncpy_s failed.%d.\n", ret);
    }

    return DCMI_OK;
}

int dcmi_copy_npu_chip_name_910_93(struct dsmi_chip_info_stru *dsmi_chip_info, struct dcmi_chip_info_v2 *chip_info)
{
    int ret;
    unsigned int end;
    char add_str[] = "Ascend";
    unsigned int add_str_len = strlen(add_str);
    unsigned int delim_pos = 0;

    if (dsmi_chip_info->chip_name[0] == '\0') {
        gplog(LOG_ERR, "Input err parameter, dsmi_chip_info chip_name is null.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if (strchr((char *)dsmi_chip_info->chip_name, '_') != NULL) {
        delim_pos = strchr((char *)dsmi_chip_info->chip_name, '_') - (char *)dsmi_chip_info->chip_name;
    } else {
        gplog(LOG_ERR, "Input err parameter, chip name format is err.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    ret = memcpy_s(chip_info->chip_name, sizeof(chip_info->chip_name), add_str, add_str_len);
    if (ret != EOK) {
        gplog(LOG_ERR, "memcpy 910_93 chipname failed.. err is %d.", ret);
        return DCMI_ERR_CODE_SECURE_FUN_FAIL;
    }

    ret = memcpy_s(chip_info->chip_name + add_str_len, sizeof(chip_info->chip_name) - add_str_len,
        dsmi_chip_info->chip_name, delim_pos);
    if (ret != EOK) {
        gplog(LOG_ERR, "memcpy 910_93 npuname first failed. err is %d.", ret);
        return DCMI_ERR_CODE_SECURE_FUN_FAIL;
    }
    if (add_str_len + delim_pos >= MAX_CHIP_NAME_LEN) {
        end = MAX_CHIP_NAME_LEN - 1;
    } else {
        end = add_str_len + delim_pos;
    }
    chip_info->chip_name[end] = '\0';

    ret = memcpy_s(chip_info->npu_name, sizeof(chip_info->npu_name), dsmi_chip_info->chip_name + delim_pos + 1,
        strlen((const char*)dsmi_chip_info->chip_name) - delim_pos);
    if (ret != EOK) {
        gplog(LOG_ERR, "memcpy 910_93 npuname second failed. err is %d.", ret);
        return DCMI_ERR_CODE_SECURE_FUN_FAIL;
    }
    return DCMI_OK;
}

int dcmi_copy_npu_chip_name_common(struct dsmi_chip_info_stru *dsmi_chip_info, struct dcmi_chip_info_v2 *chip_info)
{
    int ret = memcpy_s(chip_info->chip_name, sizeof(chip_info->chip_name), dsmi_chip_info->chip_name,
            sizeof(dsmi_chip_info->chip_name));
    if (ret != EOK) {
        gplog(LOG_ERR, "memcpy common chipname failed. err is %d.", ret);
        return DCMI_ERR_CODE_SECURE_FUN_FAIL;
    }

    ret = memcpy_s(chip_info->npu_name, sizeof(chip_info->npu_name), dsmi_chip_info->chip_name,
            sizeof(dsmi_chip_info->chip_name));
    if (ret != EOK) {
        gplog(LOG_ERR, "memcpy common npuname failed. err is %d.", ret);
        return DCMI_ERR_CODE_SECURE_FUN_FAIL;
    }
    return DCMI_OK;
}

#ifdef ORIENT_CH
STATIC int dcmi_copy_npu_chip_name_310p_ch(struct dsmi_chip_info_stru *dsmi_chip_info,
    struct dcmi_chip_info_v2 *chip_info)
{
    int ret;
    char head_310P[] = "I2";

    ret = memcpy_s(chip_info->chip_name, sizeof(chip_info->chip_name), head_310P, sizeof(head_310P));
    if (ret != EOK) {
        gplog(LOG_ERR, "memcpy I2_CH chipname failed. err is %d.", ret);
        return DCMI_ERR_CODE_SECURE_FUN_FAIL;
    }

    ret = memcpy_s(chip_info->npu_name, sizeof(chip_info->npu_name), head_310P, sizeof(head_310P));
    if (ret != EOK) {
        gplog(LOG_ERR, "memcpy I2_CH npuname failed. err is %d.", ret);
        return DCMI_ERR_CODE_SECURE_FUN_FAIL;
    }
    return DCMI_OK;
}

STATIC int dcmi_copy_npu_chip_name_910b_ch(struct dsmi_chip_info_stru *dsmi_chip_info,
    struct dcmi_chip_info_v2 *chip_info)
{
    int ret;
    char head_910B[] = "A2G";
    char *tail_pos_910B = NULL;
    unsigned int head_910B_len = strlen(head_910B);

    ret = memcpy_s(chip_info->chip_name, sizeof(chip_info->chip_name), head_910B, head_910B_len);
    if (ret != EOK) {
        gplog(LOG_ERR, "memcpy A2Gx chipname failed. err is %d.", ret);
        return DCMI_ERR_CODE_SECURE_FUN_FAIL;
    }
    ret = memcpy_s(chip_info->npu_name, sizeof(chip_info->npu_name), head_910B, head_910B_len);
    if (ret != EOK) {
        gplog(LOG_ERR, "memcpy A2Gx npuname failed. err is %d.", ret);
        return DCMI_ERR_CODE_SECURE_FUN_FAIL;
    }

    tail_pos_910B = (char *)strchr((char *)dsmi_chip_info->chip_name, 'B');
    if (tail_pos_910B == NULL) {
        gplog(LOG_ERR, "memcpy A2Gx chipname failed. ");
        return DCMI_ERR_CODE_SECURE_FUN_FAIL;
    }
    ret = memcpy_s(chip_info->chip_name + head_910B_len, sizeof(chip_info->chip_name) -
        sizeof(head_910B), tail_pos_910B + 1, strlen(tail_pos_910B + 1) + 1);
    if (ret != EOK) {
        gplog(LOG_ERR, "memcpy A2Gx chipname failed. err is %d.", ret);
        return DCMI_ERR_CODE_SECURE_FUN_FAIL;
    }
    ret = memcpy_s(chip_info->npu_name + head_910B_len, sizeof(chip_info->npu_name) -
        sizeof(head_910B), tail_pos_910B + 1, strlen(tail_pos_910B + 1) + 1);
    if (ret != EOK) {
        gplog(LOG_ERR, "memcpy A2Gx npuname failed. err is %d.", ret);
        return DCMI_ERR_CODE_SECURE_FUN_FAIL;
    }
    return DCMI_OK;
}
#endif

int dcmi_copy_npu_chip_name(struct dsmi_chip_info_stru *dsmi_chip_info, struct dcmi_chip_info_v2 *chip_info)
{
#ifdef ORIENT_CH
    // CH版本的910B和310P改写敏感词
    // 310P: dsmi_chip_info->chip_name = "310P" ==> chip_info->chip_name = "I2", chip_info->npu_name = "I2"
    // 910B: dsmi_chip_info->chip_name = "910Bx" ==> chip_info->chip_name = "A2Gx", chip_info->npu_name = "A2Gx"
    int ret;
    if (strstr((char *)dsmi_chip_info->chip_name, "310P") != NULL) {
        ret = dcmi_copy_npu_chip_name_310p_ch(dsmi_chip_info, chip_info);
        if (ret != EOK) {
            gplog(LOG_ERR, "copy I2 chipname failed. err is %d.", ret);
            return DCMI_ERR_CODE_SECURE_FUN_FAIL;
        }
    } else {
        ret = dcmi_copy_npu_chip_name_910b_ch(dsmi_chip_info, chip_info);
        if (ret != EOK) {
            gplog(LOG_ERR, "copy A2Gx chipname failed. err is %d.", ret);
            return DCMI_ERR_CODE_SECURE_FUN_FAIL;
        }
    }
    return DCMI_OK;
#else
    int ret;
    // 判断一下是否是Ascend 910_93芯片
    // 910_93: dsmi_chip_info->chip_name = "910_93xx" ==> chip_info->chip_name = "Ascend910", chip_info->npu_name = "A3"
    // 910b: dsmi_chip_info->chip_name = "910Bx" ==> chip_info->chip_name = "910Bx", chip_info->npu_name = "910Bx"
    if (dcmi_board_chip_type_is_ascend_910_93() == TRUE) {
        ret = dcmi_copy_npu_chip_name_910_93(dsmi_chip_info, chip_info);
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "copy 910_93 chipname failed.. err is %d.", ret);
            return DCMI_ERR_CODE_SECURE_FUN_FAIL;
        }
    } else {
        ret = dcmi_copy_npu_chip_name_common(dsmi_chip_info, chip_info);
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "copy common chipname failed.. err is %d.", ret);
            return DCMI_ERR_CODE_SECURE_FUN_FAIL;
        }
    }
    return DCMI_OK;
#endif
}

int dcmi_copy_npu_chip_info(struct dsmi_chip_info_stru *dsmi_chip_info, struct dcmi_chip_info_v2 *chip_info)
{
    int ret;
#ifdef ORIENT_CH
    ret = memcpy_s(chip_info->chip_type, sizeof(chip_info->chip_type), "Alan", sizeof("Alan"));
    if (ret != EOK) {
        gplog(LOG_ERR, "call memcpy_s failed. err is %d.", ret);
        return DCMI_ERR_CODE_SECURE_FUN_FAIL;
    }
#else
    ret = memcpy_s(chip_info->chip_type, sizeof(chip_info->chip_type), dsmi_chip_info->chip_type,
        sizeof(dsmi_chip_info->chip_type));
    if (ret != EOK) {
        gplog(LOG_ERR, "call memcpy_s failed. err is %d.", ret);
        return DCMI_ERR_CODE_SECURE_FUN_FAIL;
    }
#endif
    ret = dcmi_copy_npu_chip_name(dsmi_chip_info, chip_info);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_copy_npu_chip_name failed. err is %d.", ret);
        return DCMI_ERR_CODE_SECURE_FUN_FAIL;
    }

    ret = memcpy_s(chip_info->chip_ver, sizeof(chip_info->chip_ver), dsmi_chip_info->chip_ver,
        sizeof(dsmi_chip_info->chip_ver));
    if (ret != EOK) {
        gplog(LOG_ERR, "call memcpy_s failed. err is %d.", ret);
        return DCMI_ERR_CODE_SECURE_FUN_FAIL;
    }
    return DCMI_OK;
}

int dcmi_get_npu_chip_info(int card_id, int device_id, struct dcmi_chip_info_v2 *chip_info)
{
    int ret;
    int device_logic_id = 0;
    struct dsmi_chip_info_stru dsmi_chip_info = { { 0 } };
#ifndef _WIN32
    struct dsmi_computing_power_info computing_power_info = {0};
#endif
    const int aicore_cnt_type = 1;

    ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", ret);
        return ret;
    }

    ret = dsmi_get_chip_info(device_logic_id, &dsmi_chip_info);
    if (ret != DSMI_OK) {
        gplog(LOG_ERR, "call dsmi_get_chip_info failed. err is %d.", ret);
        return dcmi_convert_error_code(ret);
    }

    ret = dcmi_copy_npu_chip_info(&dsmi_chip_info, chip_info);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_copy_npu_chip_info failed. err is %d.", ret);
        return ret;
    }

#ifndef _WIN32
    if (!dcmi_board_chip_type_is_ascend_310()) {
        ret = dsmi_get_computing_power_info(device_logic_id, aicore_cnt_type, &computing_power_info);
        if (ret != DSMI_OK) {
            gplog(LOG_ERR, "call dsmi_get_computing_power_info failed. err is %d.", ret);
            return dcmi_convert_error_code(ret);
        }
        chip_info->aicore_cnt = computing_power_info.data1;
    }
#endif
    return dcmi_convert_error_code(ret);
}

int dcmi_get_npu_pcie_info(int card_id, int device_id, struct dcmi_pcie_info *pcie_idinfo)
{
    int ret;
    int device_logic_id = 0;

    /* The validity of the parameter is guaranteed by the caller, and the internal function is not judged */

    if (dcmi_is_has_pcieinfo() != TRUE) {
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", ret);
        return ret;
    }

    ret = dsmi_get_pcie_info(device_logic_id, (struct tag_pcie_idinfo *)pcie_idinfo);
    if ((ret != DSMI_OK) && (ret != DSMI_ERR_NOT_SUPPORT)) {
        gplog(LOG_ERR, "call dsmi_get_pcie_info failed. err is %d.", ret);
    }

    return dcmi_convert_error_code(ret);
}

int dcmi_get_pcie_info_win(int device_logic_id, struct dcmi_pcie_info_all *pcie_idinfo)
{
    int ret;
    struct tag_pcie_idinfo pcie_win_idinfo = {0};

    ret = dsmi_get_pcie_info(device_logic_id, &pcie_win_idinfo);
    if ((ret != DSMI_OK) && (ret != DSMI_ERR_NOT_SUPPORT)) {
        gplog(LOG_ERR, "call dsmi_get_pcie_info failed. err is %d.", ret);
    }
    pcie_idinfo->deviceid = pcie_win_idinfo.deviceid;
    pcie_idinfo->subdeviceid = pcie_win_idinfo.subdeviceid;
    pcie_idinfo->venderid = pcie_win_idinfo.venderid;
    pcie_idinfo->subvenderid = pcie_win_idinfo.subvenderid;
    pcie_idinfo->domain = 0;
    pcie_idinfo->bdf_busid = pcie_win_idinfo.bdf_busid;
    pcie_idinfo->bdf_deviceid = pcie_win_idinfo.bdf_deviceid;
    pcie_idinfo->bdf_funcid = pcie_win_idinfo.bdf_funcid;
    return DCMI_OK;
}

int dcmi_get_npu_pcie_info_v2(int card_id, int device_id, struct dcmi_pcie_info_all *pcie_idinfo)
{
    int ret;
    int device_logic_id = 0;

    /* The validity of the parameter is guaranteed by the caller, and the internal function is not judged */

    if (dcmi_is_has_pcieinfo() != TRUE) {
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", ret);
        return ret;
    }
#ifndef _WIN32
    ret = dsmi_get_pcie_info_v2(device_logic_id, (struct tag_pcie_idinfo_all *)pcie_idinfo);
#else
    ret = dcmi_get_pcie_info_win(device_logic_id, (struct tag_pcie_idinfo_all *)pcie_idinfo);
#endif
    if ((ret != DSMI_OK) && (ret != DSMI_ERR_NOT_SUPPORT)) {
        gplog(LOG_ERR, "call dsmi_get_pcie_info failed. err is %d.", ret);
    }

    return dcmi_convert_error_code(ret);
}

#ifndef _WIN32
int dcmi_get_board_info_for_develop(struct dcmi_board_info *board_info)
{
    int ret;
    char buff[2] = {0};
    unsigned int pci_id_shift;
    unsigned int bom_id_mask;
    unsigned char addr;

    if (dcmi_board_chip_type_is_ascend_310b()) {
        addr = DCMI_310B_BOARD_ID_I2C_ADDR;
        pci_id_shift = 0x3;
        bom_id_mask = 0xff;
    } else {
        addr = DCMI_DEVELOP_BOARD_ID_I2C_ADDR;
        pci_id_shift = 0x4;
        bom_id_mask = 0xf;
    }

    ret = dcmi_i2c_read_board_id(addr, (unsigned char *)buff, sizeof(buff));
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_i2c_read_board_id failed. err is %d.", ret);
        return ret;
    }

    board_info->board_id = (unsigned int)buff[1];
    board_info->pcb_id = (unsigned int)buff[0] >> pci_id_shift;
    board_info->bom_id = (unsigned int)buff[0] & bom_id_mask;
    return DCMI_OK;
}
#endif

int dcmi_get_npu_board_info(int card_id, int device_id, struct dcmi_board_info *board_info)
{
    int ret;
    /* The validity of the parameter is guaranteed by the caller, and the internal function is not judged */
    if (dcmi_board_type_is_soc_develop()) {
#ifndef _WIN32
        return dcmi_get_board_info_for_develop(board_info);
#else
        return DCMI_OK;
#endif
    } else {
        int device_logic_id = 0;
        ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", ret);
            return ret;
        }

        ret = dsmi_get_board_info(device_logic_id, (struct dsmi_board_info_stru *)board_info);
        if ((ret != DSMI_OK) && (ret != DSMI_ERR_NOT_SUPPORT)) {
            gplog(LOG_ERR, "call dsmi_get_board_info failed. err is %d.", ret);
        }

        if (dcmi_board_chip_type_is_ascend_910()) {
            // 接口返回的boardid有误，低4位是预留位，非boardid信息位，需要右移丢掉
            board_info->board_id = (board_info->board_id >> 4);
        }

        return dcmi_convert_error_code(ret);
    }
}

int dcmi_get_npu_device_power_info(int card_id, int device_id, int *power)
{
    int ret;
    int device_logic_id = 0;
    struct dsmi_power_info_stru  device_power_info = {0};

    /* The validity of the parameter is guaranteed by the caller, and the internal function is not judged */

    ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", ret);
        return ret;
    }

    ret = dsmi_get_device_power_info(device_logic_id, &device_power_info);
    if (ret == DSMI_OK) {
        *power = device_power_info.power;
    } else if (ret != DSMI_ERR_NOT_SUPPORT) {
        gplog(LOG_ERR, "call dsmi_get_device_power_info failed. err is %d.", ret);
    }

    return dcmi_convert_error_code(ret);
}

int dcmi_set_npu_device_clear_pcie_error(int card_id, int device_id)
{
#ifndef _WIN32
    int ret;
    int device_logic_id = 0;

    ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", ret);
        return ret;
    }

    ret = dsmi_clear_pcie_error_rate(device_logic_id);
    if ((ret != DSMI_OK) && (ret != DSMI_ERR_NOT_SUPPORT)) {
        gplog(LOG_ERR, "call dsmi_clear_pcie_error_rate failed. err is %d.", ret);
    }

    return dcmi_convert_error_code(ret);
#else
    return DCMI_OK;
#endif
}

int dcmi_get_npu_device_pcie_error(int card_id, int device_id, struct dcmi_chip_pcie_err_rate *pcie_err_code_info)
{
#ifndef _WIN32
    int ret;
    int device_logic_id = 0;

    if (pcie_err_code_info == NULL) {
        gplog(LOG_ERR, "input para error.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", ret);
        return ret;
    }

    ret = dsmi_get_pcie_error_rate(device_logic_id, (struct dsmi_chip_pcie_err_rate_stru *)pcie_err_code_info);
    if ((ret != DSMI_OK) && (ret != DSMI_ERR_NOT_SUPPORT)) {
        gplog(LOG_ERR, "call dsmi_get_pcie_error_rate failed. err is %d.", ret);
    }

    return dcmi_convert_error_code(ret);
#else
    return DCMI_OK;
#endif
}

int dcmi_get_npu_device_die(int card_id, int device_id, enum dcmi_die_type input_type, struct dcmi_die_id *die_id)
{
    int ret;
    char *die_type[] = {"ndie", "die"};
    int device_logic_id = 0;

    ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed.err is %d.", ret);
        return ret;
    }

    switch (input_type) {
#ifndef _WIN32
        case NDIE:
            if (dcmi_board_chip_type_is_ascend_910b() || dcmi_board_chip_type_is_ascend_910_93()) {
                gplog(LOG_ERR, "This device does not support.");
                return DCMI_ERR_CODE_NOT_SUPPORT;
            }

            ret = dsmi_get_device_ndie(device_logic_id, (struct dsmi_soc_die_stru *)die_id);
            break;
#endif
        case VDIE:
            ret = dsmi_get_device_die(device_logic_id, (struct dsmi_soc_die_stru *)die_id);
            break;
        default:
            gplog(LOG_ERR, "input_type %d is invalid.", input_type);
            return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    if ((ret != DSMI_OK) && (ret != DSMI_ERR_NOT_SUPPORT)) {
        gplog(LOG_ERR, "call dsmi_get_device_%s failed. err is %d.", die_type[input_type], ret);
    }

    return dcmi_convert_error_code(ret);
}

int dcmi_get_npu_device_health(int card_id, int device_id, unsigned int *health)
{
    int ret;
    int device_logic_id = 0;

    *health = HEALTH_UNKNOWN;
    ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d", ret);
        return ret;
    }

    ret = dsmi_get_device_health(device_logic_id, health);
    if ((ret != DSMI_OK || *health == DCMI_DEVICE_NOT_EXIST) && (ret != DSMI_ERR_NOT_SUPPORT)) {
        gplog(LOG_ERR, "dsmi_get_device_health error.(logic_id=%d, health=%u, ret=%d).", device_logic_id,
            *health, ret);
    }

    return dcmi_convert_error_code(ret);
}

int dcmi_get_npu_aicore_info(int card_id, int device_id, struct dcmi_aicore_info *aicore_info)
{
    int ret;
    int device_logic_id = 0;

    ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", ret);
        return ret;
    }

    ret = dsmi_get_aicore_info(device_logic_id, (struct dsmi_aicore_info_stru *)aicore_info);
    if ((ret != DSMI_OK) && (ret != DSMI_ERR_NOT_SUPPORT)) {
        gplog(LOG_ERR, "call dsmi_get_aicore_info failed. err is %d.", ret);
    }

    return dcmi_convert_error_code(ret);
}

int dcmi_get_npu_aicpu_info(int card_id, int device_id, struct dcmi_aicpu_info *aicpu_info)
{
    int ret;
    int device_logic_id = 0;

    ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", ret);
        return ret;
    }

    ret = dsmi_get_aicpu_info(device_logic_id, (struct dsmi_aicpu_info_stru *)aicpu_info);
    if ((ret != DSMI_OK) && (ret != DSMI_ERR_NOT_SUPPORT)) {
        gplog(LOG_ERR, "call dsmi_get_aicpu_info failed. err is %d.", ret);
    }

    return dcmi_convert_error_code(ret);
}

int dcmi_get_npu_device_boot_status(int card_id, int device_id, enum dcmi_boot_status *boot_status)
{
    int ret;
    int device_logic_id = 0;

    ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", ret);
        return ret;
    }

    ret = dsmi_get_device_boot_status(device_logic_id, (enum dsmi_boot_status *)(void *)boot_status);
    if ((ret != DSMI_OK) && (ret != DSMI_ERR_NOT_SUPPORT)) {
        gplog(LOG_ERR, "call dsmi_get_device_boot_status failed. err is %d.", ret);
    }

    return dcmi_convert_error_code(ret);
}

int dcmi_get_npu_system_time(int card_id, int device_id, unsigned int *time)
{
    int ret;
    int device_logic_id = 0;

    ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", ret);
        return ret;
    }

    ret = dsmi_get_system_time(device_logic_id, time);
    if ((ret != DSMI_OK) && (ret != DSMI_ERR_NOT_SUPPORT)) {
        gplog(LOG_ERR, "call dsmi_get_system_time failed. err is %d.", ret);
    }

    return dcmi_convert_error_code(ret);
}

int dcmi_get_npu_device_temperature(int card_id, int device_id, int *temprature)
{
    int ret;
    int device_logic_id = 0;

    ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", ret);
        return ret;
    }

    ret = dsmi_get_device_temperature(device_logic_id, temprature);
    if ((ret != DSMI_OK) && (ret != DSMI_ERR_NOT_SUPPORT)) {
        gplog(LOG_ERR, "call dsmi_get_device_temperature failed. err is %d.", ret);
    }

    return dcmi_convert_error_code(ret);
}

int dcmi_get_npu_device_voltage(int card_id, int device_id, unsigned int *voltage)
{
    int ret;
    int device_logic_id = 0;

    ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", ret);
        return DCMI_ERR_CODE_INNER_ERR;
    }

    ret = dsmi_get_device_voltage(device_logic_id, voltage);
    if ((ret != DSMI_OK) && (ret != DSMI_ERR_NOT_SUPPORT)) {
        gplog(LOG_ERR, "call dsmi_get_device_voltage failed. err is %d.", ret);
    }

    return dcmi_convert_error_code(ret);
}

STATIC int dcmi_get_npu_ecc_info_datails(int card_id, int device_id,
    int device_logic_id, enum dcmi_device_type input_type, struct dcmi_ecc_info *device_ecc_info)
{
    int ret;
    unsigned int ecc_count = 0;
    struct dsmi_ecc_pages_stru device_ecc_pages_info = {0};
    struct dcmi_ecc_common_data ddr_ecc_common_info[MAX_RECORD_ECC_ADDR_COUNT] = {0};
    struct dcmi_ecc_record_type type = {
        .read_type = MULTI_ECC_INFO_READ,
        .module_type = DCMI_DEVICE_TYPE_DDR
    };

    ret = dsmi_get_total_ecc_isolated_pages_info(device_logic_id, input_type, &device_ecc_pages_info);
    if ((ret != DSMI_OK) && (ret != DSMI_ERR_NOT_SUPPORT)) {
        gplog(LOG_ERR, "call dsmi_get_total_ecc_isolated_pages_info failed.%d.\n", ret);
        return ret;
    }

    ret = dcmi_get_multi_ecc_record_info_v2(card_id, device_id, type, &ecc_count, ddr_ecc_common_info);
    if ((ret != DSMI_OK) && (ret != DSMI_ERR_NOT_SUPPORT)) {
        gplog(LOG_ERR, "call dcmi_get_multi_ecc_record_info_v2 failed.%d.\n", ret);
        return (ret == DCMI_ERR_CODE_OPER_NOT_PERMITTED) ? DCMI_ERR_CODE_NOT_SUPPORT : ret;
    }

    // 生命周期内所有可纠正ecc错误统计
    device_ecc_info->total_single_bit_error_cnt = device_ecc_pages_info.corrected_ecc_errors_aggregate_total;
    // 生命周期内所有不可纠正ecc错误统计
    device_ecc_info->total_double_bit_error_cnt = device_ecc_pages_info.uncorrected_ecc_errors_aggregate_total;
    // 单bit错误隔离内存页数量
    device_ecc_info->single_bit_isolated_pages_cnt = device_ecc_pages_info.isolated_pages_single_bit_error;
    // 多bit错误隔离内存页数量
    device_ecc_info->double_bit_isolated_pages_cnt = device_ecc_pages_info.isolated_pages_double_bit_error;
    return ret;
}

int dcmi_get_npu_ecc_info(
    int card_id, int device_id, enum dcmi_device_type input_type, struct dcmi_ecc_info *device_ecc_info)
{
    int ret;
    int device_logic_id = 0;
    bool support_chip_type = (dcmi_board_chip_type_is_ascend_910() || dcmi_board_chip_type_is_ascend_910b() ||
                              dcmi_board_chip_type_is_ascend_310p() || dcmi_board_chip_type_is_ascend_910_93());

    ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", ret);
        return ret;
    }
    bool type_not_support = ((dcmi_board_chip_type_is_ascend_910b() || dcmi_board_chip_type_is_ascend_910_93()) &&
                             (input_type == DCMI_DEVICE_TYPE_DDR)) ||
                (dcmi_board_chip_type_is_ascend_310p() && (input_type == DCMI_DEVICE_TYPE_HBM)) ||
                (support_chip_type && (input_type != DCMI_DEVICE_TYPE_DDR && input_type != DCMI_DEVICE_TYPE_HBM));
    if (type_not_support) {
        gplog(LOG_ERR, "input type is not support. input_type=%d", input_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    ret = dsmi_get_ecc_info(device_logic_id, input_type, (struct dsmi_ecc_info_stru *)device_ecc_info);
    if ((ret != DSMI_OK) && (ret != DSMI_ERR_NOT_SUPPORT)) {
        gplog(LOG_ERR, "call dsmi_get_ecc_info failed. err is %d.", ret);
        return dcmi_convert_error_code(ret);
    }

    if (support_chip_type && !dcmi_board_type_is_soc()) {
        return dcmi_get_npu_ecc_info_datails(card_id, device_id, device_logic_id, input_type, device_ecc_info);
    }
    return dcmi_convert_error_code(ret);
}

int dcmi_clear_npu_ecc_statistics_info(int card_id, int device_id)
{
    int ret;
    int device_logic_id = 0;
    // 清除ECC隔离统计信息
    gplog(LOG_OP, "Clean Ecc Isolated Statistics Info:card_id = %d.", card_id);

    ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed.%d.\n", ret);
        return ret;
    }

    ret = dsmi_clear_ecc_isolated_statistics_info(device_logic_id);
    if ((ret != DSMI_OK) && (ret != DSMI_ERR_NOT_SUPPORT)) {
        gplog(LOG_ERR, "call dsmi_clear_ecc_isolated_statistics_info failed. err is %d.", ret);
    }

    return dcmi_convert_error_code(ret);
}

int dcmi_get_npu_device_frequency(int card_id, int device_id, int device_type, unsigned int *frequency)
{
    int ret;
    int device_logic_id = 0;

    ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", ret);
        return ret;
    }

    ret = dsmi_get_device_frequency(device_logic_id, device_type, frequency);
    if ((ret != DSMI_OK) && (ret != DSMI_ERR_NOT_SUPPORT)) {
        gplog(LOG_ERR, "call dsmi_get_device_frequency failed. err is %d.", ret);
    }

    return dcmi_convert_error_code(ret);
}

void hbm_data_transform(unsigned long long* hbm_total, unsigned long long* hbm_used)
{
    if (*hbm_total == MEMORY_SIZE_7GB_TO_MB) {
        *hbm_total += NOISE_7GB_TO_MB;
        *hbm_used += NOISE_7GB_TO_MB;
    }
    if (*hbm_total == MEMORY_SIZE_7_5GB_TO_MB) {
        *hbm_total += NOISE_7_5GB_TO_MB;
        *hbm_used += NOISE_7_5GB_TO_MB;
    }
    if (*hbm_total == MEMORY_SIZE_14GB_TO_MB) {
        *hbm_total += NOISE_14GB_TO_MB;
        *hbm_used += NOISE_14GB_TO_MB;
    }
    if (*hbm_total == MEMORY_SIZE_15GB_TO_MB) {
        *hbm_total += NOISE_15GB_TO_MB;
        *hbm_used += NOISE_15GB_TO_MB;
    }
    if (*hbm_total == MEMORY_SIZE_30GB_TO_MB) {
        *hbm_total += NOISE_30GB_TO_MB;
        *hbm_used += NOISE_30GB_TO_MB;
    }
    return;
}

int dcmi_get_npu_hbm_info(int card_id, int device_id, struct dcmi_hbm_info *hbm_info)
{
    int ret;
    int device_logic_id = 0;

    ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", ret);
        return ret;
    }

    ret = dsmi_get_hbm_info(device_logic_id, (struct dsmi_hbm_info_stru *)hbm_info);
    hbm_info->memory_size = hbm_info->memory_size / KB_TO_MB_TRANS_VALUE;
    hbm_info->memory_usage = hbm_info->memory_usage / KB_TO_MB_TRANS_VALUE;
    if (dcmi_board_chip_type_is_ascend_910b() || dcmi_board_chip_type_is_ascend_910_93()) {
        hbm_data_transform(&(hbm_info->memory_size), &(hbm_info->memory_usage));
    }
    if (ret != DSMI_OK && ret != DSMI_ERR_NOT_SUPPORT) {
        gplog(LOG_ERR, "call dsmi_get_hbm_info failed.err is %d.", ret);
    }

    return dcmi_convert_error_code(ret);
}

int dcmi_get_npu_device_memory_info_v2(int card_id, int device_id, struct dcmi_memory_info *memory_info)
{
    int ret;
    int device_logic_id = 0;

    ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", ret);
        return ret;
    }

    struct dcmi_get_memory_info_stru pdevice_memory_info = { 0 };
    ret = dcmi_get_npu_device_memory_info_v3(card_id, device_id, &pdevice_memory_info);
    if (ret == DCMI_OK) {
        memory_info->memory_size = pdevice_memory_info.memory_size;
        memory_info->freq = pdevice_memory_info.freq;
        memory_info->utiliza = pdevice_memory_info.utiliza;
    } else {
        gplog(LOG_ERR, "call dcmi_get_npu_device_memory_info_v3 failed. err is %d.", ret);
    }

    return ret;
}

void dcmi_output_memory_info(struct dsmi_get_memory_info_stru dsmi_memory_info,
    struct dcmi_get_memory_info_stru *memory_info)
{
    memory_info->memory_size = dsmi_memory_info.memory_size / KB_TO_MB_TRANS_VALUE;
    memory_info->memory_available = dsmi_memory_info.memory_available / KB_TO_MB_TRANS_VALUE;
    memory_info->freq = dsmi_memory_info.freq;
    memory_info->hugepagesize = dsmi_memory_info.hugepagesize;
    memory_info->hugepages_total = dsmi_memory_info.hugepages_total;
    memory_info->hugepages_free = dsmi_memory_info.hugepages_free;
    if (dsmi_memory_info.memory_available == DCMI_INVALID_VALUE) {
        memory_info->utiliza = DCMI_INVALID_VALUE;
    } else if ((dsmi_memory_info.memory_size == 0) ||
        (dsmi_memory_info.memory_size < dsmi_memory_info.memory_available)) {
        memory_info->utiliza = 0;
    } else {
        memory_info->utiliza = (dsmi_memory_info.memory_size - dsmi_memory_info.memory_available) *
        DIGITAL_NUM_TO_PER / dsmi_memory_info.memory_size;
    }
}

int dcmi_get_npu_device_memory_info_v3(int card_id, int device_id, struct dcmi_get_memory_info_stru *memory_info)
{
    int ret;
    int device_logic_id = 0;

    ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", ret);
        return ret;
    }

    if (dcmi_board_chip_type_is_ascend_910b() == TRUE || dcmi_board_chip_type_is_ascend_910_93() == TRUE) {
        gplog(LOG_ERR, "Device 910b 910_93 is not support.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

#ifndef _WIN32
    struct dsmi_get_memory_info_stru dsmi_memory_info = {0};
    ret = dsmi_get_memory_info_v2(device_logic_id, &dsmi_memory_info);
    if (ret == DSMI_OK) {
        dcmi_output_memory_info(dsmi_memory_info, memory_info);
    } else if (ret != DSMI_ERR_NOT_SUPPORT) {
        gplog(LOG_ERR, "call dsmi_get_memory_info_v2 failed. err is %d.", ret);
    }
#else
    struct dsmi_memory_info_stru pdevice_memory_info = {0};
    ret = dsmi_get_memory_info(device_logic_id, &pdevice_memory_info);
    if (ret == DSMI_OK) {
        bool support_chip_type = (dcmi_board_chip_type_is_ascend_310p() || dcmi_board_chip_type_is_ascend_910b() ||
            dcmi_board_chip_type_is_ascend_910());
        if (support_chip_type) {
            memory_info->memory_size = pdevice_memory_info.memory_size / KB_TO_MB_TRANS_VALUE;
        } else {
            memory_info->memory_size = pdevice_memory_info.memory_size;
        }
        memory_info->freq = pdevice_memory_info.freq;
        memory_info->utiliza = pdevice_memory_info.utiliza;
        memory_info->memory_available = memory_info->memory_size -
            memory_info->memory_size * memory_info->utiliza / DIGITAL_NUM_TO_PER;
    } else if (ret != DSMI_ERR_NOT_SUPPORT) {
        gplog(LOG_ERR, "call dsmi_get_memory_info failed. err is %d.", ret);
    }
#endif

    return dcmi_convert_error_code(ret);
}

int dcmi_get_npu_device_utilization_rate(int card_id, int device_id, int input_type, unsigned int *utilization_rate)
{
    int ret;
    int device_logic_id = 0;
    unsigned int tmp_rate = 0;

    ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d", ret);
        return ret;
    }
    /* dsmi_get_device_utilization_rate接口310场景下DDR内存占用率未考虑大页内存，
    暂时从新接口dsmi_get_memory_info_v2获取，等海思修复该问题再修改回来 */
    if (input_type == DCMI_UTILIZATION_RATE_DDR) {
        struct dcmi_get_memory_info_stru memory_info = {0};
        ret = dcmi_get_npu_device_memory_info_v3(card_id, device_id, &memory_info);
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "call dcmi_get_npu_device_memory_info_v3 failed. err is %d.", ret);
        }
        *utilization_rate = memory_info.utiliza;
        return ret;
    } else {
        ret = dsmi_get_device_utilization_rate(device_logic_id, input_type, &tmp_rate);
        if ((ret != DSMI_OK) && (ret != DSMI_ERR_NOT_SUPPORT)) {
            gplog(LOG_ERR, "call dsmi_get_device_utilization_rate failed. err is %d.", ret);
        }
        *utilization_rate = (ret == DSMI_ERR_NOT_SUPPORT) ? (tmp_rate | (0x1 << DCMI_VF_FLAG_BIT)) : tmp_rate;
        if (*utilization_rate > DCMI_USAGE_MAX) {
            gplog(LOG_INFO, "Invalid usage obtained through dsmi_get_device_utilization_rate. usage is %u",
                *utilization_rate);
            *utilization_rate = 0;
        }
    }

    return dcmi_convert_error_code(ret);
}

int dcmi_get_npu_soc_sensor_info(
    int card_id, int device_id, enum dcmi_manager_sensor_id sensor_id, union dcmi_sensor_info *sensor_info)
{
    int ret;
    int device_logic_id = 0;

    ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", ret);
        return ret;
    }

    ret = dsmi_get_soc_sensor_info(device_logic_id, sensor_id, (TAG_SENSOR_INFO *)(void *)sensor_info);
    if ((ret != DSMI_OK) && (ret != DSMI_ERR_NOT_SUPPORT)) {
        gplog(LOG_ERR, "call dsmi_get_soc_sensor_info failed. err is %d.", ret);
    }

    return dcmi_convert_error_code(ret);
}

int dcmi_get_npu_device_board_id(int card_id, int device_id, unsigned int *board_id)
{
    int ret;
    int device_logic_id = 0;
    struct dsmi_board_info_stru board_info = {0};
    const int chip_910_board_id_shif = 4;

    ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", ret);
        return ret;
    }

    ret = dsmi_get_board_info(device_logic_id, &board_info);
    if (ret == DSMI_OK) {
        if (dcmi_board_chip_type_is_ascend_910()) {
            // ascend910接口返回的boardid有误，低4位是预留位，非boardid信息位，需要右移丢掉
            *board_id = (board_info.board_id) >> chip_910_board_id_shif;
        } else {
            *board_id = board_info.board_id;
        }
    } else if (ret != DSMI_ERR_NOT_SUPPORT) {
        gplog(LOG_ERR, "call dsmi_get_board_info failed. err is %d.", ret);
    }

    return dcmi_convert_error_code(ret);
}

int dcmi_get_npu_device_component_count(int card_id, int device_id, unsigned int *component_count)
{
    int ret;
    int device_logic_id = 0;

    ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", ret);
        return ret;
    }

    ret = dsmi_get_component_count(device_logic_id, component_count);
    if ((ret != DSMI_OK) && (ret != DSMI_ERR_NOT_SUPPORT)) {
        gplog(LOG_ERR, "call dsmi_get_component_count failed. err is %d.", ret);
    }

    return dcmi_convert_error_code(ret);
}

int dcmi_get_npu_device_component_list(
    int card_id, int device_id, enum dcmi_component_type *component_table, unsigned int component_count)
{
    int ret;
    int device_logic_id = 0;

    ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", ret);
        return ret;
    }

    ret = dsmi_get_component_list(device_logic_id, (DSMI_COMPONENT_TYPE *)(void *)component_table, component_count);
    if ((ret != DSMI_OK) && (ret != DSMI_ERR_NOT_SUPPORT)) {
        gplog(LOG_ERR, "call dsmi_get_component_count failed. err is %d.", ret);
    }

    return dcmi_convert_error_code(ret);
}

int dcmi_get_npu_device_component_static_version(
    int card_id, int device_id, enum dcmi_component_type component_type, unsigned char *version_str, unsigned int len)
{
    int ret;
    int device_logic_id = 0;
    unsigned int length;

    ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", ret);
        return ret;
    }

    ret = dsmi_upgrade_get_component_static_version(
        device_logic_id, (DSMI_COMPONENT_TYPE)component_type, version_str, len, &length);
    if ((ret != DSMI_OK) && (ret != DSMI_ERR_NOT_SUPPORT)) {
        gplog(LOG_ERR, "call dsmi_upgrade_get_component_static_version failed. err is %d.", ret);
    }

    return dcmi_convert_error_code(ret);
}

int dcmi_get_npu_device_cgroup_info(int card_id, int device_id, struct dcmi_cgroup_info *cg_info)
{
    int ret;
    int device_logic_id = 0;

    ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", ret);
        return ret;
    }

    ret = dsmi_get_device_cgroup_info(device_logic_id, (struct tag_cgroup_info *)cg_info);
    if ((ret != DSMI_OK) && (ret != DSMI_ERR_NOT_SUPPORT)) {
        gplog(LOG_ERR, "call dsmi_get_device_cgroup_info failed. err is %d.", ret);
    }

    return dcmi_convert_error_code(ret);
}

int dcmi_get_npu_device_llc_perf_para(int card_id, int device_id, struct dcmi_llc_perf *perf_para)
{
    int ret;
    int device_logic_id = 0;

    ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", ret);
        return ret;
    }

    ret = dsmi_get_llc_perf_para(device_logic_id, (struct dsmi_llc_perf_stru *)perf_para);
    if ((ret != DSMI_OK) && (ret != DSMI_ERR_NOT_SUPPORT)) {
        gplog(LOG_ERR, "call dsmi_get_llc_perf_para failed. err is %d.", ret);
    }

    return dcmi_convert_error_code(ret);
}