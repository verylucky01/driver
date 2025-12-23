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
#include <string.h>
 
#include "securec.h"
#include "dsmi_common_interface_custom.h"
#include "dcmi_interface_api.h"
#include "dcmi_init_basic.h"
#include "dcmi_fault_manage_intf.h"
#include "dcmi_log.h"
#include "dcmi_common.h"
#include "dcmi_inner_cfg_manage.h"
#include "dcmi_inner_cfg_persist.h"
#include "dcmi_mcu_intf.h"
#include "dcmi_product_judge.h"
#include "dcmi_environment_judge.h"
#include "dcmi_inner_info_get.h"

#if defined DCMI_VERSION_2
int dcmi_get_device_power_info(int card_id, int device_id, int *power)
{
    int err;
    enum dcmi_unit_type device_type = NPU_TYPE;

    if (power == NULL) {
        gplog(LOG_ERR, "power is invalid.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    err = dcmi_get_device_type(card_id, device_id, &device_type);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d.", err);
        return err;
    }

    if (device_type == NPU_TYPE) {
        return dcmi_get_npu_device_power_info(card_id, device_id, power);
    } else if (device_type == MCU_TYPE) {
        if (dcmi_board_chip_type_is_ascend_910()) {
            return DCMI_ERR_CODE_NOT_SUPPORT;
        }

        return dcmi_mcu_get_power_info(card_id, power);
    } else {
        gplog(LOG_ERR, "device_type %d is not support.", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
}

int dcmi_get_device_health(int card_id, int device_id, unsigned int *health)
{
    int err;
    enum dcmi_unit_type device_type = NPU_TYPE;

    if (health == NULL) {
        gplog(LOG_ERR, "health is invalid.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    err = dcmi_get_device_type(card_id, device_id, &device_type);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d.", err);
        return err;
    }

    if (device_type == NPU_TYPE) {
        return dcmi_get_npu_device_health(card_id, device_id, health);
    } else if (device_type == MCU_TYPE) {
        return dcmi_mcu_get_health(card_id, health);
#ifndef _WIN32
    } else if (device_type == CPU_TYPE) {
        return dcmi_cpu_get_health(card_id, health);
#endif
    } else {
        gplog(LOG_ERR, "device_type %d is error.", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
}

int dcmi_get_driver_health(unsigned int *health)
{
#ifndef _WIN32
    int err;

    if (health == NULL) {
        gplog(LOG_ERR, "health is invalid.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    err = dsmi_get_driver_health(health);
    if (err != DSMI_OK) {
        gplog(LOG_ERR, "dsmi_get_driver_health failed. err is %d.", err);
    }
    return dcmi_convert_error_code(err);
#else
    return DCMI_ERR_CODE_NOT_SUPPORT;
#endif
}

int dcmi_get_device_aicore_info(int card_id, int device_id, struct dcmi_aicore_info *aicore_info)
{
    int err;
    enum dcmi_unit_type device_type = NPU_TYPE;

    if (aicore_info == NULL) {
        gplog(LOG_ERR, "aicore_info is invalid.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    err = dcmi_get_device_type(card_id, device_id, &device_type);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d.", err);
        return err;
    }

    if (device_type == NPU_TYPE) {
        return dcmi_get_npu_aicore_info(card_id, device_id, aicore_info);
    } else {
        gplog(LOG_ERR, "device_type %d is error.", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
}

int dcmi_get_device_aicpu_info(int card_id, int device_id, struct dcmi_aicpu_info *aicpu_info)
{
    int err;
    enum dcmi_unit_type device_type = NPU_TYPE;

    if (aicpu_info == NULL) {
        gplog(LOG_ERR, "aicpu_info is invalid.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    err = dcmi_get_device_type(card_id, device_id, &device_type);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d.", err);
        return err;
    }

    if (device_type == NPU_TYPE) {
        return dcmi_get_npu_aicpu_info(card_id, device_id, aicpu_info);
    } else {
        gplog(LOG_ERR, "device_type %d is error.", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
}

int dcmi_get_device_system_time(int card_id, int device_id, unsigned int *time)
{
    int err;
    enum dcmi_unit_type device_type = NPU_TYPE;

    if (time == NULL) {
        gplog(LOG_ERR, "time is invalid.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }
    err = dcmi_get_device_type(card_id, device_id, &device_type);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d.", err);
        return err;
    }

    if (device_type == NPU_TYPE) {
        return dcmi_get_npu_system_time(card_id, device_id, time);
    } else {
        gplog(LOG_ERR, "device_type %d is error.", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
}

int dcmi_get_device_temperature(int card_id, int device_id, int *temperature)
{
    int err;
    enum dcmi_unit_type device_type = NPU_TYPE;

    if (temperature == NULL) {
        gplog(LOG_ERR, "temperature is invalid.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    err = dcmi_get_device_type(card_id, device_id, &device_type);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d.", err);
        return err;
    }

    if (device_type == NPU_TYPE) {
        return dcmi_get_npu_device_temperature(card_id, device_id, temperature);
    } else if (device_type == MCU_TYPE) {
        return dcmi_mcu_get_temperature(card_id, temperature);
    } else {
        gplog(LOG_ERR, "device_type %d is not support.", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
}

int dcmi_get_device_voltage(int card_id, int device_id, unsigned int *voltage)
{
    int err;
    enum dcmi_unit_type device_type = NPU_TYPE;

    if (voltage == NULL) {
        gplog(LOG_ERR, "voltage is invalid.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    err = dcmi_get_device_type(card_id, device_id, &device_type);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d.", err);
        return err;
    }

    // 910B、910_93的标卡以外的产品形态(server)的Chip MCU暂不支持电压查询功能
    if ((dcmi_board_chip_type_is_ascend_910b() || dcmi_board_chip_type_is_ascend_910_93()) &&
        dcmi_board_type_is_server() && device_type == MCU_TYPE) {
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    if (device_type == NPU_TYPE) {
        return dcmi_get_npu_device_voltage(card_id, device_id, voltage);
    } else if (device_type == MCU_TYPE) {
        return dcmi_mcu_get_voltage(card_id, voltage);
    } else {
        gplog(LOG_ERR, "device_type %d is not support.", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
}

int dcmi_get_device_ecc_info(int card_id, int device_id, enum dcmi_device_type input_type,
    struct dcmi_ecc_info *device_ecc_info)
{
    int err;
    enum dcmi_unit_type device_type = NPU_TYPE;

    if (device_ecc_info == NULL) {
        gplog(LOG_ERR, "device_ecc_info is invalid.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if (input_type > DCMI_HBM_RECORDED_MULTI_ADDR || input_type < DCMI_DEVICE_TYPE_DDR) {
        gplog(LOG_ERR, "input type is invalid, input_type=%d", input_type);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    err = dcmi_get_device_type(card_id, device_id, &device_type);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d.", err);
        return err;
    }

    if (device_type == NPU_TYPE) {
        return dcmi_get_npu_ecc_info(card_id, device_id, input_type, device_ecc_info);
    } else {
        gplog(LOG_ERR, "device_type %d is not support.", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
}

int dcmi_get_device_frequency(int card_id, int device_id, enum dcmi_freq_type input_type, unsigned int *frequency)
{
    int err;
    enum dcmi_unit_type device_type = NPU_TYPE;

    if (frequency == NULL) {
        gplog(LOG_ERR, "frequency is NULL");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if (input_type > DCMI_FREQ_VECTORCORE_CURRENT) {
        gplog(LOG_ERR, "input_type is invalid. input_type=%d", input_type);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    err = dcmi_get_device_type(card_id, device_id, &device_type);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d.", err);
        return err;
    }

    if (dcmi_board_chip_type_is_ascend_910b() == TRUE && input_type == DCMI_FREQ_DDR) {
        gplog(LOG_ERR, "This device does not support DDR.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    if (device_type == NPU_TYPE) {
        return dcmi_get_npu_device_frequency(card_id, device_id, input_type, frequency);
#ifndef _WIN32
    } else if (device_type == CPU_TYPE) {
        return dcmi_cpu_get_device_frequency(card_id, device_id, frequency);
#endif
    } else {
        gplog(LOG_ERR, "device_type %d is not support.", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
}

int dcmi_get_device_hbm_info(int card_id, int device_id, struct dcmi_hbm_info *hbm_info)
{
    int err;
    enum dcmi_unit_type device_type = NPU_TYPE;

    if (hbm_info == NULL) {
        gplog(LOG_ERR, "hbm_info is invalid.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    err = dcmi_get_device_type(card_id, device_id, &device_type);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d.\n", err);
        return err;
    }

    if (device_type == NPU_TYPE) {
        return dcmi_get_npu_hbm_info(card_id, device_id, hbm_info);
    } else {
        gplog(LOG_ERR, "device_type %d is not support.", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
}

int dcmi_get_device_memory_info_v2(int card_id, int device_id, struct dcmi_memory_info *memory_info)
{
    int err;
    enum dcmi_unit_type device_type = NPU_TYPE;

    if (memory_info == NULL) {
        gplog(LOG_ERR, "memory_info is invalid.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    err = dcmi_get_device_type(card_id, device_id, &device_type);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d.", err);
        return err;
    }

    if (device_type == NPU_TYPE) {
        return dcmi_get_npu_device_memory_info_v2(card_id, device_id, memory_info);
#ifndef _WIN32
    } else if (device_type == CPU_TYPE) {
        return dcmi_cpu_get_memory_info(card_id, memory_info);
#endif
    } else {
        gplog(LOG_ERR, "device_type %d is not support.", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
}

int dcmi_get_device_memory_info_v3(int card_id, int device_id, struct dcmi_get_memory_info_stru *memory_info)
{
    int err;
    enum dcmi_unit_type device_type = NPU_TYPE;

    if (memory_info == NULL) {
        gplog(LOG_ERR, "memory_info is invalid.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    err = dcmi_get_device_type(card_id, device_id, &device_type);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d.", err);
        return err;
    }

    if (device_type == NPU_TYPE) {
        return dcmi_get_npu_device_memory_info_v3(card_id, device_id, memory_info);
#ifndef _WIN32
    } else if (device_type == CPU_TYPE) {
        struct dcmi_memory_info pdevice_memory_info = { 0 };
        err = dcmi_cpu_get_memory_info(card_id, &pdevice_memory_info);
        if (err == DCMI_OK) {
            memory_info->memory_size = pdevice_memory_info.memory_size;
            memory_info->freq = pdevice_memory_info.freq;
            memory_info->utiliza = pdevice_memory_info.utiliza;
        }
        return err;
#endif
    } else {
        gplog(LOG_ERR, "device_type %d is not support.", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
}

int dcmi_get_device_utilization_rate(int card_id, int device_id, int input_type, unsigned int *utilization_rate)
{
    int err;
    enum dcmi_unit_type device_type = NPU_TYPE;

    if (utilization_rate == NULL) {
        gplog(LOG_ERR, "utilization_rate is invalid.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    err = dcmi_get_device_type(card_id, device_id, &device_type);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d.", err);
        return err;
    }

    if (device_type == NPU_TYPE) {
        return dcmi_get_npu_device_utilization_rate(card_id, device_id, input_type, utilization_rate);
    } else {
        gplog(LOG_ERR, "device_type %d is not support.", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
}

int dcmi_get_device_sensor_info(int card_id, int device_id, enum dcmi_manager_sensor_id sensor_id,
    union dcmi_sensor_info *sensor_info)
{
    int err;
    enum dcmi_unit_type device_type = NPU_TYPE;
    unsigned int main_board_id;

    if (sensor_info == NULL) {
        gplog(LOG_ERR, "sensor_info is NULL");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if (sensor_id >= DCMI_SENSOR_INVALID_ID) {
        gplog(LOG_ERR, "sensor_id is invalid. sensor_id=%d", sensor_id);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    /* ATLAS_900_A3_SUPERPOD, 从die无法直接查询到光模块温度，需要转换card_id */
    if (dcmi_board_chip_type_is_ascend_910_93() && (sensor_id == DCMI_FP_TEMP_ID)) {
        err = dcmi_get_mainboard_id(card_id, 0, &main_board_id);
        if (err != DCMI_OK) {
            gplog(LOG_ERR, "Failed to query main board id of card. err is %d", err);
            return DCMI_ERR_CODE_INVALID_PARAMETER;
        }

        if (dcmi_mainboard_is_910_93(main_board_id) == TRUE) {
            card_id = dcmi_a900_a3_superpod_fp_card_id_convert(card_id, device_id);
        }
    }

    err = dcmi_get_device_type(card_id, device_id, &device_type);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d.", err);
        return err;
    }

    if (device_type == NPU_TYPE) {
        return dcmi_get_npu_soc_sensor_info(card_id, device_id, sensor_id, sensor_info);
    } else {
        gplog(LOG_ERR, "device_type %d is not support.", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
}

int dcmi_set_container_service_enable(void)
{
    int err;

    err = dsmi_enable_container_service();
    if (err != DSMI_OK) {
        gplog(LOG_OP, "dsmi_enable_container_service failed. err is %d.", err);
        return dcmi_convert_error_code(err);
    }

    gplog(LOG_OP, "enable container service success.");
    return DCMI_OK;
}

int dcmi_get_device_cgroup_info(int card_id, int device_id, struct dcmi_cgroup_info *cg_info)
{
    int err;
    enum dcmi_unit_type device_type = NPU_TYPE;

    if (cg_info == NULL) {
        gplog(LOG_ERR, "cg_info is NULL");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    err = dcmi_get_device_type(card_id, device_id, &device_type);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d.", err);
        return err;
    }

    if (device_type == NPU_TYPE) {
        return dcmi_get_npu_device_cgroup_info(card_id, device_id, cg_info);
    } else {
        gplog(LOG_ERR, "device_type %d is not support.", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
}

int dcmi_get_device_llc_perf_para(int card_id, int device_id, struct dcmi_llc_perf *perf_para)
{
    int err;
    enum dcmi_unit_type device_type = NPU_TYPE;

    if (perf_para == NULL) {
        gplog(LOG_ERR, "perf_para is NULL");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    err = dcmi_get_device_type(card_id, device_id, &device_type);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d.", err);
        return err;
    }

    if (device_type == NPU_TYPE) {
        return dcmi_get_npu_device_llc_perf_para(card_id, device_id, perf_para);
    } else {
        gplog(LOG_ERR, "device_type %d is not support.", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    return DCMI_OK;
}

int dcmi_get_device_network_health(int card_id, int device_id, enum dcmi_rdfx_detect_result *result)
{
    int err;
    enum dcmi_unit_type device_type = NPU_TYPE;

    if (result == NULL) {
        gplog(LOG_ERR, "result is NULL");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }
    if (dcmi_board_chip_type_is_ascend_910b_300i_a2() == TRUE) {
        gplog(LOG_OP, "This device does not support get device network health.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    err = dcmi_get_device_type(card_id, device_id, &device_type);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d.", err);
        return err;
    }

    if (device_type == NPU_TYPE) {
        return dcmi_get_npu_device_network_health(card_id, device_id, result);
    } else {
        gplog(LOG_ERR, "device_type %d is not support.", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    return DCMI_OK;
}

int dcmi_get_device_fan_count(int card_id, int device_id, int *count)
{
    int err;
    enum dcmi_unit_type device_type = NPU_TYPE;

    if (count == NULL) {
        gplog(LOG_ERR, "count is NULL");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    err = dcmi_get_device_type(card_id, device_id, &device_type);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d.", err);
        return err;
    }

    if (device_type == NPU_TYPE) {
        return dcmi_get_npu_device_fan_count(card_id, device_id, count);
    } else {
        gplog(LOG_ERR, "device_type %d is not support.", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    return DCMI_OK;
}

int dcmi_get_device_fan_speed(int card_id, int device_id, int fan_id, int *speed)
{
    int err;
    enum dcmi_unit_type device_type = NPU_TYPE;

    if (speed == NULL) {
        gplog(LOG_ERR, "speed is NULL");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if (fan_id < 0) {
        gplog(LOG_ERR, "fan_id %d is invalid.", fan_id);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    err = dcmi_get_device_type(card_id, device_id, &device_type);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d.", err);
        return err;
    }

    if (device_type == NPU_TYPE) {
        return dcmi_get_npu_device_fan_speed(card_id, device_id, fan_id, speed);
    } else {
        gplog(LOG_ERR, "device_type %d is not support.", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    return DCMI_OK;
}

int dcmi_get_device_resource_info(int card_id, int device_id, struct dcmi_proc_mem_info *proc_info, int *proc_num)
{
#ifndef _WIN32
    int err;
    enum dcmi_unit_type device_type = NPU_TYPE;

    if (proc_info == NULL || proc_num == NULL) {
        gplog(LOG_ERR, "proc_info or proc_num is null.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    err = dcmi_get_device_type(card_id, device_id, &device_type);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d.", err);
        return err;
    }
    bool check_result = !(dcmi_board_type_is_card() || dcmi_board_type_is_station() || dcmi_board_type_is_hilens() ||
        dcmi_board_type_is_server() || dcmi_board_type_is_model());
    if (check_result) {
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    if (device_type != NPU_TYPE) {
        gplog(LOG_ERR, "device_type %d is error.", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    err = dcmi_get_npu_proc_mem_info(card_id, device_id, proc_info, proc_num);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_npu_proc_mem_info failed. err is %d.", err);
    }

    return err;
#else
    return DCMI_ERR_CODE_NOT_SUPPORT;
#endif
}

int dcmi_set_power_state(int card_id, int device_id, struct dcmi_power_state_info_stru power_info)
{
    int err;
    enum dcmi_unit_type device_type = NPU_TYPE;

    if (!dcmi_is_in_phy_machine_root()) {
        gplog(LOG_OP, "Operation not permitted, only root user on physical machine can call this api.");
        return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
    }

    err = dcmi_get_device_type(card_id, device_id, &device_type);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d.", err);
        return err;
    }

    if (dcmi_board_chip_type_is_ascend_310b() && dcmi_get_product_type_inner() != DCMI_A200_A2_MODEL) {
        gplog(LOG_ERR, "This product(%d) does not support set power_state.", dcmi_get_product_type_inner());
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    if (device_type == NPU_TYPE) {
        err = dcmi_set_sleep_state(card_id, device_id, power_info);
        if (err != DCMI_OK) {
            gplog(LOG_ERR, "dcmi_set_sleep_state failed. err is %d.", err);
            return err;
        }
        gplog(LOG_OP, "Set the power state for chip %d of card success.sleep_time=%u.", device_id, power_info.value);
    } else {
        gplog(LOG_OP, "device_type %d is not support.", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
    return err;
}

int dcmi_get_card_customized_info(int card_id, char *info, int len)
{
    char buffer[DCMI_MCU_CUSTOMIZED_INFO_MAX_LEN] = {0};
    int length;
    int err;

    if (info == NULL || len <= 0) {
        gplog(LOG_ERR, "info or len %d is invalid.", len);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    err = dcmi_check_card_id(card_id);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "check card id %d failed %d.", card_id, err);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    bool check_support_result =
        (dcmi_board_chip_type_is_ascend_310p() || (dcmi_board_chip_type_is_ascend_310() && dcmi_board_type_is_card()));
    if (!check_support_result) {
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    err = dcmi_mcu_get_customized_info(card_id, buffer, len, &length);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_mcu_get_customized_info failed. err is %d.", err);
        return DCMI_ERR_CODE_INNER_ERR;
    }
    if (length >= len) {
        gplog(LOG_ERR, "get customized info length %d is bigger than or equal to len %d.", length, len);
        return DCMI_ERR_CODE_INNER_ERR;
    }

    err = strncpy_s(info, len, buffer, length);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, " strncpy_s failed. err is %d.", err);
        return DCMI_ERR_CODE_SECURE_FUN_FAIL;
    }

    return DCMI_OK;
}

#endif

#if defined DCMI_VERSION_1
int dcmi_get_customized_info_api(int card_id, char *data_info, int *len)
{
    char info[DCMI_MCU_CUSTOMIZED_INFO_MAX_LEN] = {0};
    int err;

    if (data_info == NULL || len == NULL) {
        gplog(LOG_ERR, "data_info or len is invalid.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    err =  dcmi_get_card_customized_info(card_id, info, sizeof(info));
    if (err != DCMI_OK) {
        if (err != DCMI_ERR_CODE_NOT_SUPPORT) {
            gplog(LOG_ERR, "dcmi_get_card_customized_info failde err is %d.", err);
        }
        return err;
    }

    err = strncpy_s(data_info, strlen(info) + 1, info, strlen(info));
    if (err != EOK) {
        gplog(LOG_ERR, "strncpy_s failde err is %d.", err);
        return DCMI_ERR_CODE_SECURE_FUN_FAIL;
    }
    *len = (int)strlen(info);
    return DCMI_OK;
}

int dcmi_set_customized_info_api(int card_id, const char *data_info, int len)
{
    return dcmi_set_card_customized_info(card_id, (char *)data_info, len);
}

int dcmi_get_aicore_info(int card_id, int device_id, struct dsmi_aicore_info_stru *aicore_info)
{
    return dcmi_get_device_aicore_info(card_id, device_id, (struct dcmi_aicore_info *)aicore_info);
}

int dcmi_get_aicpu_info(int card_id, int device_id, struct dsmi_aicpu_info_stru *aicpu_info)
{
    return dcmi_get_device_aicpu_info(card_id, device_id, (struct dcmi_aicpu_info *)aicpu_info);
}

int dcmi_get_p2p_enable(int card_id, int device_id, int *enable_flag)
{
    return dcmi_get_device_p2p_enable(card_id, device_id, enable_flag);
}

int dcmi_get_ecc_info(int card_id, int device_id, int device_type, struct dsmi_ecc_info_stru *device_ecc_info)
{
    struct dcmi_ecc_info ecc_info = {0};
    int ret;

    if (device_ecc_info == NULL) {
        gplog(LOG_ERR, "device_ecc_info is NULL");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    ret = dcmi_get_device_ecc_info(card_id, device_id, device_type, &ecc_info);
    if (ret == DCMI_OK) {
        device_ecc_info->enable_flag = ecc_info.enable_flag;
        device_ecc_info->single_bit_error_count = ecc_info.single_bit_error_cnt;
        device_ecc_info->double_bit_error_count = ecc_info.double_bit_error_cnt;
    }
    return ret;
}

int dcmi_get_hbm_info(int card_id, int device_id, struct dsmi_hbm_info_stru *device_hbm_info)
{
    return dcmi_get_device_hbm_info(card_id, device_id, (struct dcmi_hbm_info *)device_hbm_info);
}

int dcmi_get_memory_info(int card_id, int device_id, struct dcmi_memory_info_stru *pdevice_memory_info)
{
    return dcmi_get_device_memory_info_v2(card_id, device_id, (struct dcmi_memory_info *)pdevice_memory_info);
}

int dcmi_get_soc_sensor_info(int card_id, int device_id, int sensor_id, union tag_sensor_info *sensor_info)
{
    return dcmi_get_device_sensor_info(card_id, device_id, sensor_id, (union dcmi_sensor_info *)sensor_info);
}

int dcmi_get_system_time(int card_id, int device_id, unsigned int *time)
{
    return dcmi_get_device_system_time(card_id, device_id, time);
}

int dcmi_get_device_cpu_freq_info(int card_id, int device_id, int *enable_flag)
{
    int ret;
    enum dcmi_unit_type device_type = NPU_TYPE;

    if (enable_flag == NULL) {
        gplog(LOG_ERR, "enable_flag is NULL");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    ret = dcmi_get_device_type(card_id, device_id, &device_type);
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d.", ret);
        return ret;
    }

    if (dcmi_board_chip_type_is_ascend_310p_300v()) {
        if (device_type == NPU_TYPE) {
            return dcmi_get_npu_device_cpu_freq_info(card_id, device_id, enable_flag);
        } else {
            gplog(LOG_ERR, "device_type is not support.%d.", device_type);
            return DCMI_ERR_CODE_NOT_SUPPORT;
        }
    } else {
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
}
#endif

int dcmi_get_device_boot_status(int card_id, int device_id, enum dcmi_boot_status *boot_status)
{
    int err;
    enum dcmi_unit_type device_type = NPU_TYPE;

    if (boot_status == NULL) {
        gplog(LOG_ERR, "boot_status is invalid.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    err = dcmi_get_device_type(card_id, device_id, &device_type);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d.", err);
        return err;
    }

    if (device_type == NPU_TYPE) {
        return dcmi_get_npu_device_boot_status(card_id, device_id, boot_status);
    }

    gplog(LOG_ERR, "device_type %d is error.", device_type);
    return DCMI_ERR_CODE_NOT_SUPPORT;
}

int dcmi_get_npu_work_mode(int card_id, unsigned char *work_mode)
{
#ifndef _WIN32
    int ret;
    unsigned int value = DCMI_NPU_WORK_MODE_INVALID;

    if ((dcmi_board_type_is_server() == FALSE) || dcmi_board_chip_type_is_ascend_910b() ||
        dcmi_board_chip_type_is_ascend_910_93()) {
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    if (card_id < 0) {
        gplog(LOG_ERR, "input para is invalid. card_id=%d\n", card_id);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if (work_mode == NULL) {
        gplog(LOG_ERR, "work mode pointer is NULL.\n");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    ret = dsmi_get_work_mode(&value);
    if (ret != DSMI_OK) {
        gplog(LOG_ERR, "call dsmi_get_work_mode failed. err is %d\n", ret);
        return dcmi_convert_error_code(ret);
    }

    *work_mode = (value & DCMI_NPU_WORK_MODE_MASK);

    return DCMI_OK;
#else
    return DCMI_OK;
#endif
}

int dcmi_get_device_p2p_enable(int card_id, int device_id, int *enable_flag)
{
    int err;
    enum dcmi_unit_type device_type = NPU_TYPE;

    if (enable_flag == NULL) {
        gplog(LOG_ERR, "enable_flag is invalid.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    err = dcmi_get_device_type(card_id, device_id, &device_type);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d.", err);
        return err;
    }

    if (device_type == NPU_TYPE) {
        // 200soc, 200EP, 300卡, 500, 500A2，200A2小站均支持该命令
        if (dcmi_board_chip_type_is_ascend_310() || dcmi_board_chip_type_is_ascend_310b()) {
            *enable_flag = 0;
            return DCMI_OK;
        }
    }

    return DCMI_ERR_CODE_NOT_SUPPORT;
}

int dcmi_get_device_mac(int card_id, int device_id, int mac_id, char *mac_addr, unsigned int len)
{
    int err;
    enum dcmi_unit_type device_type = NPU_TYPE;

    if (!dcmi_is_in_phy_machine_root() && dcmi_board_chip_type_is_ascend_910b_300i_a2() == TRUE) {
        gplog(LOG_OP, "Operation not permitted, only root user on physical machine can call this api.");
        return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
    }

    if (mac_addr == NULL) {
        gplog(LOG_ERR, "input para mac_addr is NULL.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }
    if (len < MAC_ADDR_LEN) {
        gplog(LOG_ERR, "input para len is invalid. len=%u.", len);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    err = dcmi_get_device_type(card_id, device_id, &device_type);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d.", err);
        return err;
    }

    if (dcmi_board_chip_type_is_ascend_910b_300i_a2() == TRUE) {
        gplog(LOG_ERR, "This device does not support get device mac.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    if (device_type == NPU_TYPE) {
        return dcmi_get_npu_device_mac(card_id, device_id, mac_id, mac_addr, len);
    } else {
        gplog(LOG_ERR, "device_type %d is not support.", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    return DCMI_OK;
}

int dcmi_get_nve_level(int card_id, int device_id, int *nve_level)
{
    int err;
    unsigned char level = 0;

    if (nve_level == NULL) {
        gplog(LOG_ERR, "nve_level is NULL");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if (dcmi_board_chip_type_is_ascend_310() && dcmi_board_type_is_soc()) {
        err = dcmi_get_user_config(card_id, device_id, "get_nve_level", sizeof(unsigned char), &level);
        if (err != DCMI_OK) {
            gplog(LOG_ERR, "dcmi_get_user_config failed. err is %d.", err);
            return err;
        }

        *nve_level = (int)level;
    } else {
        gplog(LOG_ERR, "This device does not support get nve level.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    return DCMI_OK;
}

int dcmi_get_device_gateway(int card_id, int device_id, enum dcmi_port_type input_type, int port_id,
    struct dcmi_ip_addr *gateway)
{
    int err;
    enum dcmi_unit_type device_type = NPU_TYPE;

    if (gateway == NULL) {
        gplog(LOG_ERR, "gateway is NULL");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if (input_type >= DCMI_INVALID_PORT) {
        gplog(LOG_ERR, "input_type is invalid. input_type=%d", input_type);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }
    if (dcmi_board_chip_type_is_ascend_310b() == TRUE ||
        dcmi_board_chip_type_is_ascend_910b_300i_a2() == TRUE) {
        gplog(LOG_OP, "This device does not support get device gateway.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    err = dcmi_get_device_type(card_id, device_id, &device_type);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d.", err);
        return err;
    }

    if (device_type == NPU_TYPE) {
        return dcmi_get_npu_device_gateway(card_id, device_id, input_type, port_id, gateway);
    } else {
        gplog(LOG_ERR, "device_type %d is not support.", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    return DCMI_OK;
}

int dcmi_get_device_ip(int card_id, int device_id, enum dcmi_port_type input_type, int port_id, struct dcmi_ip_addr *ip,
    struct dcmi_ip_addr *mask)
{
    int err;
    enum dcmi_unit_type device_type = NPU_TYPE;

    if (ip == NULL) {
        gplog(LOG_ERR, "input para ip is NULL.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }
    if (mask == NULL) {
        gplog(LOG_ERR, "input para mask is NULL.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }
    if (input_type >= DCMI_INVALID_PORT) {
        gplog(LOG_ERR, "input para input_type is invalid. input_type=%d.", input_type);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if (dcmi_board_chip_type_is_ascend_310b() == TRUE ||
        dcmi_board_chip_type_is_ascend_910b_300i_a2() == TRUE) {
        gplog(LOG_OP, "This device does not support get device ip.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    err = dcmi_get_device_type(card_id, device_id, &device_type);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d.", err);
        return err;
    }

    if (device_type == NPU_TYPE) {
        return dcmi_get_npu_device_ip(card_id, device_id, input_type, port_id, ip, mask);
    } else {
        gplog(LOG_ERR, "device_type %d is not support.", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
}

int dcmi_get_device_share_enable(int card_id, int device_id, int *enable_flag)
{
    int err;
    enum dcmi_unit_type device_type = NPU_TYPE;

    if (enable_flag == NULL) {
        gplog(LOG_ERR, "enable_flag is NULL");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    err = dcmi_get_device_type(card_id, device_id, &device_type);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d.", err);
        return err;
    }

    if (device_type == NPU_TYPE) {
        return dcmi_get_npu_device_share_enable(card_id, device_id, enable_flag);
    } else {
        gplog(LOG_INFO, "device_type is not support.%d.", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
}

int dcmi_get_device_cpu_num_config(int card_id, int device_id, unsigned char *buf, unsigned int buf_size)
{
    int err;
    enum dcmi_unit_type device_type = NPU_TYPE;

    if (buf == NULL || buf_size != DCMI_CPU_NUM_CFG_LEN) {
        gplog(LOG_ERR, "cpu_num_config is null or buf size %u is invaild", buf_size);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    // 支持310P与310B
    if (!(dcmi_board_chip_type_is_ascend_310p() || dcmi_board_chip_type_is_ascend_310b())) {
        gplog(LOG_OP, "This device does not support get cpu num cfg.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    err = dcmi_get_device_type(card_id, device_id, &device_type);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d.", err);
        return err;
    }

    if (device_type == NPU_TYPE) {
        return dcmi_get_user_config(card_id, device_id, "cpu_num_cfg", buf_size, buf);
    } else {
        gplog(LOG_INFO, "device_type is not support.%d.", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
}

int dcmi_get_capability_group_info(int card_id, int device_id, int ts_id, int group_id,
    struct dcmi_capability_group_info *group_info, int group_count)
{
    int err;
    enum dcmi_unit_type device_type = INVALID_TYPE;

    if (!dcmi_is_in_phy_machine()) {
        gplog(LOG_OP, "Operation not permitted, only user on physical machine can call this api.");
        return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
    }

    if ((ts_id != DCMI_TS_AICORE) || (group_info == NULL) || (group_count < DCMI_CAPABILITY_GROUP_MIN_COUNT_NUM) ||
        group_id < DCMI_CAPABILITY_GROUP_GROUP_ID_ALL || group_id >= DCMI_CAPABILITY_GROUP_MAX_COUNT_NUM) {
        gplog(LOG_ERR, "parameter invalid. ts_id=%d, group_id=%d, group_count=%d", ts_id, group_id, group_count);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if ((group_id == DCMI_CAPABILITY_GROUP_GROUP_ID_ALL) && (group_count < DCMI_CAPABILITY_GROUP_MAX_COUNT_NUM)) {
        gplog(LOG_ERR, "group_count [%d] invalid, the value should be bigger than 4!", group_count);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    err = dcmi_get_device_type(card_id, device_id, &device_type);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d.", err);
        return err;
    }

    if ((device_type == NPU_TYPE) && (dcmi_check_capability_group_support_type() == DCMI_OK)) {
        err = dcmi_npu_get_capability_group_info(card_id, device_id, ts_id, group_id, group_info, group_count);
        if (err != DCMI_OK) {
            gplog(LOG_ERR, "get capability group failed. card_id=%d, device_id=%d, ts_id=%d, group_id=%d, err=%d",
                card_id, device_id, ts_id, group_id, err);
            return err;
        }

        return DCMI_OK;
    } else {
        gplog(LOG_ERR, "The device does not support this api.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
}

int dcmi_get_capability_group_aicore_usage(int card_id, int device_id, int group_id, int *rate)
{
    int err;
    enum dcmi_unit_type device_type = INVALID_TYPE;
    if (!dcmi_is_in_phy_machine()) {
        gplog(LOG_OP, "Operation not permitted, only user on physical machine can call this api.");
        return DCMI_ERR_CODE_OPER_NOT_PERMITTED;
    }

    if (rate == NULL) {
        gplog(LOG_ERR, "rate invalid.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if ((group_id < 0) || (group_id >= DCMI_CAPABILITY_GROUP_MAX_COUNT_NUM)) {
        gplog(LOG_ERR, "group_id [%d] invalid, the value should be 0~3!", group_id);
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    err = dcmi_get_device_type(card_id, device_id, &device_type);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d.", err);
        return err;
    }

    if ((device_type == NPU_TYPE) && (dcmi_check_capability_group_support_type() == DCMI_OK)) {
        err = dcmi_npu_get_capability_group_aicore_usage(card_id, device_id, group_id, rate);
        if (err != DCMI_OK) {
            gplog(LOG_ERR, "get capability aicore usage failed. card_id=%d, device_id=%d, group_id=%d, err=%d",
                card_id, device_id, group_id, err);
            return err;
        }

        return DCMI_OK;
    } else {
        gplog(LOG_ERR, "The device does not support this api.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
}

int dcmi_get_custom_op_status(int card_id, int device_id, int *enable_flag)
{
    int err;
    enum dcmi_unit_type device_type = NPU_TYPE;

    if (enable_flag == NULL) {
        gplog(LOG_ERR, "enable_flag is NULL");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    if (dcmi_board_chip_type_is_ascend_910_93() != TRUE) {
        gplog(LOG_ERR, "This device does not support querying custom-op.");
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }

    err = dcmi_get_device_type(card_id, device_id, &device_type);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_get_device_type failed. err is %d.", err);
        return err;
    }

    if (device_type == NPU_TYPE) {
        return dcmi_get_npu_custom_op_status(card_id, device_id, enable_flag);
    } else {
        gplog(LOG_INFO, "device_type is not support.%d.", device_type);
        return DCMI_ERR_CODE_NOT_SUPPORT;
    }
}

int dcmi_get_custom_op_config_recover_mode(unsigned int *mode)
{
    int err;

    err = dcmi_check_custom_op_config_recover_mode_is_permitted("get");
    if (err != DCMI_OK) {
        return err;
    }

    if (mode == NULL) {
        gplog(LOG_ERR, "mode is invalid.");
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    err = dcmi_cfg_get_custom_op_config_recover_mode(mode);
    if (err != DCMI_OK) {
        gplog(LOG_ERR, "get config recover mode failed. err=%d", err);
        return DCMI_ERR_CODE_INNER_ERR;
    }
    return DCMI_OK;
}
