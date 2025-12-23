/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef HCCN_COMMON_H
#define HCCN_COMMON_H

#ifndef CONFIG_LLT
#define STATIC static
#else
#define STATIC
#endif

#define HCCN_CHECK_USER_IS_ROOT "root"
#define HCCN_USER_NAME_LEN 32
#define HCCN_USER_IP_LEN 32

int hccn_check_usr_identify();
int hccn_get_usr_name();
int hccn_get_usr_ip(void);
char *hccn_get_g_usr_name(void);
char *hccn_get_g_usr_ip(void);
#endif
