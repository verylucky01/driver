/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef BBOX_KERNEL_KLOG_PARSER_H
#define BBOX_KERNEL_KLOG_PARSER_H

#include "bbox_ddr_int.h"

struct printk_management {
    u64 printk_start;
    u64 printk_next;
    u64 console_next;
    u64 printk_buf_size;
    u64 power_flag;
    u64 printk_area_head_backup_area;
};

#pragma pack(1)
struct printk_log {
    u64 ts_nsec;     /* timestamp in nanoseconds */
    u16 len;        /* length of entire record */
    u16 text_len;    /* length of text buffer */
    u16 dict_len;    /* length of dictionary buffer */
    u8 facility;    /* syslog facility */
    u8 flags : 5;   /* internal record flags */
    u8 level : 3;   /* syslog level */
};
#pragma pack()

bbox_status bbox_klog_save_fs_printk_info_log(const void *buffer, u32 len, const char *log_path);
bool bbox_klog_is_printk_info_log(const char *manage_data);

#endif
