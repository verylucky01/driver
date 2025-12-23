/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef __DCMI_FAULT_MANAGE_INTF_H__
#define __DCMI_FAULT_MANAGE_INTF_H__
#include "dcmi_interface_api.h"

#define DCMI_MIN_ERR_INFO_LEN          48           //  errorcode对应的描述，长度最小为48字节
#define DCMI_GET_EVENT_WAIT_TIME_MAX   30000        /* 30000ms(30s) */

/* The DSMI error code must be the same as that in the dsmi_common_interface.h file. */
#define DCMI_ERR_CODE_MAX_NUM           20
#define DSMI_OK                         0
#define DSMI_ERR_NO_DEVICE              1
#define DSMI_ERR_INVALID_DEVICE         2
#define DSMI_ERR_INVALID_VALUE          3
#define DSMI_ERR_INVALID_HANDLE         4
#define DSMI_ERR_INNER_ERR              7
#define DSMI_ERR_PARA_ERROR             8
#define DSMI_ERR_NOT_EXIST              11
#define DSMI_ERR_DEVICE_BUSY            13
#define DSMI_ERR_WAIT_TIMEOUT           16
#define DSMI_ERR_IOCRL_FAIL             17
#define DSMI_ERR_PARA_INVALID           22
#define DSMI_ERR_SEND_MESG              27
#define DSMI_ERR_FILE_OPS               38
#define DSMI_ERR_OPER_NOT_PERMITTED     46
#define DSMI_ERR_TRY_AGAIN              51
#define DSMI_ERR_MEMORY_OPT_FAIL        58
#define DSMI_ERR_PARTITION_NOT_RIGHT    86
#define DSMI_ERR_RESOURCE_OCCUPIED      87
#define DSMI_ERR_NOT_SUPPORT            0xfffe

#define CPU_ALARM_KEY_SECTION_NUM    6
#define CPU_ALARM_FILE               "/run/all_active_alarm"
#define CPU_NO_ALARM_SECTION_NUM     3
#define CPU_ALARM_EXIST_SECTION_NUM  6
#define CPU_ALARM_COUNT_INDEX        1
#define CPU_ALARM_ID_INDEX           0
#define CPU_ALARM_EXIST_SECTION_NUM  6

struct err_code_map_node {
    int dsmi_err_code;
    int dcmi_err_code;
};

int dcmi_convert_error_code(int dsmi_err_code);

int dcmi_cpu_get_device_errorcode(int card_id, int* error_count, unsigned int *error_code_list, unsigned int list_len);

int dcmi_get_npu_device_errorcode(
    int card_id, int device_id, int *errorcount, unsigned int *errorcode_list, unsigned int list_len);

int dcmi_get_npu_device_errorcode_string(
    int card_id, int device_id, int error_code, unsigned char *error_info, int buff_size);

int dcmi_subscribe_npu_fault_event(int card_id, int device_id, struct dcmi_event_filter filter,
    dcmi_fault_event_callback handler);

#endif /* __DCMI_FAULT_MANAGE_INTF_H__ */