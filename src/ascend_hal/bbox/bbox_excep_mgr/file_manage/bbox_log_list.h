/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef BBOX_LOG_LIST_H
#define BBOX_LOG_LIST_H

#include "bbox_int.h"
#include "bbox_list.h"

#define DATE_MAXLEN 32
#define LOG_MAXLEN 256
#define FILE_NAME_MAXLEN 64U

struct bbox_log_file_list {
    struct bbox_list his_list;
    struct bbox_list log_list;
    struct bbox_list dir_list;
};

struct log_node {
    struct list_head list;
    char tmstmp[DATE_MAXLEN]; // time stamp, as YYYYMMDDhhmmss-xxxxxx
    char log_str[LOG_MAXLEN];  // log string
};

typedef struct dir_node {
    struct list_head list;
    char tmstmp[DATE_MAXLEN]; // subdir (time stamp, as YYYYMMDDhhmmss-xxxxxx)
    u64 dirsize;              // whole dir size
} dir_node_t, excpt_node_t;

// log list, from history.log
bbox_status bbox_dev_log_list_add(u32 dev_id, const buff *buf);
void *bbox_dev_log_list_take_out(u32 dev_id);
void bbox_dev_log_list_for_each(u32 dev_id, const bbox_list_elem_func func, const arg_void *arg);
u32 bbox_dev_log_list_get_node_num(u32 dev_id);
u32 bbox_dev_log_list_get_last_seq(u32 dev_id);
bbox_status bbox_log_list_init_node(struct log_node *node, const char *tmstmp, const char *log_str);
struct log_node *bbox_dev_log_list_create_node(const char *tmstmp, const char *log_str);

// dir list, from timestamp dir
bbox_status bbox_dev_dir_list_add(u32 dev_id, const buff *buf);
bbox_status bbox_dev_dir_list_del(u32 dev_id, const buff *buf);
void *bbox_dev_dir_list_take_out(u32 dev_id);
void *bbox_dev_dir_list_for_each(u32 dev_id, const bbox_list_elem_func func, const arg_void *arg);
u32 bbox_dev_dir_list_get_node_num(u32 dev_id);

// his list, from log + dir
bbox_status bbox_dev_hist_list_add(u32 dev_id, const buff *buf);
s32 bbox_dev_hist_list_del(u32 dev_id, const buff *buf);
void *bbox_dev_hist_list_take_out(u32 dev_id);
void *bbox_dev_hist_list_for_each(u32 dev_id, const bbox_list_elem_func func, const arg_void *arg);

bbox_status bbox_log_file_list_init(void);
void bbox_log_file_list_final(void);

#endif

