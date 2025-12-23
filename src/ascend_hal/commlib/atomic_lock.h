/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef ATOMIC_LOCK_H
#define ATOMIC_LOCK_H

/**
 * @atomic lock error numbers.
 */
#define ATOMIC_LOCK_TIME_DEFAULT   384000000    /* 10s */

typedef enum lock_status {
    LOCK_RELEASED = 0,        /* not occupied */
    LOCK_OCCUPIED = 1,        /* occupied */
} LOCK_STATUS;

typedef enum atomic_lock_error {
    ATOMIC_LOCK_ERROR_NONE = 0,              /**< success */
    ATOMIC_LOCK_ERROR_PARA_ERR = 1,          /**wrong parameter */
    ATOMIC_LOCKDRV_ERROR_WAIT_TIMEOUT = 2,   /**wait timeout */
}ATOMIC_LOCK_ERROR;

struct atomic_lock {
    int lock_status;                     // lock status
    unsigned long long lock_get_time;    // get lock time
};

struct drv_pthread_atomic {
    int lock_status;                     // lock status
};

#define ATOMIC_LOCK_THREAD_INITIALIZER  {LOCK_RELEASED}

/**
* @brief init atomic lock
* @attention null
* @param [in] atomic_lock: struct atomic_lock
* @return 0 for success, others for fail
*/
void init_atomic_lock(struct atomic_lock *atomic_lock);

/**
* @brief get atomic lock
* @attention null
* @param [in] lock: point of struct atomic_lock
* @param [in] timeout: time
* @return 0 for success, others for fail
*/
int get_atomic_lock(struct atomic_lock *lock, unsigned long long timeout);

/**
* @brief release atomic lock
* @attention null
* @param [in] lock: point of struct atomic_lock
*/
void release_atomic_lock(struct atomic_lock *lock);

void drv_pthread_atomic_init(struct drv_pthread_atomic *lock);
int drv_pthread_atomic_lock(struct drv_pthread_atomic *lock, unsigned long long timeout);
void drv_pthread_atomic_unlock(struct drv_pthread_atomic *lock);
#endif
