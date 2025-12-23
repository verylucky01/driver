/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "bbox_system_api.h"
#include "bbox_common.h"

s32 bbox_open(const char *path_name, s32 flags, s32 mode)
{
    return mmOpen2(path_name, flags, (MODE)mode);
}

bbox_status bbox_close(s32 fd)
{
    BBOX_CHK_EXPR_ACTION(fd < 0, return BBOX_FAILURE, "fd is less than 0.");
    return (mmClose(fd) == EN_OK) ? BBOX_SUCCESS : BBOX_FAILURE;
}

bbox_status bbox_chmod(const char *path_name, s32 mode)
{
    return (mmChmod(path_name, mode) == EN_OK) ? BBOX_SUCCESS : BBOX_FAILURE;
}

bbox_status bbox_mkdir(const char *path_name, s32 mode)
{
    return (mmMkdir(path_name, (mmMode_t)mode) == EN_OK) ? BBOX_SUCCESS : BBOX_FAILURE;
}

bbox_status bbox_chdir(const char *path_name)
{
    return (mmChdir(path_name) == EN_OK) ? BBOX_SUCCESS : BBOX_FAILURE;
}

s32 bbox_scan_dir(const char *dirp, bbox_dirent ***namelist, mmFilter filter, mmSort compar)
{
    return mmScandir(dirp, namelist, filter, compar);
}

void bbox_scan_dir_free(bbox_dirent **entry_list, s32 count)
{
    mmScandirFree(entry_list, count);
}

FILE *bbox_fopen(const char *path_name, const char *mode)
{
    return fopen(path_name, mode);
}

s32 bbox_fclose(FILE *stream)
{
    BBOX_CHK_EXPR_ACTION(stream == NULL, return BBOX_FAILURE, "stream is NULL.");
    return fclose(stream);
}

s32 bbox_fgetc(FILE *stream)
{
    return fgetc(stream);
}

off_t bbox_lseek(s32 fd, off_t offset, s32 whence)
{
    return mmLseek(fd, offset, whence);
}

bbox_status bbox_stat64(const char *path, bbox_stat64_t *stat)
{
    return (mmStat64Get(path, stat) == EN_OK) ? BBOX_SUCCESS : BBOX_FAILURE;
}

s32 bbox_write(s32 fd, const void *buf, u32 count)
{
    return (s32)mmWrite(fd, (void *)(uintptr_t)buf, count);
}

s32 bbox_read(s32 fd, void *buf, u32 count)
{
    return (s32)mmRead(fd, buf, count);
}

bbox_status bbox_fsync(s32 fd)
{
    return (mmFsync2(fd) == EN_OK) ? BBOX_SUCCESS : BBOX_FAILURE;
}

bbox_status bbox_statfs(const char *path, bbox_disk_size *disk_size)
{
    return (mmGetDiskFreeSpace(path, disk_size) == EN_OK) ? BBOX_SUCCESS : BBOX_FAILURE;
}

s32 bbox_rename(const char *oldpath, const char *newpath)
{
    return rename(oldpath, newpath);
}

bbox_status bbox_unlink(const char *path_name)
{
    return (mmUnlink(path_name) == EN_OK) ? BBOX_SUCCESS : BBOX_FAILURE;
}

bbox_status bbox_msleep(u32 milli_sec)
{
    return (mmSleep(milli_sec) == EN_OK) ? BBOX_SUCCESS : BBOX_FAILURE;
}

bbox_status bbox_get_time_of_day(struct timeval *time_val)
{
    mmTimeval tm = {0};
    if (bbox_check_feature(FEATURE_CLOCK) == true) {
        struct timespec ts = { 0 };

        if (bbox_get_dpclk() == CLOCK_RELTIME) {
            if (mmGetTimeOfDay(&tm, NULL) != EN_OK) {
                return BBOX_FAILURE;
            }
        } else {
            if (clock_gettime(CLOCK_VIRTUAL, &ts) != BBOX_SUCCESS) {
                BBOX_PERROR("clock_gettime", "");
                return BBOX_FAILURE;
            }
            tm.tv_sec = ts.tv_sec;
            tm.tv_usec = ts.tv_nsec / KILO;
        }
    } else {
        if (mmGetTimeOfDay(&tm, NULL) != EN_OK) {
            return BBOX_FAILURE;
        }
    }

    time_val->tv_sec = tm.tv_sec;
    time_val->tv_usec = tm.tv_usec;

    return BBOX_SUCCESS;
}

bbox_status bbox_local_time_r(const time_t *timep, struct tm *result)
{
    if (mmLocalTimeR(timep, result) != EN_OK) {
        return BBOX_FAILURE;
    }
    return BBOX_SUCCESS;
}

char *bbox_str_tok_r(char *str, const char *delim, char **saveptr)
{
    return mmStrTokR(str, delim, saveptr);
}

bbox_status bbox_thread_create(bbox_thread_t *thread, bbox_thread_block *block)
{
    return (mmCreateTask(thread, block) == EN_OK) ? BBOX_SUCCESS : BBOX_FAILURE;
}

bbox_status bbox_thread_join(bbox_thread_t *thread)
{
    return (mmJoinTask(thread) == EN_OK) ? BBOX_SUCCESS : BBOX_FAILURE;
}

bbox_status bbox_thread_set_name(bbox_thread_t *thread, const char *name)
{
    return (mmSetThreadName(thread, name) == EN_OK) ? BBOX_SUCCESS : BBOX_FAILURE;
}

/*
 * @brief       : initialize thread condition and condition lock
 * @param [in]  : cond      condition to be initialized, must not be null
 * @return      : 0 on success otherwise -1
 */
bbox_status bbox_thread_cond_init(struct bbox_cond *cond)
{
    signed int ret = mmCondInit(&cond->cond);
    if (ret != EN_OK) {
        BBOX_ERR("call mmCondInit failed(%d).", ret);
        return BBOX_FAILURE;
    }

    ret = mmCondLockInit(&cond->lock);
    if (ret != EN_OK) {
        BBOX_ERR("call mmCondLockInit failed(%d).", ret);
        (void)mmCondDestroy(&cond->cond);
        return BBOX_FAILURE;
    }

    return BBOX_SUCCESS;
}

/*
 * @brief       : deinitialize thread condition and condition lock
 * @param [in]  : cond      condition to be de-initialized, must not be null
 * @return      : 0 on success otherwise -1
 */
bbox_status bbox_thread_cond_destroy(struct bbox_cond *cond)
{
    signed int ret1 = mmCondDestroy(&cond->cond);
    if (ret1 != EN_OK) {
        BBOX_ERR("call mmCondDestroy failed(%d).", ret1);
    }

    signed int ret2 = mmCondLockDestroy(&cond->lock);
    if (ret2 != EN_OK) {
        BBOX_ERR("call mmCondLockDestroy failed(%d).", ret2);
    }

    return ((ret1 == EN_OK) && (ret2 == EN_OK)) ? BBOX_SUCCESS : BBOX_FAILURE;
}

/*
 * @brief       : lock thread condition
 * @param [in]  : cond      condition to be locked, must not be null
 * @return      : 0 on success otherwise -1
 */
bbox_status bbox_thread_cond_lock(struct bbox_cond *cond)
{
    return (mmCondLock(&cond->lock) == EN_OK) ? BBOX_SUCCESS : BBOX_FAILURE;
}

/*
 * @brief       : unlock thread condition
 * @param [in]  : cond      condition to be unlocked, must not be null
 * @return      : 0 on success otherwise -1
 */
bbox_status bbox_thread_cond_unlock(struct bbox_cond *cond)
{
    return (mmCondUnLock(&cond->lock) == EN_OK) ? BBOX_SUCCESS : BBOX_FAILURE;
}

/*
 * @brief       : wake up thread condition
 * @param [in]  : cond      condition to be waked up, must not be null
 * @return      : 0 on success otherwise -1
 */
bbox_status bbox_thread_cond_signal(struct bbox_cond *cond)
{
    return (mmCondNotify(&cond->cond) == EN_OK) ? BBOX_SUCCESS : BBOX_FAILURE;
}

/*
 * @brief       : sleep and wait thread condition signal
 * @param [in]  : cond      condition to sleep, must not be null
 * @param [in]  : m_sec      sleep time before wake up without signal
 * @return      : 0 on success otherwise -1
 */
bbox_status bbox_thread_cond_timed_wait(struct bbox_cond *cond, u32 m_sec)
{
    // need mmpa return the error code, currently not support yet.
    return (mmCondTimedWait(&cond->cond, &cond->lock, m_sec) == EN_OK) ? BBOX_SUCCESS : BBOX_FAILURE;
}

/*
 * @brief       : initial lock
 * @param [in]  : lock      lock pointer
 * @return      : 0 on success otherwise -1
 */
bbox_status bbox_lock_init(bbox_lock_t *lock)
{
    return (mmMutexInit(lock) == EN_OK) ? BBOX_SUCCESS : BBOX_FAILURE;
}

/*
 * @brief       : destroy lock
 * @param [in]  : lock      lock pointer
 * @return      : 0 on success otherwise -1
 */
bbox_status bbox_lock_destroy(bbox_lock_t *lock)
{
    return (mmMutexDestroy(lock) == EN_OK) ? BBOX_SUCCESS : BBOX_FAILURE;
}

/*
 * @brief       : lock
 * @param [in]  : lock      lock pointer
 * @return      : 0 on success otherwise -1
 */
bbox_status bbox_lock_get(bbox_lock_t *lock)
{
    return (mmMutexLock(lock) == EN_OK) ? BBOX_SUCCESS : BBOX_FAILURE;
}

/*
 * @brief       : unlock
 * @param [in]  : lock      lock pointer
 * @return      : 0 on success otherwise -1
 */
bbox_status bbox_lock_release(bbox_lock_t *lock)
{
    return (mmMutexUnLock(lock) == EN_OK) ? BBOX_SUCCESS : BBOX_FAILURE;
}
