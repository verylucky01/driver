/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef _HDC_PPC_H_
#define _HDC_PPC_H_


#define QLEN 10
#define SOCK_NAME_LEN 30
#ifndef PKT_SIZE_1G
#define PKT_SIZE_1G (1024 * 1024 * 1024U)  // 1G Byte
#endif
#define PPC_DIR_DEFAULT "/home/HwHiAiUser/hdc_ppc/"
#define PPC_USER_ROOT "root"
#define PPC_WORK_NAME "hdc_ppc"
#ifdef CFG_SOC_PLATFORM_HELPER_V51
#define PPC_FILE_PERMISSION_WRITE 0660
#else
#define PPC_FILE_PERMISSION_WRITE 0640
#endif

enum sock_type {
    SOCK_CLIENT = 0,
    SOCK_SERVER
};


#endif
