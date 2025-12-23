/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef BBOX_TOOL_FS_H
#define BBOX_TOOL_FS_H

#include "bbox_int.h"

s32 bbox_save_buf_to_fs(const char *log_path, const char *file_name,
                   const buff *buf, u32 len, u32 is_append);
bbox_status bbox_mkdir_recur(const char *dir_path);
bbox_status bbox_age_add_folder(const char *old_path, const char *folder, char *new_path, u32 len);
s32 bbox_format_path(char *buf, u32 len, const char *parent, const char *child);
s32 bbox_format_device_path(char *buf, u32 len, const char *parent, u32 dev);
s32 bbox_write_done_file(const char *path, s32 stat);
bbox_status bbox_dir_chmod(const char *log_path, s32 mode, u32 rec_num);
s32 bbox_save_log_buf_to_fs(const char *log_path, const char *file_name,
                      const buff *buf, u32 len, u32 is_append);
bbox_status bbox_create_bbox_sub_path(char *sub_path, u32 len, const char *log_path);
bbox_status bbox_create_mntn_sub_path(char *sub_path, u32 len, const char *log_path);
bbox_status bbox_create_log_sub_path(char *sub_path, u32 len, const char *log_path);
#endif
