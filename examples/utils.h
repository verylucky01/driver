/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef UTILS_H
#define UTILS_H

#define LOG_ERR(format, ...) \
    printf("[ERROR] [%s %d]" format, __func__, __LINE__, ## __VA_ARGS__)
#define LOG_WARN(format, ...) \
    printf("[WARN] [%s %d]" format, __func__, __LINE__, ## __VA_ARGS__)
#define LOG_INFO(format, ...) \
    printf("[INFO] [%s %d] " format, __func__, __LINE__, ## __VA_ARGS__)

#define CHECK_ERROR(call) \
    do { \
        int __ret = (call); \
        if (__ret != 0) { \
            LOG_ERR("Operation failed: return error code %d.\n", __ret); \
            return -1; \
        } \
    } while (0)

#endif