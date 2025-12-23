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
    struct dcmi_pcie_info pcie_info = {0};
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
        ret = dcmi_get_device_pcie_info(card_id_list[i], device_id, &pcie_info);
        if (ret != NPU_OK) {
            printf("Failed to get pcie info. ret=%d\n", ret);
            return ret;
        }
        printf("==================== DCMI PCIE信息 ====================\n");
        printf("设备ID (deviceid): \t0x%08X\n", pcie_info.deviceid);
        printf("厂商ID (venderid): \t0x%08X\n", pcie_info.venderid);
        printf("厂商子ID (subvenderid): \t0x%08X\n", pcie_info.subvenderid);
        printf("设备子ID (subdeviceid): \t0x%08X\n", pcie_info.subdeviceid);
        printf("BDF-总线ID (bdf_busid): \t0x%02X\n", pcie_info.bdf_busid);
        printf("BDF-设备ID (bdf_deviceid): \t0x%02X\n", pcie_info.bdf_deviceid);
        printf("BDF-功能ID (bdf_funcid): \t0x%02X\n", pcie_info.bdf_funcid);
        printf("=======================================================\n");
    }
    return ret;
}