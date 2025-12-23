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
    struct dcmi_flash_info_stru flash_info = {0};
    unsigned int flash_count = 0;
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
        ret = dcmi_get_device_flash_count(card_id_list[i], device_id, &flash_count);
        if (ret != NPU_OK) {
            printf("Failed to get flash count. ret=%d\n", ret);
            return ret;
        }
        for (int i = 0; i < flash_count; i++) {
            ret = dcmi_get_device_flash_info(card_id_list[i], device_id, i, &flash_info);
            if (ret != NPU_OK) {
                printf("Failed to get flash info. ret=%d\n", ret);
                return ret;
            }
            printf("==================== DCMI Flash 信息 ====================\n");
            printf("闪存ID (flash_id):        0x%llX\n", flash_info.flash_id);
            printf("设备ID (device_id):       0x%04X\n", flash_info.device_id);
            printf("厂商ID (vendor):          0x%04X\n", flash_info.vendor);
            printf("制造商ID (manufacturer_id): 0x%04X\n", flash_info.manufacturer_id);
            printf("健康状态 (state):         0x%08X (%s)\n", flash_info.state,
                   (flash_info.state == 0x8)  ? "正常" :
                   (flash_info.state == 0x10) ? "非正常" :
                                                "未知状态");
            printf("总大小 (size):            0x%llX 字节 = ", flash_info.size);
            if (flash_info.size >= 1024 * 1024 * 1024) {
                printf("%.2f GB\n", (double)flash_info.size / (1024 * 1024 * 1024));
            } else if (flash_info.size >= 1024 * 1024) {
                printf("%.2f MB\n", (double)flash_info.size / (1024 * 1024));
            } else if (flash_info.size >= 1024) {
                printf("%.2f KB\n", (double)flash_info.size / 1024);
            } else {
                printf("%llu 字节\n", flash_info.size);
            }
            printf("擦除单元数量 (sector_count): %u\n", flash_info.sector_count);
            printf("=======================================================\n");
        }
    }
    return ret;
}