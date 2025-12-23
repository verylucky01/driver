/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef BBOX_LOG_COMMON_H
#define BBOX_LOG_COMMON_H

#include "bbox_int.h"
#include "bbox_perror.h"
#include "bbox_list.h"
#include "bbox_log_list.h"

#define DIR_MAXLEN 256
#define DELETED_DIR_MAXLEN 263
#define HOST_DATE_MAXLEN 30
#define DIR_RECURSION_DEPTH 10

#define TIME_STAMP_HEAD_STR     "TimeStamp ["
#define TIME_STAMP_TAIL_STR     "]"
#define PRINT_VALID_STR(str) (((str) != NULL) ? (str) : "UNKNOWN")

#define DIR_SIZE 4096U

#define FILE_RW_MODE ((u32)M_IRUSR | (u32)M_IWUSR)
#define DIR_MODE ((u32)FILE_RW_MODE | (u32)M_IXUSR)
#define FILE_RO_MODE (M_IRUSR)
#define DIR_CHECK_MODE ((u32)(M_IRWXU | M_UMASK_GRPREAD | M_UMASK_GRPEXEC | M_UMASK_OTHREAD | M_UMASK_OTHEXEC))

#define HISTORYLOG "history.log"
#define DONE_FILE "DONE"
#define DELETE_SUFFIX ".delete"

#define DEVDIR_STR "device-"
#define DEVDIR_FORMAT DEVDIR_STR"%u"

#define CURRENT_DIRNAME "."
#define PARENT_DIRNAME ".."

#if defined(OS_TYPE) && defined(WIN64) && OS_TYPE == WIN64
#define OS_ROOT_DIR "C:\\"
#define OS_PATH_SPLIT '\\'
#define OS_DIR_SLASH "\\"
#else // OS_TYPE == LINUX
#define OS_ROOT_DIR "/"
#define OS_PATH_SPLIT '/'
#define OS_DIR_SLASH "/"
#endif

#define OS_DEV_PATH_FORMAT   "%s" OS_DIR_SLASH "%s"
#define OS_EXCPT_PATH_FORMAT OS_DEV_PATH_FORMAT OS_DIR_SLASH "%s"
#define OS_SUB_PATH_FORMAT   OS_DEV_PATH_FORMAT OS_DIR_SLASH "%s" OS_DIR_SLASH "%s"
#define OS_HISTLOG_FORMAT    OS_DEV_PATH_FORMAT OS_DIR_SLASH HISTORYLOG
#define OS_PATH_FORMAT       "%s" OS_DIR_SLASH "%s"
#define OS_PATH_FORMAT_TIME  "%s" OS_DIR_SLASH "%s_%s%s"
#define OS_PATH_FORMAT_DONE  "%s" OS_DIR_SLASH "%s_%s"

#define TMSTMP_OLD_FORMAT_LEN 21U  // old is total 21 bytes
#define TMSTMP_FORMAT_LEN 24U      // total 24 bytes
#define TMSTMP_SPLIT_IDX 14U       // the '-' in time-stamp format

enum bbox_done_stat {
    STORE_STARTING  = 0,
    STORE_COMPLETED = 1,
    STORE_DONE      = 2,
    STORE_FAILED    = 3,
    STORE_MAX
};

s32 bbox_dir_same_log(const buff *element, const arg_void *arg);
s32 bbox_dir_relate_log(const buff *element, const arg_void *arg);
s32 bbox_check_device_seq(const buff *element, const arg_void *arg);
s32 bbox_hist_log_get_entry_tmstmp(const char *log_str, char *tms, u32 len);
bool bbox_hist_log_is_valid_tmstmp(const char *s);
bool bbox_hist_log_is_valid_old_tmstmp(const char *s);
bool bbox_hist_log_is_deleted_tmstmp(const char *dir);
void *bbox_except_node_create_by_scan(struct dir_node *dirstat);

#endif /* BBOX_LOG_COMMON_H */
