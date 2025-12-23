/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <signal.h>

#include "atomic_lock.h"

#define ATOMIC_SET(x, y) __sync_lock_test_and_set((x), (y))
#define CAS(ptr, oldval, newval) __sync_bool_compare_and_swap(ptr, oldval, newval)
#define ATOMIC_LOCK_INTERVAL_TIME    (3840)   // 100us : TICK
#define ATOMIC_LOCK_SLEEP_TIME    (1000)   // 1ms : us
#define DRV_PTHREAD_ATOMIC_SLEEP_TIME (100)   // 100us

#if defined(ATOMIC_LOCK_UT) || defined(USER_BUFF_MANAGE_UT)
#define ATOMIC_LOCK_GET_CUR_SYSTEM_COUNTER(cnt) (cnt = 1000000)
#define ATOMIC_LOCK_MODULE "ATOMIC_LOCK"
#define ATOMIC_LOCK_LOG_ERR(format, ...) printf("[%s] [%s %d] " format, ATOMIC_LOCK_MODULE, __func__, __LINE__, ## __VA_ARGS__)
#define ATOMIC_LOCK_LOG_EVENT(format, ...) printf("[%s] [%s %d] " format, ATOMIC_LOCK_MODULE, __func__, __LINE__, ## __VA_ARGS__)

#elif defined CFG_FEATURE_SYSLOG
#include <syslog.h>
#if defined(__arm__) || defined(__aarch64__)
#define ATOMIC_LOCK_GET_CUR_SYSTEM_COUNTER(cnt) asm volatile("mrs %0, CNTVCT_EL0" : "=r"(cnt) :)
#else
#define ATOMIC_LOCK_GET_CUR_SYSTEM_COUNTER(cnt) do { \
    unsigned int high, low; \
    asm volatile("rdtsc" : "=a" (low), "=d" (high)); \
    cnt = ((unsigned long long)high << 32) | ((unsigned long long)low); \
} while (0);
#endif
#define ATOMIC_LOCK_LOG_ERR(fmt, ...) syslog(LOG_ERR, "[%s %d] " fmt, __func__, __LINE__, ##__VA_ARGS__)
#define ATOMIC_LOCK_LOG_EVENT(fmt, ...) syslog(LOG_NOTICE, "[%s %d] " fmt, __func__, __LINE__, ##__VA_ARGS__)

#else
#ifndef EMU_ST
#include "dmc_user_interface.h"
#else
#include "ascend_inpackage_hal.h"
#include "ut_log.h"
#endif
#if defined(__arm__) || defined(__aarch64__)
#define ATOMIC_LOCK_GET_CUR_SYSTEM_COUNTER(cnt) asm volatile("mrs %0, CNTVCT_EL0" : "=r"(cnt) :)
#else
#define ATOMIC_LOCK_GET_CUR_SYSTEM_COUNTER(cnt) do { \
    unsigned int high, low; \
    asm volatile("rdtsc" : "=a" (low), "=d" (high)); \
    cnt = ((unsigned long long)high << 32) | ((unsigned long long)low); \
} while (0);
#endif
#define ATOMIC_LOCK_LOG_ERR(format, ...) do { \
    DRV_ERR(HAL_MODULE_TYPE_COMMON, format "\n", ##__VA_ARGS__); \
} while (0);
#define ATOMIC_LOCK_LOG_EVENT(format, ...) do { \
    DRV_NOTICE(HAL_MODULE_TYPE_COMMON, format "\n", ##__VA_ARGS__); \
} while (0);
#endif

#ifdef ATOMIC_LOCK_UT
#define STATIC
#else
#define STATIC static
#endif

/***********************************************************************************************************
Function: Initialize the atomic lock and set the member variables of the lock to 0.
Input parameter: lock (pointer to the atomic lock)
Output parameter: None
Return value: 0 or an error code
************************************************************************************************************/
void init_atomic_lock(struct atomic_lock *atomic_lock)
{
    atomic_lock->lock_status = LOCK_RELEASED;
    atomic_lock->lock_get_time = 0;
}

STATIC void do_get_atomic_lock(struct atomic_lock *lock)
{
    ATOMIC_LOCK_GET_CUR_SYSTEM_COUNTER(lock->lock_get_time);      // Record the time when the lock is obtained.
}

/***********************************************************************************************************
Function: Obtain the atomic lock and update the member variables in the lock.
Input parameter: lock (pointer to the atomic lock)
          timeout Timeout interval
Output parameter: None
Return value: 0 or an error code
************************************************************************************************************/
int get_atomic_lock(struct atomic_lock *lock, unsigned long long timeout)
{
    unsigned long long start_time = 0;
    unsigned long long cur_time = 0;
    unsigned long long sleep_time = 0;
    unsigned long long sleep_start_time = 0;
    unsigned int sleep_num = 0;

    if (lock == NULL) {
        return ATOMIC_LOCK_ERROR_PARA_ERR;
    }

#ifndef ATOMIC_LOCK_UT
    if (timeout == 0) {
        while (!CAS(&lock->lock_status, LOCK_RELEASED, LOCK_OCCUPIED)) {}
    } else {
        ATOMIC_LOCK_GET_CUR_SYSTEM_COUNTER(start_time);
        sleep_time = start_time;
        while (!CAS(&lock->lock_status, LOCK_RELEASED, LOCK_OCCUPIED)) {
            ATOMIC_LOCK_GET_CUR_SYSTEM_COUNTER(cur_time);
#ifndef EMU_ST
            if ((cur_time - start_time) >= timeout) {
                ATOMIC_LOCK_LOG_EVENT("Timeout process. (startTime=%llu, lastSleepStartTime=%llu,"
                                      "lastSleepEndTime=%llu, curTime=%llu, sleepNum=%u)\n",
                                      start_time, sleep_start_time, sleep_time, cur_time, sleep_num);
                return ATOMIC_LOCKDRV_ERROR_WAIT_TIMEOUT;
            }
#endif
            if ((cur_time - sleep_time) >= (unsigned long long)ATOMIC_LOCK_INTERVAL_TIME) {
#ifndef EMU_ST
                sleep_num++;
                ATOMIC_LOCK_GET_CUR_SYSTEM_COUNTER(sleep_start_time);
#endif
                (void)usleep(ATOMIC_LOCK_SLEEP_TIME);
                ATOMIC_LOCK_GET_CUR_SYSTEM_COUNTER(sleep_time);
            }
        }
    }
#endif
    do_get_atomic_lock(lock);
    return ATOMIC_LOCK_ERROR_NONE;
}

/***********************************************************************************************************
Function: Release the atomic lock and set the member variables of the lock to 0.
Input parameter: lock (pointer to the atomic lock)
Output parameter: None
Return value: None
************************************************************************************************************/
void release_atomic_lock(struct atomic_lock *lock)
{
    lock->lock_get_time = 0;
    CAS(&lock->lock_status, LOCK_OCCUPIED, LOCK_RELEASED);
}

/* atomic lock in one process */
void drv_pthread_atomic_init(struct drv_pthread_atomic *lock)
{
    lock->lock_status = LOCK_RELEASED;
}

int drv_pthread_atomic_lock(struct drv_pthread_atomic *lock, unsigned long long timeout)
{
    unsigned long long start_time = 0;
    unsigned long long cur_time = 0;
    unsigned long long sleep_time = 0;

    if (lock == NULL) {
        return ATOMIC_LOCK_ERROR_PARA_ERR;
    }
#ifndef ATOMIC_LOCK_UT
    if (timeout == 0) {
        while (!CAS(&lock->lock_status, LOCK_RELEASED, LOCK_OCCUPIED)) {}
    } else {
        ATOMIC_LOCK_GET_CUR_SYSTEM_COUNTER(start_time);
        sleep_time = start_time;
        while (!CAS(&lock->lock_status, LOCK_RELEASED, LOCK_OCCUPIED)) {
            ATOMIC_LOCK_GET_CUR_SYSTEM_COUNTER(cur_time);
#ifndef EMU_ST
            if ((cur_time - start_time) >= timeout) {
                return ATOMIC_LOCKDRV_ERROR_WAIT_TIMEOUT;
            }
#endif
            if ((cur_time - sleep_time) >= (unsigned long long)ATOMIC_LOCK_INTERVAL_TIME) {
                (void)usleep(DRV_PTHREAD_ATOMIC_SLEEP_TIME);
                ATOMIC_LOCK_GET_CUR_SYSTEM_COUNTER(sleep_time);
            }
        }
    }
#endif
    return ATOMIC_LOCK_ERROR_NONE;
}

void drv_pthread_atomic_unlock(struct drv_pthread_atomic *lock)
{
#ifndef ATOMIC_LOCK_UT
    CAS(&lock->lock_status, LOCK_OCCUPIED, LOCK_RELEASED);
#endif
}

