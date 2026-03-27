/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef SVM_USER_ADAPT_H
#define SVM_USER_ADAPT_H

#ifndef EMU_ST /* Simulation ST is required and cannot be deleted. */
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <securec.h>
#include <sys/ioctl.h>

#include "dmc_user_interface.h"
#else
#include <pthread.h>
#include "ut_alloc.h"
#include "ut_fops_ex.h"
#include "ut_sem.h"
#include "ut_task.h"
#endif

#define svm_ua_free    free

#ifndef EMU_ST /* Simulation ST is required and cannot be deleted. */
#define svm_file_open                   open
#define svm_file_close                  close
#define svm_user_mmap                   mmap
#define svm_user_munmap                 munmap
#define svm_ua_madvise                  madvise
#define svm_ua_calloc                   calloc
#define svm_memcpy_s                    memcpy_s
#define svm_memset_s                    memset_s
#define svm_sem_wait                    sem_wait
#define svm_getpid                      getpid

static inline int svm_user_ioctl(int fd, unsigned long cmd, void *arg) {
    int ret = ioctl(fd, cmd, arg);
    if (ret != 0) {
        share_log_read_err(HAL_MODULE_TYPE_DEVMM);
    }
    return ret;
}

#else
#define svm_file_open                   ut_fops_open
#define svm_file_close                  ut_fops_release
#define svm_user_ioctl(fd, cmd, ...)    ut_fops_ioctl((fd), (cmd), ##__VA_ARGS__)
#define svm_user_mmap                   ut_fops_mmap
#define svm_user_munmap                 ut_fops_munmap
#define svm_ua_madvise                  ut_madvise
#define svm_ua_calloc                   ut_calloc
#define svm_memcpy_s                    ut_memcpy
#define svm_memset_s                    ut_memset
#define svm_sem_wait                    ut_sem_wait
#define svm_getpid                      ut_getpid
#endif

#endif
