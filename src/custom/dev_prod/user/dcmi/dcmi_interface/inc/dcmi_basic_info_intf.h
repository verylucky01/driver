/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef __DCMI_BASIC_INFO_INTF_H__
#define __DCMI_BASIC_INFO_INTF_H__
#include "dcmi_interface_api.h"

#define DCMI_VERSION_MIN_LEN         16            /* 存放dcmi版本号的内存空间，大小不能小于16 */
#define OS_VERSION_FILE              "/etc/os-release"
#define COMPONENT_VERSION_MIN_LEN    64       /* 存放固件版本号信息的内存空间, 大小不能小于64Byte。 */
#define DCMI_PRODUCT_TYPE_MIN_LEN    32       /* 存放product type 的内存空间，大小不能小于32 */
 
#if defined DCMI_VERSION_1
#define DRIVER_VERSION_MIN_LEN       64        /* 存放版本号信息的内存空间, 大小不能小于64Byte。 */
#endif

int dcmi_get_device_logic_id(int *device_logic_id, int card_id, int device_id);

#endif /* __DCMI_BASIC_INFO_INTF_H__ */