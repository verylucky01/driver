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
#define BUF_SIZE 16
int main(int argc, char **argv)
{
    int ret;
    int card_count = 0;
    int card_id_list[MAX_CARD_NUM] = {0};
    char *config_name = "mac_info";
    unsigned char buff[BUF_SIZE] = {0};
    unsigned char buf[BUF_SIZE] = {0x65, 0x67, 0x68, 0x69, 0x65, 0x67, 0x68, 0x69,
                                   0x65, 0x67, 0x68, 0x69, 0x65, 0x67, 0x68, 0x69};
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
    ret = dcmi_set_user_config(card_id_list[0], 0, config_name, BUF_SIZE, buf);
    if (ret != 0) {
        printf("dcmi set user config ret=%d\n", ret);
        return ret;
    }
    printf("dcmi_set_user_config ok\n");
    ret = dcmi_get_user_config(card_id_list[0], 0, config_name, BUF_SIZE, buff);
    if (ret != 0) {
        printf("dcmi get user config ret=%d\n", ret);
        return ret;
    }
    printf("mac_info:");
    for (int i = 0; i < BUF_SIZE; i++) {
        printf("%02x ", buff[i]);
    }
    printf("\n");
    return ret;
}