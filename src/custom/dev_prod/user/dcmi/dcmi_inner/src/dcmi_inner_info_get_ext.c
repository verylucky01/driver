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
#include "dcmi_product_judge.h"
#include "dcmi_i2c_operate.h"
#include "dcmi_inner_cfg_manage.h"
#include "dcmi_elabel_operate.h"
#include "dcmi_mcu_intf.h"
#include "dcmi_inner_info_get.h"
#include "dcmi_environment_judge.h"

int dcmi_get_npu_device_gateway(
    int card_id, int device_id, enum dcmi_port_type input_type, int port_id, struct dcmi_ip_addr *gateway)
{
    int ret;
    int device_logic_id = 0;

    ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", ret);
        return ret;
    }

    ret = dsmi_get_gateway_addr(device_logic_id, input_type, port_id, (ip_addr_t *)gateway);
    if ((ret != DSMI_OK) && (ret != DSMI_ERR_NOT_SUPPORT)) {
        gplog(LOG_ERR, "call dsmi_get_gateway_addr failed. err is %d.", ret);
    }

    return dcmi_convert_error_code(ret);
}

int dcmi_get_npu_device_ip(int card_id, int device_id, enum dcmi_port_type input_type, int port_id,
    struct dcmi_ip_addr *ip, struct dcmi_ip_addr *mask)
{
    int ret;
    int device_logic_id = 0;

    ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", ret);
        return ret;
    }

    ret = dsmi_get_device_ip_address(device_logic_id, input_type, port_id, (ip_addr_t *)ip, (ip_addr_t *)mask);
    if (ret == DSMI_ERR_NO_DEVICE) {
        gplog(LOG_ERR, "call dsmi_get_device_ip_address failed. IP not exist. err is %d.", ret);
        return DCMI_ERR_CODE_CONFIG_INFO_NOT_EXIST;
    } else if ((ret != DSMI_OK) && (ret != DSMI_ERR_NOT_SUPPORT)) {
        gplog(LOG_ERR, "call dsmi_get_device_ip_address failed. err is %d.", ret);
    }

    return dcmi_convert_error_code(ret);
}

int dcmi_get_npu_device_network_health(int card_id, int device_id, enum dcmi_rdfx_detect_result *result)
{
    int ret;
    int device_logic_id = 0;

    ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", ret);
        return ret;
    }

    ret = dsmi_get_network_health(device_logic_id, (DSMI_NET_HEALTH_STATUS *)(void *)result);
    if ((ret != DSMI_OK) && (ret != DSMI_ERR_NOT_SUPPORT)) {
        gplog(LOG_ERR, "call dsmi_get_network_health failed. err is %d.", ret);
    }

    return dcmi_convert_error_code(ret);
}

int dcmi_get_npu_device_fan_count(int card_id, int device_id, int *count)
{
    int ret;
    int device_logic_id = 0;

    ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", ret);
        return ret;
    }

    ret = dsmi_get_fan_count(device_logic_id, count);
    if ((ret != DSMI_OK) && (ret != DSMI_ERR_NOT_SUPPORT)) {
        gplog(LOG_ERR, "call dsmi_get_fan_count failed. err is %d.", ret);
    }

    return dcmi_convert_error_code(ret);
}

int dcmi_get_npu_device_fan_speed(int card_id, int device_id, int fan_id, int *speed)
{
    int ret;
    int device_logic_id = 0;

    ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", ret);
        return ret;
    }

    ret = dsmi_get_fan_speed(device_logic_id, fan_id, speed);
    if ((ret != DSMI_OK) && (ret != DSMI_ERR_NOT_SUPPORT)) {
        gplog(LOG_ERR, "call dsmi_get_fan_speed failed. err is %d.", ret);
    }

    return dcmi_convert_error_code(ret);
}

int dcmi_get_npu_device_user_config(
    int card_id, int device_id, const char *config_name, unsigned int buf_size, unsigned char *buf)
{
    int ret;
    int device_logic_id = 0;
    if (strcmp(config_name, "cpu_num_cfg") == 0 && buf_size != DCMI_CPU_NUM_CFG_LEN) {
        gplog(LOG_ERR, "buf_size invalid, the value should be 16!");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", ret);
        return ret;
    }

    ret = dsmi_get_user_config(device_logic_id, config_name, buf_size, buf);
    if ((ret != DSMI_OK) && (ret != DSMI_ERR_NOT_SUPPORT)) {
        gplog(LOG_ERR, "call dsmi_get_user_config failed. err is %d.", ret);
    }

    return dcmi_convert_error_code(ret);
}

int dcmi_i2c_get_elable_info(int card_id, int item_type, char *elable_data, int *len)
{
    int err = 0;
    unsigned short data_len;
    err = dcmi_elabel_get_data(item_type, (unsigned char *)elable_data, MAX_LENTH, &data_len);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_elabel_get_data failed.%d.\n", err);
        return err;
    } else {
        *len = data_len;
    }
    return err;
}

int dcmi_i2c_get_npu_device_elable_info(int card_id, struct dcmi_elabel_info *elabel_info)
{
    int err;
    int length = 0;

    err = dcmi_i2c_get_elable_info(card_id, ELABEL_ITEM_ID_BOARD_PRD_NAME, elabel_info->product_name, &length);
    if (err != DCMI_OK || strlen(elabel_info->product_name) == 0) {
        gplog(LOG_ERR, "dcmi_i2c_get_elable_info product name failed.%d.", err);
        dcmi_set_default_elabel_str(elabel_info->product_name, sizeof(elabel_info->product_name));
    }

    err = dcmi_i2c_get_elable_info(card_id, ELABEL_ITEM_MODEL, elabel_info->model, &length);
    if (err != DCMI_OK || strlen(elabel_info->model) == 0) {
        gplog(LOG_ERR, "dcmi_i2c_get_elable_info model failed.%d.", err);
        dcmi_set_default_elabel_str(elabel_info->model, sizeof(elabel_info->model));
    }

    err = dcmi_i2c_get_elable_info(card_id, ELABEL_ITEM_ID_BOARD_MANF, elabel_info->manufacturer, &length);
    if (err != DCMI_OK || strlen(elabel_info->manufacturer) == 0) {
        gplog(LOG_ERR, "dcmi_i2c_get_elable_info manufacturer failed.%d.", err);
        dcmi_set_default_elabel_str(elabel_info->manufacturer, sizeof(elabel_info->manufacturer));
    }

    err = dcmi_i2c_get_elable_info(card_id, ELABEL_ITEM_ID_BOARD_SN, elabel_info->serial_number, &length);
    if (err != DCMI_OK || strlen(elabel_info->serial_number) == 0) {
        gplog(LOG_ERR, "dcmi_i2c_get_elable_info serial_number failed.%d.", err);
        dcmi_set_default_elabel_str(elabel_info->serial_number, sizeof(elabel_info->serial_number));
    }

    return DCMI_OK;
}

int dcmi_get_npu_device_elable_info(int card_id, int device_id, struct dcmi_elabel_info *elabel_info)
{
    int err, ret;
    int device_logic_id = 0;
    int dcmi_elabel_sn = 0;
    int dcmi_elabel_mf = 0;
    int dcmi_elabel_pn = 0;
    int dcmi_elabel_model = 0;
    int length = 0;

    err = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_logic_id failed.%d.", err);
        return err;
    }

    dcmi_get_elabel_item_it(&dcmi_elabel_sn, &dcmi_elabel_mf, &dcmi_elabel_pn, &dcmi_elabel_model);
    err = dsmi_dft_get_elable(device_logic_id, dcmi_elabel_sn, elabel_info->serial_number, &length);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dsmi_dft_get_elable sn failed.%d.", err);
        dcmi_set_default_elabel_str(elabel_info->serial_number, sizeof(elabel_info->serial_number));
    }
    err = dsmi_dft_get_elable(device_logic_id, dcmi_elabel_mf, elabel_info->manufacturer, &length);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dsmi_dft_get_elable mf failed.%d.", err);
        dcmi_set_default_elabel_str(elabel_info->manufacturer, sizeof(elabel_info->manufacturer));
    }
    err = dsmi_dft_get_elable(device_logic_id, dcmi_elabel_pn, elabel_info->product_name, &length);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dsmi_dft_get_elable pn failed.%d.", err);
        dcmi_set_default_elabel_str(elabel_info->product_name, sizeof(elabel_info->product_name));
    }
    err = dsmi_dft_get_elable(device_logic_id, dcmi_elabel_model, elabel_info->model, &length);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dsmi_dft_get_elable model failed.%d.", err);
        dcmi_set_default_elabel_str(elabel_info->model, sizeof(elabel_info->model));
    }

    ret = dcmi_convert_error_code(err);
    if (ret == DCMI_ERR_CODE_NOT_SUPPORT) {
        return ret;
    }
    return DCMI_OK;
}

int dcmi_get_npu_device_share_enable(int card_id, int device_id, int *enable_flag)
{
    int err;
    int device_logic_id = 0;
    const int device_share_main_cmd = 0x8001;
    const int device_share_sub_cmd = 0;
    unsigned int size = (unsigned int)sizeof(int);

    err = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", err);
        return DCMI_ERR_CODE_INNER_ERR;
    }

    err = dsmi_get_device_info(device_logic_id, device_share_main_cmd, device_share_sub_cmd,
        (void *)enable_flag, &size);
    if ((err != DSMI_OK) && (err != DSMI_ERR_NOT_SUPPORT)) {
        gplog(LOG_ERR, "call dsmi_get_device_info failed. err is %d.", err);
    }

    return dcmi_convert_error_code(err);
}

int dcmi_get_npu_device_ssh_enable(int card_id, int device_id, int *enable_flag)
{
    int err;
    int device_logic_id = 0;
    unsigned char ssh_status = 0;

    err = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed.%d.\n", err);
        return DCMI_ERR_CODE_INNER_ERR;
    }
    err = dsmi_get_user_config(device_logic_id, "ssh_status", 1, &ssh_status);
    if ((err != DSMI_OK) && (err != DSMI_ERR_NOT_SUPPORT)) {
        gplog(LOG_ERR, "call dsmi_get_user_config failed. err is %d.", err);
        return dcmi_convert_error_code(err);
    }
    *enable_flag = (int)ssh_status;
    return DCMI_OK;
}

int dcmi_get_npu_device_aicpu_count_info(int card_id, int device_id, unsigned char *count)
{
    int err;
    enum dcmi_unit_type device_type = NPU_TYPE;

    if (count == NULL) {
        gplog(LOG_ERR, "count is invalid.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    err = dcmi_get_device_type(card_id, device_id, &device_type);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d.", err);
        return err;
    }

    if (device_type == NPU_TYPE) {
        return dcmi_get_user_config(card_id, device_id, "aicpu_config", sizeof(unsigned char), count);
    } else {
        gplog(LOG_ERR, "device_type %d is error.", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
}

int dcmi_get_npu_device_list(int *device_list, int list_size, int *device_count)
{
    int err;
    int count;

    /* Atlas 500 存在不带Asccend 310发货场景 */
    if (access("/run/minid_not_present", F_OK) == 0) {
        *device_count = 0;
        return DCMI_OK;
    }

    err = dsmi_get_device_count(&count);
    if (err != DSMI_OK) {
        gplog(LOG_ERR, "dsmi_get_device_count failed. err is %d.", err);
        return dcmi_convert_error_code(err);
    }

    if (count > list_size) {
        gplog(LOG_ERR, "dsmi_get_device_count count %d list_size %d.", count, list_size);
        return DCMI_ERR_CODE_INNER_ERR;
    }

    err = dsmi_list_device(device_list, count);
    if (err != DSMI_OK) {
        gplog(LOG_ERR, "dsmi_list_device failed. err is %d.", err);
        return dcmi_convert_error_code(err);
    }

    *device_count = count;
    return DCMI_OK;
}

int dcmi_get_npu_fault_event(int card_id, int device_id, int timeout, struct dcmi_event_filter filter,
    struct dcmi_event *event)
{
#ifndef _WIN32
    int ret;
    int ret_clr;
    int device_logic_id = 0;

    ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", ret);
        return ret;
    }

    ret = dsmi_read_fault_event(device_logic_id, timeout, *(struct dsmi_event_filter *)&filter,
        (struct dsmi_event *)event);
    if (ret != DSMI_OK) {
        gplog(LOG_ERR, "call dsmi_read_fault_event failed. err is %d.", ret);
        ret_clr = memset_s(event, sizeof(struct dsmi_event), 0, sizeof(struct dsmi_event));
        if (ret_clr != 0) {
            gplog(LOG_ERR, "memset_s event failed. err is %d", ret_clr);
        }
    }
    return dcmi_convert_error_code(ret);
#else
    gplog(LOG_ERR, "call dsmi_read_fault_event failed. not supprot in windows.");
    return DCMI_ERR_CODE_NOT_SUPPORT;
#endif
}

int dcmi_get_npu_device_dvpp_ratio_info(int card_id, int device_id, struct dcmi_dvpp_ratio *usage)
{
    int err;
    int device_logic_id = 0;
    unsigned int size = (unsigned int)sizeof(int);
    int buf = 0;

    err = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", err);
        return err;
    }

    err = dsmi_get_device_info(device_logic_id, DSMI_MAIN_CMD_DVPP, DSMI_SUB_CMD_DVPP_VDEC_RATE, (void *)&buf,
                               &size);
    if ((err != DSMI_OK) && (err != DSMI_ERR_NOT_SUPPORT)) {
        gplog(LOG_ERR, "dsmi_get_device_info vdec failed. err is %d.", err);
        return dcmi_convert_error_code(err);
    }
    usage->vdec_ratio = buf;

    err = dsmi_get_device_info(device_logic_id, DSMI_MAIN_CMD_DVPP, DSMI_SUB_CMD_DVPP_VPC_RATE, (void *)&buf, &size);
    if ((err != DSMI_OK) && (err != DSMI_ERR_NOT_SUPPORT)) {
        gplog(LOG_ERR, "dsmi_get_device_info vpc failed. err is %d.", err);
        return dcmi_convert_error_code(err);
    }
    usage->vpc_ratio = buf;

    err = dsmi_get_device_info(device_logic_id, DSMI_MAIN_CMD_DVPP, DSMI_SUB_CMD_DVPP_VENC_RATE, (void *)&buf,
                               &size);
    if ((err != DSMI_OK) && (err != DSMI_ERR_NOT_SUPPORT)) {
        gplog(LOG_ERR, "dsmi_get_device_info venc failed. err is %d.", err);
        return dcmi_convert_error_code(err);
    }
    usage->venc_ratio = (err == DSMI_ERR_NOT_SUPPORT) ? (int)((unsigned int)buf | (0x1 << DCMI_VF_FLAG_BIT)) : buf;

    err = dsmi_get_device_info(device_logic_id, DSMI_MAIN_CMD_DVPP, DSMI_SUB_CMD_DVPP_JPEGE_RATE, (void *)&buf,
                               &size);
    if ((err != DSMI_OK) && (err != DSMI_ERR_NOT_SUPPORT)) {
        gplog(LOG_ERR, "dsmi_get_device_info jpege failed. err is %d.", err);
        return dcmi_convert_error_code(err);
    }
    usage->jpege_ratio = (err == DSMI_ERR_NOT_SUPPORT) ? (int)((unsigned int)buf | (0x1 << DCMI_VF_FLAG_BIT)) : buf;

    err = dsmi_get_device_info(device_logic_id, DSMI_MAIN_CMD_DVPP, DSMI_SUB_CMD_DVPP_JPEGD_RATE, (void *)&buf,
                               &size);
    if ((err != DSMI_OK) && (err != DSMI_ERR_NOT_SUPPORT)) {
        gplog(LOG_ERR, "dsmi_get_device_info jpegd failed. err is %d.", err);
        return dcmi_convert_error_code(err);
    }
    usage->jpegd_ratio = buf;

    return dcmi_convert_error_code(err);
}

int dcmi_get_npu_proc_mem_info(int card_id, int device_id, struct dcmi_proc_mem_info *proc_info, int *proc_num)
{
    int err;
    int i;
    struct dsmi_resource_para para = {0};
    struct dsmi_resource_info info = {0};
    int proc_pids[MAX_PROC_NUM_IN_DEVICE] = {0};
    unsigned long mem_usage = 0;
    int logic_id = 0;

    para.owner_type = DSMI_PROCESS_RESOURCE;
    para.resource_type = DSMI_DEV_PROCESS_PID;
    info.buf = proc_pids;
    info.buf_len = sizeof(int) * MAX_PROC_NUM_IN_DEVICE;

    err = dcmi_get_device_logic_id(&logic_id, card_id, device_id);
    if (err != DSMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_logic_id failed. err is %d.", err);
        return err;
    }

    err = dsmi_get_resource_info((unsigned int)logic_id, &para, &info);
    if (err != DSMI_OK) {
        gplog(LOG_ERR, "dsmi_get_resource_info get pids failed. err is %d.", err);
        return dcmi_convert_error_code(err);
    }

    for (i = 0; (i < MAX_PROC_NUM_IN_DEVICE) && (proc_pids[i] != 0); i++) {
        para.resource_type = DSMI_DEV_PROCESS_MEM;
        para.owner_id = (unsigned int)proc_pids[i];
        info.buf = &mem_usage;
        info.buf_len = sizeof(mem_usage);
        err = dsmi_get_resource_info((unsigned int)logic_id, &para, &info);
        if (err != DSMI_OK) {
            gplog(LOG_ERR, "dsmi_get_resource_info failed get memory. err is %d.", err);
            return dcmi_convert_error_code(err);
        }
        proc_info[i].proc_id = proc_pids[i];
        proc_info[i].proc_mem_usage = mem_usage;
        mem_usage = 0;
    }
    *proc_num = i;
    return DCMI_OK;
}

int dcmi_npu_get_capability_group_info(int card_id, int device_id, int ts_id, int group_id,
    struct dcmi_capability_group_info *group_info, int group_count)
{
    int ret;
    int device_logic_id = 0;
    int group_count_num = 0;

    struct dsmi_capability_group_info capability_group_info[DCMI_CAPABILITY_GROUP_MAX_COUNT_NUM] = {0};

    ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", ret);
        return ret;
    }

    ret = dsmi_get_capability_group_info(device_logic_id, ts_id, group_id, capability_group_info, group_count);
    if (ret != DSMI_OK) {
        gplog(LOG_ERR, "dsmi_get_capability_group failed. err is %d", ret);
        return dcmi_convert_error_code(ret);
    }

    if (group_id == DCMI_CAPABILITY_GROUP_GROUP_ID_ALL) {
        group_count_num = DCMI_CAPABILITY_GROUP_MAX_COUNT_NUM;
    } else {
        group_count_num = DCMI_CAPABILITY_GROUP_MIN_COUNT_NUM;
    }

    ret = memcpy_s(group_info, group_count_num * sizeof(struct dcmi_capability_group_info),
        capability_group_info, group_count_num * sizeof(struct dsmi_capability_group_info));
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "memcpy_s dcmi_capability_group_info failed. err is %d.", ret);
        return DCMI_ERR_CODE_SECURE_FUN_FAIL;
    }

    return ret;
}

int dcmi_npu_get_capability_group_aicore_usage(int card_id, int device_id, int group_id, int *rate)
{
    int i;
    int ret;
    int count = 0;
    int device_logic_id = 0;
    int ts_id = DSMI_TS_AICORE;
    char aicore_utilization[DCMI_AICORE_NUM_MAX] = {0};
    unsigned int aicore_list_size = DCMI_AICORE_NUM_MAX;
    struct dsmi_capability_group_info capability_group_info = {0};

    ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", ret);
        return ret;
    }

    ret = dsmi_get_capability_group_info(device_logic_id, ts_id, group_id, &capability_group_info, 1);
    if (ret != DSMI_OK) {
        gplog(LOG_ERR, "dsmi_get_capability_group failed. err is %d", ret);
        return dcmi_convert_error_code(ret);
    }

    ret = dsmi_get_device_info(device_logic_id, DSMI_MAIN_CMD_TS,
        DSMI_TS_SUB_CMD_AICORE_UTILIZATION_RATE, (void *)aicore_utilization, &aicore_list_size);
    if (ret != DSMI_OK) {
        gplog(LOG_ERR, "dsmi_get_device_info failed. err is %d", ret);
        return dcmi_convert_error_code(ret);
    }

    // 根据aicore_mask的bit获取到占用的aic编号
    for (i = 0; (i < (int)aicore_list_size) && ((unsigned int)count < capability_group_info.aicore_number); i++) {
        if (((capability_group_info.aicore_mask[0] >> i) & 1) == 1) {
            *rate += aicore_utilization[i];
            count++;
        }
    }

    if (count != 0) {
        *rate /= count;
    } else {
        *rate = 0;
    }

    return DCMI_OK;
}

int dcmi_get_npu_device_cpu_freq_info(int card_id, int device_id, int *enable_flag)
{
    int ret;
    int device_logic_id = 0;
    unsigned char cpu_freq_status = 0;

    ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed.%d.", ret);
        return DCMI_ERR_CODE_INNER_ERR;
    }
    ret = dsmi_get_user_config(device_logic_id, "get_cpu_freq", CPU_FREQ_UP_CONFIG_LEN, &cpu_freq_status);
    if ((ret != DSMI_OK) && (ret != DSMI_ERR_NOT_SUPPORT)) {
        gplog(LOG_ERR, "call dsmi_get_user_config failed. err is %d.", ret);
        return dcmi_convert_error_code(ret);
    }
    *enable_flag = (int)cpu_freq_status;
    return DCMI_OK;
}

#if defined DCMI_VERSION_2
int dcmi_get_device_boot_area(int card_id, int device_id, int *status)
{
    int err;
    enum dcmi_unit_type device_type = NPU_TYPE;

    if (status == NULL) {
        gplog(LOG_ERR, "status is NULL");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if (dcmi_board_type_is_station()) {
        err = dcmi_get_device_type(card_id, device_id, &device_type);
        if (err != DCMI_OK) {
            gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d.", err);
            return err;
        }
        if (device_type == MCU_TYPE) {
            return dcmi_mcu_get_boot_area(card_id, status);
        } else {
            gplog(LOG_INFO, "device_type is not support.%d.", device_type);
            return DCMI_ERR_CODE_NOT_SUPPORT;
        }
    } else {
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
}
#endif

STATIC int check_permission_of_get_vnpu_memory(void *buf)
{
    int ret;
    unsigned int env_flag = ENV_PHYSICAL;

    if (dcmi_check_run_not_root() || dcmi_check_run_in_vm()) {
        gplog(LOG_OP, "Operation not permitted, VMs and non-root users are not supported.");
        return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
    }

    if (dcmi_check_run_in_docker()) {
        ret = dcmi_get_environment_flag(&env_flag);
        if (ret != DCMI_OK) {
            gplog(LOG_ERR, "call dcmi_get_environment_flag failed. err is %d.", ret);
            return ret;
        }
        if (env_flag != ENV_PHYSICAL_PRIVILEGED_CONTAINER) {
            gplog(LOG_OP, "Operation not permitted, only privileged containers are supported.");
            return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
        }
    }

    if (buf == NULL) {
        gplog(LOG_ERR, "buf is NULL.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if ((dcmi_board_chip_type_is_ascend_310p() != TRUE) || (dcmi_board_type_is_soc() == TRUE)) {
        gplog(LOG_OP, "This device does not support getting vnpu memory.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    return DCMI_OK;
}

STATIC int dcmi_get_vnpu_memory(void *buf)
{
    int ret;
    struct dsmi_resource_para para = {0};
    struct dsmi_resource_info info = {0};
    struct dcmi_vdev_query_stru *single_query = NULL;
    unsigned long long data = 0;
    unsigned int vdev_id = 0;

    ret = check_permission_of_get_vnpu_memory(buf);
    if (ret != DCMI_OK) {
        return ret;
    }
    single_query = (struct dcmi_vdev_query_stru *)buf;
    vdev_id = single_query->vdev_id;

    // 310P为DDR内存，无HBM，获取TOTAL
    para.owner_type = DSMI_VDEV_RESOURCE;
    para.resource_type = DSMI_DEV_DDR_TOTAL;
    info.buf = &data;
    info.buf_len = sizeof(data);
    ret = dsmi_get_resource_info(vdev_id, &para, &info);
    if (ret != DSMI_OK) {
        gplog(LOG_ERR, "Failed to get DDR_TOTAL. err is %d.", ret);
        return dcmi_convert_error_code(ret);
    }
    single_query->query_info.computing.vdev_memory_total = data;

    // 获取FREE
    data = 0;
    para.resource_type = DSMI_DEV_DDR_FREE;
    ret = dsmi_get_resource_info(vdev_id, &para, &info);
    if (ret != DSMI_OK) {
        gplog(LOG_ERR, "Failed to get DDR_FREE. err is %d.", ret);
        return dcmi_convert_error_code(ret);
    }
    single_query->query_info.computing.vdev_memory_free = data;

    return DCMI_OK;
}

int dcmi_get_npu_device_info(
    int card_id, int device_id, enum dcmi_main_cmd main_cmd, unsigned int sub_cmd, void *buf, unsigned int *size)
{
    int ret;
    int device_logic_id = 0;

    if (main_cmd == DCMI_MAIN_CMD_SEC && sub_cmd == DCMI_SEC_SUB_CMD_PSS) {
        //  pkcs使能整机生效，与device_id, card_id无关，参数不会使用，此处强制写0
        ret = dsmi_get_device_info(0, (DSMI_MAIN_CMD)main_cmd, sub_cmd, buf, size);
        if ((ret != DSMI_OK) && (ret != DSMI_ERR_NOT_SUPPORT)) {
            gplog(LOG_ERR, "call dsmi_get_device_info failed. err is %d.", ret);
        }
        return dcmi_convert_error_code(ret);
    }

    ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", ret);
        return ret;
    }

    ret = dsmi_get_device_info(device_logic_id, (DSMI_MAIN_CMD)main_cmd, sub_cmd, buf, size);
    if ((ret != DSMI_OK) && (ret != DSMI_ERR_NOT_SUPPORT)) {
        gplog(LOG_ERR, "call dsmi_get_device_info failed. err is %d.", ret);
        return dcmi_convert_error_code(ret);
    }

    if ((main_cmd == DCMI_MAIN_CMD_VDEV_MNG) &&
        (sub_cmd == DCMI_VMNG_SUB_CMD_GET_VDEV_ACTIVITY)) {
        ret = dcmi_get_vnpu_memory(buf);
        if ((ret != DCMI_OK)) {
            gplog(LOG_ERR, "Get vnpu memory failed. err is %d.", ret);
            return ret;
        }
        return DCMI_OK;
    }

    return (ret == DSMI_ERR_NOT_SUPPORT) ? dcmi_convert_error_code(ret) : DCMI_OK;
}

int dcmi_get_npu_device_mac_count(int card_id, int device_id, int *count)
{
    int ret;
    int device_logic_id = 0;

    ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", ret);
        return ret;
    }

    ret = dsmi_get_mac_count(device_logic_id, count);
    if ((ret != DSMI_OK) && (ret != DSMI_ERR_NOT_SUPPORT)) {
        gplog(LOG_ERR, "call dsmi_get_mac_count failed. err is %d.", ret);
    }

    return dcmi_convert_error_code(ret);
}

int dcmi_get_npu_device_mac(int card_id, int device_id, int mac_id, char *mac_addr, unsigned int len)
{
    int ret;
    int device_logic_id = 0;

    ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", ret);
        return ret;
    }

    ret = dsmi_get_mac_addr(device_logic_id, mac_id, mac_addr, len);
    if ((ret != DSMI_OK) && (ret != DSMI_ERR_NOT_SUPPORT)) {
        gplog(LOG_ERR, "call dsmi_get_mac_addr failed. err is %d.", ret);
    }

    return dcmi_convert_error_code(ret);
}

int dcmi_get_custom_op_secverify_enable(int card_id, int device_id, unsigned char *enable)
{
    int ret;
    int device_logic_id = 0;
    enum dcmi_unit_type device_type = NPU_TYPE;

    // 仅支持物理机root + 虚机的root
    if (!(dcmi_is_in_phy_machine_root() || dcmi_is_in_vm_root())) {
        return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
    }

    if (card_id < 0) {
        gplog(LOG_ERR, "input para is invalid. card_id=%d\n", card_id);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if (enable == NULL) {
        gplog(LOG_ERR, "enable pointer is NULL.\n");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if (!(dcmi_board_chip_type_is_ascend_910b() || dcmi_board_chip_type_is_ascend_910_93())) {
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    ret = dcmi_get_device_type(card_id, device_id, &device_type);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d", ret);
        return ret;
    }
    
    // chip为mcu时不涉及查询enable
    if (device_type != NPU_TYPE) {
        *enable = 0;
        return DCMI_OK;
    }

    ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", ret);
        return ret;
    }

    ret = dsmi_get_user_config(device_logic_id, "sign_auth_enable", sizeof(unsigned char), enable);
    if (ret != DSMI_OK) {
        gplog(LOG_ERR, "call dsmi_get_custom_op_secverify_enable failed. err is %d\n", ret);
        return dcmi_convert_error_code(ret);
    }

    return DCMI_OK;
}

int dcmi_get_custom_op_secverify_mode(int card_id, int device_id, unsigned int *mode)
{
    int ret;
    int device_logic_id = 0;
    unsigned int mode_size = sizeof(unsigned int);

    // 仅支持物理机root + 虚机的root
    if (!(dcmi_is_in_phy_machine_root() || dcmi_is_in_vm_root())) {
        return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
    }
    
    if (card_id < 0) {
        gplog(LOG_ERR, "input para is invalid. card_id=%d\n", card_id);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if (mode == NULL) {
        gplog(LOG_ERR, "mode pointer is NULL.\n");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if (!(dcmi_board_chip_type_is_ascend_910b() || dcmi_board_chip_type_is_ascend_910_93())) {
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
    
    ret = dcmi_get_device_logic_id(&device_logic_id, card_id, device_id);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "call dcmi_get_device_logic_id failed. err is %d.", ret);
        return ret;
    }

    ret = dsmi_get_device_info(device_logic_id, (DSMI_MAIN_CMD)DCMI_MAIN_CMD_SEC, DCMI_SEC_SUB_CMD_CUST_SIGN_FLAG,
        mode, &mode_size);
    if (ret != DSMI_OK) {
        gplog(LOG_ERR, "call dsmi_get_device_info failed. err is %d\n", ret);
        return dcmi_convert_error_code(ret);
    }

    return DCMI_OK;
}