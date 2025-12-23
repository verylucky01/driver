/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef __LQ_DCMI_LOG_H__
#define __LQ_DCMI_LOG_H__

#include <string.h>

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#define DCMI_LOG_DIR "/home/log/plog"
#define DCMI_LOG_PROFIX ".log"
#define DCMI_BAKLOG_PROFIX ".log.bak"

#ifdef _WIN32
#define COMMON_LOG_DIR  "C:\\Program Files\\Huawei\\npu-smi\\log\\"
#else
#define COMMON_LOG_DIR  "/var/log"
#endif

#define COMMON_LOG_PREFIX "lqdcmitools_"
#define COMMON_LOG_SUFFIX ".log"
#define COMMON_BAKLOG_SUFFIX ".log.bak"

#define LOG_ERR  "LOG_ERR"
#define LOG_INFO "LOG_INFO"
#define LOG_OP   "LOG_OP"

void lq_dcmi_log_fun(const char *mod, const char *fmt, ...);

#ifdef _WIN32
#define __FILENAME__ (strrchr(__FILE__, '\\') ? (strrchr(__FILE__, '\\') + 1) : __FILE__)

#define LQ_DCMI_TYPE_LOG(mod, fmt, ...) do { \
    lq_dcmi_log_fun(mod, "[%s,%s,%d]:" fmt "\r\n", __FILENAME__, __FUNCTION__, __LINE__, __VA_ARGS__); \
} while (0)

#define gplog(mod, fmt, ...)   LQ_DCMI_TYPE_LOG(mod, fmt, __VA_ARGS__)

#else
#define __FILENAME__ (strrchr(__FILE__, '/') ? (strrchr(__FILE__, '/') + 1) : __FILE__)

#define LQ_DCMI_TYPE_LOG(mod, fmt, args...) do { \
    lq_dcmi_log_fun(mod, "[%s,%s,%d]:" fmt "\r\n", __FILENAME__, __FUNCTION__, __LINE__, ##args); \
} while (0)

#define gplog(mod, fmt, args...) LQ_DCMI_TYPE_LOG(mod, fmt, ##args)

#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __LQ_DCMI_LOG_H__ */
