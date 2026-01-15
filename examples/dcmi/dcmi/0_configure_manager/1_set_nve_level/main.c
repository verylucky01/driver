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
#include "dcmi_interface_api.h"
#define NPU_OK (0)
int main(int argc, char **argv)
{
    int ret;
    int card_count = 0;
    int card_id_list[MAX_CARD_NUM] = {0};
    int level = 0;
    int device_id = 0;
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
    ret = dcmi_set_nve_level(card_id_list[0], device_id, 3);
    if (ret != NPU_OK) {
        printf("Failed to set nve level. ret=%d\n", ret);
        return ret;
    }
    ret = dcmi_get_nve_level(card_id_list[0], device_id, &level);
    if (ret != NPU_OK) {
        printf("Failed to get nve level. ret=%d\n", ret);
        if(ret == -8255)
        {
            printf("该设备不支持\n");
        }
        return ret;
    }
    /*
    0：功耗和算力档位为low
    1：功耗和算力档位为middle
    2：功耗和算力档位为high
    3：功耗和算力档位为full
    */
    printf("level =%d\n", level);
    return ret;
}