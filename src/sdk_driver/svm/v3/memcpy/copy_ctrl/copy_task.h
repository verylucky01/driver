/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */
#ifndef COPY_TASK_H
#define COPY_TASK_H

#include "ka_base_pub.h"
#include "ka_list_pub.h"
#include "ka_task_pub.h"
#include "ka_common_pub.h"

#include "comm_kernel_interface.h"

#include "svm_pub.h"
#include "copy_pub.h"
#include "dma_map_kernel.h"

/* subtask flag def */
#define SVM_COPY_SUBTASK_SYNC           (1ULL << 0ULL)
#define SVM_COPY_SUBTASK_AUTO_RECYCLE   (1ULL << 1ULL)  /* Will destroy subtask after copy finished. */

struct svm_copy_res {
    int host_tgid;
    bool is_src;
    bool copy_use_va;
    int ssid;
    struct svm_global_va va_info;
    void *dma_handle;
    struct svm_dma_addr_info dma_info;
};

/* todo: add ref for dma_desc_submit */
struct svm_copy_subtask {
    ka_list_head_t node;

    struct svm_copy_task *copy_task;

    struct copy_va_info va_info;

    u32 flag;
    enum svm_cpy_dir dir;

    struct svm_copy_res src_res;
    struct svm_copy_res dst_res;

    u64 dma_node_num;
    struct devdrv_dma_node *dma_nodes;
};

struct svm_copy_subtask_list {
    ka_task_spinlock_t spinlock;
    ka_list_head_t head;
    u64 num;
};

struct svm_copy_async_subtask_ret {
    ka_atomic64_t to_wait_num;
    ka_semaphore_t sem;
    u32 dma_ret;
};

struct svm_copy_task {
    ka_kref_t ref;

    ka_device_t *dev;
    u32 udevid;     /* use which dev's dma engine to copy */
    u64 dev_page_size;
    u32 instance;

    struct svm_copy_subtask_list subtasks_list;
    struct svm_copy_async_subtask_ret async_subtasks_ret;
};

struct svm_copy_subtask *svm_copy_subtask_create(struct svm_copy_task *copy_task,
    struct copy_va_info *info, u32 flag);
void svm_copy_subtask_destroy(struct svm_copy_subtask *subtask);
int svm_copy_subtask_submit(struct svm_copy_subtask *subtask);

struct svm_copy_task *svm_copy_task_create(u32 udevid);
void svm_copy_task_destroy(struct svm_copy_task *copy_task);
int svm_copy_task_submit(struct svm_copy_task *copy_task);
int svm_copy_task_wait(struct svm_copy_task *copy_task);

u32 svm_copy_task_get_dma_node_num(struct svm_copy_task *copy_task);

#define SVM_COPY_SIZE_768KB                    (768ULL * SVM_BYTES_PER_KB)
#define SVM_COPY_SIZE_1280KB                   (1280ULL * SVM_BYTES_PER_KB)
#define SVM_COPY_SIZE_3MB                      (3ULL * SVM_BYTES_PER_MB)

#define SVM_COPY_SUBTASK_GRAIN_SIZE_256KB      (256ULL * SVM_BYTES_PER_KB)
#define SVM_COPY_SUBTASK_GRAIN_SIZE_512KB      (512ULL * SVM_BYTES_PER_KB)
#define SVM_COPY_SUBTASK_GRAIN_SIZE_1MB        (1ULL * SVM_BYTES_PER_MB)
#define SVM_COPY_SUBTASK_GRAIN_SIZE_2MB        (2ULL * SVM_BYTES_PER_MB)

#define SVM_COPY_LAST_SUBTASK_GRAIN_SIZE_4MB   (4ULL * SVM_BYTES_PER_MB)

static inline u64 svm_get_subtask_grain_size(u64 size)
{
    /*
     * size greater than DEVMM_COPY_MIN_LEN_MULTI_BLK(512k), start use multi block
     * according to the test result, the speed of copy in the following page num is higher
     * use 4K page size to get blk num, if page size is 64K or 2M, dma can get higher performance
     * here just computes first async memcpy size, second blk if left size less than 4M submit dma once
     */
    if (size <= SVM_COPY_SIZE_768KB) {
        return SVM_COPY_SUBTASK_GRAIN_SIZE_256KB;
    } else if (size <= SVM_COPY_SIZE_1280KB) {
        return SVM_COPY_SUBTASK_GRAIN_SIZE_512KB;
    } else if (size <= SVM_COPY_SIZE_3MB) {
        return SVM_COPY_SUBTASK_GRAIN_SIZE_1MB;
    } else {
        return SVM_COPY_SUBTASK_GRAIN_SIZE_2MB;
    }
}

static inline u64 svm_get_subtask_size(u64 size, u64 grain_size)
{
    return (size <= SVM_COPY_LAST_SUBTASK_GRAIN_SIZE_4MB) ? size : ka_base_min(size, grain_size);
}

#endif

