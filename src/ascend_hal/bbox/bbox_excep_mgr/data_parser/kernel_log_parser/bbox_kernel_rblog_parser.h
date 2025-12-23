/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef BBOX_KERNEL_RB_LOG_PARSER_H
#define BBOX_KERNEL_RB_LOG_PARSER_H

#include "bbox_ddr_int.h"

struct rb_dev_printk_info {
    char subsystem[16];
    char device[48];
};

struct rb_printk_info {
    u64 seq;        /* sequence number */
    u64 ts_nsec;     /* timestamp in nanoseconds */
    u16 text_len;    /* length of text message */
    u8 facility;
    u8 flags : 5;
    u8 level : 3;
    u32 caller_id;

    struct rb_dev_printk_info dev_info;
};

struct rb_data_blk_lpos {
    unsigned long begin;
    unsigned long next;
};

struct rb_desc {
    unsigned long state_var;
    struct rb_data_blk_lpos text_blk_lpos;
};

struct rb_data_ring {
    u32 size_bits;
    char *data;
    unsigned long head_lpos;
    unsigned long tail_lpos;
};

struct rb_desc_ring {
    u32 count_bits;
    struct rb_desc *descs;
    struct rb_printk_info *infos;
    unsigned long head_id;
    unsigned long tail_id;
#ifdef CFG_FEATURE_KERNEL_LOG_ADAPT
    unsigned long last_finalized_id;
#endif
};

struct rb_printk {
    struct rb_desc_ring desc_ring;
    struct rb_data_ring text_data_ring;
    unsigned long fail;
};

struct rb_printk_area_size {
    unsigned long printk_start;
    unsigned long printk_buf_size;
    unsigned long end_flag;
};

struct rb_printk_record {
    struct rb_printk_info *info;
    char *text_buf;
    u32 text_buf_size;
};

bbox_status bbox_klog_save_fs_ring_buffer_log(const void *buffer, u32 len, const char *log_path);
bool bbox_klog_is_ring_buffer_log(const char *manage_data);

#endif
