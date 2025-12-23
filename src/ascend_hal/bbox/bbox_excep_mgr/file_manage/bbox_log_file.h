/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef BBOX_LOG_FILE_H
#define BBOX_LOG_FILE_H

#include "bbox_int.h"

#define INVALID_FD (-1)

s32 bbox_format_device_path(char *buf, u32 len, u32 dev);
s32 bbox_format_excpt_path(char *buf, u32 len, u32 dev, const char *tm_stmp);
s32 bbox_format_hist_log(char *buf, u32 len, u32 dev);
s32 bbox_format_path(char *buf, u32 len, const char *parent, const char *child);
s32 bbox_format_path_time(char *buf, u32 len, const char *parent, const char *child);

bbox_status bbox_create_excpt_path(u32 dev_id, const char *tmstmp);
bbox_status bbox_create_bbox_sub_path(char *sub_path, u32 len, const char *log_path);
bbox_status bbox_create_log_sub_path(char *sub_path, u32 len, const char *log_path);
bbox_status bbox_create_mntn_sub_path(char *sub_path, u32 len, const char *log_path);

s32 bbox_hist_log_open(u32 dev);

#endif
