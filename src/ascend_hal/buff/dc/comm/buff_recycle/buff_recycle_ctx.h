/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef BUFF_RECYCLE_CTX_H
#define BUFF_RECYCLE_CTX_H

#include <sys/types.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

#include "ascend_hal_error.h"
#include "drv_user_common.h"

#include "drv_buff_common.h"

#define MEM_LIST_NUM BUFF_LIST_TYPE_MAX

struct block_mem_ctx {
    struct buff_recycle_ctx *recycle_ctx;
    int valid;
    int type;
    int status;
    unsigned int devid;
    void *mng;
    struct list_head node;
    pthread_mutex_t mutex;
};

struct proc_mng_ctx {
    int pool_id;
    int pid;
    unsigned int block_num;
    unsigned int free_num;
    unsigned long long task_id;
    unsigned long long exit_task_id;

    pthread_mutex_t alloc_tmp_list_mutex;
    struct list_head alloc_list_tmp;

    pthread_mutex_t alloc_list_mutex;
    struct list_head alloc_list[MEM_LIST_NUM];
};

struct buff_recycle_ctx {
    struct list_head node;

    pthread_t thread;

    int status;
    int wait_flag;
    int wake_up_cnt;
    unsigned int period;
    unsigned int min_period;
    long period_ns;
    sem_t sem;

    struct proc_mng_ctx p_mng;
};

struct buff_recycle_ctx_mng {
    unsigned int ctx_num;
    struct list_head head;
    pthread_rwlock_t rwlock;
};

struct buff_recycle_ctx_mng *buff_get_recycle_ctx_mng(void);

drvError_t buff_recycle_ctx_create(int pool_id, struct buff_recycle_ctx **out_ctx);
struct buff_recycle_ctx *buff_get_recycle_ctx(int pool_id);

#endif