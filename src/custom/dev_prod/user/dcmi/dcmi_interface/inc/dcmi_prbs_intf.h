/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef __DCMI_PRBS_INTF_H__
#define __DCMI_PRBS_INTF_H__
#include "dcmi_interface_api.h"

#define DCMI_PRBS_RETRY_TIMES       50
#define DCMI_PRBS_RETRY_WATI_TIME   100000
 
#if defined DCMI_VERSION_1

enum PRBS_ADAPT_MODE {
    PRBS_TX_SERDES_TRIGGER,
    PRBS_TX_RETIMER_HOST_ADAPT,
    PRBS_RX_SERDES_MEDIA_ADAPT,
    PRBS_RX_RETIMER_MEDIA_ADAPT,
    PRBS_RETIMER_TX_ENABLE,
};
 
#endif
 
#endif /* __DCMI_PRBS_INTF_H__ */