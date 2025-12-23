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
#include "dcmi_interface_api.h"
#define MAX_CARD_NUMBER (16)
#define NPU_OK (0)
int main()
{
    int ret;
    int card_count = 0;
    int device_count = 0;
    int card_id;
    int card_id_list[8] = {0};
    int device_id = 0;
    enum dcmi_reset_channel inband_channel = INBAND_CHANNEL;
    ret = dcmi_init();
    if (ret != NPU_OK) {
        printf("Failed to init dcmi.\n");
        return ret;
    }
    ret = dcmi_get_card_num_list(&card_count, card_id_list, MAX_CARD_NUMBER);
    if (ret != NPU_OK) {
        printf("Failed to get card number,ret is %d\n", ret);
        return ret;
    }

    // 复位
    ret = dcmi_set_device_reset(card_id_list[0], device_id, inband_channel);
    if (ret != NPU_OK) {
        printf("dcmi_set_device_reset fail! card_id is %d , device_id is %d, channel_type: %d, ret: %d\n",
               card_id_list[0], device_id, inband_channel, ret);
    } else {
        printf("dcmi_set_device_reset successful! card_id is %d, device_id:%d, channel_type: %d\n", card_id_list[0],
               device_id, inband_channel);
    }
    return 0;
}