/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef __HDC_ADAPT_H__
#define __HDC_ADAPT_H__

#include "hdc_cmn.h"

#define PPC_PATH_MAX 128

void drv_hdc_trans_type_mutex_init(void);
hdcError_t hdc_set_init_info(void);
int hdc_get_chip_type(void);
void hdc_pcie_init_sleep(void);
void hdc_phandle_get_sleep(void);
hdcError_t drv_hdc_recv_msg_body_ret_check(signed int ret);
signed int hdc_pcie_client_destroy(mmProcess handle, signed int devId, signed int serviceType);
hdcError_t halHdcClientWakeUp(HDC_CLIENT client);
hdcError_t halHdcServerWakeUp(HDC_SERVER server);
extern char g_ppc_dirs[PPC_PATH_MAX];

#endif