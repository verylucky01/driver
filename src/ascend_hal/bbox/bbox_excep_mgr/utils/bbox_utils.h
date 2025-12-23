/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef BBOX_UTILS_H
#define BBOX_UTILS_H

#include "bbox_int.h"

/* Time stamp format is YYYYMMDDhhmmss-xxxxxxxxx */
#define TMSTMP_LEN 24

bbox_status bbox_time_to_str(char *buf, u32 len, const bbox_time_t *tm);
void bbox_host_time_to_str(char *buf, u32 len, struct timeval *tv);
void bbox_log_time_to_str(char *buf, u32 len, struct timeval *tv);
void bbox_get_date(const bbox_time_t *tm, char *date, u32 len);
u32 bbox_get_seq_from_time(const char *time_str);

struct bbox_data_info {
    char *buffer;
    size_t size;
    char *next;
    size_t used;
    size_t empty;
};

bbox_status bbox_data_init(struct bbox_data_info *info, size_t size);
bbox_status bbox_data_clear(struct bbox_data_info *info);
bbox_status bbox_data_reinit(struct bbox_data_info *info);
bbox_status bbox_data_print(struct bbox_data_info *info, const char *fmt, ...);


static inline u64 bbox_address_mask(u64 addr)
{
#ifdef CFG_BUILD_DEBUG
    return addr;
#else
    UNUSED(addr);
    return (u64)0xffffffffULL;
#endif
}

#endif
