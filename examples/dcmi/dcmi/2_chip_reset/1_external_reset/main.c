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
#include <unistd.h>
#include "dcmi_interface_api.h"
#define NPU_OK (0)
int main()
{
    int ret;
    int card_count = 0;
    int device_count = 0;
    int card_id;
    int card_id_list[8] = {0};
    int device_id;
    int state = 0;
    enum dcmi_reset_channel outband_channel = OUTBAND_CHANNEL;
    ret = dcmi_init();
    if (ret != NPU_OK) {
        printf("Failed to init dcmi.\n");
        return ret;
    }
    ret = dcmi_get_card_num_list(&card_count, card_id_list, MAX_CARD_NUM);
    if (ret != NPU_OK) {
        printf("Failed to get card number,ret is %d\n", ret);
        return ret;
    }
    for (card_id = 0; card_id < card_count; card_id++) {
        ret = dcmi_get_device_num_in_card(card_id_list[card_id], &device_count);
        if (ret != NPU_OK) {
            printf("dcmi_get_device_num_in_card failed! card_id is %d ,ret: %d\n", card_id_list[card_id], ret);
            return ret;
        }
        for (device_id = 0; device_id <= device_count; device_id++) {
            // 查询带外通道
            ret = dcmi_get_device_outband_channel_state(card_id_list[card_id], device_id, &state);
            if (ret != NPU_OK) {
                printf("dcmi_get_device_outband_channel_state fail! card_id is %d , device_id is %d, ret: %d\n",
                       card_id_list[card_id], device_id, ret);
                if (ret == -8255) {
                    printf("该设备不支持\n");
                }
                return ret;
            } else {
                printf("dcmi_get_device_outband_channel_state successful! card_id is %d, device_id:%d\n",
                       card_id_list[card_id], device_id);
            }
            // 预复位
            ret = dcmi_set_device_pre_reset(card_id_list[card_id], device_id);
            if (ret != NPU_OK) {
                printf("dcmi_set_device_pre_reset fail! card_id is %d , device_id is %d, ret: %d\n",
                       card_id_list[card_id], device_id, ret);
                return ret;
            } else {
                printf("dcmi_set_device_pre_reset successful! card_id is %d, device_id:%d\n", card_id_list[card_id],
                       device_id);
            }
            // 复位
            ret = dcmi_set_device_reset(card_id_list[card_id], device_id, outband_channel);
            if (ret != NPU_OK) {
                printf("dcmi_set_device_reset fail! card_id is %d , device_id is %d, channel_type: %d, ret: %d\n",
                       card_id_list[card_id], device_id, outband_channel, ret);
                return ret;
            } else {
                printf("dcmi_set_device_reset successful! card_id is %d, device_id:%d, channel_type: %d\n",
                       card_id_list[card_id], device_id, outband_channel);
            }
            sleep(3);
            // 重新扫描
            ret = dcmi_set_device_rescan(card_id_list[card_id], device_id);
            if (ret != NPU_OK) {
                printf("dcmi_set_device_rescan fail! card_id is %d , device_id is %d, ret: %d\n", card_id_list[card_id],
                       device_id, ret);
                return ret;
            } else {
                printf("dcmi_set_device_rescan successful! card_id is %d, device_id:%d\n", card_id_list[card_id],
                       device_id);
            }
        }
    }
    return 0;
}