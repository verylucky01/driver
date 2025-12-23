/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef BBOX_FS_API_H
#define BBOX_FS_API_H

#include "bbox_int.h"
#include "bbox_utils.h"
#include "bbox_system_api.h"
#include "bbox_perror.h"
#include "list.h"

#define ARRAY_TO_STR(str, array, size) do { \
    (array)[(size) - 1] = '\0';             \
    (str) = (array);                        \
} while (0)

typedef struct {
    const char *path;
    const char *name;
    s32 fd;
} file_hdl_t;

s32 bbox_save_buf_to_fs(const char *log_path, const char *file_name,
                   const buff *buf, u32 len, u32 is_append);
s32 bbox_save_log_buf_to_fs(const char *log_path, const char *file_name,
                      const buff *buf, u32 len, u32 is_append);
bbox_status bbox_age_add_folder(const char *old_path, const char *folder, char *new_path, u32 len);
bbox_status bbox_mkdir_recur(const char *dir);
bbox_status bbox_create_dir(const char *dir, u32 d_len, s32 is_tmstmp_dir);
bbox_status bbox_dir_chmod(const char *log_path, s32 mode, u32 rec_num);
bbox_status bbox_get_free_disk(u64 *free_space);
u32 bbox_get_dir_device(const char *path);
u64 bbox_get_dir_size(const char *dir);
bbox_status bbox_del_file(const char *file_name);
bbox_status bbox_del_dir(const char *dir);
bbox_status bbox_del_tms_dir_secure(u32 dev, const char *tmstmp);
bbox_status bbox_scan_tms_dir(u32 dev_id, struct list_head *list);
bbox_status bbox_save_file(const char *dir, const char *name, const buff *buf, u32 len);
s32 bbox_get_line(char **line_ptr, size_t *n, FILE *stream);
const char *bbox_dir_tok(const char *dir, u32 *phy_id, const char **tmstmp);


static inline bbox_status bbox_data_save_to_fs(struct bbox_data_info *info, const char *log_path, const char *file_name)
{
    (void)bbox_save_buf_to_fs(log_path, file_name, info->buffer, (u32)(info->used), BBOX_TRUE);
    (void)bbox_data_reinit(info);
    return BBOX_SUCCESS;
}

#endif
