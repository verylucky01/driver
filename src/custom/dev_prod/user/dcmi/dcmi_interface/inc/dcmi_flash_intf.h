/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef __DCMI_FLASH_INTF_H__
#define __DCMI_FLASH_INTF_H__
#include "dcmi_interface_api.h"
 
int dcmi_get_npu_device_flash_count(int card_id, int device_id, unsigned int *flash_count);
 
int dcmi_get_npu_device_flash_info(
    int card_id, int device_id, unsigned int flash_index, struct dcmi_flash_info *flash_info);
 
#endif /* __DCMI_FLASH_INTF_H__ */