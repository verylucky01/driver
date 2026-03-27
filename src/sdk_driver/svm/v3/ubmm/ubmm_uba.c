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
#include "ka_base_pub.h"
#include "ka_memory_pub.h"
#include "ka_task_pub.h"
#include "ka_system_pub.h"

#include "svm_kern_log.h"
#include "svm_pub.h"

struct ubmm_genpool_data {
    unsigned long cursor;
    unsigned long base;
    unsigned long size;
};

static ka_gen_pool_t *uba_pool = NULL;
static struct ubmm_genpool_data uba_pool_data;

/* emu st kernel is old than 5.0.0 */
static unsigned long ubmm_genpool_algo(unsigned long *map,
			unsigned long size,
			unsigned long start,
			unsigned int nr,
			void *data, ka_gen_pool_t *pool,
			unsigned long start_addr)        /* start_addr is 0 when kernel < 5.0.0 */
{
    struct ubmm_genpool_data *pool_data = (struct ubmm_genpool_data *)data;
    unsigned long start_bit;

    start_bit = ka_base_gen_pool_first_fit(map, size, pool_data->cursor, nr, data, pool, start_addr);
    if (start_bit >= size) {
        /* failed, start from 0 */
        start_bit = ka_base_gen_pool_first_fit(map, size, 0, nr, data, pool, start_addr);
    }

    /* update cursor when success */
    if (start_bit < size) {
        pool_data->cursor = (start_bit + nr) % size;
    }

    return start_bit;
}

KA_BASE_DEFINE_GEN_POOL_ALGO(ubmm_genpool_algo)

static int _ubmm_uba_pool_create(u64 uba_base, u64 uba_size)
{
    ka_gen_pool_t *pool = NULL;
    int min_alloc_order = KA_MM_HPAGE_SHIFT; /* Hardware constraints. */
    int ret;

    pool = ka_base_gen_pool_create(min_alloc_order, KA_NUMA_NO_NODE);
    if (pool == NULL) {
        svm_err("Create gen pool failed. (uba_base=0x%llx; uba_size=0x%llx)\n", uba_base, uba_size);
        return -EFAULT;
    }

    uba_pool_data.cursor = 0;
    uba_pool_data.base = uba_base;
    uba_pool_data.size = uba_size;
    ka_base_gen_pool_set_algo(pool, KA_BASE_GEN_POOL_ALGO_NAME(ubmm_genpool_algo), (void *)&uba_pool_data);

    ret = ka_base_gen_pool_add(pool, (unsigned long)uba_base, (unsigned long)uba_size, KA_NUMA_NO_NODE);
    if (ret != 0) {
        ka_base_gen_pool_destroy(pool);
        svm_err("Add to gen pool failed. (uba_base=0x%llx; uba_size=0x%llx)\n", uba_base, uba_size);
        return ret;
    }

    uba_pool = pool;
    return 0;
}

int ubmm_uba_pool_create(u64 uba_base, u64 uba_size)
{
    static KA_TASK_DEFINE_MUTEX(mutex);
    int ret = 0;

    ka_task_mutex_lock(&mutex);
    if (uba_pool == NULL) {
        ret = _ubmm_uba_pool_create(uba_base, uba_size);
    }
    ka_task_mutex_unlock(&mutex);

    return ret;
}

void ubmm_uba_pool_destroy(void)
{
    size_t avail, total;

    if (uba_pool == NULL) {
        return;
    }

    avail = ka_base_gen_pool_avail(uba_pool);
    total = ka_base_gen_pool_size(uba_pool);
    if (avail != total) {
        svm_warn("Leak uba mem. (avail_size=0x%lx; total_size=0x%lx)\n", avail, total);
    }

    ka_base_gen_pool_destroy(uba_pool);
    uba_pool = NULL;
}

int ubmm_alloc_uba(u64 size, u64 *uba)
{
    if (uba_pool == NULL) {
        return -ENODEV;
    }

    if (size > uba_pool_data.size) {
        svm_err("Invalid value. (size=0x%llx)\n", size);
        return -EINVAL;
    }

    *uba = (u64)ka_base_gen_pool_alloc(uba_pool, size);
    if (*uba == 0) {
        return -ENOMEM;
    }

    return 0;
}

int ubmm_free_uba(u64 uba, u64 size)
{
    u64 start = uba;
    u64 end = uba + size;

    if (uba_pool == NULL) {
        return -ENODEV;
    }

    if ((start < uba_pool_data.base) || (end > uba_pool_data.base + uba_pool_data.size) || (end <= start)) {
        svm_err("Invalid value. (uba=0x%llx; size=0x%llx)\n", uba, size);
        return -EINVAL;
    }

    ka_base_gen_pool_free(uba_pool, (unsigned long)uba, (unsigned long)size);

    return 0;
}

int ubmm_get_uba_pool(u64 *uba_base, u64 *total_size, u64 *avail_size)
{
    if (uba_pool == NULL) {
        return -ENODEV;
    }

    *uba_base = (u64)uba_pool_data.base;
    *total_size = (u64)uba_pool_data.size;
    *avail_size = (u64)ka_base_gen_pool_avail(uba_pool);
    return 0;
}

