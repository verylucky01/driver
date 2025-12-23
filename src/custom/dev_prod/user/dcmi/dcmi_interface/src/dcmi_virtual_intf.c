/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <stdbool.h>
#include <errno.h>
#include "securec.h"
#include "dsmi_common_interface_custom.h"
#include "dcmi_fault_manage_intf.h"
#include "dcmi_log.h"
#include "dcmi_product_judge.h"
#include "dcmi_inner_cfg_persist.h"
#include "dcmi_inner_info_get.h"
#include "dcmi_environment_judge.h"
#include "dcmi_virtual_intf.h"

int dsmi_config_enable(int device_id, CONFIG_ITEM config_item, DSMI_DEVICE_TYPE device_type, int enable_flag);
int dsmi_get_enable(int device_id, CONFIG_ITEM config_item, DSMI_DEVICE_TYPE device_type, int *enable_flag);

const char *template_name_310p[DCMI_310P_TEMPLATE_NUM] = {
    "vir01", "vir02", "vir02_1c", "vir04", "vir04_3c", "vir04_3c_ndvpp", "vir04_4c_dvpp"
};

const char *template_name_910[DCMI_910_TEMPLATE_NUM] = {
    "vir02", "vir04", "vir08", "vir16"
};

const char *template_name_910b_bin0[DCMI_910B_BIN0_TEMPLATE_NUM] = {
    "vir12_3c_32g", "vir06_1c_16g"
};


const char *template_name_910b_bin1[DCMI_910B_BIN1_TEMPLATE_NUM] = {
    "vir10_3c_32g", "vir05_1c_16g"
};

const char *template_name_910b_bin2[DCMI_910B_BIN2_TEMPLATE_NUM] = {
    "vir10_3c_16g", "vir10_4c_16g_m", "vir10_3c_16g_nm", "vir05_1c_8g"
};

int dcmi_get_template_conf_for_310p(int card_id, const char *template_name, struct dcmi_create_vdev_in *vdev_conf)
{
    int ret, idx;
    struct dcmi_computing_template resource = { { 0 } };

    for (idx = 0; idx < DCMI_310P_TEMPLATE_NUM; idx++) {
        if (strcmp(template_name, template_name_310p[idx]) == 0) {
            break;
        }
    }

    if (idx == DCMI_310P_TEMPLATE_NUM) {
        gplog(LOG_ERR, "template_name doesn't match any in template.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    ret = dcmi_get_template_info_by_name(card_id, template_name, &resource);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_template_info_by_name failed. ret is %d.", ret);
        return DCMI_ERR_CODE_INNER_ERR;
    }

    vdev_conf->computing.aic = (float)resource.aicore_num;
    vdev_conf->computing.memory_size = resource.mem_size;
    vdev_conf->computing.device_aicpu = (unsigned short)resource.aicpu_num;
    vdev_conf->media.vpc = resource.vpc;
    vdev_conf->media.jpegd = resource.jpegd;
    vdev_conf->media.jpege = resource.jpege;
    vdev_conf->media.venc = resource.venc;
    vdev_conf->media.vdec = resource.vdec;

    ret = strncpy_s(vdev_conf->name, sizeof(vdev_conf->name), template_name, strlen(template_name));
    if (ret != EOK) {
        return DCMI_ERR_CODE_SECURE_FUN_FAIL;
    }

    return DCMI_OK;
}

int dcmi_get_template_conf_for_910(int card_id, const char *template_name, struct dcmi_create_vdev_in *vdev_conf)
{
    int ret, idx;
    struct dcmi_computing_template resource = { { 0 } };

    for (idx = 0; idx < DCMI_910_TEMPLATE_NUM; idx++) {
        if (strcmp(template_name, template_name_910[idx]) == 0) {
            break;
        }
    }

    if (idx == DCMI_910_TEMPLATE_NUM) {
        gplog(LOG_ERR, "template_name doesn't match any in template.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    ret = dcmi_get_template_info_by_name(card_id, template_name, &resource);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_template_info_by_name failed. ret is %d.", ret);
        return DCMI_ERR_CODE_INNER_ERR;
    }

    vdev_conf->computing.aic = (float)resource.aicore_num;
    vdev_conf->computing.memory_size = resource.mem_size;
    vdev_conf->media.vpc = resource.vpc;
    vdev_conf->media.jpegd = resource.jpegd;
    vdev_conf->media.jpege = resource.jpege;
    vdev_conf->media.pngd = resource.pngd;
    vdev_conf->media.vdec = resource.vdec;

    ret = strncpy_s(vdev_conf->name, sizeof(vdev_conf->name), template_name, strlen(template_name));
    if (ret != EOK) {
        return DCMI_ERR_CODE_SECURE_FUN_FAIL;
    }

    return DCMI_OK;
}

int dcmi_check_910b_template_name_inner(const char *template_name, struct dcmi_bin_type_910b_map *bin_type_map_list)
{
    int template_num, idx;
    const char **supported_template;
    struct dcmi_bin_type_910b_map *type_map;

    int board_id = dcmi_get_board_id_inner();
    for (type_map = bin_type_map_list; type_map->supported_template!= NULL; type_map++) {
        if (board_id == type_map->dcmi_board_id) {
            template_num = type_map->template_num;
            supported_template = type_map->supported_template;
            break;
        }
    }
    
    if (type_map->supported_template == NULL) {
        return DCMI_ERR_CODE_INNER_ERR;
    }

    for (idx = 0; idx < template_num; idx++) {
        if (strcmp(template_name, supported_template[idx]) == 0) {
            break;
        }
    }

    if (idx == template_num) {
        gplog(LOG_ERR, "template_name doesn't match any in template.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }
    return DCMI_OK;
}

int dcmi_check_910b_template_name(const char *template_name)
{
    int ret;
    struct dcmi_bin_type_910b_map bin_type_map_list[] = {
        {DCMI_A900T_POD_A1_BIN0_BOARD_ID, DCMI_910B_BIN0_TEMPLATE_NUM, template_name_910b_bin0},
        {DCMI_A900T_POD_A1_BIN0_P3_BOARD_ID, DCMI_910B_BIN0_TEMPLATE_NUM, template_name_910b_bin0},
        {DCMI_A200T_BOX_A1_BIN0_BOARD_ID, DCMI_910B_BIN0_TEMPLATE_NUM, template_name_910b_bin0},
        {DCMI_A300T_A1_BIN0_BOARD_ID, DCMI_910B_BIN0_TEMPLATE_NUM, template_name_910b_bin0},
        {DCMI_A900T_POD_A1_BIN3_BOARD_ID, DCMI_910B_BIN0_TEMPLATE_NUM, template_name_910b_bin0},
        {DCMI_A900T_POD_A1_BIN3_P3_BOARD_ID, DCMI_910B_BIN0_TEMPLATE_NUM, template_name_910b_bin0},
        {DCMI_A200T_BOX_A1_BIN3_BOARD_ID, DCMI_910B_BIN0_TEMPLATE_NUM, template_name_910b_bin0},
        {DCMI_A800T_POD_A2_BIN0_BOARD_ID, DCMI_910B_BIN0_TEMPLATE_NUM, template_name_910b_bin0},
        {DCMI_A900T_POD_A1_BIN2_BOARD_ID, DCMI_910B_BIN2_TEMPLATE_NUM, template_name_910b_bin2},
        {DCMI_A900T_POD_A1_BIN2_P3_BOARD_ID, DCMI_910B_BIN2_TEMPLATE_NUM, template_name_910b_bin2},
        {DCMI_A900T_POD_A1_BIN2X_BOARD_ID, DCMI_910B_BIN2_TEMPLATE_NUM, template_name_910b_bin2},
        {DCMI_A900T_POD_A1_BIN2X_P3_BOARD_ID, DCMI_910B_BIN2_TEMPLATE_NUM, template_name_910b_bin2},
        {DCMI_A900T_POD_A1_BIN2X_1_BOARD_ID, DCMI_910B_BIN2_TEMPLATE_NUM, template_name_910b_bin2},
        {DCMI_A800I_POD_A2_BIN4_1_PCIE_BOARD_ID, DCMI_910B_BIN2_TEMPLATE_NUM, template_name_910b_bin2},
        {DCMI_A900T_POD_A1_BIN2X_1_P3_BOARD_ID, DCMI_910B_BIN2_TEMPLATE_NUM, template_name_910b_bin2},
        {DCMI_A300T_A1_BIN2_BOARD_ID, DCMI_910B_BIN2_TEMPLATE_NUM, template_name_910b_bin2},
        {DCMI_A200T_BOX_A1_BIN2_BOARD_ID, DCMI_910B_BIN2_TEMPLATE_NUM, template_name_910b_bin2},
        {DCMI_A800I_POD_A2_BIN2_BOARD_ID, DCMI_910B_BIN2_TEMPLATE_NUM, template_name_910b_bin2},
        {DCMI_A800I_POD_A2_BIN2_1_BOARD_ID, DCMI_910B_BIN2_TEMPLATE_NUM, template_name_910b_bin2},
        {DCMI_A900T_POD_A1_BIN1_BOARD_ID, DCMI_910B_BIN1_TEMPLATE_NUM, template_name_910b_bin1},
        {DCMI_A900T_POD_A1_BIN1_P3_BOARD_ID, DCMI_910B_BIN1_TEMPLATE_NUM, template_name_910b_bin1},
        {DCMI_A300T_A1_BIN1_300W_BOARD_ID, DCMI_910B_BIN1_TEMPLATE_NUM, template_name_910b_bin1},
        {DCMI_A300T_A1_BIN1_350W_BOARD_ID, DCMI_910B_BIN1_TEMPLATE_NUM, template_name_910b_bin1},
        {DCMI_A200T_BOX_A1_BIN1_BOARD_ID, DCMI_910B_BIN1_TEMPLATE_NUM, template_name_910b_bin1},
        {DCMI_A800T_POD_A2_BIN1_BOARD_ID, DCMI_910B_BIN1_TEMPLATE_NUM, template_name_910b_bin1},
        {DCMI_A300I_A2_BIN2_BOARD_ID, DCMI_910B_BIN2_TEMPLATE_NUM, template_name_910b_bin2},
        {DCMI_A300I_A2_BIN2_64G_BOARD_ID, DCMI_910B_BIN2_TEMPLATE_NUM, template_name_910b_bin2},
        {0, 0, NULL}
    };

    ret = dcmi_check_910b_template_name_inner(template_name, bin_type_map_list);
    if (ret != DCMI_OK) {
        return ret;
    }

    return DCMI_OK;
}

int dcmi_get_template_conf_for_910b(int card_id, const char *template_name, struct dcmi_create_vdev_in *vdev_conf)
{
    int ret;
    struct dcmi_computing_template resource = { { 0 } };

    ret = dcmi_check_910b_template_name(template_name);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "template_name doesn't match any in template.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    ret = dcmi_get_template_info_by_name(card_id, template_name, &resource);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_template_info_by_name failed. ret is %d.", ret);
        return DCMI_ERR_CODE_INNER_ERR;
    }
    if (resource.aicpu_num == SHARE_AICPU) {
        // 算力切分模板共享aicpu时，参数传入0xffff
        vdev_conf->computing.device_aicpu = 0xffff;
    } else {
        vdev_conf->computing.device_aicpu = (unsigned short)resource.aicpu_num;
    }
    vdev_conf->computing.aic = (float)resource.aicore_num;
    vdev_conf->computing.memory_size = resource.mem_size;
    vdev_conf->media.vpc = resource.vpc;
    vdev_conf->media.jpegd = resource.jpegd;
    vdev_conf->media.jpege = resource.jpege;
    vdev_conf->media.venc = resource.venc;
    vdev_conf->media.vdec = resource.vdec;

    ret = strncpy_s(vdev_conf->name, sizeof(vdev_conf->name), template_name, strlen(template_name));
    if (ret != EOK) {
        return DCMI_ERR_CODE_SECURE_FUN_FAIL;
    }

    return DCMI_OK;
}

int dcmi_get_template_conf(int card_id, const char *template_name, struct dcmi_create_vdev_in *vdev_conf)
{
    switch (dcmi_get_board_chip_type()) {
        case DCMI_CHIP_TYPE_D310P:
            return dcmi_get_template_conf_for_310p(card_id, template_name, vdev_conf);
        case DCMI_CHIP_TYPE_D910:
            return dcmi_get_template_conf_for_910(card_id, template_name, vdev_conf);
        case DCMI_CHIP_TYPE_D910B:
            return dcmi_get_template_conf_for_910b(card_id, template_name, vdev_conf);
        default:
            return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    return DCMI_OK;
}

int dcmi_check_set_vnpu_permission(void)
{
    // (1) 310非root或虚拟机或容器都返回权限错误
    // (2) 310p 支持直通vm中进行算力切分以及宿主机root算力切分
    // (3) 910非root或vm都返回权限错误
    // (4) 910B/910_93 支持物理机root/特权容器root
    switch (dcmi_get_board_chip_type()) {
        case DCMI_CHIP_TYPE_D310:
            if (!dcmi_is_in_phy_machine_root()) {
                gplog(LOG_OP, "Operation not permitted, only root user on physical machine can call this api.");
                return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
            }
            break;
        case DCMI_CHIP_TYPE_D310P:
            if (dcmi_check_run_not_root()) {
                gplog(LOG_OP, "Operation not permitted, only root user on phy machine,"\
                    " privileged docker or vm can call this api.");
                return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
            }
            break;
        case DCMI_CHIP_TYPE_D910:
            if (dcmi_check_run_not_root() || dcmi_check_run_in_vm()) {
                gplog(LOG_OP, "Operation not permitted, only root user on physical machine or"\
                    " privileged docker can call this api.");
                return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
            }
            break;
        case DCMI_CHIP_TYPE_D910B:
        case DCMI_CHIP_TYPE_D910_93:
            if (!(dcmi_is_in_phy_machine_root() || dcmi_is_in_phy_privileged_docker_root())) {
                gplog(LOG_OP, "Operation not permitted, only root user on physical machine or"\
                    " privileged docker can call this api.");
                return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
            }
            break;
        default:
            gplog(LOG_ERR, "Unsupported chip type.");
            return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    return DCMI_OK;
}

bool dcmi_check_vnpu_is_docker_mode(void)
{
    int mode;

    int err = dcmi_get_vdevice_mode(&mode);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_vdevice_mode failed. err is %d.", err);
        return false;
    }

    return (mode == 0) ? true : false;
}

int dcmi_find_vnpu_in_vm_mode_for_one_chip(int cardid, int chipid, int *exit_flag)
{
    int find = 0;
    int ret;
    int ret1;
    struct dcmi_pcie_info_all pcie_info = {0};
    char pcie_bdf_buff[16] = {0};
    char mdev_info_path[MAX_STR_LENTH] = {0};
    char mdev_info_context[MAX_STR_LENTH];

    ret = dcmi_get_device_pcie_info_v2(cardid, chipid, &pcie_info);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_pcie_info_v2 failed. cardid %d chip %d err is %d", cardid, chipid, ret);
        return ret;
    }
    ret = snprintf_s(pcie_bdf_buff, sizeof(pcie_bdf_buff), sizeof(pcie_bdf_buff) - 1, "%04x:%02x:%02x.%x",
        pcie_info.domain, pcie_info.bdf_busid, pcie_info.bdf_deviceid, pcie_info.bdf_funcid);
    ret1 = snprintf_s(mdev_info_path, sizeof(mdev_info_path), sizeof(mdev_info_path) - 1,
        "/sys/bus/pci/devices/%s/mdev_info", pcie_bdf_buff);
    if ((ret <= 0) || (ret1 <= 0)) {
        gplog(LOG_ERR, "snprintf_s failed, ret %d ret1 %d", ret, ret1);
        return DCMI_ERR_CODE_SECURE_FUN_FAIL;
    }
    FILE *fp = fopen((const char *)mdev_info_path, "r");
    if (fp == NULL) {
        gplog(LOG_ERR, "open file %s failed, errno %d", mdev_info_path, errno);
        return DCMI_ERR_CODE_FILE_OPERATE_FAIL;
    }
    while (!feof(fp)) {
        (void)memset_s(mdev_info_context, sizeof(mdev_info_context), 0, sizeof(mdev_info_context));
        if (fgets(mdev_info_context, sizeof(mdev_info_context), fp) == NULL) {
            break;
        }
        if (strstr(mdev_info_context, "[vd] uuid ") != NULL) {
            find = 1;
            break;
        }
    }
    (void)fclose(fp);
    *exit_flag = find;
    return DCMI_OK;
}

int dcmi_find_vnpu_in_vm_mode(void)
{
    int card_id_list[MAX_CARD_NUM] = {0};
    int card_count = 0;
    int card_index;
    int npu_count = 0;
    int mcu_id;
    int cpu_id;
    int npu_id;
    int ret;
    int exit_flag;

    ret = dcmi_get_card_list(&card_count, card_id_list, MAX_CARD_NUM);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_card_list failed. err is %d", ret);
        return ret;
    }

    for (card_index = 0; card_index < card_count; card_index++) {
        ret = dcmi_get_device_id_in_card(card_id_list[card_index], &npu_count, &mcu_id, &cpu_id);
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "dcmi_get_device_id_in_card card %d failed. err is %d\n", card_id_list[card_index], ret);
            continue;
        }

        for (npu_id = 0; npu_id < npu_count; npu_id++) {
            if (dcmi_find_vnpu_in_vm_mode_for_one_chip(card_id_list[card_index], npu_id, &exit_flag) != DCMI_OK) {
                continue;
            }
            if (exit_flag == 1) {
                return DCMI_OK;
            }
        }
    }
    return DCMI_ERR_CODE_INNER_ERR;
}

int dcmi_find_vnpu_in_docker_mode(void)
{
    int card_id_list[MAX_CARD_NUM] = {0};
    int card_count = 0;
    int npu_count = 0;
    struct dcmi_soc_total_resource total_resource = { 0 };
    unsigned int resource_len = sizeof(total_resource);
    int mcu_id;
    int cpu_id;
    int ret;
    int npu_id;
    int card_index;

    ret = dcmi_get_card_list(&card_count, card_id_list, MAX_CARD_NUM);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_card_list failed. err is %d", ret);
        return ret;
    }

    for (card_index = 0; card_index < card_count; card_index++) {
        ret = dcmi_get_device_id_in_card(card_id_list[card_index], &npu_count, &mcu_id, &cpu_id);
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "dcmi_get_device_id_in_card card %d failed. err is %d\n", card_id_list[card_index], ret);
            continue;
        }

        for (npu_id = 0; npu_id < npu_count; npu_id++) {
            ret = dcmi_get_device_info(card_id_list[card_index], npu_id, DCMI_MAIN_CMD_VDEV_MNG,
                DCMI_VMNG_SUB_CMD_GET_TOTAL_RESOURCE, &total_resource, &resource_len);
            if (ret != DCMI_OK) {
                gplog(LOG_ERR, "get total resource failed. card_id=%d chip_id=%d ret=%d\n", card_id_list[card_index],
                    npu_id, ret);
                return ret;
            }

            if (total_resource.vdev_num > 0) {
                gplog(LOG_INFO, "find vnpu. card_id=%d chip_id=%d vdev_num=%u\n", card_id_list[card_index], npu_id,
                    total_resource.vdev_num);
                return DCMI_OK;
            }
        }
    }

    return DCMI_ERR_CODE_INNER_ERR;
}

bool dcmi_check_card_is_split_phy(int card_id)
{
    int ret;
    struct dcmi_soc_total_resource soc_total_resource = { 0 };
    unsigned int resource_len;
    unsigned char work_mode;

    /* 910场景下 SMP模式不支持算力切分;不需要判断是否为vnpu */
     /* 910标卡环境不区分AMP和SMP;不需要判断工作模式 */
    if (dcmi_board_chip_type_is_ascend_910() && (!dcmi_board_type_is_card())) {
        ret = dcmi_get_npu_work_mode(card_id, &work_mode);
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "failed to query npu work mode. err is %d.\n", ret);
            return FALSE;
        }
        if (work_mode == DCMI_NPU_WORK_MODE_SMP) {
            /* 910的SMP模式不支持算力切分,此处直接返回false */
            return FALSE;
        }
    }

    if (card_id == 0xff) {
        if (dcmi_find_vnpu_in_docker_mode() == DCMI_OK) {
            return TRUE;
        } else {
            return FALSE;
        }
    } else {
        resource_len = sizeof(soc_total_resource);
        ret = dcmi_get_device_info(card_id, 0, DCMI_MAIN_CMD_VDEV_MNG,
            DCMI_VMNG_SUB_CMD_GET_TOTAL_RESOURCE, &soc_total_resource, &resource_len);
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "get total resource failed. card_id=%d ret is %d", card_id, ret);
        }
        if (soc_total_resource.vdev_num > 0) {
            return TRUE;
        }
    }
    return FALSE;
}

int dcmi_save_create_vnpu_cfg(int card_id, int device_id, int logic_id, struct dcmi_create_vdev_out *vdev,
    const char *template_name)
{
    int err;
    unsigned int device_phy_id;
    unsigned int recover_enable;

    if ((dcmi_is_in_phy_machine() == TRUE) ||
        ((dcmi_board_chip_type_is_ascend_910() == TRUE) && (dcmi_check_run_in_docker() == TRUE)) ||
        ((dcmi_board_chip_type_is_ascend_910b() == TRUE) && (dcmi_is_in_phy_privileged_docker_root() == TRUE)) ||
        (dcmi_board_chip_type_is_ascend_310p() == TRUE)) {
        err = dcmi_cfg_get_config_recover_mode(&recover_enable);
        if (err != DCMI_OK) {
            gplog(LOG_ERR, "dcmi_cfg_get_config_recover_mode failed. err is %d", err);
            return err;
        }

        if (recover_enable == DCMI_CFG_RECOVER_ENABLE) {
            err = dcmi_get_device_phyid_from_logicid((unsigned int)logic_id, &device_phy_id);
            if (err != DCMI_OK) {
                gplog(LOG_ERR, "dcmi_get_device_phyid_from_logicid failed. err is %d", err);
                return err;
            }

            err = dcmi_cfg_insert_creat_vnpu_cmdline(device_phy_id, vdev, card_id, device_id, template_name);
            if (err != DCMI_OK) {
                gplog(LOG_ERR, "dcmi_cfg_insert_creat_vnpu_cmdline failed. err is %d", err);
                return err;
            }
        }
    }

    return DCMI_OK;
}

int dcmi_save_destroy_vnpu_cfg(int card_id, int device_id, int logic_id, unsigned int vdev_id)
{
    int err;
    unsigned int device_phy_id;
    unsigned int recover_enable;

    if ((dcmi_is_in_phy_machine() == TRUE) ||
        ((dcmi_board_chip_type_is_ascend_910() == TRUE) && (dcmi_check_run_in_docker() == TRUE)) ||
        ((dcmi_board_chip_type_is_ascend_910b() == TRUE) && (dcmi_is_in_phy_privileged_docker_root() == TRUE)) ||
        (dcmi_board_chip_type_is_ascend_310p() == TRUE)) {
        err = dcmi_cfg_get_config_recover_mode(&recover_enable);
        if (err != DCMI_OK) {
            gplog(LOG_ERR, "dcmi_cfg_get_config_recover_mode failed. err is %d", err);
            return err;
        }

        if (recover_enable == DCMI_CFG_RECOVER_ENABLE) {
            err = dcmi_get_device_phyid_from_logicid((unsigned int)logic_id, &device_phy_id);
            if (err != DCMI_OK) {
                gplog(LOG_ERR, "dcmi_get_device_phyid_from_logicid failed. err is %d", err);
                return err;
            }

            err = dcmi_cfg_insert_destroy_vnpu_cmdline(device_phy_id, vdev_id, card_id, device_id);
            if (err != DCMI_OK) {
                gplog(LOG_ERR, "dcmi_cfg_insert_destroy_vnpu_cmdline failed. err is %d", err);
                return err;
            }
        }
    }

    return DCMI_OK;
}

void dcmi_record_template_conf(int card_id, int device_id, const char *template_name,
    struct dcmi_create_vdev_in vdev_conf)
{
    gplog(LOG_INFO, "card_id:[%d] device_id:[%d] template_name:[%s]", card_id, device_id, template_name);
    gplog(LOG_INFO, "alloc aic:      [%f]", vdev_conf.computing.aic);
    gplog(LOG_INFO, "alloc memory:   [%llu]", vdev_conf.computing.memory_size);
    gplog(LOG_INFO, "alloc aicpu_num:[%hu]", vdev_conf.computing.device_aicpu);
    gplog(LOG_INFO, "alloc vpc:      [%f]", vdev_conf.media.vpc);
    gplog(LOG_INFO, "alloc vdec:     [%f]", vdev_conf.media.vdec);
    gplog(LOG_INFO, "alloc jpegd:    [%f]", vdev_conf.media.jpegd);
    gplog(LOG_INFO, "alloc pngd:     [%f]", vdev_conf.media.pngd);
    gplog(LOG_INFO, "alloc venc:     [%f]", vdev_conf.media.venc);
    gplog(LOG_INFO, "alloc jpege:    [%f]", vdev_conf.media.jpege);
}

int set_disable_sriov(int card_id, int device_id, int err)
{
    struct dcmi_soc_total_resource soc_total_resource = { 0 };
    unsigned int resource_len = sizeof(soc_total_resource);
    unsigned int disable_sriov = DCMI_SRIOV_DISABLE;
    int ret;

    // 仅910B算力切分需要使能sriov
    if (!dcmi_board_chip_type_is_ascend_910b()) {
        return err;
    }
    ret = dcmi_get_device_info(card_id, device_id, DCMI_MAIN_CMD_VDEV_MNG,
        DCMI_VMNG_SUB_CMD_GET_TOTAL_RESOURCE, &soc_total_resource, &resource_len);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "get total resource failed. ret is %d", ret);
        return ret;
    }

    // 若当前没有切分的虚拟设备，关闭sriov使能
    if (soc_total_resource.vdev_num == 0) {
        ret = dcmi_set_sriov_cfg(card_id, device_id, &disable_sriov);
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "dcmi_set_sriov_cfg failed. err is %d.", ret);
            return ret;
        }
    }
    return DCMI_OK;
}

int dcmi_set_sriov_cfg(int card_id, int device_id, unsigned int* sriov_cfg)
{
    int ret;
    int device_logic_id = 0;

    ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", ret);
        return ret;
    }

    ret = dsmi_set_device_info(device_logic_id, (DSMI_MAIN_CMD)DCMI_MAIN_CMD_VDEV_MNG,
        DCMI_VMNG_SUB_CMD_SET_SRIOV_SWITCH, sriov_cfg, sizeof(unsigned int));
    if ((ret != DSMI_OK) && (ret != DSMI_ERR_NOT_SUPPORT)) {
        gplog(LOG_ERR, "call dsmi_set_device_info failed. err is %d.", ret);
    }

    return dcmi_convert_error_code(ret);
}

int dcmi_npu_create_vdevice(int card_id, int device_id, struct dcmi_create_vdev_res_stru *vdev,
    struct dcmi_create_vdev_out *out)
{
    struct dcmi_create_vdev_in create_vdev_cfg = { { 0 } };
    int err;
    int device_logic_id = 0;
    const char *template_name = vdev->template_name;

    err = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", err);
        return err;
    }

    err = dcmi_get_template_conf(card_id, template_name, &create_vdev_cfg);
    dcmi_record_template_conf(card_id, device_logic_id, template_name, create_vdev_cfg);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_template_conf failed. err is %d", err);
        return err;
    }

    create_vdev_cfg.base.vfg_id = vdev->vfg_id;
    create_vdev_cfg.computing.memory_size = 0; // memory入参为0表示dsmi侧会自动切分内存资源
    err = dsmi_create_vdevice(device_logic_id, vdev->vdev_id, (struct dsmi_create_vdev_res_stru *)&create_vdev_cfg,
        (struct dsmi_create_vdev_result *)out);
    if (err != DSMI_OK) {
        gplog(LOG_ERR, "dsmi_create_vdevice failed. err is %d", err);
        return dcmi_convert_error_code(err);
    }

    err = dcmi_save_create_vnpu_cfg(card_id, device_id, device_logic_id, out, template_name);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_save_create_vnpu_cfg failed. err is %d", err);
        (void)dsmi_destroy_vdevice(device_logic_id, out->vdev_id);
    }
    return err;
}


int dcmi_npu_destroy_vdevice(int card_id, int device_id, int vdev_id)
{
    int err;
    int device_logic_id = 0;
    unsigned int device_phy_id;
    unsigned int recover_enable;
    unsigned int cfg_not_exist = FALSE;
    struct dcmi_create_vdev_out out = { 0 };
    struct dcmi_create_vdev_res_stru vdev = { 0 };

    err = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", err);
        return err;
    }

    err = dcmi_get_device_phyid_from_logicid((unsigned int)device_logic_id, &device_phy_id);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_phyid_from_logicid failed. err is %d", err);
        return err;
    }

    err = dcmi_cfg_get_config_recover_mode(&recover_enable);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_cfg_get_config_recover_mode failed. err is %d", err);
        return err;
    }

    // vdev_id的值0xffff(65535)被含义为删除所有所属设备下的所有虚拟设备，不会出现正常使用设备id为0xffff的场景
    if ((recover_enable == DCMI_CFG_RECOVER_ENABLE) && (vdev_id != 0xffff)) {
        err = dcmi_cfg_get_create_vnpu_template(device_phy_id, (unsigned int)vdev_id, &vdev);
        if ((err != DCMI_OK) && (err != DCMI_ERR_CODE_DEVICE_NOT_EXIST)) {
            gplog(LOG_ERR, "dcmi_cfg_get_create_vnpu_template failed. err is %d", err);
            return err;
        } else if (err == DCMI_ERR_CODE_DEVICE_NOT_EXIST) {
            cfg_not_exist = TRUE;
        }
    }
    err = dsmi_destroy_vdevice(device_logic_id, vdev_id);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dsmi_destroy_vdevice failed. err is %d, vdev_id is %d", err, vdev_id);
        return dcmi_convert_error_code(err);
    }

    if (cfg_not_exist == FALSE) {
        err = dcmi_save_destroy_vnpu_cfg(card_id, device_id, device_logic_id, vdev_id);
        if (err != DCMI_OK) {
            gplog(LOG_ERR, "dcmi_save_destroy_vnpu_cfg failed. err is %d", err);
            if (recover_enable == DCMI_CFG_RECOVER_ENABLE) {
                (void)dcmi_create_vdevice(card_id, device_id, &vdev, &out);
            }
        }
    }
    return err;
}

int dcmi_vnpu_work_mode_is_support(unsigned int *support)
{
    int ret;
    unsigned char work_mode = 0xff;
    int card_id_list[MAX_CARD_NUM] = {0};
    int card_count = 0;
    int index;

    /* 910b默认AMP，标卡和服务器都支持 */
    bool support_chip_type = dcmi_board_chip_type_is_ascend_910b();
    if (support_chip_type) {
        *support = TRUE;
        return DCMI_OK;
    }

    /* 标卡形态支持vnpu操作 */
    support_chip_type = ((dcmi_board_chip_type_is_ascend_310p() || dcmi_board_chip_type_is_ascend_910()) &&
        dcmi_board_type_is_card());
    if (support_chip_type) {
        *support = TRUE;
        return DCMI_OK;
    }

    support_chip_type = (dcmi_board_chip_type_is_ascend_910() && dcmi_board_type_is_server());
    if (support_chip_type) {
        ret = dcmi_get_card_list(&card_count, card_id_list, MAX_CARD_NUM);
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "dcmi_get_card_list failed. err is %d", ret);
            return ret;
        }

        for (index = 0; index < card_count; index++) {
            ret = dcmi_get_npu_work_mode(card_id_list[index], &work_mode);
            if (ret != DCMI_OK) {
                gplog(LOG_ERR, "dcmi_get_npu_work_mode card id %d failed. ret is %d", card_id_list[index], ret);
                continue;
            } else {
                break;
            }
        }
        /* 只有AMP模式支持vnpu相关操作 0: AMP模式，1：SMP模式 */
        *support = (work_mode == 0) ? TRUE : FALSE;
        if (*support == FALSE) {
            gplog(LOG_ERR, "SMP does not support dcmi_get_vnpu_config_recover_mode.");
        }
    } else {
        *support = FALSE;
    }
    return DCMI_OK;
}

int dcmi_check_vnpu_chip_is_vir(int card_id, int device_id, int *vir_flag)
{
    struct dcmi_chip_info chip_info = { { 0 } };

    int ret = dcmi_get_device_chip_info(card_id, device_id, &chip_info);
    if (ret != DCMI_OK) {
        return ret;
    }

    if (strstr((char *)chip_info.chip_name, "vir") != NULL) {
        *vir_flag = 1;
    }
    return DCMI_OK;
}

int dcmi_all_vnpu_is_vir(int *all_vir_flag)
{
    int ret;
    int vir_flag = 0;
    int card_list[MAX_CARD_NUM] = {0};
    int card_count = 0;
    int index;
    int npu_count = 0;
    int mcu_id;
    int cpu_id;
    int npu_id;

    ret = dcmi_get_card_list(&card_count, card_list, MAX_CARD_NUM);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_card_list failed. err is %d", ret);
        return ret;
    }

    for (index = 0; index < card_count; index++) {
        ret = dcmi_get_device_id_in_card(card_list[index], &npu_count, &mcu_id, &cpu_id);
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "dcmi_get_device_id_in_card card %d failed. err is %d\n", card_list[index], ret);
            continue;
        }

        for (npu_id = 0; npu_id < npu_count; npu_id++) {
            ret = dcmi_check_vnpu_chip_is_vir(card_list[index], npu_id, &vir_flag);
            if (ret != DCMI_OK) {
                gplog(LOG_ERR, "check vnpu vir failed. cardid %d chipid %d ret:%d\n", card_list[index], npu_id, ret);
                return ret;
            }

            if (vir_flag == 0) {
                gplog(LOG_INFO, "find non-vir npu. cardid %d chipid %d\n", card_list[index], npu_id);
                *all_vir_flag = 0;
                break;
            }
        }
    }

    return DCMI_OK;
}

int dcmi_check_chip_is_in_split_mode(int card_id, int device_id)
{
    /* 覆盖各场景是否为切分判断通用函数 */
    int vir_flag = 1;
    unsigned int env_flag = 0;
    int ret = 0;
    int device_logic_id = 0;
    struct dcmi_soc_total_resource soc_total_resource = { 0 };
    unsigned int resource_len;
    ret = dcmi_get_environment_flag(&env_flag);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "Failed to get dcmi_get_environment_flag.\n");
        return DCMI_ERR_CODE_INNER_ERR;
    }
    if (env_flag == ENV_PHYSICAL_PRIVILEGED_CONTAINER ||
        env_flag == ENV_PHYSICAL) {
        ret = dcmi_get_device_logic_id(&device_logic_id, card_id, 0);
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", ret);
            return ret;
        }
        resource_len = sizeof(soc_total_resource);
        ret = dsmi_get_device_info(device_logic_id, (DSMI_MAIN_CMD)DCMI_MAIN_CMD_VDEV_MNG,
            DCMI_VMNG_SUB_CMD_GET_TOTAL_RESOURCE, &soc_total_resource, &resource_len);
        if (dcmi_convert_error_code(ret) != DCMI_OK) {
            gplog(LOG_ERR, "get total resource failed. card_id=%d ret is %d", card_id, ret);
        }
        if (soc_total_resource.vdev_num > 0) {
            return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
        } else {
            return DCMI_OK;
        }
    } else {
        ret = dcmi_all_vnpu_is_vir(&vir_flag);
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "Failed to get dcmi_all_vnpu_is_vir.\n");
            return DCMI_ERR_CODE_INNER_ERR;
        }
        if (vir_flag == 1) {
            return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
        }
        return DCMI_OK;
    }
}

int dcmi_create_vdevice(int card_id, int device_id, struct dcmi_create_vdev_res_stru *vdev,
    struct dcmi_create_vdev_out *out)
{
    enum dcmi_unit_type device_type = INVALID_TYPE;
    unsigned int enable_sriov = DCMI_SRIOV_ENABLE;
    int err = dcmi_check_set_vnpu_permission();
    if (err != DCMI_OK) {
        gplog(LOG_OP, "dcmi_create_vdevice call dcmi_check_set_vnpu_permission failed. err is %d", err);
        return err;
    }

    if (vdev == NULL || out == NULL) {
        gplog(LOG_ERR, "vdev or out is NULL.");
        gplog(LOG_OP, "dcmi_create_vdevice parameter vdev or out is NULL.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    err = dcmi_get_device_type(card_id, device_id, &device_type);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d.", err);
        gplog(LOG_OP, "dcmi_create_vdevice call dcmi_get_device_type failed. err is %d", err);
        return err;
    }

    if (device_type == NPU_TYPE) {
        if (dcmi_board_chip_type_is_ascend_910b()) { // 910B调用算力切分相关接口，需先使能sriov
            err = dcmi_set_sriov_cfg(card_id, device_id, &enable_sriov);
            if (err != DCMI_OK) {
                gplog(LOG_ERR, "dcmi_set_sriov_cfg enable failed. err is %d.", err);
                return err;
            }
        }
        err = dcmi_npu_create_vdevice(card_id, device_id, vdev, out);
        if (err != DCMI_OK) {
            gplog(LOG_ERR, "dcmi_npu_create_vdevice failed. err is %d.", err);
            gplog(LOG_OP, "create vnpu failed. card_id = %d, device_id = %d, vdev_id = %u, vfg_id = %u,"
                " template_name = %s. err is %d", card_id, device_id, vdev->vdev_id, vdev->vfg_id,
                vdev->template_name, err);
            return err;
        }
    } else {
        gplog(LOG_OP, "device_type %d is not support.", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
    gplog(LOG_OP, "create vnpu success. card_id = %d, device_id = %d, vdev_id = %u, vfg_id = %u, template_name = %s",
        card_id, device_id, out->vdev_id, out->vfg_id, vdev->template_name);
    return DCMI_OK;

    err = set_disable_sriov(card_id, device_id, err);
    if (err != DCMI_OK && dcmi_board_chip_type_is_ascend_910b()) {
        gplog(LOG_ERR, "set_disable_sriov failed. ret is %d", err);
    }
    return err;
}

int dcmi_set_destroy_vdevice(int card_id, int device_id, unsigned int vdevid)
{
    enum dcmi_unit_type device_type = INVALID_TYPE;

    int err = dcmi_check_set_vnpu_permission();
    if (err != DCMI_OK) {
        gplog(LOG_OP, "dcmi_set_destroy_vdevice call dcmi_check_set_vnpu_permission failed. err is %d", err);
        return err;
    }

    err = dcmi_get_device_type(card_id, device_id, &device_type);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d.", err);
        gplog(LOG_OP, "dcmi_set_destroy_vdevice call dcmi_get_device_type failed. err is %d", err);
        return err;
    }

    if (device_type == NPU_TYPE) {
        err = dcmi_npu_destroy_vdevice(card_id, device_id, vdevid);
        if (err != DCMI_OK) {
            gplog(LOG_ERR, "dcmi_npu_destroy_vdevice failed. err is %d.", err);
            gplog(LOG_OP, "destroy vnpu failed. card_id = %d, device_id = %d vdev_id = %u.", card_id, device_id,
                vdevid);
            return err;
        }
    } else {
        gplog(LOG_OP, "device_type %d is not support.", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
    err = set_disable_sriov(card_id, device_id, err);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "set_disable_sriov failed. ret is %d", err);
        return err;
    }

    gplog(LOG_OP, "destroy vnpu success. card_id = %d, device_id = %d vdev_id = %u", card_id, device_id, vdevid);
    return DCMI_OK;
}

int dcmi_get_vdevice_mode(int *mode)
{
    int ret;

    if (dcmi_get_board_chip_type() != DCMI_CHIP_TYPE_D310P) {
        if (!dcmi_is_in_phy_machine_root() &&
            !(dcmi_board_chip_type_is_ascend_910b() && dcmi_is_in_phy_privileged_docker_root())) {
            // 310 and 910 only support creating vnpu in phy_machine, 910B support in phymachine and privileged docker
            gplog(LOG_OP, "Operation not permitted, only root user on physical machine can call this api.");
            return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
        }
    } else { // 310P supports creating vnpu on phy_machine and vm with root user
        if (!(dcmi_is_in_phy_machine_root() || dcmi_is_in_vm_root())) {
            gplog(LOG_OP, "Operation not permitted, only root user on phy machine or vm can call this api.");
            return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
        }
    }

    if (mode == NULL) {
        gplog(LOG_ERR, "mode is NULL");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    ret = dsmi_get_enable(-1, VDEV_MODE_CONFIG_ITEM, DSMI_DEVICE_TYPE_NONE, mode);
    if (ret != DSMI_OK) {
        gplog(LOG_ERR, "dsmi_get_enable failed. ret is %d.", ret);
        return dcmi_convert_error_code(ret);
    }

    return DCMI_OK;
}

int dcmi_check_vdevice_set_mode_with_curr(int mode)
{
    int cur_mode;
    int err = dcmi_get_vdevice_mode(&cur_mode);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_vdevice_mode failed. err is %d.", err);
        return FALSE;
    }

    if (cur_mode == mode) {
        gplog(LOG_OP, "VNPU mode is currently %s mode, no need to set",
            (mode == VDAVINCI_VM) ? "VM" : "docker");
        return TRUE;
    }
    return FALSE;
}

int dcmi_check_set_vdevice_mode_parameter(int mode)
{
    unsigned char work_mode;
    if (mode == VDAVINCI_VM) {
        if ((!(dcmi_board_chip_type_is_ascend_310p() && dcmi_board_type_is_card()) && !dcmi_board_type_is_server()) ||
            dcmi_board_chip_type_is_ascend_910_93()) {
            gplog(LOG_OP, "This device does not support setting vm mode.");
            return DCMI_ERR_CODE_NOT_SUPPORT;
        }

        if (dcmi_check_vdevice_set_mode_with_curr(mode)) {
            return DCMI_OK;
        }

        if (dcmi_get_npu_work_mode(1, &work_mode) == DCMI_OK) {
            if (work_mode != DCMI_NPU_WORK_MODE_AMP) {
                gplog(LOG_OP, "The device is not in AMP mode(%u) and does not support setting to VM mode.", work_mode);
                return DCMI_ERR_CODE_NOT_SUPPORT;
            }
        }

        /* 容器模式到虚机模式接口限制：用户只能在未切分设备时，执行算力切分模式切换的动作 */
        if (dcmi_find_vnpu_in_docker_mode() == DCMI_OK) {
            gplog(LOG_OP, "The docker vnpu exists and cannot be set VM mode.");
            return DCMI_ERR_CODE_RESOURCE_OCCUPIED;
        }
    } else if (mode == VDAVINCI_DOCKER) {
        bool support_chip_type = (dcmi_board_chip_type_is_ascend_310p() || dcmi_board_chip_type_is_ascend_910() ||
            dcmi_board_chip_type_is_ascend_910b());
        if (!support_chip_type) {
            gplog(LOG_OP, "This device does not support setting docker mode.");
            return DCMI_ERR_CODE_NOT_SUPPORT;
        }

        if (dcmi_check_vdevice_set_mode_with_curr(mode)) {
            return DCMI_OK;
        }

        if (dcmi_find_vnpu_in_vm_mode() == DCMI_OK) {
            gplog(LOG_OP, "The vm vnpu exists and cannot be set docker mode.");
            return DCMI_ERR_CODE_RESOURCE_OCCUPIED;
        }
    } else {
        gplog(LOG_ERR, "mode is invalid! mode=%d", mode);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    return DCMI_OK;
}

int dcmi_set_vdevice_mode(int mode)
{
    int ret;

    if (!dcmi_is_in_phy_machine_root() &&
        !(dcmi_board_chip_type_is_ascend_910b() && dcmi_is_in_phy_privileged_docker_root())) {
        gplog(LOG_OP, "Operation not permitted, only root user on physical machine can call this api.");
        return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
    }

    ret = dcmi_check_set_vdevice_mode_parameter(mode);
    if (ret != DCMI_OK) {
        return ret;
    }

    ret = dsmi_config_enable(-1, VDEV_MODE_CONFIG_ITEM, DSMI_DEVICE_TYPE_NONE, mode);
    if (ret != DSMI_OK) {
        gplog(LOG_OP, "dsmi_config_enable failed. ret is %d.", ret);
        return dcmi_convert_error_code(ret);
    }

    gplog(LOG_OP, "set vnpu mode success. mode = %d", mode);
    return DCMI_OK;
}

int dcmi_check_vnpu_config_recover_mode_is_permitted(const char *operate_mode)
{
    int vir_flag = 1;

    if ((dcmi_board_chip_type_is_ascend_910() == TRUE)) {
        if (!dcmi_is_in_phy_machine_root()) {
            gplog(LOG_OP, "Operation not permitted, only root user on physical machine can call this api.");
            return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
        }
    } else if (dcmi_board_chip_type_is_ascend_310p() == TRUE) {
        if (!(dcmi_is_in_phy_machine_root() || dcmi_is_in_vm_root())) {
            gplog(LOG_OP, "Operation not permitted, only root user on physical machine or vm can call this api.");
            return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
        }
    } else if ((dcmi_board_chip_type_is_ascend_910b() == TRUE)) {
        if (!(dcmi_is_in_phy_privileged_docker_root() || dcmi_is_in_phy_machine_root())) {
            gplog(LOG_OP,
                "Operation not permitted, only root user on physical machine or privilegd docker can call this api.");
            return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
        }
    } else {
        gplog(LOG_OP, "This device does not support %s vnpu config recover mode.", operate_mode);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    if ((dcmi_all_vnpu_is_vir(&vir_flag) == DCMI_OK) && (vir_flag == 1)) {
        gplog(LOG_OP, "This device does not support %s the vnpu config recovery mode, because it is all vir.",
            operate_mode);
        return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
    }

    return DCMI_OK;
}

int dcmi_set_vnpu_config_recover_mode(unsigned int mode)
{
    int err;
    unsigned int work_mode_support = FALSE;

    err = dcmi_check_vnpu_config_recover_mode_is_permitted("set");
    if (err != DCMI_OK) {
        return err;
    }

    err = dcmi_vnpu_work_mode_is_support(&work_mode_support);
    if (err != DCMI_OK) {
        gplog(LOG_OP, "dcmi_vnpu_work_mode_is_support failed. err=%d", err);
        return DCMI_ERR_CODE_INNER_ERR;
    }

    if (work_mode_support == FALSE) {
        gplog(LOG_OP, "This device work mode is not support set vnpu config recover mode.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    if ((mode != DCMI_CFG_RECOVER_ENABLE) && (mode != DCMI_CFG_RECOVER_DISABLE)) {
        gplog(LOG_ERR, "mode [%u] is invalid.", mode);
        gplog(LOG_OP, "dcmi_set_vnpu_config_recover_mode parameter mode [%u] is invalid.", mode);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    err = dcmi_cfg_set_config_recover_mode(mode);
    if (err != DCMI_OK) {
        gplog(LOG_OP, "set vnpu config recover mode %u failed. err=%d", mode, err);
        return DCMI_ERR_CODE_INNER_ERR;
    }

    gplog(LOG_OP, "dcmi set vnpu config recover mode %s success",
        (mode == DCMI_CFG_RECOVER_ENABLE) ? "enable" : "disable");
    return DCMI_OK;
}

int dcmi_get_vnpu_config_recover_mode(unsigned int *mode)
{
    int err;
    unsigned int work_mode_support = FALSE;

    err = dcmi_check_vnpu_config_recover_mode_is_permitted("get");
    if (err != DCMI_OK) {
        return err;
    }
    err = dcmi_vnpu_work_mode_is_support(&work_mode_support);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_vnpu_work_mode_is_support failed. err=%d", err);
        return DCMI_ERR_CODE_INNER_ERR;
    }

    if (work_mode_support == FALSE) {
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    if (mode == NULL) {
        gplog(LOG_ERR, "mode is invalid.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    err = dcmi_cfg_get_config_recover_mode(mode);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "get config recover mode failed. err=%d", err);
        return DCMI_ERR_CODE_INNER_ERR;
    }
    return DCMI_OK;
}