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
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <linux/pci.h>

#ifndef _WIN32
#include "ascend_hal.h"
#endif
#include "securec.h"
#include "dsmi_common_interface_custom.h"
#include "dcmi_interface_api.h"
#include "dcmi_init_basic.h"
#include "dcmi_fault_manage_intf.h"
#include "dcmi_log.h"
#include "dcmi_virtual_intf.h"
#include "dcmi_inner_cfg_manage.h"
#include "dcmi_os_adapter.h"
#include "dcmi_product_judge.h"
#include "dcmi_environment_judge.h"
#include "dcmi_inner_info_get.h"
#include "dcmi_npu_link_intf.h"

STATIC int dcmi_get_npu_hccs_link_bandwidth_info(int card_id, int device_id,
    struct dcmi_hccs_link_bandwidth_info *hccs_link_bandwidth_info)
{
    int ret;
    int device_logic_id = 0;
    unsigned int out_size = sizeof(struct dcmi_hccs_link_bandwidth_info);
 
    ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "Call dcmi_get_device_logic_id failed. err is %d.", ret);
        return ret;
    }
 
    ret = dsmi_get_device_info(device_logic_id, DSMI_MAIN_CMD_HCCS_BANDWIDTH, DSMI_HCCS_CMD_GET_BANDWIDTH,
        (void *)hccs_link_bandwidth_info, &out_size);
    if ((ret != DSMI_OK) && (ret != DSMI_ERR_NOT_SUPPORT)) {
        gplog(LOG_ERR, "Call dsmi_get_device_info failed. err is %d.", ret);
    }
    
    return dcmi_convert_error_code(ret);
}

int dcmi_get_hccs_link_bandwidth_info(int card_id, int device_id,
    struct dcmi_hccs_bandwidth_info *hccs_bandwidth_info)
{
    int i, ret;
    double time_ms_to_s;
    struct dcmi_hccs_link_bandwidth_info hccs_link_bandwidth_info;
    enum dcmi_unit_type device_type = INVALID_TYPE;
    if (hccs_bandwidth_info == NULL) {
        gplog(LOG_ERR, "Hccs_link_bandwidth_info is NULL\n");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }
    hccs_link_bandwidth_info.profiling_time = hccs_bandwidth_info->profiling_time;

    if (!dcmi_board_type_is_server() || (!dcmi_board_chip_type_is_ascend_910b() &&
        !dcmi_board_chip_type_is_ascend_910_93())) {
        gplog(LOG_OP, "This device does not support get hccs link bandwidth info.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
    if (dcmi_check_chip_is_in_split_mode(card_id, device_id) == DCMI_ERR_CODE_OPER_NOT_PERMITTED) {
        gplog(LOG_ERR, "Operation not permitted, this api cannot be called in split mode");
        return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
    }
    ret = dcmi_get_device_type(card_id, device_id, &device_type);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "Dcmi_get_device_type failed. err is %d.\n", ret);
        return ret;
    }
 
    if (device_type == NPU_TYPE) {
        ret = dcmi_get_npu_hccs_link_bandwidth_info(card_id, device_id, &hccs_link_bandwidth_info);
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "Dcmi_get_hccs_link_bandwidth_info failed. ret is %d.\n", ret);
            return ret;
        }
    } else {
        gplog(LOG_ERR, "Device_type %d is not support.", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
    time_ms_to_s = (double)hccs_link_bandwidth_info.profiling_time / PROFILING_MS_TRANS_TO_S;
    for (i = 0; i < DCMI_HCCS_MAX_PCS_NUM; i++) {
        hccs_bandwidth_info->tx_bandwidth[i] = (double)hccs_link_bandwidth_info.tx_bandwidth[i] *
            HCCS_SINGLE_PACKETS_BYTES / time_ms_to_s / PROFILING_BYTE_TRANS_TO_GMBYTE;
        hccs_bandwidth_info->rx_bandwidth[i] = (double)hccs_link_bandwidth_info.rx_bandwidth[i] *
            HCCS_SINGLE_PACKETS_BYTES / time_ms_to_s / PROFILING_BYTE_TRANS_TO_GMBYTE;
        hccs_bandwidth_info->total_txbw += hccs_bandwidth_info->tx_bandwidth[i];
        hccs_bandwidth_info->total_rxbw += hccs_bandwidth_info->rx_bandwidth[i];
    }
    return ret;
}

STATIC int dcmi_get_npu_pcie_link_bandwidth_info(int card_id, int device_id,
    struct dcmi_pcie_link_bandwidth_info *pcie_link_bandwidth_info)
{
    int ret;
    int device_logic_id = 0;
    unsigned int out_size = sizeof(struct dcmi_pcie_link_bandwidth_info);

    ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", ret);
        return ret;
    }

    ret = dsmi_get_device_info(device_logic_id, DSMI_MAIN_CMD_PCIE_BANDWIDTH, DSMI_PCIE_CMD_GET_BANDWIDTH,
        (void *)pcie_link_bandwidth_info, &out_size);
    if ((ret != DSMI_OK) && (ret != DSMI_ERR_NOT_SUPPORT)) {
        gplog(LOG_ERR, "call dsmi_get_device_info failed. err is %d.", ret);
    }

    return dcmi_convert_error_code(ret);
}

int dcmi_get_pcie_link_bandwidth_info(int card_id, int device_id,
    struct dcmi_pcie_link_bandwidth_info *pcie_link_bandwidth_info)
{
    int ret;
    unsigned int env_flag = ENV_PHYSICAL;
    enum dcmi_unit_type device_type = INVALID_TYPE;

    if (dcmi_check_chip_is_in_split_mode(card_id, device_id) == DCMI_ERR_CODE_OPER_NOT_PERMITTED) {
        gplog(LOG_ERR, "Operation not permitted, this api cannot be called in split mode");
        return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
    }

    if (pcie_link_bandwidth_info == NULL) {
        gplog(LOG_ERR, "pcie_link_bandwidth_info is NULL\n");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if (dcmi_check_run_in_docker()) {
        ret = dcmi_get_environment_flag(&env_flag);
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "call dcmi_get_environment_flag failed. err is %d.", ret);
            return DCMI_ERR_CODE_INNER_ERR;
        }
        if (env_flag != ENV_PHYSICAL_PRIVILEGED_CONTAINER && env_flag != ENV_VIRTUAL_PRIVILEGED_CONTAINER) {
            gplog(LOG_OP, "Operation not permitted, only privileged containers are supported. Current env_flag is %u",
                env_flag);
            return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
        }
    }

    if (!dcmi_board_chip_type_is_ascend_910b()) {
        gplog(LOG_OP, "This device does not support get pcie link bandwidth info.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    ret = dcmi_get_device_type(card_id, device_id, &device_type);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d.\n", ret);
        return ret;
    }

    if (device_type == NPU_TYPE) {
        return dcmi_get_npu_pcie_link_bandwidth_info(card_id, device_id, pcie_link_bandwidth_info);
    } else {
        gplog(LOG_ERR, "device_type %d is not support.", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
}

STATIC int dcmi_get_npu_hccs_avail_credit_info(int card_id, int device_id,
    struct dcmi_hccs_credit_info *hccs_avail_credit_info)
{
    int ret;
    int device_logic_id = 0;
    unsigned int out_size = sizeof(struct dcmi_hccs_credit_info);

    ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", ret);
        return ret;
    }

    ret = dsmi_get_device_info(device_logic_id, DSMI_MAIN_CMD_HCCS, DSMI_HCCS_CMD_GET_CREDIT_INFO,
        (void *)hccs_avail_credit_info, &out_size);
    if ((ret != DSMI_OK) && (ret != DSMI_ERR_NOT_SUPPORT)) {
        gplog(LOG_ERR, "call dsmi_get_device_info failed. err is %d.", ret);
    }
    
    return dcmi_convert_error_code(ret);
}

int dcmi_get_hccs_avail_credit_info(int card_id, int device_id,
    struct dcmi_hccs_credit_info *hccs_avail_credit_info)
{
    int ret;
    enum dcmi_unit_type device_type = INVALID_TYPE;

    if (hccs_avail_credit_info == NULL) {
        gplog(LOG_ERR, "hccs_avail_credit_info is NULL\n");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if (!dcmi_board_chip_type_is_ascend_910_93()) {
        gplog(LOG_OP, "This device does not support get hccs avail credit info.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    if (dcmi_check_chip_is_in_split_mode(card_id, device_id) == DCMI_ERR_CODE_OPER_NOT_PERMITTED) {
        gplog(LOG_ERR, "Operation not permitted, this api cannot be called in split mode");
        return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
    }

    ret = dcmi_get_device_type(card_id, device_id, &device_type);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d.\n", ret);
        return ret;
    }

    if (device_type == NPU_TYPE) {
        ret = dcmi_get_npu_hccs_avail_credit_info(card_id, device_id, hccs_avail_credit_info);
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "dcmi_get_npu_hccs_avail_credit_info failed. ret is %d.\n", ret);
            return ret;
        }
    } else {
        gplog(LOG_ERR, "device_type %d is not support.", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
    return ret;
}

STATIC int dcmi_hal_to_npu_topo_type(int hal_topo_type)
{
    switch (hal_topo_type) {
        case HAL_TOPOLOGY_HCCS:
            return DCMI_TOPO_TYPE_HCCS;
        case HAL_TOPOLOGY_PIX:
            return DCMI_TOPO_TYPE_PIX;
        case HAL_TOPOLOGY_PIB:
            return DCMI_TOPO_TYPE_PXB;
        case HAL_TOPOLOGY_PHB:
            return DCMI_TOPO_TYPE_PHB;
        case HAL_TOPOLOGY_SYS:
            return DCMI_TOPO_TYPE_SYS;
        case HAL_TOPOLOGY_SIO:
            return DCMI_TOPO_TYPE_SIO;
        case HAL_TOPOLOGY_HCCS_SW:
            return DCMI_TOPO_TYPE_HCCS_SW;
        default:
            return DCMI_TOPO_TYPE_BUTT;
    }
}

int dcmi_get_phyid_by_cardid_and_devid(int card_id, int device_id, unsigned int *phyid)
{
    int logic_id = 0;
    int ret;

    ret = dcmi_get_device_logic_id(&logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_logic_id failed. ret is %d.", ret);
        return ret;
    }

    ret = dcmi_get_device_phyid_from_logicid((unsigned int)logic_id, phyid);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_phyid_from_logicid failed. ret is %d", ret);
        return ret;
    }

    return DCMI_OK;
}

STATIC int dcmi_query_topo_type(int card_id1, int card_id2, int device_id1, int device_id2, int *topo_type)
{
    int ret;
    int64_t value;
    unsigned int phyid1, phyid2;

    ret = dcmi_get_phyid_by_cardid_and_devid(card_id1, device_id1, &phyid1);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "Get phyid by card1 failed. card id:%d, device id:%d.", card_id1, device_id1);
        return ret;
    }

    ret = dcmi_get_phyid_by_cardid_and_devid(card_id2, device_id2, &phyid2);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "Get phyid by card2 failed. card id:%d, device id:%d.", card_id2, device_id2);
        return ret;
    }

    if (phyid1 == phyid2) {
        *topo_type = DCMI_TOPO_TYPE_SELF;
        return DCMI_OK;
    }
    ret = halGetPairPhyDevicesInfo((int)phyid1, (int)phyid2, 0, &value);
    if (ret) {
        gplog(LOG_ERR, "get pcie chain hccs status failed. ret is %d", ret);
        *topo_type = DCMI_TOPO_TYPE_BUTT;
        return DCMI_OK;
    }

    *topo_type = dcmi_hal_to_npu_topo_type(value);

    return DCMI_OK ;
}

STATIC int dcmi_topo_read_line_info_from_file(const char *cfg_file_path, char *content)
{
    FILE *fp = NULL;
    int ret = 0;
    char file_string[MAX_LENTH] = {0};
    char real_conf_path[PATH_MAX + 1] = {0};

    if (realpath(cfg_file_path, real_conf_path) == NULL) {
        gplog(LOG_ERR, "node name invalid.\n");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    fp = fopen(real_conf_path, "r");
    if (fp == NULL) {
        gplog(LOG_ERR, "file open failed. errno is %d\n", errno);
        gplog(LOG_ERR, "path is %s", real_conf_path);
        return DCMI_ERR_CODE_FILE_OPERATE_FAIL;
    }

    if (fgets(file_string, sizeof(file_string), fp) == NULL) {
        gplog(LOG_ERR, "failed to fgets.fgets get NULL");
        (void)fclose(fp);
        fp = NULL;
        return DCMI_ERR_CODE_FILE_OPERATE_FAIL;
    }

    int len = strlen(file_string);
    if ((len > 0) && (file_string[len - 1] == '\n')) {
        file_string[len - 1] = 0;
    }

    ret = snprintf_s(content, TOPO_INFO_MAX_LENTH, TOPO_INFO_MAX_LENTH - 1, "%s", file_string);
    if (ret <= 0) {
        gplog(LOG_ERR, "failed to snprintf_s. ret is %d\n", ret);
        (void)fclose(fp);
        fp = NULL;
        return DCMI_ERR_CODE_SECURE_FUN_FAIL;
    }

    (void)fclose(fp);
    fp = NULL;
    return DCMI_OK;
}

STATIC int dcmi_get_cpu_affinity_by_device_id(int card_id, int device_id, char *affinity_cpu, int *length)
{
    int ret, numa_node = 0;
    struct dcmi_pcie_info_all topo_dcmi_pcie_info = {0};
    char numa_file_path[MAX_LENTH] = {0};
    char content[TOPO_INFO_MAX_LENTH] = {0};
    char affinity_file_path[MAX_LENTH] = {0};

    ret = dcmi_get_device_pcie_info_v2(card_id, device_id, &topo_dcmi_pcie_info);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "Failed to get device pcie info. (card_id=[%d]; device_id=[%d])", card_id, device_id);
        return ret;
    }

    ret = snprintf_s(numa_file_path, MAX_LENTH, MAX_LENTH - 1,
        "%s/%04x:%02x:%02x.%1u/numa_node", SYS_BUS_PCI_DEVICE_PATH, topo_dcmi_pcie_info.domain,
        topo_dcmi_pcie_info.bdf_busid, topo_dcmi_pcie_info.bdf_deviceid, topo_dcmi_pcie_info.bdf_funcid);
    if (ret <= 0) {
        gplog(LOG_ERR, "failed to snprintf_s(). ret is %d", ret);
        return DCMI_ERR_CODE_SECURE_FUN_FAIL;
    }

    ret = dcmi_topo_read_line_info_from_file(numa_file_path, content);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "failed to dcmi_topo_read_line_info_from_file. ret is %d", ret);
        return ret;
    }

    ret = sscanf_s(content, "%d", &numa_node);
    if (ret <= 0) {
        gplog(LOG_ERR, "failed to sscanf_s. ret is %d", ret);
        return DCMI_ERR_CODE_SECURE_FUN_FAIL;
    }

    if (numa_node < 0) {
        numa_node = 0;
    }

    ret = snprintf_s(affinity_file_path, MAX_LENTH, MAX_LENTH - 1,
        "/sys/devices/system/node/node%d/cpulist", numa_node);
    if (ret <= 0) {
        gplog(LOG_ERR, "failed to snprintf_s. ret is %d", ret);
        return DCMI_ERR_CODE_SECURE_FUN_FAIL;
    }

    ret = dcmi_topo_read_line_info_from_file(affinity_file_path, affinity_cpu);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "failed to dcmi_topo_read_line_info_from_file. ret is %d", ret);
        return ret;
    }

    *length = strlen(affinity_cpu);
    return DCMI_OK;
}

int dcmi_get_topo_info_by_device_id(int card_id1, int device_id1,
    int card_id2, int device_id2, int *topo_type)
{
    int ret;
    enum dcmi_unit_type device_type1 = INVALID_TYPE;
    enum dcmi_unit_type device_type2 = INVALID_TYPE;

    if (topo_type == NULL) {
        gplog(LOG_ERR, "topo_type is NULL\n");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if ((!dcmi_board_chip_type_is_ascend_910b()) && (!dcmi_board_chip_type_is_ascend_910_93())) {
        gplog(LOG_OP, "This device does not support get Topo.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    ret = dcmi_get_device_type(card_id1, device_id1, &device_type1);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type for card1 failed. err is %d.", ret);
        return ret;
    }

    ret = dcmi_get_device_type(card_id2, device_id2, &device_type2);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type for card2 failed. err is %d.", ret);
        return ret;
    }
    
    if (device_type1 == NPU_TYPE && device_type2 == NPU_TYPE) {
        return dcmi_query_topo_type(card_id1, card_id2, device_id1, device_id2, topo_type);
    } else {
        gplog(LOG_ERR, "device_type %d and %d is not support.", device_type1, device_type2);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
}

int dcmi_get_affinity_cpu_info_by_device_id(int card_id, int device_id, char *affinity_cpu, int *length)
{
    int ret;
    enum dcmi_unit_type device_type = INVALID_TYPE;
    unsigned int main_board_id = 0;

    if (affinity_cpu == NULL || length  == NULL) {
        gplog(LOG_ERR, "Affinity_cpu or length is NULL\n");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }
    
    main_board_id = dcmi_get_maindboard_id_inner();
    if (!(dcmi_board_chip_type_is_ascend_910b() || main_board_id == DCMI_A_X_910_93_MAIN_BOARD_ID)) {
        gplog(LOG_OP, "This device does not support get Cpu affinity.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    ret = dcmi_get_device_type(card_id, device_id, &device_type);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d.\n", ret);
        return ret;
    }

    if (device_type == NPU_TYPE) {
        return  dcmi_get_cpu_affinity_by_device_id(card_id, device_id, affinity_cpu, length);
    } else {
        gplog(LOG_ERR, "device_type %d is not support.", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
}

STATIC int dcmi_pci_dev_open(const char *base, unsigned char bus, unsigned char devfn, int oflag)
{
    int ret = 0;
    char input_path[PATH_MAX] = {0x00};
    char std_path[PATH_MAX + 1] = {0x00};

    if (!base) {
        gplog(LOG_ERR, "input para is invalid.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

#ifdef X86
    ret = snprintf_s(input_path,
        sizeof(input_path),
        sizeof(input_path) - 1,
        "%s/%02x/%02x.%d",
        base,
        bus,
        PCI_SLOT(devfn),
        PCI_FUNC(devfn));
#endif

#ifdef ARM
    ret = snprintf_s(input_path,
        sizeof(input_path),
        sizeof(input_path) - 1,
        "%s/%04x:%02x/%02x.%d",
        base,
        DCMI_PCI_PCI_DOMAIN,
        bus,
        PCI_SLOT(devfn),
        PCI_FUNC(devfn));
#endif
    if (ret <= DCMI_OK) {
        gplog(LOG_ERR, "call snprintf_s failed.%d.\n", ret);
    }

    ret = dcmi_file_realpath_disallow_nonexist(input_path, std_path, sizeof(std_path));
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "file path is invalid.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    ret = open(std_path, oflag);
    if (ret < DCMI_OK) {
        return DCMI_ERR_CODE_FILE_OPERATE_FAIL;
    }

    return ret;
}

static void dcmi_pci_dev_close(int fd)
{
    close(fd);
    return;
}

static int dcmi_pci_dev_write(
    unsigned char bus, unsigned char devfn, const char buf[], unsigned int len, unsigned int pos)
{
    int ret;
    int fd;

    if (buf == NULL) {
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    fd = dcmi_pci_dev_open(DCMI_PCI_PROC_BUS_PCI_PATH, bus, devfn, O_RDWR);
    if (fd < 0) {
        return fd;
    }

    ret = pwrite(fd, buf, (size_t)len, (off_t)pos);
    if (ret < DCMI_OK) {
        gplog(LOG_ERR,
            "write[%02x:%02x.%x] at %u for %uB failed, ret=%d\n",
            bus,
            PCI_SLOT(devfn),
            PCI_FUNC(devfn),
            pos,
            len,
            ret);
        dcmi_pci_dev_close(fd);
        return DCMI_ERR_CODE_FILE_OPERATE_FAIL;
    }

    if (ret != (int)len) {
        gplog(LOG_ERR,
            "tried to write[%02x:%02x.%x] at %u for %uB, but only %d succeed\n",
            bus,
            PCI_SLOT(devfn),
            PCI_FUNC(devfn),
            pos,
            len,
            ret);
        dcmi_pci_dev_close(fd);
        return DCMI_ERR_CODE_FILE_OPERATE_FAIL;
    }

    dcmi_pci_dev_close(fd);

    return DCMI_OK;
}

static void pci_trans_param(const char *param, unsigned char *bus, unsigned char *devfn)
{
#define NUM_SSCANF_BDF 3
    int ret;

    unsigned int input_bus = DCMI_PCI_INVALID_VALUE;
    unsigned int input_dev = DCMI_PCI_INVALID_VALUE;
    unsigned int input_func = DCMI_PCI_INVALID_VALUE;

    ret = sscanf_s(param, "0000:%x:%x.%x", &input_bus, &input_dev, &input_func);
    if (ret < NUM_SSCANF_BDF) {
        gplog(LOG_ERR, "pci_trans_param sscanf_s param failed(%d)!\n", ret);
    }

    bool check_input_invalid = (((int)(input_bus) == DCMI_PCI_INVALID_VALUE) ||
        ((int)input_dev == DCMI_PCI_INVALID_VALUE) || ((int)input_func == DCMI_PCI_INVALID_VALUE));
    if (check_input_invalid) {
        ret = sscanf_s(param, "%x:%x.%x", &input_bus, &input_dev, &input_func);
        if (ret < NUM_SSCANF_BDF) {
            gplog(LOG_ERR, "pci_trans_param sscanf_s param failed(%d)!\n", ret);
        }

        check_input_invalid = (((int)(input_bus) == DCMI_PCI_INVALID_VALUE) ||
            ((int)input_dev == DCMI_PCI_INVALID_VALUE) || ((int)input_func == DCMI_PCI_INVALID_VALUE));
        if (check_input_invalid) {
            gplog(LOG_ERR, "Invalid input pcie parameter, please check. The parameter is %s. Error code %d \n", param,
                ret);
            return;
        }
    }

    *bus = (unsigned char)(input_bus);
    *devfn = PCI_DEVFN(input_dev, input_func);

    return;
}

int dcmi_pci_write_conf_byte(const char *param, unsigned int pos, unsigned char byte)
{
    unsigned char bus = DCMI_PCI_INVALID_VALUE;
    unsigned char devfn = DCMI_PCI_INVALID_VALUE;

    pci_trans_param(param, &bus, &devfn);

    return dcmi_pci_dev_write(bus, devfn, (char *)&byte, 1, pos);
}

STATIC int dcmi_get_npu_serdes_quality_info(int card_id, int device_id, unsigned int macro_id,
                                            struct dcmi_serdes_quality_info *serdes_quality_info)
{
    int ret;
    int device_logic_id = 0;
    unsigned int out_size = sizeof(struct dcmi_serdes_quality_info);
 
    ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", ret);
        return ret;
    }
 
    serdes_quality_info->macro_id = macro_id;
    ret = dsmi_get_device_info(device_logic_id, DSMI_MAIN_CMD_SERDES, DSMI_SERDES_SUB_CMD_QUALITY_INFO,
        (void *)serdes_quality_info, &out_size);
    if ((ret != DSMI_OK) && (ret != DSMI_ERR_NOT_SUPPORT)) {
        gplog(LOG_ERR, "call dsmi_get_device_info failed. err is %d.", ret);
    }
 
    return dcmi_convert_error_code(ret);
}

int check_serdes_environment_is_invalid(int card_id, int device_id, int macro_id)
{
    int ret;
    int max_id;
    unsigned int main_board_id;
    int chip_type = DCMI_CHIP_TYPE_INVALID;

    if (dcmi_check_chip_is_in_split_mode(card_id, device_id) == DCMI_ERR_CODE_OPER_NOT_PERMITTED) {
        gplog(LOG_OP, "In the vNPU scenario, this device does not support dcmi_get_serdes_quality_info.");
        return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
    }

    chip_type = dcmi_get_board_chip_type();
    switch (chip_type) {
        case DCMI_CHIP_TYPE_D910B:
            max_id = MAX_MACRO_ID;
            break;
        case DCMI_CHIP_TYPE_D310P:
            max_id = MAX_310P_MACRO_ID;
            break;
        case DCMI_CHIP_TYPE_D910_93:
            ret = dcmi_get_mainboard_id(card_id, device_id, &main_board_id);
            if (ret != DCMI_OK) {
                gplog(LOG_ERR, "Failed to query main board id of card. err is %d", ret);
                return ret;
            }
            max_id = (main_board_id == DCMI_A_X_910_93_MAIN_BOARD_ID) ? MAX_A_X_MACRO_ID : MAX_H60_ID;
            break;
        default:
            gplog(LOG_OP, "This device does not support get eye info.");
            return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    if (macro_id > max_id || macro_id == RES_MACRO_ID || macro_id < MIN_310P_MACRO_ID) {
        gplog(LOG_ERR, "macro_id is invalid. macro_id is %d", macro_id);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    return DCMI_OK;
}

int dcmi_get_serdes_quality_info(int card_id, int device_id, unsigned int macro_id,
                                 struct dcmi_serdes_quality_info *serdes_quality_info)
{
    int ret;
    enum dcmi_unit_type device_type = INVALID_TYPE;
 
    if (serdes_quality_info == NULL) {
        gplog(LOG_ERR, "serdes_quality_info is NULL");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    ret = check_serdes_environment_is_invalid(card_id, device_id, macro_id);
    if (ret != DCMI_OK) {
        return ret;
    }

    ret = dcmi_get_device_type(card_id, device_id, &device_type);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d.", ret);
        return ret;
    }
 
    if (device_type == NPU_TYPE) {
        return dcmi_get_npu_serdes_quality_info(card_id, device_id, macro_id, serdes_quality_info);
    } else {
        gplog(LOG_ERR, "device_type %d is not support.", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
}