/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "dcmi_for_om.h"
#include "dcmi_log.h"
#include "dcmi_interface_api.h"
#include "dcmi_elabel_operate.h"


int dcmi_for_om_get_device_memory_size(int card_id, int device_id, unsigned long long *memory_size)
{
    int ret;
    struct dcmi_get_memory_info_stru memory_info = {0};

    if (memory_size == NULL) {
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }

    ret = dcmi_get_device_memory_info_v3(card_id, device_id, &memory_info);
    if (ret != DCMI_OK) {
        return ret;
    }
    *memory_size = memory_info.memory_size;
    return DCMI_OK;
}

int dcmi_for_om_get_board_sn(int card_id, unsigned char *data, unsigned short data_size, unsigned short *data_len)
{
    (void)card_id;
    return dcmi_elabel_get_data(ELABEL_ITEM_ID_BOARD_SN, data, data_size, data_len);
}

int dcmi_for_om_get_product_sn(int card_id, unsigned char *data, unsigned short data_size, unsigned short *data_len)
{
    (void)card_id;
    return dcmi_elabel_get_data(ELABEL_ITEM_ID_PRD_SN, data, data_size, data_len);
}

int dcmi_for_om_get_product_time(int card_id, unsigned char *data, unsigned short data_size, unsigned short *data_len)
{
    (void)card_id;
    return dcmi_elabel_get_data(ELABEL_ITEM_ID_MFG_DATE, data, data_size, data_len);
}

int dcmi_for_om_get_asstag(int card_id, unsigned char *data, unsigned short data_size, unsigned short *data_len)
{
    (void)card_id;
    return dcmi_elabel_get_data(ELABEL_ITEM_ID_EXTEND, data, data_size, data_len);
}

int dcmi_for_om_set_asstag(int card_id, unsigned char *data, unsigned short data_size, unsigned short data_len)
{
    (void)card_id;
    return dcmi_elabel_set_data(ELABEL_ITEM_ID_EXTEND, data, data_size, data_len);
}

int get_elabel_func_hooker(GET_ELABEL_FUNC_HOOKEE hal_hook_func)
{
    if (hal_hook_func == NULL) {
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }
    return hal_hook_func(dcmi_for_om_get_board_sn, dcmi_for_om_get_product_sn, dcmi_for_om_get_asstag,
        dcmi_for_om_set_asstag, dcmi_for_om_get_product_time);
}

int hal_npu_func_hooker(HAL_NPU_FUNC_HOOKEE hal_hook_func)
{
    int ret;

    if (hal_hook_func == NULL) {
        return DCMI_ERR_CODE_INVALID_PARAMETER;
    }
    
    ret = dcmi_init();
    if (ret != DCMI_OK) {
        gplog(LOG_ERR, "dcmi_init failed. ret is %d", ret);
        return DCMI_ERR_CODE_INNER_ERR;
    }

    return hal_hook_func(dcmi_get_card_list, dcmi_get_device_num_in_card, dcmi_get_device_health,
        dcmi_get_device_temperature, dcmi_get_device_power_info, dcmi_for_om_get_device_memory_size,
        dcmi_get_device_utilization_rate);
}
