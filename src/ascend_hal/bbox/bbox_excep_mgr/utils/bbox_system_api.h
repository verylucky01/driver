/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef BBOX_SYSTEM_API_H
#define BBOX_SYSTEM_API_H

#include <stddef.h>
#include "mmpa_api.h"
#include "bbox_int.h"
#include "bbox_print.h"

// MMPA functions
s32 bbox_open(const char *path_name, s32 flags, s32 mode);
bbox_status bbox_close(s32 fd);
bbox_status bbox_chmod(const char *path_name, s32 mode);
bbox_status bbox_mkdir(const char *path_name, s32 mode);
bbox_status bbox_chdir(const char *path_name);
typedef mmDirent bbox_dirent;
s32 bbox_scan_dir(const char *dirp, bbox_dirent ***namelist, mmFilter filter, mmSort compar);
void bbox_scan_dir_free(bbox_dirent **entry_list, s32 count);
FILE *bbox_fopen(const char *path_name, const char *mode);
s32 bbox_fclose(FILE *stream);
s32 bbox_fgetc(FILE *stream);
off_t bbox_lseek(s32 fd, off_t offset, s32 whence);
typedef mmStat64_t bbox_stat64_t;
bbox_status bbox_stat64(const char *path, bbox_stat64_t *stat);
s32 bbox_write(s32 fd, const void *buf, u32 count);
s32 bbox_read(s32 fd, void *buf, u32 count);
bbox_status bbox_fsync(s32 fd);
typedef mmDiskSize bbox_disk_size;
bbox_status bbox_statfs(const char *path, bbox_disk_size *disk_size);
s32 bbox_rename(const char *oldpath, const char *newpath);
bbox_status bbox_unlink(const char *path_name);
bbox_status bbox_msleep(u32 milli_sec);
bbox_status bbox_get_time_of_day(struct timeval *time_val);
bbox_status bbox_local_time_r(const time_t *timep, struct tm *result);
char *bbox_str_tok_r(char *str, const char *delim, char **saveptr);

// MMPA thread functions
#if defined(OS_TYPE) && defined(WIN64) && OS_TYPE == WIN64
#define THREAD_PERROR(func, msg, err_idx) BBOX_PERROR(func, msg)
#else
#define THREAD_PERROR(func, msg, err_idx) BBOX_ERR("[%s]%s:(%d)function execute failed.", func, msg, err_idx)
#endif

typedef mmThread bbox_thread_t;
typedef mmUserBlock_t bbox_thread_block;
static inline void bbox_thread_set_block(bbox_thread_block *block, void *(*func)(void *), void *args)
{
    block->procFunc = func;
    block->pulArg = args;
}

bbox_status bbox_thread_create(bbox_thread_t *thread, bbox_thread_block *block);
bbox_status bbox_thread_join(bbox_thread_t *thread);
bbox_status bbox_thread_set_name(bbox_thread_t *thread, const char *name);

struct bbox_cond {
    mmCond cond;
    mmMutexFC lock;
};
bbox_status bbox_thread_cond_init(struct bbox_cond *cond);
bbox_status bbox_thread_cond_destroy(struct bbox_cond *cond);
bbox_status bbox_thread_cond_signal(struct bbox_cond *cond);
bbox_status bbox_thread_cond_lock(struct bbox_cond *cond);
bbox_status bbox_thread_cond_unlock(struct bbox_cond *cond);
bbox_status bbox_thread_cond_timed_wait(struct bbox_cond *cond, u32 m_sec);

typedef mmMutex_t bbox_lock_t;
bbox_status bbox_lock_init(bbox_lock_t *lock);
bbox_status bbox_lock_destroy(bbox_lock_t *lock);
bbox_status bbox_lock_get(bbox_lock_t *lock);
bbox_status bbox_lock_release(bbox_lock_t *lock);


// memroy function
static inline void *bbox_malloc(size_t size)
{
    BBOX_CHK_EXPR_ACTION(size == 0, return NULL, "size is 0.");
    void *ptr = malloc(size);
    if (ptr == NULL) {
        return NULL;
    }
    (void)memset_s(ptr, size, 0, size);
    return ptr;
}

static inline void bbox_free(void *ptr)
{
    if (ptr != NULL) {
        free(ptr);
    }
}

#define BBOX_SAFE_FREE(p) do { if ((p) != NULL) { free(p); (p) = NULL; } } while (0)

#endif /* BBOX_SYSTEM_API_H */
