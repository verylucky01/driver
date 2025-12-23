/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef BBOX_PERROR_H
#define BBOX_PERROR_H

#include <errno.h>
#include "bbox_int.h"

#if defined(OS_TYPE) && defined(WIN64) && OS_TYPE == WIN64
#include <windows.h>
#define ERRNO_IS_FILE_NOT_FOUND (get_last_error() == ERROR_FILE_NOT_FOUND)
#define ERRNO_IS_PATH_NOT_FOUND (get_last_error() == ERROR_PATH_NOT_FOUND)
#define ERRNO_IS_NOT_FOUND ((ERRNO_IS_FILE_NOT_FOUND) || (ERRNO_IS_PATH_NOT_FOUND))

static inline const WCHAR *format_err_str(DWORD err_idx, WCHAR *err_str, DWORD str_max_len)
{
    DWORD ret = format_message(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                              NULL, err_idx,
                              MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                              err_str, str_max_len, NULL);
    return (ret != 0) ? err_str : NULL;
}

#else // OS_TYPE is LINUX
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#define ERRNO_IS_NOT_FOUND (errno == ENOENT)

static inline s32 get_last_error(void)
{
    return errno;
}

static inline const char *format_err_str(s32 err_idx, char *err_str, u32 str_max_len)
{
    // XSI-compliant version: int strerror_r(int errnum, char *buf, size_t buflen)
    // GNU-specific version: char *strerror_r(int errnum, char *buf, size_t buflen)
    intptr_t ret_str = (intptr_t)strerror_r(err_idx, err_str, str_max_len);
    return ((ret_str != 0) && (ret_str != -1)) ? (char *)ret_str : err_str;
}
#endif

#endif // BBOX_PERROR_H
