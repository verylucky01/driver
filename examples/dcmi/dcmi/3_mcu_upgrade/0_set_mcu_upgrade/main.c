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
#include <unistd.h>
#include <string.h>
#include "dcmi_interface_api.h"
#define NPU_OK (0)
int main(int argc, char **argv)
{
    int ret;
    int card_count = 0;
    int card_id_list[MAX_CARD_NUM] = {0};
    int device_id = 0;
    char version_str[16] = {0};
    enum dcmi_upgrade_type input_type = MCU_UPGRADE_START;
    int status = 0;
    int progress = 0;
    const int total = 100;
    ret = dcmi_init();
    if (ret != NPU_OK) {
        printf("Failed to init dcmi.\n");
        return ret;
    }
    ret = dcmi_get_card_num_list(&card_count, card_id_list, MAX_CARD_NUM);
    if (ret != NPU_OK) {
        printf("Failed to get card number.\n");
        return ret;
    }
    printf("card count is %d \n", card_count);
    ret = dcmi_get_mcu_version(card_id_list[0], version_str, sizeof(version_str));
    if (ret != NPU_OK) {
        printf("Failed to get mcu version. ret=%d\n", ret);
        return ret;
    }
    ret = dcmi_set_mcu_upgrade_stage(card_id_list[0], input_type);
    if (ret != NPU_OK) {
        printf("Failed to start mcu upgrade.ret=%d\n", ret);
        return ret;
    }
    while (1) {
        sleep(2);
        ret = dcmi_get_mcu_upgrade_status(card_id_list[0], &status, &progress);
        if (ret != 0) {
            printf("Failed to get mcu upgrade status ret=%d\n", ret);
            return ret;
        }
        if (status == 0) {
            printf("升级成功\n");
            return 0;
        } else if (status == 2) {
            printf("不支持升级\n");
            return -1;
        } else if (status == 3) {
            printf("升级失败\n");
            return -2;
        } else if (status == 4) {
            printf("获取状态失败\n");
            return -3;
        }
    }
}