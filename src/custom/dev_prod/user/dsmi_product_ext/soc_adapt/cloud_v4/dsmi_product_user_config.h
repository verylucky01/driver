/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef __DSMI_PRODUCT_USER_CONFIG_H
#define __DSMI_PRODUCT_USER_CONFIG_H

#include "devdrv_user_config_common.h"

#define UC_PROD_ITEM_MAX_NUM            1               // 新增配置项时，需要同步修改

#define GUID_INFO                       "guid_info"
#define GUID_INFO_BLOCK_OFFSET          0x2130000       // 具体范围查看 g_flash_ctrl
#define GUID_INFO_CONFIG_SIZE           32

const struct user_config_item_product g_user_config_info_product[UC_PROD_ITEM_MAX_NUM] = {
    {GUID_INFO, NULL, FLASH_BLOCK_COMMON,
        GUID_INFO_BLOCK_OFFSET, 0, GUID_INFO_CONFIG_SIZE,
        SUPPORT_SET_USER_CFG | SUPPORT_GET_USER_CFG, UC_PRO_AUTHORITY_USER_WR, UC_SUPPORT_ALL,
        NULL, NULL},
};

#endif
