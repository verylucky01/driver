/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef BBOX_KERNEL_LOG_PARSER_H
#define BBOX_KERNEL_LOG_PARSER_H

#include "bbox_ddr_int.h"

#define PAGE_SIZE 0x1000
struct printk_area_head {
    char ro1[PAGE_SIZE];
    char manage_data[PAGE_SIZE];
    char ro2[PAGE_SIZE];
};

#define PRINTK_MANAGEMENT_ENG_FLAG 0xa5a5a5a55a5a5a5auL

bbox_status bbox_klog_dump(const void *buffer, u32 len, const char *log_path);
s32 bbox_klog_save_buf_to_fs(const char *text_log, u32 text_len, u64 ts_usec, const char *log_path);
void bbox_klog_save_all_log_buf_to_fs(const void *log_buf, u32 len, const char *log_path);

#endif
