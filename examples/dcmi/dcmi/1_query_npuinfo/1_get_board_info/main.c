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
    struct dcmi_board_info_stru board_info = {0};
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
    for (int i = 0; i < card_count; i++) {
        ret = dcmi_get_board_info(card_id_list[i], device_id, &board_info);
        if (ret != NPU_OK) {
            printf("Failed to get board info. ret=%d\n", ret);
            return ret;
        }
        printf("==================== DCMI 单板信息 ====================\n");
        printf("单板ID (board_id):        0x%08X\n", board_info.board_id);
        printf("PCB版本编号 (pcb_id):     0x%08X\n", board_info.pcb_id);
        printf("BOM版本编号 (bom_id):     0x%08X\n", board_info.bom_id);
        printf("槽位号信息 (slot_id):     0x%02X (十进制：%u)\n", board_info.slot_id, board_info.slot_id);
        printf("=======================================================\n");
    }
    return ret;
}