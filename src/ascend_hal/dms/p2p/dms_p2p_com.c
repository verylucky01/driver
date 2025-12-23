/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "dms_device_info.h"
#include "dms_user_common.h"
#include "dms_p2p_com.h"

int dms_set_p2p_com_info(unsigned int dev_id, unsigned int sub_cmd, void *buf, unsigned int size)
{
    return DmsSetDeviceInfo(dev_id, (DSMI_MAIN_CMD)DMS_MAIN_CMD_P2P_COM, sub_cmd, buf, size);
}